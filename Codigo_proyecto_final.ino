#include <WiFi.h>                       // Incluye la biblioteca WiFi para conectar el ESP32 a una red inalámbrica
#include <ThingSpeak.h>                 // Incluye la biblioteca ThingSpeak para enviar datos a ThingSpeak
#include <SPI.h>                        // Incluye la biblioteca para la comunicación SPI (Serial Peripheral Interface)
#include "max6675.h"                    // Incluye la biblioteca para el sensor de temperatura MAX6675

// Credenciales WiFi
const char* ssid = "iPhoneDV";          // Define el SSID (nombre) de la red WiFi a la que se conectará
const char* password = "David12345";    // Define la contraseña de la red WiFi

// Credenciales de ThingSpeak
unsigned long myChannelNumber = 2368512;// Define el número de canal de ThingSpeak para enviar datos
const char * myWriteAPIKey = "BOJZOWKDXML541IN"; // Define la clave de API de escritura para ThingSpeak

// Configuración MAX6675
int thermoSCLK = 19;                    // Define el pin de reloj para los sensores MAX6675
int thermoCS1 = 18;                     // Define el pin de selección de chip para la primera termocupla
int thermoCS2 = 5;                      // Define el pin de selección de chip para la segunda termocupla
int thermoMISO = 23;                    // Define el pin de salida de datos maestro para los sensores MAX6675

MAX6675 termocupla1(thermoSCLK, thermoCS1, thermoMISO); // Inicializa la primera termocupla MAX6675
MAX6675 termocupla2(thermoSCLK, thermoCS2, thermoMISO); // Inicializa la segunda termocupla MAX6675

// Configuración del sensor de lluvia
const int pinSensorLluvia = 34;         // Define el pin al que está conectado el sensor de lluvia

WiFiClient cliente;                     // Crea un objeto cliente para la conexión WiFi

void setup() {
  Serial.begin(9600);                   // Inicia la comunicación serial a 9600 baudios

  // Intento de conexión a WiFi
  Serial.println("Conectando a WiFi..."); // Imprime un mensaje en la consola serial
  WiFi.begin(ssid, password);            // Inicia la conexión WiFi con las credenciales definidas

  // Esperar hasta que se conecte a WiFi
  while (WiFi.status() != WL_CONNECTED) { // Mientras la conexión WiFi no esté establecida
    delay(500);                          // Espera 500 milisegundos
    Serial.print(".");                   // Imprime un punto en la consola serial
  }

  Serial.println("\nWiFi Conectado.");   // Una vez conectado, imprime un mensaje en la consola serial
  Serial.print("Dirección IP: ");        // Imprime un mensaje solicitando la dirección IP
  Serial.println(WiFi.localIP());        // Imprime la dirección IP asignada al ESP32

  ThingSpeak.begin(cliente);             // Inicia la conexión con ThingSpeak
  pinMode(pinSensorLluvia, INPUT);       // Configura el pin del sensor de lluvia como entrada
}

void loop() {
  // Leer temperaturas
  float temp1 = termocupla1.readCelsius(); // Lee la temperatura en Celsius de la primera termocupla
  float temp2 = termocupla2.readCelsius(); // Lee la temperatura en Celsius de la segunda termocupla

  // Leer sensor de lluvia
  int estadoLluvia = digitalRead(pinSensorLluvia); // Lee el estado del sensor de lluvia (alto/bajo)

  // Imprimir en Serial (para depuración)
  Serial.print("Temperatura 1: ");       // Imprime un mensaje sobre la primera temperatura
  Serial.println(temp1);                  // Imprime la primera temperatura
  Serial.print("Temperatura 2: ");       // Imprime un mensaje sobre la segunda temperatura
  Serial.println(temp2);                  // Imprime la segunda temperatura
  Serial.print("Estado de Lluvia: ");    // Imprime un mensaje sobre el estado del sensor de lluvia
  Serial.println(estadoLluvia);           // Imprime el estado del sensor de lluvia

  // Enviar a ThingSpeak
  ThingSpeak.setField(1, temp1);         // Asigna la primera temperatura al campo 1 de ThingSpeak
  ThingSpeak.setField(2, temp2);         // Asigna la segunda temperatura al campo 2 de ThingSpeak
  ThingSpeak.setField(3, estadoLluvia);  // Asigna el estado de lluvia al campo 3 de ThingSpeak

  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); // Envía los datos a ThingSpeak

  delay(10000);                          // Espera 10 segundos (10000 milisegundos) antes de repetir el loop
}
