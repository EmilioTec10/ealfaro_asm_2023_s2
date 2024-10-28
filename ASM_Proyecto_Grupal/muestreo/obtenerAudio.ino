// Función para recibir los datos de audio en formato de 12 bits
uint16_t recibir12Bits(uint8_t bit) {
    static uint16_t bufferBits = 0; // Buffer para almacenar los bits
    static uint8_t bitCount = 0;    // Contador de bits

    // Desplazar el buffer hacia la izquierda y añadir el nuevo bit
    bufferBits = (bufferBits << 1) | bit;
    bitCount++;

    // Imprimir el bit recibido y el estado actual del buffer
    Serial.print("Bit recibido: ");
    Serial.print(bit);
    Serial.print(" | Buffer actual: ");
    Serial.print(bufferBits, BIN);
    Serial.print(" | Contador de bits: ");
    Serial.println(bitCount);

    // Si se han recibido 12 bits, devolver el valor y reiniciar el contador
    if (bitCount == 12) {
        uint16_t resultado = bufferBits; // Guardar el resultado
        bufferBits = 0; // Reiniciar el buffer
        bitCount = 0;   // Reiniciar el contador
        Serial.print("Resultado completo recibido: ");
        Serial.println(resultado);
        return resultado; // Devolver el valor de 12 bits
    }

    // Si no se han recibido 12 bits aún, devolver un valor especial (por ejemplo, 0xFFFF)
    return 0xFFFF;
}

// Función para obtener muestras de audio desde la entrada serial
// Función para obtener muestras de audio desde la entrada serial
uint16_t obtenerMuestraAudio() {
    uint16_t valor = 0xFFFF;
    while (valor == 0xFFFF) {
        if (Serial.available() > 0) {
            char receivedBit = Serial.read(); // Leer el bit recibido

            // Llamar a la función para procesar los bits
            if (receivedBit == '0' || receivedBit == '1') {
                valor = recibir12Bits(receivedBit - '0'); // Convierte el carácter '0' o '1' a 0 o 1
            } else {
                Serial.print("Carácter inválido recibido: ");
                Serial.println(receivedBit); // Imprimir si se recibe un carácter que no es '0' o '1'
            }
        }
    }
    // Imprimir el valor recibido una vez que se hayan recibido 12 bits completos
    Serial.print("Valor recibido (12 bits): ");
    Serial.println(valor);
    return valor;
}

