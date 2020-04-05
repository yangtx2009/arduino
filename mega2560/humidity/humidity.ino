// humidity & temp sensor
#include "DHT.h"
#define DHTPIN A0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// active buzzer
#define BUZZER_PIN A1

//// laser
//#define LASER_PIN 7

// photoresistor
#define PR_PIN A2


// Infrared receiver
#include <IRremote.h>
#define IR_PIN 2
IRrecv irrecv(IR_PIN);
decode_results results;

// OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

void setup() {
  int oldPos = 0;
  
  Serial.begin(9600);
  Serial.println("Initializing");
  dht.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  // pinMode(LASER_PIN, OUTPUT);
  pinMode(PR_PIN, INPUT);

  irrecv.enableIRIn();

  // OLED I2C Display
  // https://startingelectronics.org/tutorials/arduino/modules/OLED-128x64-I2C-display/
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds
  
  display.clearDisplay();
  
  // display.drawPixel(10, 10, SSD1306_WHITE);
  // display.display();
  // delay(2000);

  display.clearDisplay();
  display.drawPixel(0, 0, SSD1306_WHITE);
  display.drawPixel(127, 0, SSD1306_WHITE);
  display.drawPixel(0, 31, SSD1306_WHITE);
  display.drawPixel(127, 31, SSD1306_WHITE);
  display.display();
  delay(2000);

  // testdrawchar();
}

void loop() {
  String buffer = String("");
  // put your main code here, to run repeatedly:
  // digitalWrite(BUZZER_PIN, HIGH);
  // digitalWrite(LASER_PIN, HIGH);
  
  digitalWrite(BUZZER_PIN, LOW);
  // digitalWrite(LASER_PIN, LOW);

  // photoresisitor
  int lightValue = analogRead(PR_PIN);
  if (lightValue < 500){
    Serial.print("Light On:");
    buffer = buffer + String("Light On:");
  }
  else{
    Serial.print("Light Off:");
    buffer = buffer + String("Light Off:");
  }
  Serial.println(lightValue);
  buffer = buffer + String(lightValue) + String("\n");

  // IR receiver
  if (irrecv.decode(&results)){ 
    Serial.print("IR receiver: ");
    Serial.println(results.value, HEX);
    irrecv.resume();
  } 
  
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();
  Serial.print("Humidity:");
  Serial.print(humidity);
  Serial.println("%");
  buffer = buffer + String("Humidity:")+String(humidity)+String("%\n");
  Serial.print("Temperature:");
  Serial.print(temp);
  Serial.println("°C");
  buffer = buffer + String("Temperature:")+String(temp)+String("C\n");

  // display on OLED
  showstring(buffer);
  
  Serial.println("\n");
  delay(500);
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void showstring(String str) {
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  for(int16_t i=0; i<str.length(); i++) {
    display.write(str.charAt(i));
  }

  display.display();
  delay(500);
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}
