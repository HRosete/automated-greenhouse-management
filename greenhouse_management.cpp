//Libraries 
#include <Time.h>   
#include <Wire.h>   
#include <DS1307RTC.h> 
#include <dht.h> 

// DEFINE INPUT PINS 
// For temp & humidity; DHT22 
#define DHT22_PIN1 2      
#define DHT22_PIN2 3 

// For soil moisture 
int readmoistd1 = 37; 
int readmoistd2 = 36; 


// DEFINE OUTPUT PINS 
//FOR TEMP 
int FAN1 = 41;  // digital output for fans      
int FAN2 = 40; 
int FAN3 = 39; 
int FAN4 = 38; 

//FOR MOISTURE 
int VALVE = 44; // digital output for moisture

//FOR LIGHTING  
int L1 = 42;    // digital output for light1 
int L2 = 43;    // digital output for light2 
int L3 = 45;    // digital output for light3 

//For Stepper Motor 
int stepm = 4;  // PWM 
int dirm = 35;  // Direction 


// DEFINE VARIABLES 
//1st DHT22 
float hum1; 
float temp1; 

//2nd DHT22 
float hum2;     //Stores humidity value 
float temp2; 

// Stepper Motor 
int delay_1 = 10; 
int delay_2 = 1000; 
int margin_3 = 100; 
int margin_4 = 10; 


void setup()  { 
    // initialize serial communications: 
    Serial.begin(9600);

    // initialize the FANS as an output: 
    pinMode(FAN1, OUTPUT); 
    pinMode(FAN2, OUTPUT); 
    pinMode(FAN3, OUTPUT); 
    pinMode(FAN4, OUTPUT);

    // initialize the moistureOutput as an output: 
    pinMode(VALVE, OUTPUT); 
    pinMode(L1, OUTPUT); 
    pinMode(L2, OUTPUT); 
    pinMode(L3, OUTPUT); 
    pinMode(stepm, OUTPUT); 
    pinMode(dirm, OUTPUT); 
    pinMode(readmoistd1, INPUT); 
    pinMode(readmoistd2, INPUT); 
}


void loop() 
{ 
    //Variables where time is stored 
    int h = hour(); 
    int m = minute(); 
    int s = second();

    //Variables where temp and humidity are stored 
    int chk1 = DHT.read22(DHT22_PIN1); 
    hum1 = DHT.humidity; 
    temp1 = DHT.temperature; 
    
    int chk2 = DHT.read22(DHT22_PIN2); 
    hum2 = DHT.humidity; 
    temp2 = DHT.temperature; 
    
    //Variables where LDR values are stored 
    int sensorValue = analogRead(A0); 
    int sensorValue3 = analogRead(A1); 
    
    //Variables where moisture is stored: 
    //Analog 
    int readmoista1 = analogRead(A2); 
    int readmoista2 = analogRead(A3); 
    //Digital 
    int moistd1 = digitalRead(readmoistd1); 
    int moistd2 = digitalRead(readmoistd2); 
    
    //Calculating LDR resistance 
    float sensorValue2 = sensorValue*(5.0/1024.0); 
    float reso = (sensorValue2*5000.0)/(5.0 - sensorValue2); 
    float sensorValue4 = sensorValue3*(5.0/1024.0); 
    float reso2 = (sensorValue4*5000.0)/(5.0 - sensorValue4); 
    
    if ((h >= 6) && (h < 11)){ 
        CtrlTemp(temp1, temp2); 
        ValveOn(h, readmoista1, moistd1, readmoista2, moistd2); 
    } 
    
    if ((h >=11) && (h < 16) && ((temp1 >= 30) || (temp2 >= 30))){ 
        OnFanLines();     
        delay(1000); 
        MoveStepper1(); 
        OffFanLines(); 
    } 
    else 
    { 
        OnFanLines(); 
        delay(1000); 
        MoveStepper2(); 
        OffFanLines(); 
    } 
    
    
    //Time when Lights will turn on 
    if (h >= 6 && h < 22){ 
        LightsOn1(h, reso, reso2); 
        LightsOn2(h, reso, reso2); 
        LightsOn3(h, reso, reso2); 
    } 
    
    //Print values for verification 
    Serial.print(hum1);         //humidity 1 
    Serial.print("\t"); 
    Serial.print(temp1);        //temperature 1 
    Serial.print("\t"); 
    Serial.print(hum2);         //humidity 2 
    Serial.print("\t"); 
    Serial.print(temp2);        //temperature 2 
    Serial.print("\t"); 
    Serial.print(sensorValue2); //voltage of LDR 1 
    Serial.print("\t"); 
    Serial.print(reso);         //resistance of LDR 1 
    Serial.print("\t"); 
    Serial.print(sensorValue4); //voltage of LDR 2 
    Serial.print("\t"); 
    Serial.print(reso2);        //resistance of LDR 2 
    Serial.print("\t"); 
    Serial.print(readmoista1);  //soil moisture analog 1 
    Serial.print("\t"); 
    Serial.print(moistd1);      //soil moisture digital 1 
    Serial.print("\t"); 
    Serial.print(readmoista2);  //soil moisture analog 2 
    Serial.print("\t"); 
    Serial.println(moistd2);    //soil moisture digital 2 
    
    delay(5000); 
    } 
    
    
//Function for turning on fans 
void CtrlTemp(float intemp1, float intemp2){ 
    if ((intemp1 >= 28) || (intemp2 >= 28)){ 
        digitalWrite(FAN1, HIGH); 
        digitalWrite(FAN2, HIGH); 
        digitalWrite(FAN3, HIGH);     
        digitalWrite(FAN4, HIGH); 
    } 
    else 
    { 
        digitalWrite(FAN1, LOW); 
        digitalWrite(FAN2, LOW); 
        digitalWrite(FAN3, LOW); 
        digitalWrite(FAN4, LOW); 
    } 
} 


//Function for turning on water valve 
void ValveOn(int timeh2, int inmoista1, int inmoistd1, int inmoista2, int inmoistd2){ 
    if (((timeh2 == 6) || (timeh2 == 17)) && (inmoistd1 == 1) && (inmoistd2 == 1) && ((inmoista1 >= 570) || (inmoista2 >= 570))){ 
        digitalWrite(VALVE, HIGH); 
    } 
    else 
    { 
        digitalWrite(VALVE, LOW); 
    } 
}
    
//Function for turning on 1st light 
void LightsOn1(int timeh3, float resist1, float resist12){ 
    if (((timeh3 >= 17) && (timeh3 < 19)) || ((timeh3 < 17) && ((resist1 > 400) || (resist12 > 400)))){ 
        digitalWrite(L1, HIGH); 
    } 
    else 
    { 
        digitalWrite(L1, LOW);  
    } 
} 


//Function for turning on 2nd light 
void LightsOn2(int timeh4, float resist2, float resist22){ 
    if ((timeh4 >= 17) && (timeh4 < 20) || ((timeh4 < 17) && ((resist2 > 400) || (resist22 > 400)))){ 
        digitalWrite(L2, HIGH); 
    } 
    else 
    {     
        digitalWrite(L2, LOW);  
    } 
} 


//Function for turning on 3rd light 
void LightsOn3(int timeh5, float resist3, float resist32){ 
  if ((timeh5 >= 17) && (timeh5 < 21) || ((timeh5 < 17) && ((resist3 > 400) || (resist32 > 400)))){ 
    digitalWrite(L3, HIGH); 
  } 
  else 
  { 
    digitalWrite(L3, LOW); 
  } 
} 
 
//Function for turning on stepper motor 
void MoveStepper1(){ 
  digitalWrite(dirm,HIGH); // The motor moves in a particular direction 
  // makes 100000 pulses 
  for(int x = 0; x < margin_4; x++){  
    for(int x = 0; x < margin_3; x++) { 
        digitalWrite(stepm,HIGH);  
        delayMicroseconds(delay_1);  
        digitalWrite(stepm,LOW);  
        delayMicroseconds(delay_1); 
    } 
  } 
} 


//Another function for turning on stepper motor 
void MoveStepper2(){ 

  digitalWrite(dirm,LOW); //Changes the direction

  // makes 10000 pulses 
  for(int x = 0; x < margin_4; x++){  
    for(int x = 0; x < margin_3; x++) { 
        digitalWrite(stepm,HIGH);  
        delayMicroseconds(delay_1);  
        digitalWrite(stepm,LOW);  
        delayMicroseconds(delay_1); 
    } 
  } 
} 


//Turn on power lines for the fans; supporting function for stepper motor operation 
void OnFanLines(){ 
    digitalWrite(FAN1, HIGH); 
    digitalWrite(FAN2, HIGH); 
    digitalWrite(FAN3, HIGH); 
    digitalWrite(FAN4, HIGH); 
}


//Turn off power lines for the fans; another supporting function for stepper motor operation 
void OffFanLines(){ 
    digitalWrite(FAN1, LOW); 
    digitalWrite(FAN2, LOW); 
    digitalWrite(FAN3, LOW); 
    digitalWrite(FAN4, LOW); 
}
