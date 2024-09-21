#Note: code may have errors that have yet to be corrected

#include <SPI.h> //allows code/data transfer to occur with the RFID reader
#include <RFID.h> //allows reading from or sending code to an RFID reader using the assigned card or tag
#include <Servo.h> //allows arduino Uno to have control over the motor connected

RFID rfid(10,0); //10 represents pin #10 on the Uno R3 (connects to SDA on RFID reader; allows data from code to be sent to the reader)
                 //0 is a spacer (code won't run if it's rfid(10), RST connection was not needed)
unsigned char str[MAX_LEN]; //MAX_LEN = 10 (length of serial numbers output; needed when using rfid.findcard)

String serial[2] = {"2111210217", "1153117013"};  //RFID serial numbers to grant access to (have power to lock/unlock); found after scanning card/tag

Servo lock; //calling Servo variable "lock" representing the motor used
boolean locked = true; //declaring boolean variable "locked" and setting it equal to true (indicating that motor will start in locked position)

void setup() //user-defined setup function (stores all of the initiating conditions required to start the program)
{ //begins user-defined function body
  Serial.begin(9600); //allows strings output to appear in the COM3 (such as "Card scanned", "Place card or tag near reader", etc) **needed to test the scanning process**
                      //9600 is the baud rate (960 characters per second);appears in COM3
  SPI.begin(); //initiates and allows code/data transfer to occur with the RFID reader
  rfid.init(); //initializing use of RFID reader (allows card or tag serial numbers to be recorded)
  lock.attach(3); //the motor used for the lock mechanism is attached to pin #3 on the Uno R3
  lock.write(130); //allows the motor to start from a locked position every time the code is ran (equates a 180 degree angle)
  Serial.println("Place card or tag near reader"); //prints line telling user to place the card or the tag near the reader to scan
                                                   //**only needed to test the card/tag scanning**
} //ends user-defined function body

void loop() //user-defined loop function that will repeat every time a card or tag is scanned
{ //begins user-defined function body
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK) //If statement will run if RFID reader detects a card or tag; if found, it will scan for str (from str[MAX_LEN])
  { //begins if statement
    Serial.println("Card/tag scanned"); //if a card or tag is scanned by the RFID reader, the code will notify that a card/tag has been scanned
                                        //**only needed to test the card/tag scanning**
    String SNum = ""; //declaring string variable SNum (represents serial number; initially blank; will be updated as soon as a card or tag is scanned)
    if (rfid.anticoll(str) == MI_OK) //Prevents several cards/tags from being scanned at the same time; anticoll represents anticollision detection
    { //begins if statement
      Serial.print("The card/tag's serial number is : "); //allows serial number to be next to this output (missing ln compared to the other lines)
      for (int i = 0; i < 4; i++) //Loop will run and display the four sets of numbers from each serial number (serial numbers are divided into 4 sections)
      {                           //**needed for me to find the serial number of the tag or card to give them locking/unlocking access** 
        SNum = SNum + str[i]; //runs until all serial numbers from that card/tag are output together into a string of a large integer
      } //ends for loop
      Serial.println(SNum); //outputs the serial number (whether from the tag itself or the card)
      checkAccess(SNum); //Runs user-defined checkAccess function to verify if the serial number scanned has access to open the safe
    } //ends if-statement
    rfid.selectTag(str); //prevents motor from glitching, prevents several rapid outputs of scanning at once (str from str[MAX_LEN])
  } //ends if statement
  rfid.halt(); //prevents motor from glitching (allows it to rotate 90 degrees once each time card/tag is scanned rather than locking and unlocking rapidly; similar function to rfid.selectTag)
} //ends user-defined function body

void checkAccess (String SNum) //User-defined function that will check if the read serial number has access to opening and unlocking the safe
{ //begins user-defined function
  boolean AccessGranted = false; //declaring boolean variable AccessGranted and setting it equal to false (meaning access has not been granted initially)
  for (int i = 0; i < 4; i++) //For loop will repeat while i is less than or equal to 4 (4 represents the sets of serial numbers that have access to the safe)
  { //begins for loop
    if(serial[i] == "2111210217") //code will run if the serial number is equal to the string typed in 
    { //begins if statement
      AccessGranted = true; //if the card or tag scanned has the same serial number as the string from the condition inside the if statement, then access will be granted
      if (locked == true) //code will run if the locked variable is equal to true (meaning safe is locked)
      { //begins if statement
          Serial.println ("Safe is locked"); //code will print line notifying that safe has been locked
          lock.write(130); //motor mechanism will rotate to lock safe
          locked = false; //locked will condition will be set equal to false (meaning safe will be opened next time card or tag is scanned)
      } //ends if statement
      else if (serial[i] == "1153117013") //code will run if the serial number is equal to the string typed in 
      { //ends else if statement
          Serial.println ("Safe is locked"); //code will print line notifying that safe has been locked
          lock.write(130); //motor mechanism will rotate to lock safe
          locked = false; //locked will condition will be set equal to false (meaning safe will be opened next time card or tag is scanned)
      } //ends else if statement
      else if (locked == false) //code will run if the locked variable is equal to false (meaning safe is opened)
      { //begins else if statement
          Serial.println ("Safe is opened"); //code will print line notifying that safe has been opened
          lock.write(30); //motor mechanism will rotate to open safe (30 equates to a 90 degree angle that the motor arm makes)
          locked = true; //locked condition will be set equal to true (meaning safe will be locked next time card or tag is scanned)
      } //ends else if statement
    } //ends if statement
  } //ends for loop
} //ends user-defined checkAccess function

#To develop a better understanding of RFIDs and programming through the Arduino software, the following sources were used:
#To learn about RFID: https://lastminuteengineers.com/how-rfid-works-rc522-arduino-tutorial/ 
#Learning about RFID libraries and testing different ways RFID reader can be used: https://www.arduino.cc/reference/en/libraries/mfrc522/ , https://github.com/addicore/AddicoreRFID 
#To understand Arduino language references: https://www.arduino.cc/reference/en/ 
#To learn about SPI library: https://www.arduino.cc/reference/en/language/functions/communication/spi/ 
#To learn about Servo library: https://www.arduino.cc/reference/en/libraries/servo/ 

