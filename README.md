# **RFID Safe Vault Prototype Project**  
**Problem Addressed**: I wanted to make a safe vault mechanism to protect valuable objects I had in my room and prevent them from being damaged by others while I was not at home.  

**Functionality**: The OLED display prompts the user to scan their tag. The RFID reader will read the tag's UID once held at a close proximity. If the reader detects a tag with an accepted UID, the OLED display will show a message indicating that access was granted, and the servo motor will move its arm to unlock the door. Once the person is done storing valuables in the safe vault, they will scan the tag again, the servo motor's arm will enter its locked position, and the OLED display will show that the safe vault is locked.  

*Note*: A card may alternatively be used, as long as the accepted UID is updated in the code.

Materials Used:
* RFID-RC522 [reader, card, tag]
* Servo motor
* GME12864 OLED Display

<img width="600" height="550" alt="Image" src="https://github.com/user-attachments/assets/db3af0da-2538-4c0e-9071-1e5f05a03246" />
