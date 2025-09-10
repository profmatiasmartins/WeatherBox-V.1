#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_BMP085.h>
#include <SimpleDHT.h>
#include <WiFiManager.h> // Biblioteca WiFiManager

// Configuração ThingSpeak
const char* server = "http://api.thingspeak.com/update";
const char* apiKey = "QOH89PFRZIQK4VLB";

// Sensores
#define DHT_PIN 4
SimpleDHT11 dht11;
Adafruit_BMP085 bmp;

// Intervalo de envio (ms)
const long interval = 15000;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Iniciando estação meteorológica...");

  // ==== WiFiManager ====
  WiFiManager wm;

  // Se quiser resetar as configs salvas, descomente:
  // wm.resetSettings();

  if (!wm.autoConnect("WeatherBox_V1", "12345678")) { 
    Serial.println("Falha ao conectar e tempo limite atingido.");
    ESP.restart(); // Reinicia se não conectar
  }

  Serial.println("Wi-Fi conectado!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());

  // Inicializa BMP180
  if (!bmp.begin()) {
    Serial.println("Erro ao inicializar BMP180!");
    while (1);
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    //  Lê DHT11
    byte temperatureDHT = 0;
    byte humidityDHT = 0;
    int err = dht11.read(DHT_PIN, &temperatureDHT, &humidityDHT, NULL);
    if (err != SimpleDHTErrSuccess) {
      Serial.println("Erro ao ler DHT11");
      return;
    }

    // Lê BMP180
    float temperatureBMP = bmp.readTemperature();
    float pressureBMP = bmp.readPressure() / 100.0; // hPa

    // Exibe no Serial
    Serial.println("----- Dados da estação -----");
    Serial.print("DHT11 -> Temp: "); Serial.print((int)temperatureDHT); Serial.print(" °C, Umid: "); Serial.print((int)humidityDHT); Serial.println(" %");
    Serial.print("BMP180 -> Temp: "); Serial.print(temperatureBMP); Serial.print(" °C, Pressão: "); Serial.print(pressureBMP); Serial.println(" hPa");
    Serial.println("-----------------------------");

    // Envia para ThingSpeak
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = String(server) + "?api_key=" + apiKey +
                   "&field1=" + String((int)temperatureDHT) +   // Temperatura DHT11
                   "&field2=" + String((int)humidityDHT) +     // Umidade DHT11
                   "&field3=" + String(pressureBMP) +          // Pressão BMP180
                   "&field4=" + String(temperatureBMP);        // Temperatura BMP180
      http.begin(url);
      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.print("Enviado para ThingSpeak, código HTTP: ");
        Serial.println(httpCode);
      } else {
        Serial.print("Erro ao enviar: ");
        Serial.println(httpCode);
      }
      http.end();
    } else {
      Serial.println("Wi-Fi desconectado!");
    }
  }
}
