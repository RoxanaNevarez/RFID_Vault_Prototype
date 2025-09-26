// Project Updated: 09.04.2025
// Updates: added an additional motor and a "lock mode" to increase security measures 
// Guidance: ChatGPT
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== OLED SETUP =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // reset pin not used
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== RFID SETUP =====
#define SDA_PIN 10 // D10
#define RST_PIN 9 // D9
MFRC522 rfid(SDA_PIN, RST_PIN);

// ===== SERVO SETUP =====
bool locked = true; // servo motors start in locked position (arm attachments are horizontal)
#define SERVO_PIN 6 // D6 - upper motor
Servo upperServo;
#define SERVO2_PIN 2 // D2 - bottom motor
Servo bottomServo;

// ==== BUZZER SETUP ====
#define BUZZER_PIN 4 // D4
int failedAttempts = 0;

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

  upperServo.attach(SERVO_PIN);
  upperServo.write(30); // locked position
  bottomServo.attach(SERVO2_PIN);
  bottomServo.write(100); // locked position

  // displayMessage - shown on OLED 
  // Serial.println() - shown on Serial Monitor

  // ==== Initialize OLED ====
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  {
    Serial.println(F("OLED init failed"));
    while (1);
  }
  displayMessage("Scan Valid Tag/Card"); 
}

 // ==== Buzzer Conditions ====
 void beepSuccess() // when access is granted 
 {
  tone(BUZZER_PIN, 1000); // high pitch (1000 Hz)
  delay(150);
  noTone(BUZZER_PIN);
}

void beepFailure() // when access is denied
{
  for (int i = 0; i < 2; i++) 
  {
    tone(BUZZER_PIN, 200); // low pitch (200 Hz)
    delay(100);
    noTone(BUZZER_PIN);
    delay(100); // pause between beeps
  }
}

void beepLockOut() // when access is denied more than 3 consecutive times
{
    displayMessage("LOCK OUT  WARNING!");
    Serial.print("Locked Out!");
    tone(BUZZER_PIN, 200); // low pitch (200 Hz)
    delay(5000); // hold beep for 5 seconds
    noTone(BUZZER_PIN); // constant beep stops
}
void loop() 
{
  // ==== Wait for a New Tag/Card ====
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
        Serial.println("Access Granted! Safe Vault Unlocked"); 
        displayMessage("Access    Granted");
        beepSuccess(); // beeping pattern when motors enter unlocked position
        upperServo.write(128); // unlocked position (upper servo arm moves CCW)
        bottomServo.write(0); // unlocked position (bottom servo arm moves CW)
        delay(1000); // maintains previous strings for 1 second
        displayMessage("Safe Vault Unlocked"); // reminds user that the safe vault is unlocked
        locked = false;
        failedAttempts = 0; // count of failed unlocking attempts resets to 0 after a successful unlocking attempt
    } 
    else 
    {
        Serial.println("Safe Vault Locked!");  
        displayMessage("Safe Vault Locked"); 
        beepSuccess(); // beeping pattern when motors enter locked position
        upperServo.write(30); // locked position (upper servo arm moves CW)
        bottomServo.write(100); // locked position (bottom servo arm moves CCW)
        locked = true;
        delay(2000); // displays previous strings for 2 seconds
        displayMessage("Scan Valid Tag/Card"); 
        Serial.println("Scan Valid Tag/Card");
    }
    
  } 
  else 
  {
    beepFailure(); // beeping pattern when invalid card/tag is scanned
    failedAttempts++; // failed unlocking attempts are recorded and increase by 1 
    Serial.println("Access Denied!"); 
    // char (nameBuffer)[20] - 20 character max buffer arrays that will store formatted strings (joining text and numbers)
    // sprintf - formats and stores string into buffer arrays
    char serialBuffer[20];  
    sprintf(serialBuffer, "Fails: %d", failedAttempts); 
    Serial.println(serialBuffer);
    char displayBuffer[20];  
    sprintf(displayBuffer, "Access    Denied    Fails: %d", failedAttempts); 
    displayMessage(displayBuffer); 
    if (failedAttempts >= 3) // safe vault enters "lock mode" once it encounters 3+ consecutive failed attempts
   {
    beepLockOut(); // beeping pattern when safe vault is in "lock mode"
    Serial.println(" ");
   }
    delay(1500);
    Serial.println("Scan Valid Tag/Card");
    displayMessage("Scan Valid Tag/Card");
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
  display.setCursor(0, 16); // adjusted to move text higher on the OLED display
  display.println(msg);
  display.display();
}
