// Definir los pines
const int bombaPin1 = 7;           // Pin del MOSFET que controla la bomba 1 (para subir el nivel de agua)
const int bombaPin2 = 8;           // Pin del MOSFET que controla la bomba 2 (para devolver el agua)
const int potenciometroPin = A1;   // Pin del potenciómetro
const int sensorAguaPin = A2;      // Pin del sensor de nivel de agua

// Constantes PID
double Kp = 1.0;                   // Constante proporcional (ajustar según necesite)
double Ki = 0.2;                   // Constante integral (ajustar según necesite)
double Kd = 0.5;                   // Constante derivativa (ajustar según necesite)
const int zonaMuerta = 5;          // Zona muerta reducida para mayor precisión

// Filtro de Media Móvil
const int numLecturas = 10;        // Número de lecturas para el filtro de media móvil
int lecturas[numLecturas];         // Array para almacenar las lecturas
int indiceLectura = 0;
int sumaLecturas = 0;

// Variables PID
double errorPrevio = 0;
double integral = 0;
unsigned long tiempoPrevio = 0;

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
  int nivelDeseado = map(valorPotenciometro, 0, 1023, 120, 570);

  // Leer y filtrar el valor del sensor de agua
  int lecturaSensor = analogRead(sensorAguaPin);
  sumaLecturas -= lecturas[indiceLectura];
  lecturas[indiceLectura] = lecturaSensor;
  sumaLecturas += lecturaSensor;
  indiceLectura = (indiceLectura + 1) % numLecturas;

  // Calcular el promedio filtrado del sensor
  int nivelActual = sumaLecturas / numLecturas;

  // Calcular el error
  double error = nivelDeseado - nivelActual;

  // Aplicar la zona muerta
  if (abs(error) < zonaMuerta) {
    digitalWrite(bombaPin1, LOW);
    digitalWrite(bombaPin2, LOW);
    Serial.println("Error dentro de la zona muerta, ambas bombas apagadas.");
    return; // Salir del loop si el error está dentro de la zona muerta
  }

  // Calcular el tiempo transcurrido
  unsigned long tiempoActual = millis();
  double deltaTiempo = (tiempoActual - tiempoPrevio) / 1000.0; // Convertir a segundos

  // Calcular términos PID
  integral += error * deltaTiempo;
  double derivada = (error - errorPrevio) / deltaTiempo;
  double salidaPID = (Kp * error) + (Ki * integral) + (Kd * derivada);

  // Limitar la salida del PID para un control más gradual
  salidaPID = constrain(salidaPID, -255, 255);

  // Control de las bombas basado en la salida PID
  if (salidaPID > 0) {
    analogWrite(bombaPin1, salidaPID); // Usa PWM para control gradual de la bomba 1
    digitalWrite(bombaPin2, LOW);
    Serial.println("Bomba 1 encendida (subiendo nivel de agua).");
  } else if (salidaPID < 0) {
    analogWrite(bombaPin2, -salidaPID); // Usa PWM para control gradual de la bomba 2
    digitalWrite(bombaPin1, LOW);
    Serial.println("Bomba 2 encendida (devolviendo agua).");
  }

  // Imprimir valores para monitoreo
  Serial.print("Nivel deseado: ");
  Serial.println(nivelDeseado);
  Serial.print("Nivel actual filtrado: ");
  Serial.println(nivelActual);
  Serial.print("Salida PID: ");
  Serial.println(salidaPID);

  // Actualizar variables previas
  errorPrevio = error;
  tiempoPrevio = tiempoActual;

  delay(100); // Espera antes de la siguiente lectura
}
