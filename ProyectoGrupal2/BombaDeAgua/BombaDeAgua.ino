// Definir los pines
const int bombaPin1 = 7;            // Pin del MOSFET que controla la bomba 1 (para subir el nivel de agua)
const int bombaPin2 = 8;            // Pin del MOSFET que controla la bomba 2 (para devolver el agua)
const int potenciometroPin = A1;    // Pin del potenciómetro
const int sensorAguaPin = A2;       // Pin del sensor de nivel de agua

// Constantes PID para bomba 1
double Kp1 = 1.0;
double Ki1 = 0.2;
double Kd1 = 0.5;

// Constantes PID para bomba 2
double Kp2 = 1.0;
double Ki2 = 0.2;
double Kd2 = 0.5;

const int zonaMuerta = 5;          // Zona muerta para ambas bombas
const int histeresis = 20;         // Margen de histéresis

// Filtro de Media Móvil
const int numLecturas = 10;
int lecturas[numLecturas];
int indiceLectura = 0;
int sumaLecturas = 0;

// Variables PID para bomba 1
double errorPrevio1 = 0;
double integral1 = 0;
unsigned long tiempoPrevio1 = 0;

// Variables PID para bomba 2
double errorPrevio2 = 0;
double integral2 = 0;
unsigned long tiempoPrevio2 = 0;

// Estado de activación de la bomba
bool bomba1Activa = false;
bool bomba2Activa = false;

void setup() {
  Serial.begin(9600);

  // Configurar pines
  pinMode(bombaPin1, OUTPUT);
  pinMode(bombaPin2, OUTPUT);

  // Inicializar todas las lecturas a 0 para el filtro
  for (int i = 0; i < numLecturas; i++) {
    lecturas[i] = 0;
  }

  digitalWrite(bombaPin1, LOW);
  digitalWrite(bombaPin2, LOW);
}

void loop() {
  // Leer y mapear el valor del potenciómetro
  int valorPotenciometro = analogRead(potenciometroPin);
  int nivelDeseado = map(valorPotenciometro, 0, 1023, 300, 500);

  // Leer y filtrar el valor del sensor de agua
  int lecturaSensor = analogRead(sensorAguaPin);
  sumaLecturas -= lecturas[indiceLectura];
  lecturas[indiceLectura] = lecturaSensor;
  sumaLecturas += lecturaSensor;
  indiceLectura = (indiceLectura + 1) % numLecturas;

  // Calcular el promedio filtrado del sensor
  int nivelActual = sumaLecturas / numLecturas;

  // Calcular el error común
  double error = nivelDeseado - nivelActual;

  // Aplicar la zona muerta para evitar movimientos innecesarios
  if (abs(error) < zonaMuerta) {
    // Apagar ambas bombas si el error está dentro de la zona muerta
    digitalWrite(bombaPin1, LOW);
    digitalWrite(bombaPin2, LOW);
    bomba1Activa = false;
    bomba2Activa = false;
    Serial.println("Error dentro de la zona muerta, ambas bombas apagadas.");
    return;
  }

  unsigned long tiempoActual = millis();
  double deltaTiempo = (tiempoActual - tiempoPrevio1) / 1000.0; // Convertir a segundos

  if (error > histeresis && !bomba2Activa) {
    // PID para Bomba 1 (subir nivel de agua) solo si bomba 2 está inactiva y el error es mayor que la histéresis
    integral1 += error * deltaTiempo;
    double derivada1 = (error - errorPrevio1) / deltaTiempo;
    double salidaPID1 = (Kp1 * error) + (Ki1 * integral1) + (Kd1 * derivada1);
    salidaPID1 = constrain(salidaPID1, 0, 255);

    analogWrite(bombaPin1, salidaPID1);
    digitalWrite(bombaPin2, LOW);
    bomba1Activa = true;
    bomba2Activa = false;
    Serial.println("Bomba 1 encendida (subiendo nivel de agua).");

    // Actualizar variables previas para bomba 1
    errorPrevio1 = error;
    tiempoPrevio1 = tiempoActual;

  } else if (error < -histeresis && !bomba1Activa) {
    // PID para Bomba 2 (bajar nivel de agua) solo si bomba 1 está inactiva y el error es menor que la histéresis
    double deltaTiempo2 = (tiempoActual - tiempoPrevio2) / 1000.0;
    integral2 += error * deltaTiempo2;
    double derivada2 = (error - errorPrevio2) / deltaTiempo2;
    double salidaPID2 = (Kp2 * error) + (Ki2 * integral2) + (Kd2 * derivada2);
    salidaPID2 = constrain(-salidaPID2, 0, 255);

    analogWrite(bombaPin2, salidaPID2);
    digitalWrite(bombaPin1, LOW);
    bomba1Activa = false;
    bomba2Activa = true;
    Serial.println("Bomba 2 encendida (devolviendo agua).");

    // Actualizar variables previas para bomba 2
    errorPrevio2 = error;
    tiempoPrevio2 = tiempoActual;
  } else if (error < histeresis && error > -histeresis) {
    // Apagar ambas bombas si el error es menor que la histéresis (dentro del rango cercano al nivel deseado)
    digitalWrite(bombaPin1, LOW);
    digitalWrite(bombaPin2, LOW);
    bomba1Activa = false;
    bomba2Activa = false;
    Serial.println("Ambas bombas apagadas (dentro de histéresis).");
  }

  // Imprimir valores para monitoreo
  Serial.print("Nivel deseado: ");
  Serial.println(nivelDeseado);
  Serial.print("Nivel actual filtrado: ");
  Serial.println(nivelActual);
  Serial.print("Error: ");
  Serial.println(error);

  delay(10); // Pequeño retardo para evitar lecturas rápidas
}
