#define DIODE 7
#define MANUAL_SWITCH 8
bool manualState = true;
int measurePeriod = 500;
unsigned long prevMillis;
int toLumens(float raw){
  float Vout = raw * (5 / float(1023));// Conversion analog to voltage.
  float RLDR = (10000 * (5 - Vout))/Vout; // Conversion voltage to resistance.
  int phys=500/(RLDR/1000); // Conversion resitance to lumen.
  return phys;
}
void setup(){
  Serial.begin(9600);
  pinMode(DIODE, OUTPUT);
  pinMode(MANUAL_SWITCH, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}
void loop(){
	if(Serial.available() > 0){
  		manualState = (Serial.readString().startsWith("1")) ? true : false;
      	digitalWrite(DIODE, LOW);
    }
  	if(manualState){
      	int switchState = digitalRead(MANUAL_SWITCH);
  		digitalWrite(DIODE, switchState);
    }
  
  	// Measure on every 0.5 seconds.
  	if(millis() - prevMillis >= measurePeriod){
    	float temp = ((analogRead(A0) * 0.00488) - 0.5) * 100;
      	int lighting = toLumens(analogRead(A1));
      	String tempLighting = ((String) temp) + "C" + ((String)lighting) + "L";
      	Serial.println(tempLighting); // Temp in C and lighting in lumens.
    	prevMillis = millis();
    }        
}