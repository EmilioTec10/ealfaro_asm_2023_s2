// Definir los pines
const int bombaPin1 = 7;           // Pin del MOSFET que controla la bomba 1 (para subir el nivel de agua)
const int bombaPin2 = 8;           // Pin del MOSFET que controla la bomba 2 (para devolver el agua)
const int potenciometroPin = A1;   // Pin del potenciómetro

int valorPotenciometroPrevio = 0;
const int tolerancia = 5;          // Define un margen de tolerancia para evitar fluctuaciones
const int estabilidadRequerida = 3; // Número de lecturas estables necesarias para activar el cambio
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

  // Comparar el valor actual del potenciómetro con el valor previo considerando la tolerancia
  if (valorPotenciometro > valorPotenciometroPrevio + tolerancia) {
    // Si el valor del potenciómetro ha subido más allá de la tolerancia, incrementar contador
    contadorEstable++;
    if (contadorEstable >= estabilidadRequerida) {
      // Si el cambio es estable, activar la bomba 1 y apagar la bomba 2
      digitalWrite(bombaPin1, HIGH);
      digitalWrite(bombaPin2, LOW);
      Serial.println("Bomba 1 encendida (subiendo nivel de agua).");
      contadorEstable = 0;
    }
  } else if (valorPotenciometro < valorPotenciometroPrevio - tolerancia) {
    // Si el valor del potenciómetro ha bajado más allá de la tolerancia, incrementar contador
    contadorEstable++;
    if (contadorEstable >= estabilidadRequerida) {
      // Si el cambio es estable, activar la bomba 2 y apagar la bomba 1
      digitalWrite(bombaPin1, LOW);
      digitalWrite(bombaPin2, HIGH);
      Serial.println("Bomba 2 encendida (devolviendo agua).");
      contadorEstable = 0;
    }
  } else {
    // Si el valor del potenciómetro está dentro de la tolerancia, apagar ambas bombas
    digitalWrite(bombaPin1, LOW);
    digitalWrite(bombaPin2, LOW);
    Serial.println("Ambas bombas apagadas.");
    contadorEstable = 0; // Reiniciar el contador ya que el valor es estable
  }

  // Imprimir los valores en el Monitor Serial para depuración
  Serial.print("Valor anterior: ");
  Serial.println(valorPotenciometroPrevio);
  Serial.print("Valor actual: ");
  Serial.println(valorPotenciometro);

  // Guardar el valor actual del potenciómetro para la siguiente iteración
  valorPotenciometroPrevio = valorPotenciometro;

  delay(100); // Espera 100 ms antes de la siguiente lectura
}
