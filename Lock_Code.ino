// Project Updated: 08.06.2025
// Updates: included OLED display, buzzer, and updated libraries 
// Assistance: ChatGPT
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h> // needed for OLED display
#include <Servo.h>
#include <Adafruit_GFX.h> // needed for OLED display
#include <Adafruit_SSD1306.h> // needed for OLED display

// ===== OLED SETUP =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // reset pin not used
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== RFID SETUP =====
#define SS_PIN 10 // D10
#define RST_PIN 9 // D9
MFRC522 rfid(SS_PIN, RST_PIN);

// ===== SERVO SETUP =====
#define SERVO_PIN 6 // D6
Servo myServo;
bool locked = true;  // servo starts in locked position (servo arm along +x-axis)

// ==== BUZZER SETUP ====
#define BUZZER_PIN 4 // D4

// ===== AUTHORIZED UID =====
byte authorizedUID[] = { 0x73, 0x1F, 0xAA, 0xD }; // tag UID will work
                                                  // card UID: {0XD3, 0XC, 0X66, 0X11} will not work - testing purposes

void setup() 
{
  Serial.begin(9600); // baud rate
  Wire.begin();
  SPI.begin();
  rfid.PCD_Init();
  pinMode(BUZZER_PIN, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(30);  // locked position (servo arm along +x-axis)

  // ==== Initialize OLED ====
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  {
    Serial.println(F("OLED init failed"));
    while (1);
  }
  displayMessage("Scan your tag..."); // displayed on OLED
}

 // ==== Buzzer Conditions ====
 void beepSuccess() // when access is granted and when servo motor arm enters locked position
 {
  tone(BUZZER_PIN, 1000);  // high pitch (1000 Hz)
  delay(150);
  noTone(BUZZER_PIN);
}

void beepFailure() // when access is denied
{
  for (int i = 0; i < 2; i++) 
  {
    tone(BUZZER_PIN, 200);  // low pitch (200 Hz)
    delay(100);
    noTone(BUZZER_PIN);
    delay(100);             // pause between beeps
  }
}

void loop() 
{
  // ==== Wait for a New Card ====
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) 
  {
    return;
  }
  // ==== Serial Monitor Display ====
  Serial.print("UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) 
  {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (isAuthorized(rfid.uid.uidByte)) 
  {
    if (locked) 
    {
        Serial.println("Access Granted! Safe Vault Unlocked"); // displayed on serial monitor 
        displayMessage("Access    Granted"); // displayed on OLED 
        beepSuccess();
        myServo.write(128); // unlocked position (servo arm along +y-axis)
        delay(1000);
        displayMessage("Unlocked");
        locked = false;
    } 
    else 
    {
        Serial.println("Safe Vault Locked!"); // displayed on serial monitor 
        displayMessage("Locked"); // displayed on OLED 
        beepSuccess();
        myServo.write(30);   // locked position (servo arm along +x-axis)
        locked = true;
        delay(2000); // displays "Locked" for 2 seconds
        displayMessage("Scan your tag..."); // displayed message gets updated
    }
    
  } 
  else 
  {
    Serial.println("Access Denied! Scan Valid Tag"); // displayed on serial monitor 
    displayMessage("Access    Denied"); // displayed on OLED 
    beepFailure();
    delay(2000); // displays "Access Denied" for 2 seconds
    displayMessage("Scan your tag...");
  }

  // ==== Halt and Stop Encryption ====
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

bool isAuthorized(byte *uid) 
{
  for (byte i = 0; i < 4; i++) 
  {
    if (uid[i] != authorizedUID[i]) return false;
  }
  return true;
}

void displayMessage(const char *msg) 
{
  display.clearDisplay();
  display.setTextSize(2); // large text for OLED display
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println(msg);
  display.display();
}


