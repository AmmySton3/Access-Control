#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
String tagId = "None";
byte nuidPICC[4];

#define buzzer 12
const int relayPin = 10;
const int push_btn_pin = 11;
int pushBTN_state;

void setup() {
  Serial.begin(9600);
  Serial.println("System initialized");
  pinMode(buzzer, OUTPUT);
  pinMode(push_btn_pin, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Ensure the relay is initially off (door locked)
  nfc.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  powerOn();
  Serial.println("Setup completed successfully");
}

void loop() {
      lcd.setCursor(1, 0);
      lcd.print("BURHANI-INFOSYS");
      lcd.setCursor(6, 1);
      lcd.print("LTD");
    
      // Read the button state
      pushBTN_state = digitalRead(push_btn_pin);

    if (pushBTN_state == LOW) { // LOW means the button is pressed
          openDoor();  // Call the refactored function
         
         }else{ 
                 
          if (nfc.tagPresent()) {
    
            NfcTag tag = nfc.read();
            tag.print();
            tagId = tag.getUidString();
            tagId.replace(" ", "");
            Serial.print("Encoded Tag ID: ");
            Serial.println(tagId);
        
            // Decode the encoded tag ID
            String decodedTagId = decodeTagData(tagId);
            Serial.print("Decoded Tag ID: ");
            Serial.println(decodedTagId);
            checkValidCard(decodedTagId); 
      }
         }
}

//Check Card if is valid
void checkValidCard(String decodedTagId) {
    if(decodedTagId == "gcujeafb" || decodedTagId == "gcgvgbfb" || 
       decodedTagId == "jdwwwuag" || decodedTagId == "gdhauwvd" || 
       decodedTagId == "ujjrcgea" || decodedTagId == "ihjfutuh" || 
       decodedTagId == "tsgjfwfr" || decodedTagId == "ivcugtct" ||
       decodedTagId == "cegrvbaa" || decodedTagId == "adjfvdbu" ||
       decodedTagId == "vrbavfia" || decodedTagId == "uduaubbu" ||
       decodedTagId == "udbjjgbd" || decodedTagId == "sicjhwbc" ) {
        
        openDoor();  // Call the refactored function
        
    } else {
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("INVALID CARD");
        Serial.println("Invalid Card");
        delay(5000);
        tone(buzzer, 2000, 100);
        lcd.clear();
    }
}

//Function to open the door
void openDoor() {
    digitalWrite(relayPin, LOW); // Unlock door
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("WELCOME");
    lcd.setCursor(1, 1); // Ensure it's the second row
    lcd.print("DOOR IS OPEN");
    delay(5000); // Keep the door unlocked for 5 seconds
    tone(buzzer, 2000, 200);
    digitalWrite(relayPin, HIGH); // Lock door
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("GOODBYE");
    lcd.setCursor(1, 1);
    lcd.print("DOOR IS LOCKED");
    delay(1000);
    lcd.clear();
}

// Function to decode encrypted tag data
String decodeTagData(String encoded) {
   String decoded = "";
   for (int i = 0; i < encoded.length(); i++) {
      char c = encoded.charAt(i);
      switch (c) {
         case '0': decoded += 'a'; break;
         case '1': decoded += 'b'; break;
         case '2': decoded += 'c'; break;
         case '3': decoded += 'd'; break;
         case '4': decoded += 'e'; break;
         case '5': decoded += 'f'; break;
         case '6': decoded += 'g'; break;
         case '7': decoded += 'h'; break;
         case '8': decoded += 'i'; break;
         case '9': decoded += 'j'; break;
         case 'A': decoded += 'r'; break;
         case 'B': decoded += 's'; break;
         case 'C': decoded += 't'; break;
         case 'D': decoded += 'u'; break;
         case 'E': decoded += 'v'; break;
         case 'F': decoded += 'w'; break;
         default: decoded += c; break; // keep unknown characters as is
      }
   }
   return decoded;
}

/* Power On Device */
void powerOn() {
  lcd.setCursor(0, 0);
  lcd.print("Please Wait");
  delay(500);
  for (int i = 0; i < 6; i++) {
    lcd.setCursor(11 + i, 0);
    lcd.print(".");
    delay(1000);
  }
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("WELCOME");
  delay(1000);
  lcd.clear();
}
