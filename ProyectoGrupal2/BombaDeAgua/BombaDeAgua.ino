// Definir los pines
const int bombaPin1 = 7;           // Pin del MOSFET que controla la bomba 1 (para subir el nivel de agua)
const int bombaPin2 = 8;           // Pin del MOSFET que controla la bomba 2 (para devolver el agua)
const int potenciometroPin = A1;   // Pin del potenciómetro
const int sensorAguaPin = A2;      // Pin del sensor de nivel de agua

// Constantes y variables
const int tolerancia = 5;               // Tolerancia para cambios en el potenciómetro
const int margenSensor = 20;            // Margen de variación permitida para el sensor de agua
const int estabilidadRequerida = 3;     // Número de lecturas estables necesarias para activar el cambio

int valorPotenciometroPrevio = 0;
int contadorEstable = 0;

void setup() {
  // Inicializar comunicación serial
  Serial.begin(9600);

  // Configurar pines
  pinMode(bombaPin1, OUTPUT);
  pinMode(bombaPin2, OUTPUT);

  // Asegurarse de que las bombas estén apagadas al inicio
  digitalWrite(bombaPin1, LOW);
  digitalWrite(bombaPin2, LOW);
}

void loop() {
  // Leer el valor analógico del potenciómetro (0 a 1023)
  int valorPotenciometro = analogRead(potenciometroPin);

  // Mapeo del valor del potenciómetro al rango de 100 a 450
  int valorObjetivo = map(valorPotenciometro, 0, 1023, 100, 470);

  // Leer el valor del sensor de agua
  int valorSensorAgua = analogRead(sensorAguaPin);

  // Comparar el valor del sensor con el objetivo y verificar si está fuera del margen permitido
  if (valorSensorAgua < (valorObjetivo - margenSensor)) {
    // Si el valor del sensor es menor que el objetivo, enciende la bomba 1 para subir el nivel de agua
    digitalWrite(bombaPin1, HIGH);
    digitalWrite(bombaPin2, LOW);
    Serial.println("Bomba 1 encendida (subiendo nivel de agua).");

  } else if (valorSensorAgua > (valorObjetivo + margenSensor)) {
    // Si el valor del sensor es mayor que el objetivo, enciende la bomba 2 para bajar el nivel de agua
    digitalWrite(bombaPin1, LOW);
    digitalWrite(bombaPin2, HIGH);
    Serial.println("Bomba 2 encendida (devolviendo agua).");

  } else {
    // Si el valor del sensor está dentro del margen permitido, apaga ambas bombas
    digitalWrite(bombaPin1, LOW);
    digitalWrite(bombaPin2, LOW);
    Serial.println("Nivel de agua estable, ambas bombas apagadas.");
  }

  // Imprimir los valores en el Monitor Serial para depuración
  Serial.print("Valor objetivo (potenciómetro): ");
  Serial.println(valorObjetivo);
  Serial.print("Valor del sensor de agua: ");
  Serial.println(valorSensorAgua);

  delay(100); // Espera 100 ms antes de la siguiente lectura
}
