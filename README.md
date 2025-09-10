WeatherBox

O WeatherBox é uma estação meteorológica de baixo custo baseada em ESP32. 
Utiliza os sensores DHT11 (temperatura e umidade) e BMP180 (pressão atmosférica)
para coletar dados ambientais e enviá-los para a plataforma ThingSpeak, permitindo acompanhamento remoto.

Funcionalidades
Leitura de temperatura e umidade (DHT11)
Medição de pressão atmosférica e altitude (BMP180)
Envio de dados para o ThingSpeak
Visualização em tempo real pelo aplicativo ThingView
Hardware utilizado
ESP32
Sensor DHT11
Sensor BMP180
Jumpers e protoboard ou PCB

Como usar
Configure as credenciais Wi-Fi e a API Key do ThingSpeak no código
Faça o upload para o ESP32 usando a Arduino IDE ou PlatformIO
Acompanhe os dados pelo ThingSpeak ou ThingView
