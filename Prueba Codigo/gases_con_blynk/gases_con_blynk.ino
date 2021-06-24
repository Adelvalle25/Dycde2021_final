#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <MQUnifiedsensor.h>

//Definitions
#define placa "ESP8266"
#define Voltage_Resolution 5
#define pin A0 //Analog input 0 of your arduino
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  
//#define calibration_button 13 //Pin to calibrate your sensor

//Declare Sensor
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);




BlynkTimer timer;
char auth[] = "sZnKemegg0DYo_JSPIhn5FRHYaSdJ2RF";
char ssid[] = "UNE_01E5";
char pass[] = "00002514208087";
int n;
WidgetTerminal terminal(V1);
WidgetTerminal Termi(V2);

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(16,OUTPUT);
  pinMode(5,OUTPUT);

  terminal.clear();

 MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  
  
  MQ135.init(); 
  
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
  }
  MQ135.setR0(calcR0/10);
  
  if(isinf(calcR0)) {terminal.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){terminal.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
}


void loop()
{
  Blynk.run();
  timer.run();
  n=analogRead(A0);
  if(n>190)
  {
     digitalWrite(16,LOW);
    digitalWrite(5,HIGH);   
  }
  if(n<190)
  {
      digitalWrite(16,HIGH);
      digitalWrite(5,LOW);
  }
 MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin

  MQ135.setA(605.18); MQ135.setB(-3.937); // Configurate the ecuation values to get CO concentration
  float CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(77.255); MQ135.setB(-3.18); // Configurate the ecuation values to get Alcohol concentration
  float Alcohol = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(110.47); MQ135.setB(-2.862); // Configurate the ecuation values to get CO2 concentration
  float CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(44.947); MQ135.setB(-3.445); // Configurate the ecuation values to get Tolueno concentration
  float Tolueno = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(102.2 ); MQ135.setB(-2.473); // Configurate the ecuation values to get NH4 concentration
  float NH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(34.668); MQ135.setB(-3.369); // Configurate the ecuation values to get Acetona concentration
  float Acetona = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  terminal.print("-------------------------------------\n");
  terminal.print("CO: "); terminal.print(CO2 + 400);terminal.print("\n");
  terminal.print("Alcohol: "); terminal.print(Tolueno); terminal.print("\n");
  terminal.print("CO2:  "); terminal.print(NH4); terminal.print("\n");
  terminal.print("Acetona:  "); terminal.print(Acetona);terminal.print("\n");
  terminal.print("-------------------------------------\n");
  Termi.print(1231);
  delay(800);
 

}
