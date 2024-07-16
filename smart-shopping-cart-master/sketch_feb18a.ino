#include <SPI.h>
#include <MFRC522.h>
#include <Vector.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
LiquidCrystal_I2C lcd(0x27, 16, 2);

String product_name = "";
byte product_price, sum = 0;
bool tag73909111Read = false;
bool tagA37FFD16Read = false;
bool isLowOnFunds = false;
int amount = 40;
int balance = 0;
int buzzer = 8;

Vector<byte> product_prices; 

void setup() {

  //initial buzzer as output
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);   // Initiate a serial communication
  lcd.init(); //initialize the lcd
  lcd.backlight();

  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print(" Smart shopping");        // print message at (0, 0)
  lcd.setCursor(2, 1);         // move cursor to   (2, 1)
  lcd.print("    Cart");
while (!Serial);                       // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
            SPI.begin();                             // Init SPI bus
            mfrc522.PCD_Init();
            Serial.print("MFRC522 software version = ");
            Serial.println(mfrc522.PCD_ReadRegister(mfrc522.VersionReg),HEX);// Init MFRC522
            delay(4);                                             // Optional delay. Some board do need more time after init to be ready, see Readme
            mfrc522.PCD_DumpVersionToSerial();      // Show details of PCD - MFRC522 Card Reader details
            Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}
void loop() {
           // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
           // Look for new cards
           if ( ! mfrc522.PICC_IsNewCardPresent()) 
           {
             return;
           }
         
           // Select one of the cards
           if ( ! mfrc522.PICC_ReadCardSerial()) 
           {
             return;
           }
           // Dump debug info about the card; PICC_HaltA() is automatically called
           Serial.print("UID tag :");
         String content= "";
         byte letter;
         for (byte i = 0; i < mfrc522.uid.size; i++) 
         {
            Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
            Serial.print(mfrc522.uid.uidByte[i], HEX);
            content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
            content.concat(String(mfrc522.uid.uidByte[i], HEX));
       
           //  Serial.println(" access");
         }
         Serial.println();
         Serial.print("Message : ");
         content.toUpperCase();
         if (content.substring(1) == "73 90 91 11") //change here the UID of the card/cards that you want to give access
         {
          acknowledgeRead();
          product_name = "Malt";
          product_price = 20;
          tag73909111Read = !tag73909111Read;
          Serial.println("Authorized access");
          lcd.clear();                 // clear display
          lcd.setCursor(0, 0);         // move cursor to   (0, 0)
          lcd.print("   Authorized");        // print message at (0, 0)
          lcd.setCursor(2, 1);         // move cursor to   (2, 1)
          lcd.print("   access"); // print message at (2, 1)
          Serial.println();
          delay(3000);
          if( tag73909111Read){
            product_prices.PushBack(product_price);
            Serial.println(product_name + " added to your cart with price " + product_price);  
            lcd.clear();                 // clear display
            lcd.setCursor(0, 0);
            lcd.print(product_name);
            delay(600);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("  Added to cart");
         
           }
           else{
             for(byte i = 0; i < product_prices.Size();i++){
               if (product_prices[i] == product_price){
                 product_prices.Erase(product_prices[i]);
               }
             }
             sum  = sum - product_price;
             Serial.println(product_name + " has been removed from your cart " + product_price + " has deducted from total amount payable ");
              lcd.clear();                 // clear display
              lcd.setCursor(0, 0);
              lcd.print(product_name + " removed from cart");
              lcd.setCursor(2,1);
              lcd.print(product_price + "deducted from total amount");
           }
           
         }
          if (content.substring(1) == "A3 7F FD 16") //change here the UID of the card/cards that you want to give access
         {
          acknowledgeRead();
          product_name = "Bread";
          product_price = 10;
          tagA37FFD16Read = !tagA37FFD16Read;
          Serial.println("Authorized access");
          lcd.clear();                 // clear display
          lcd.setCursor(0, 0);         // move cursor to   (0, 0)
          lcd.print("   Authorized");        // print message at (0, 0)
          lcd.setCursor(2, 1);         // move cursor to   (2, 1)
          lcd.print("   access"); // print message at (2, 1
          Serial.println();
          delay(1000);
          if(tagA37FFD16Read){
            product_prices.PushBack(product_price);
            Serial.println(product_name + " added to your cart with price " + product_price);
            lcd.clear();                 // clear display
            lcd.setCursor(0, 0);
            lcd.print(product_name + "  added");
            lcd.setCursor(2,1);
            lcd.print("with price " + product_price); 
          
          }
           else{
             for(byte i = 0; i < product_prices.Size();i++){
               if (product_prices[i] == product_price){
                 product_prices.Erase(product_prices[i]);
               }
             }
             sum  = sum - product_price;
             Serial.println(product_name + " has been removed from your cart " + product_price + " has deducted from total amount payable ");
             lcd.clear();                 // clear display
             lcd.setCursor(0, 0);
             lcd.print(product_name + " removed");
             lcd.setCursor(2,1);
             lcd.print(product_price + " deducted from total amount");
             
           }
         }
//          else{
//            Serial.println("Access Denied");
//            delay(1000);
//          }

         if (content.substring(1) == "35 3C 16 AD") //change here the UID of the card/cards that you want to give access
         {
          acknowledgeRead();
                
          balance = amount - sum;
//               for(int i=0;i<6;i++)
//               {
//               digitalWrite(buz, HIGH);
//               delay(70);
//               digitalWrite(buz, LOW);
//               delay(70);
//               
//               }
                
               // restore the prices collection to empty
            if(sum != 0){
              Serial.println(amount);
                  if ((balance) >= 0 ){ 
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("  Bill Paid ");
                  delay(2000);
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print(" Thankyou for ");
                  lcd.setCursor(0, 1);
                  lcd.print("  shopping");
                  delay(2000);
                  delay(2000);

                  sum = 0;
                  amount = balance;
                  tag73909111Read = false;
                  tagA37FFD16Read = false;
                }
                else{
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("  low on funds ");
                  isLowOnFunds = true;
                  delay(2000);
                }
              } 
              else {
                lcd.clear();                 // clear display
                lcd.setCursor(0, 0);
                lcd.print("   Cart empty");
                delay(3000);
                lcd.setCursor(0, 0);         // move cursor to   (0, 0)
                lcd.print(" Smart shopping");        // print message at (0, 0)
                lcd.setCursor(2, 1);         // move cursor to   (2, 1)
                lcd.print("    Cart");
              }
          //                //Serial.println("Bill Paid thankyou for shopping ");
          //                 lcd.setCursor(0, 0);
          //                 lcd.print(" Smart Shopping ");
          //                 lcd.setCursor(0, 1);
          //                 lcd.print("   trolly ");
          //                 delay(200);      
                  }
                          
                          
                
          //                 lcd.clear();
          //                 lcd.setCursor(0, 0);
          //                 lcd.print("Bill amount Rs: ");
          //                 lcd.setCursor(0, 1);
          //                 lcd.print(s);
          //                 Serial.println("total amount is: " +findTotalAmountPayable(product_prices));
          //                 delay(100);

                for(byte i =0; i < product_prices.Size(); i++){
                  Serial.println(product_prices[i]);
                  sum = sum + product_prices[i];
                  product_prices.Erase(i);
                }
                delay(3000);
                if (sum > 0) {
                  Serial.print("Total Amount payable: ");
                  Serial.println(sum);
                  lcd.clear();                 // clear display
                  lcd.setCursor(0, 0);
                  lcd.print(" Amount payable");
                  lcd.setCursor(2,1);
                  lcd.print(sum);
                  if(isLowOnFunds){
                    delay(3000);
                    lcd.clear();
                    lcd.setCursor(0, 0);         // move cursor to   (0, 0)
                    lcd.print(" Smart shopping");        // print message at (0, 0)
                    lcd.setCursor(2, 1);         // move cursor to   (2, 1)
                    lcd.print("    Cart"); 
                    sum = 0;
                    amount = balance;
                    tag73909111Read = false;
                    tagA37FFD16Read = false;
                  }
                }
                else{
                  lcd.clear();                 // clear display
                  lcd.setCursor(0, 0);
                  lcd.print("   Cart empty");
                  delay(3000);
                  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
                  lcd.print(" Smart shopping");        // print message at (0, 0)
                  lcd.setCursor(2, 1);         // move cursor to   (2, 1)
                  lcd.print("    Cart");
                }
                  
}

void acknowledgeRead(){
    tone(buzzer, 1000);
    delay(300);
    noTone(buzzer);
}