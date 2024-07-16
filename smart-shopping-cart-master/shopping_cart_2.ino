// #include < Wire .h> we are removing this because it is already added in liquid crystal library
//#include <LiquidCrystal_I2C.h>
#include <Vector.h>
#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
#include <SPI.h> //library responsible for communicating of SPI bus
#define SS_PIN    21
#define RST_PIN   22
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16
#define greenPin     12
#define redPin       32
//used in authentication
MFRC522::MIFARE_Key key;
//authentication return status code
MFRC522::StatusCode status;
// Defined pins to module RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 

/*
FUNCTION DECLARATION
*/

void businessLogic(String content);
void outputToLcd();
void acknowledgeRead();
void outputAccessToLcd();
void addingNewProduct(String product_name, byte product_price);
void removeProduct(String product_name, byte product_price);
void assignValues(String product, byte price);
void lowFunds();
void startUpMsg();
void billPaid();
void do_default_check();
void resetParams();
void welcomeMsg();
 
//LiquidCrystal_I2C lcd (0x27, 16,2);

/*
GLOBAL VARIABLES
*/

String product_name = "";
byte product_price, sum = 0;
bool tag73909111Read = false;
bool tagA37FFD16Read = false;
bool isLowOnFunds = false;
int amount = 40;
int balance = 0;
int buzzer = 8;
Vector<byte> product_prices; 



void  setup () {
  Serial.begin(115200);

//  lcd.init(); 

//  welcomeMsg();

  while(!Serial);
  // Init MFRC522
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.print("MFRC522 software version = ");
  Serial.println(mfrc522.PCD_ReadRegister(mfrc522.VersionReg),HEX);// Init MFRC522
  delay(4);                                             // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();      // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  Serial.println();
}
 
void  loop () {
   // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
           // Look for new cards
   if (! mfrc522.PICC_IsNewCardPresent()) 
   {
      return;
   }
 
   // Select one of the cards
   if (! mfrc522.PICC_ReadCardSerial()) 
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
   businessLogic(content);
}


/*
FUNCTION DEFINITIONS
*/

void welcomeMsg(){
//  lcd.backlight ();
//  lcd.print(" welcome ");
//  // print the Message on the LCD.
//  lcd.setCursor(0,1); 
//  lcd.print ( "Shopping Cart" );  
}

void acknowledgeRead(){
    tone(buzzer, 1000);
    delay(300);
    noTone(buzzer);
}

void outputAccessToLcd(){
  Serial.println("Authorized access");
//  lcd.clear();                 // clear display
//  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
//  lcd.print("   Authorized");        // print message at (0, 0)
//  lcd.setCursor(2, 1);         // move cursor to   (2, 1)
//  lcd.print("   access"); // print message at (2, 1)
  Serial.println();
}

void billPaid(){
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("  Bill Paid ");
//  delay(2000);
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print(" Thankyou for ");
//  lcd.setCursor(0, 1);
//  lcd.print("  shopping");
}

void resetParams(){
  sum = 0;
  amount = balance;
  tag73909111Read = false;
  tagA37FFD16Read = false;
}

void addingNewProduct(String product_name,byte product_price){
  product_prices.PushBack(product_price);
  Serial.println(product_name + " added to your cart with price " + product_price);  
//  lcd.clear();                 // clear display
//  lcd.setCursor(0, 0);
//  lcd.print(product_name);
//  delay(600);
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("  Added to cart");
}

void removeProduct(String product_name, byte product_price){
for(byte i = 0; i < product_prices.Size();i++){
   if (product_prices[i] == product_price){
     product_prices.Erase(product_prices[i]);
   }
}
  sum  = sum - product_price;
  Serial.println(product_name + " has been removed from your cart " + product_price + " has deducted from total amount payable ");
//  lcd.clear();                 // clear display
//  lcd.setCursor(0, 0);
//  lcd.print(product_name + " removed");
//  lcd.setCursor(2,1);
//  lcd.print(product_price + " deducted from total amount");
}

void assignValues(String product,byte price){
   product_name = product;
   product_price = price;
}

void lowFunds(){
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("  low on funds ");
  isLowOnFunds = true;
  delay(2000);
}

void startUpMsg(){
//  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
//  lcd.print(" Smart shopping");        // print message at (0, 0)
//  lcd.setCursor(2, 1);         // move cursor to   (2, 1)
//  lcd.print("    Cart"); 
}

void businessLogic(String content){
  if(content.substring(1) == "73 90 91 11") //change here the UID of the card/cards that you want to give access
  {
//          acknowledgeRead();
          assignValues("Malt", 20);
          tag73909111Read = !tag73909111Read;
          outputAccessToLcd();
          delay(3000);
          if(tag73909111Read){
            addingNewProduct(product_name, product_price);
           }
           else{
             removeProduct(product_name, product_price);
           }
           
   }
   if ( content.substring(1) == "A3 7F FD 16" ) //change here the UID of the card/cards that you want to give access
   {
//          acknowledgeRead();
     assignValues("bread", 10);
     tagA37FFD16Read = !tagA37FFD16Read;
      outputAccessToLcd();
      delay(1000);
      if(tagA37FFD16Read){
        addingNewProduct(product_name, product_price);
      }
       else{
         removeProduct("bread", 10);
       }
   }
//          else{
//            Serial.println("Access Denied");
//            delay(1000);
//          }

   if (content.substring(1) == "35 3C 16 AD"){
//          acknowledgeRead();
          
      balance = amount - sum;
      if(sum != 0){
        Serial.println(amount);
            if ((balance) >= 0 ){ 
              billPaid();
              delay(2000);
              delay(2000);
              resetParams();
            }
            else{
              lowFunds();
            }
       } 
        else {
//          lcd.clear();
//          lcd.setCursor(0, 0);
//          lcd.print("   Cart empty");
            delay(3000);
            startUpMsg();
        }     
    }
    
    do_default_check();       
}

void do_default_check() {
  for(byte i =0; i < product_prices.Size(); i++){
    Serial.println(product_prices[i]);
    sum = sum + product_prices[i];
    product_prices.Erase(i);
  }
  delay(3000);
  Serial.print("Total Amount payable: ");
  if (sum > 0) {
//    lcd.clear();                 // clear display
//    lcd.setCursor(0, 0);
//    lcd.print(" Amount payable");
//    lcd.setCursor(2,1);
//    lcd.print(sum);
    if(isLowOnFunds){
      delay(3000);
//      lcd.clear();
      startUpMsg();
      resetParams();
    }
  }Serial.println(sum);
}
