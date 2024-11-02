// Definir los pines
const int bombaPin = 7;           // Pin del MOSFET que controla la bomba
const int potenciometroPin = A1;   // Pin del potenciómetro
const int triggerPin = 2;          // Pin de trigger del sensor ultrasónico
const int echoPin = 3;             // Pin de echo del sensor ultrasónico

void setup() {
  // Inicializar comunicación serial
  Serial.begin(9600);

  // Configurar pines
  pinMode(bombaPin, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Leer el valor analógico del potenciómetro (0 a 1023)
  int valorPotenciometro = analogRead(potenciometroPin);

  // Mapear el valor del potenciómetro a una distancia deseada (15 cm a 5 cm)
  int distanciaDeseada = map(valorPotenciometro, 0, 1023, 15, 5);

  // Enviar pulso al sensor ultrasónico
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Leer el tiempo de duración del pulso de retorno
  long duracion = pulseIn(echoPin, HIGH);

  // Calcular la distancia medida en centímetros
  int distanciaMedida = duracion * 0.034 / 2;

  // Control de la bomba basado en la distancia deseada
  if (distanciaMedida > distanciaDeseada) {
    // Si la distancia medida es mayor que la deseada, encender la bomba
    digitalWrite(bombaPin, HIGH);
    Serial.println("Bomba encendida.");
  } else {
    // Si la distancia medida es igual o menor que la deseada, apagar la bomba
    digitalWrite(bombaPin, LOW);
    Serial.println("Bomba apagada.");
  }

  // Imprimir los valores en el Monitor Serial
  Serial.print("Valor del potenciometro: ");
  Serial.println(valorPotenciometro);
  Serial.print("Distancia deseada: ");
  Serial.print(distanciaDeseada);
  Serial.println(" cm");
  Serial.print("Distancia medida por el sensor ultrasonico: ");
  Serial.print(distanciaMedida);
  Serial.println(" cm");

  delay(1000); // Espera 1 segundo antes de la siguiente lectura
}
