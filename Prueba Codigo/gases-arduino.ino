int pin_mq = 2;

void setup() { 
  Serial.begin(9600);
  pinMode(pin_mq, INPUT);
}


void loop() {
  
 
  boolean mq_estado = digitalRead(pin_mq);//Leemos el sensor
  //lee si hay un gas o humo cerca
  if(mq_estado) //si la salida del sensor es 1
  {
    Serial.println("Sin presencia de gas");
  }
  else //si la salida del sensor es 0
  {
    Serial.println("gas detectado");
  }
 
  int adc_MQ = analogRead(A0); //Lemos la salida analógica del MQ
  float voltaje = adc_MQ * (5.0 / 1023.0); //Convertimos la lectura en un valor de voltaje
  
  Serial.print("adc:");
  Serial.print(adc_MQ);
  Serial.print("    voltaje:");
  Serial.println(voltaje);
  delay(1000);
}
