import serial
import time
import wave

# Configuración del puerto serie
port = 'COM3'  # Cambia esto según el puerto en el que esté conectado el ESP32
baud_rate = 115200
ser = serial.Serial(port, baud_rate)

# Abre el archivo de audio
audio_file = wave.open('audio.wav', 'rb')

# Lee los datos del audio
frame_rate = audio_file.getframerate()
num_frames = audio_file.getnframes()
audio_data = audio_file.readframes(num_frames)

# Convierte el audio en una secuencia de bits (simplificación)
# Aquí podrías implementar una rutina para convertir el audio en bits
bitstream = ''.join(format(byte, '08b') for byte in audio_data)

# Enviar los bits al ESP32
for bit in bitstream:
    ser.write(bit.encode())  # Enviar el bit al ESP32
    time.sleep(1 / frame_rate)  # Ajusta el tiempo de envío según la tasa de muestreo

# Cierra el archivo de audio y el puerto serie
audio_file.close()
ser.close()
