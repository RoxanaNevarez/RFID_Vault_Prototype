# **RFID Safe Vault Prototype Project**  
**Problem Addressed**: I wanted to make a safe vault mechanism to protect valuable objects I had in my room and prevent them from being damaged by others while I was not at home.  

**Functionality**: The OLED display prompts the user to scan their tag. The RFID reader will read the tag's UID once held at a close proximity. If the reader detects a tag with an accepted UID, the OLED display will show a message indicating that access was granted, the buzzer will beep at a high pitch once, and the servo motor will move its arm to unlock the door. While the person is storing their valuables in the safe vault, the OLED display will remind the user that the safe vault is unlocked. Once the person is done storing valuables in the safe vault, they will scan the tag again, the servo motor's arm will enter its locked position, the buzzer will beep at a high pitch once, and the OLED display will show that the safe vault is locked. After a few seconds, the OLED display will reset to its default text, prompting the user to scan their tag. If the user scans a tag or card with an unrecognized UID, the OLED display will show that access was denied, the buzzer will beep twice at a lower pitch, and the OLED display will reset to prompt the user to scan a tag again.  

*Note*: A card may alternatively be used, as long as the corresponding UID is applied in the code.

Materials Used:
* RFID-RC522 [reader, card, tag]
* Servo motor
* GME12864 OLED display
* Passive buzzer

<img width="650" height="600" alt="Image" src="https://github.com/user-attachments/assets/12a9ac03-f158-4e79-a443-c5f43a31a672" />
