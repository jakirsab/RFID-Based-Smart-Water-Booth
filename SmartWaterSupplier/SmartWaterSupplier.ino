#include <Wire.h>                      
#include <LiquidCrystal_I2C.h>       

LiquidCrystal_I2C lcd(0x3F,20,4);     

int captureSignalAccessed = 3;
int captureSignalDenied = 2;
int CSS;
int CSA;
int CSD;
int ck=0;


int relayPin = 11;
int trigTW = 5;
int trigCGW = 6;
int trigCG = 7;
int echoTW = 8;
int echoCGW = 9;
int echoCG = 10;
long durTW;
long durCGW;
long durCG;
int totalWaterLevel;
int checkGlassWater;
int checkGlass;

int countG=0;
void setup()
{

  lcd.begin();  
  Serial.begin(9600);                  

  pinMode(trigTW,OUTPUT);
  pinMode(trigCGW,OUTPUT);
  pinMode(trigCG,OUTPUT);
  pinMode(echoTW,INPUT);
  pinMode(echoCGW,INPUT);
  pinMode(echoCG,INPUT);
  pinMode(relayPin,OUTPUT);


  pinMode(captureSignalAccessed,INPUT);
  pinMode(captureSignalDenied,INPUT);
  
  // Print a message to the LCD.
  lcd.backlight();                

  lcd.setCursor(0,0);           
  //lcd.print("Smart Water Suppler.");
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0


void sessionEnd(){

  lcd.setCursor(0,1);  
  lcd.print("                    ");
  lcd.setCursor(2,1);          
  lcd.print("Payment Completed.");
  
  lcd.setCursor(0,2);             
  lcd.print("                    ");
  lcd.setCursor(1,2);
  lcd.print("Total Price: ");
  lcd.print(countG*2);
  lcd.print(" TK.");
  
  lcd.setCursor(0,3); 
  lcd.print("                    ");
  lcd.setCursor(0,3);            
  lcd.print("Thank You for use me");

  countG =0;
  delay(3000);
  return;
}
void startFilling(){
  //resetFunc();  //call reset
  CSA = digitalRead(captureSignalAccessed);
  CSD = digitalRead(captureSignalDenied);
  delay(1000);

  if(CSA == 0){
    digitalWrite(relayPin,LOW);
    countG++;
    ck = 1;
    sessionEnd();
    return;
  }

  digitalWrite(trigCG,LOW);
  delayMicroseconds(2);
  digitalWrite(trigCG,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigCG,LOW);
  durCG = pulseIn(echoCG,HIGH);
  checkGlass = (durCG*0.034)/2;

  digitalWrite(trigCGW,LOW);
  delayMicroseconds(2);
  digitalWrite(trigCGW,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigCGW,LOW);
  durCGW = pulseIn(echoCGW,HIGH);
  checkGlassWater = (durCGW*0.034)/2;
  Serial.println(checkGlassWater);
  checkGlassWater = map(checkGlassWater,4,20,0,100);
  checkGlassWater = 100-checkGlassWater;

  lcd.setCursor(0,3); 
  lcd.print("                    ");
  lcd.setCursor(0,3);            
  lcd.print("Current Glass: ");
  if(checkGlassWater<0){
    checkGlassWater = 0;
  }
  lcd.print(checkGlassWater);
  lcd.print("%");
  
  
  
  if((checkGlassWater>80 && checkGlassWater<100) || checkGlass>5){
    digitalWrite(relayPin,LOW);
    countG++;

//    lcd.setCursor(0,3);             
//    lcd.print("                    ");
    
    return;
  }
  else{
    digitalWrite(relayPin,HIGH);
    delay(500);
  }
  return startFilling();
//  lcd.setCursor(0,3);             
//  lcd.print("                    ");
}



void boothStarting(){
  CSA = digitalRead(captureSignalAccessed);
  CSD = digitalRead(captureSignalDenied);
  delay(2000);

  ck =0;
  digitalWrite(trigTW,LOW);
  delayMicroseconds(2);
  digitalWrite(trigTW,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigTW,LOW);
  durTW = pulseIn(echoTW,HIGH);
  totalWaterLevel = (durTW*0.034)/2;
  Serial.println(totalWaterLevel);
  totalWaterLevel = map(totalWaterLevel,5,25,0,100);

  totalWaterLevel = 100 - totalWaterLevel;
  lcd.setCursor(0,1); 
  lcd.print("                    "); 
  lcd.setCursor(0,1);           
  lcd.print("Available Water:");
  
  lcd.print(totalWaterLevel);
  lcd.print("%");

  lcd.setCursor(0,2);  
  lcd.print("                    "); 
  lcd.setCursor(0,2);         
  lcd.print("Suplied: ");
  lcd.print(countG);
  lcd.print(" Glasses.");

  checkGlassWater =0;
  lcd.setCursor(0,3);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("Current Glass: ");
  lcd.print(checkGlassWater);
  lcd.print("%");

  digitalWrite(trigCG,LOW);
  delayMicroseconds(2);
  digitalWrite(trigCG,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigCG,LOW);
  durCG = pulseIn(echoCG,HIGH);
  checkGlass = (durCG*0.034)/2;

  //Serial.println(checkGlass);
  if(checkGlass<=5 && checkGlass>=4){
    startFilling();
  }

//  Serial.print("AccessIN Loop:");
//  Serial.println(CSA);
//  Serial.print("DeniedIN Loop:");
//  Serial.println(CSD);
//  delay(2000);

  if(CSA == 1){
    boothStarting();
  }
  else{
    if(ck !=1){
      sessionEnd();
    }
    loop();
  }
    
}

void defaultInfo(){
  lcd.setCursor(0,0);           
  lcd.print("Mineral Water Booth.");
  lcd.setCursor(0,1); 
  lcd.print("                    ");
  lcd.setCursor(1,1);            
  lcd.print("Punch Your Card...");

  lcd.setCursor(0,2);  
  lcd.print("                    ");
  lcd.setCursor(2,2);          
  lcd.print("To Access Water.");

  lcd.setCursor(0,3);             
  lcd.print("                    ");
  lcd.setCursor(1,3);
  lcd.print("Price: 2TK / Glass");

}


void denied(){
    CSA = digitalRead(captureSignalAccessed);
    CSD = digitalRead(captureSignalDenied);
    delay(2000);
  
    lcd.setCursor(0,1); 
    lcd.print("                    ");
    lcd.setCursor(4,1);            
    lcd.print("Sorry! (-_-)");
    lcd.setCursor(0,2);
    lcd.print("                    "); 
    lcd.setCursor(3,2);            
    lcd.print("Access Denied!");
    lcd.setCursor(0,3);  
    lcd.print("                    ");  
    lcd.setCursor(5,3);         
    lcd.print("Try Again.");

    delay(500);
    if(CSD == 0)
      loop();
    else 
      denied();
}

void loop()
{
  defaultInfo();
  
  CSA = digitalRead(captureSignalAccessed);
  CSD = digitalRead(captureSignalDenied);
  delay(500);
  
  Serial.print("Access Pin: ");
  Serial.println(CSA);
  Serial.print("Denied Pin: ");
  Serial.println(CSD);
  
  if(CSA == 1 && CSD ==0){
    lcd.setCursor(0,1); 
    lcd.print("                    ");
    lcd.setCursor(2,1);            
    lcd.print("Congratulation!");
    lcd.setCursor(0,2);      
    lcd.print("                    ");
    lcd.setCursor(2,2);       
    lcd.print("Access Granted.");
    lcd.setCursor(0,3); 
    lcd.print("                    ");
    lcd.setCursor(2,3);            
    lcd.print("Have a Good Day!");

    delay(500);

    countG = 0;
    boothStarting();
  }
  
  else if(CSD == 1 && CSA ==0){
    denied();
    //delay(500);
  }
  else{
    defaultInfo();
  }
  
  //delay(1000);
   resetFunc();  //call reset
}
