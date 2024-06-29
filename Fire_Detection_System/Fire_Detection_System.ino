#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define DHTPIN 2          // GPIO2 for DHT11 sensor
#define DHTTYPE DHT11     // DHT11 sensor type
#define MQ2PIN A0         // MQ2 sensor analog pin
#define FLAMEPIN 13       // GPIO13 (D7) for Flame sensor
#define BUZZERPIN 14      // GPIO14 for Buzzer

DHT dht(DHTPIN, DHTTYPE);


#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  


#define OLED_SDA 4        // GPIO4 (D2)
#define OLED_SCL 5        // GPIO5 (D1)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// WiFi settings
const char* ssid = "Airtel_9883015426";             // Replace with your network SSID
const char* password = "RSSR@110323";     // Replace with your network password

// ThingSpeak settings
unsigned long myChannelNumber = yourchannelid;  // Replace with your ThingSpeak channel ID
const char* myWriteAPIKey = "yourwriteapikey";       // Replace with your ThingSpeak Write API Key    // Replace with your ThingSpeak Write API Key

WiFiClient client;

// Timer for buzzer
unsigned long buzzerOnTime = 0;
bool buzzerIsOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(MQ2PIN, INPUT);
  pinMode(FLAMEPIN, INPUT);
  pinMode(BUZZERPIN, OUTPUT);

  dht.begin();

  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000); 
  display.clearDisplay();
  display.display();


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  
  ThingSpeak.begin(client);
}

void loop() {
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  
  int gasValue = analogRead(MQ2PIN);

  
  int flameStatus = digitalRead(FLAMEPIN);

  // Display values on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Temp: ");
  display.print(t);
  display.println(" C");

  display.print("Humidity: ");
  display.print(h);
  display.println(" %");

  display.print("Gas: ");
  display.print(gasValue);

  display.setCursor(0, 40);
  if (flameStatus == HIGH) {
    display.println("Fire: OFF");
  } else {
    display.println("Fire: ON");
  }

  display.display();

  // In flame sensor if the value is low then fire is detected if not then it remains high
  if ( gasValue >1000 ||flameStatus == LOW) {
    digitalWrite(BUZZERPIN, HIGH);  // Turn on buzzer
    if (!buzzerIsOn) {
      buzzerIsOn = true;
    }
  } else {
    digitalWrite(BUZZERPIN, LOW);   // Turn off buzzer
    buzzerIsOn = false;
  }

  // Increment buzzer on time if the buzzer is on
  if (buzzerIsOn) {
    buzzerOnTime += 2;  // Increment by 2 seconds
  }

  // Send data to ThingSpeak
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  ThingSpeak.setField(3, gasValue);
  ThingSpeak.setField(4, flameStatus);
  ThingSpeak.setField(5, (float)buzzerOnTime);  

  int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(response == 200){
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(response));
  }

  delay(10000);  // Update every 10 seconds
}
