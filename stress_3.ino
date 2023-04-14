#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_ADS1X15.h>


Adafruit_ADS1115 ads;  
SoftwareSerial BTSerial(10, 11);  
const int BT_BAUD_RATE = 9600;

int16_t gsrValue;
int16_t prevGsrValue;
int16_t diff = 0;

int16_t stressValue = 10;

void setup() 
{
  Serial.begin(9600);
  BTSerial.begin(BT_BAUD_RATE);
  ads.begin();  

  delay(6000);

  prevGsrValue = ads.readADC_SingleEnded(0);

}

void loop() 
{

      if (BTSerial.available()) 
      {
          String message = BTSerial.readString();
          if(message == "R") 
          { 
            Serial.println("Recalibrate..."); 
            setup();
          }
      }

     diff = 0;    
     gsrValue = ads.readADC_SingleEnded(0);   
     
     diff = gsrValue - prevGsrValue;

     if(stressValue < 60)

     {
          if(diff >= 40)
          {
              // stressValue = stressValue * (1 - (10/100)); 
              int t = 0;
              t = diff/40;       
              stressValue -= t;
              
          }

          else if(diff < 0 && abs(diff) >= 40)
          {
              int t = 0;
              t = abs(diff)/40;       
              stressValue += t;
          }

          else if(diff > 0 && diff<40)
          {
             --stressValue;
          }

          else if(diff <= 0 && abs(diff)<40 )
          {
             ++stressValue;
          }

      }

      else
      {
          if(diff < 0 && abs(diff)>=60)
          {
              int t = 0;
              t = abs(diff)/60;       
              stressValue += t;
          }

          else
          {
              int t = 0;
              t = abs(diff)/60;       
              stressValue -= t;
          }
      }

      prevGsrValue = gsrValue;

      if (stressValue < 10.00) 
      {
        stressValue = 10.00;
      } 
      else if (stressValue > 100.00) 
      {
         stressValue = 100.00;
      }
  
      Serial.println(gsrValue);
      Serial.println(stressValue);
      Serial.println(".....");
   
      


      BTSerial.println("--------------------");
      BTSerial.print("  GSR Value: ");
      BTSerial.println(stressValue);
      BTSerial.println("--------------------");

      delay(2000); 

}





 
 