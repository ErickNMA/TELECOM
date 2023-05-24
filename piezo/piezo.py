import serial
import matplotlib.pyplot as plt
import numpy as np
import time as t
import scienceplots





# Configurações de plot:
plt.style.use([
    'grid',
    'retro'
])
plt.rcParams['lines.linewidth'] = 3
plt.rcParams['font.size'] = 20





# Identificação da porta COM:
def get_port():
    for i in range(20): 
        try:
            return serial.Serial(f'COM{i}', 115200) 
        except:
            pass

# Leitura de dado:
def getData():
    if(port.in_waiting):
        try:
            return port.readline().decode().rstrip('\n')
        except:
            port.read()
            return ''





# Preparando a comunicação:
port = get_port()
#port = serial.Serial('COM{12', 115200) 
port.close()
port.open()
port.reset_input_buffer()
port.reset_output_buffer()
t.sleep(2)






# Amostragem:
if(not input("\n\nPressione 'ENTER' para começar a amostragem")):
        port.write('START'.encode())

while(getData() != 'BEGIN'):
    pass

data = []
while(True):
    text = getData()
    if(text == 'END'):
        break
    if(text):
        data.append(int(text))

port.close()





# Plot do gráfico:

x = []
for i in data:
    if(i>50):
        x.append(1)
    else:
        x.append(0)

time = np.arange(0, 3, 1e-2)
plt.figure()
plt.plot(time, data)
plt.yticks(np.append(np.arange(0, 1023, (2**8)), [1023]))
plt.title('Leitura dos Sinais')
plt.ylabel('Analog Read')
plt.xlabel('Tempo [s]')
plt.show()