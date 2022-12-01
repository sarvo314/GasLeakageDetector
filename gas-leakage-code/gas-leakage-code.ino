#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int redLed = 3;
int greenLed = 4;
int buzzer = 2;
int smokeA0 = 5;
int messageSent;
int sensorThres = 700;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void SendMessage()
{
  Serial.print("\r");
  delay(1000);                  
  Serial.print("AT+CMGF=1\r");    //to set the mode of message as sms in the GSM Module
  delay(1000);
  Serial.print("AT+CMGS=\"+91xxxxxxx\"\r");    //Number to which you want to send the sms +91 is the country code
  delay(1000);
  Serial.print("Gas is leaking in your house. Open the windows\r");   //The text of the message to be sent
  delay(1000);
  Serial.write(0x1A); // sends ctrl+z end of message
  delay(1000);   
}

void setup()
{
  //SIM
  messageSent = 0;
  Serial.begin(2400);  //Baud rate of the GSM/GPRS Module 

  //SMOKE
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(smokeA0, INPUT);
  pinMode(buzzer, OUTPUT);

  //LCD 
  lcd.begin();
  lcd.backlight();
}

void loop()
{
  //GAS 
  int analogSensor = digitalRead(smokeA0); //finds out if the sensor has detected any gas leakage
  //THERE IS GAS
  if (analogSensor == 0)
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(redLed, HIGH); //red led is on
    digitalWrite(greenLed, LOW); //green led is off
    lcd.print("Gas Leak!"); //LCD displays a message
    if(messageSent < 2)
    {
        SendMessage();
        messageSent += 1;
    } //send a phone message
  }
  //THERE IS NO GAS
  else
  {
    digitalWrite(buzzer, HIGH); 
    digitalWrite(redLed, LOW); //red led is off
    digitalWrite(greenLed, HIGH); //green led is on
    lcd.print("No Gas Detected!"); //LCD displays a message
    messageSent = 0 ;
    
  }
  delay(100);
  lcd.clear();
}
