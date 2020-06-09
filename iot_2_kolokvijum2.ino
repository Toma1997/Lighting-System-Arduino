#include <LiquidCrystal.h>
#define BUTTON_SYSTEM_STATE 2
#define BUTTON_AUTO_LED_STATE 3
#define RELAY_PIN 8
bool manualState = true;
int relayToDiodeState = LOW;
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
void lcdPrint(String row1, String row2){
  	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(row1);
    lcd.setCursor(0, 1);
    lcd.print(row2);  
}
void changeSystemState(){
  manualState = !manualState;
  if(manualState){
  	relayToDiodeState = LOW;
  	digitalWrite(RELAY_PIN, relayToDiodeState);
    lcdPrint("Aktiviran je", "manuleni rezim");
  } else {
    lcdPrint("Aktiviran je", "automatski rezim");
  }
  Serial.println(manualState);
}
void changeDiodeState(){
  if(manualState){
    lcdPrint("Manuleni je", "rezim rada !");
  } else {
  	relayToDiodeState = !relayToDiodeState;
    digitalWrite(RELAY_PIN, relayToDiodeState);
    String msg = (relayToDiodeState == HIGH) ? "Dioda upaljena !" : "Dioda ugasena !";
    lcdPrint(msg, "");
  }
}
void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(BUTTON_SYSTEM_STATE, INPUT);
  pinMode(BUTTON_AUTO_LED_STATE, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_SYSTEM_STATE), changeSystemState, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_AUTO_LED_STATE), changeDiodeState, RISING);
}
void loop() {
	if(Serial.available() > 0){
      	String tempLight = Serial.readString();
        int delim1 = tempLight.indexOf("C");
      	int delim2 = tempLight.indexOf("L");
        String temp = "Temp: " + tempLight.substring(0, delim1) + " C";
        String light = "Light: " + tempLight.substring(delim1+1, delim2) + " lm";
      	lcdPrint(temp, light);
	} 
}