import wave
import math
import struct
import os

os.makedirs('assets', exist_ok=True)

def generate_tone(filename, frequency, duration, volume=0.5, type='sine', decay=False):
    sample_rate = 44100
    num_samples = int(sample_rate * duration)
    
    with wave.open(filename, 'w') as wav_file:
        wav_file.setnchannels(1)
        wav_file.setsampwidth(2)
        wav_file.setframerate(sample_rate)
        
        for i in range(num_samples):
            t = float(i) / sample_rate
            env = 1.0
            if decay:
                env = max(0, 1.0 - (i / num_samples))
                
            if type == 'sine':
                value = math.sin(2.0 * math.pi * frequency * t)
            elif type == 'noise':
                import random
                value = random.uniform(-1.0, 1.0)
                
            sample = int(value * env * volume * 32767.0)
            wav_file.writeframesraw(struct.pack('<h', sample))

# UI Click: short high pitch pop
generate_tone('assets/click.wav', 1200, 0.05, 0.3, 'sine', decay=True)

# Shoot: noise with decay (basic gunshot placeholder)
generate_tone('assets/shoot.wav', 400, 0.2, 0.5, 'noise', decay=True)

print("Sounds generated.")
