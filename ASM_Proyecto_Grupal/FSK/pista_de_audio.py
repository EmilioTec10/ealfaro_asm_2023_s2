import serial
import time
import wave
import struct
from pydub import AudioSegment

# Configuración del puerto serie
port = 'COM14'  # Cambia esto según el puerto en el que esté conectado el ESP32
baud_rate = 9600
ser = serial.Serial(port, baud_rate, timeout=1)

# Frecuencia de muestreo deseada en Hz (10 kHz)
sampling_frequency = 10000
interval = 1.0 / sampling_frequency  # Intervalo en segundos para la frecuencia de muestreo

# Abre el archivo de audio usando pydub para convertirlo si es necesario
audio_file_path = 'audio.wav'
audio = AudioSegment.from_file(audio_file_path)

# Convertir el audio a mono si no lo es
if audio.channels != 1:
    print("El archivo de audio no es mono. Convirtiendo a mono...")
    audio = audio.set_channels(1)

# Convertir la frecuencia de muestreo del audio a 10,000 Hz si es diferente
if audio.frame_rate != sampling_frequency:
    print(f"Convirtiendo la frecuencia de muestreo de {audio.frame_rate} Hz a {sampling_frequency} Hz...")
    audio = audio.set_frame_rate(sampling_frequency)

# Guardar el archivo convertido temporalmente en formato WAV para abrirlo con wave
temp_audio_path = "temp_audio.wav"
audio.export(temp_audio_path, format="wav")

# Abre el archivo de audio convertido en modo lectura binaria
audio_file = wave.open(temp_audio_path, 'rb')

# Lee el número total de frames
num_frames = audio_file.getnframes()

try:
    while True:
        # Leer un frame (muestra) del archivo de audio
        frame_data = audio_file.readframes(1)

        if not frame_data:
            # Si llegamos al final del archivo, rebobinamos para reproducir en bucle
            audio_file.rewind()
            frame_data = audio_file.readframes(1)

        # Convierte la muestra a un valor de 16 bits (asumiendo formato PCM)
        audio_sample = struct.unpack('<h', frame_data)[0]  # '<h' para formato little-endian de 16 bits

        # Escalar el valor de 16 bits a 12 bits, similar a un ADC de 12 bits (0-4095)
        valor_adc = (audio_sample + 32768) * 4095 // 65535

        # Imprimir el valor escalado para verificar si cambia
        print(f"Valor ADC escalado: {valor_adc}")

        # Enviar el valor al ESP32 en formato binario de 12 bits
        valor_binario = format(valor_adc, '012b')
        print(f"Enviando valor binario: {valor_binario}")  # Mostrar el valor binario en la consola para depuración
        ser.write(valor_binario.encode())  # Enviar el valor en formato binario





        # Leer la respuesta del ESP32
        if ser.in_waiting > 0:  # Verificar si hay datos disponibles para leer
            response = ser.readline().decode('utf-8', errors='ignore').strip()  # Leer la línea de respuesta
            if response:
                print("ESP32 dice:", response)  # Imprimir la respuesta en la consola de Python

        # Esperar el intervalo de muestreo
        time.sleep(interval)

except KeyboardInterrupt:
    print("Interrupción del usuario. Finalizando transmisión.")

finally:
    # Cierra el archivo de audio y el puerto serie
    audio_file.close()
    ser.close()