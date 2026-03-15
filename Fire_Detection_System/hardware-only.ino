#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

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

// Timer for buzzer
unsigned long buzzerOnTime = 0;
bool buzzerIsOn = false;

void setup() {
  Serial.begin(115200);

  pinMode(MQ2PIN, INPUT);
  pinMode(FLAMEPIN, INPUT);
  pinMode(BUZZERPIN, OUTPUT);

  dht.begin();

  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  int gasValue = analogRead(MQ2PIN);
  int flameStatus = digitalRead(FLAMEPIN);

  // OLED Display
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

  // Buzzer Logic
  if (gasValue > 1000 || flameStatus == LOW) {
    digitalWrite(BUZZERPIN, HIGH);

    if (!buzzerIsOn) {
      buzzerIsOn = true;
    }
  } 
  else {
    digitalWrite(BUZZERPIN, LOW);
    buzzerIsOn = false;
  }

  // Buzzer timer
  if (buzzerIsOn) {
    buzzerOnTime += 2;
  }

  delay(2000);
}
