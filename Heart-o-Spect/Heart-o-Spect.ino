/*
 * Author: Devyani Gohokar
 * Project: Heart-O-Spect
 */
#define BLYNK_PRINT Serial
#include <SimpleTimer.h>   //including the library of SimpleTimer
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
char auth[] = "AUTH";

// Your WiFi credentials.
char ssid[] = "SSID";
char pass[] = "Password";

//  Variables
const int PulseWire = A0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = D2;          
int Threshold = 510;           // Determine which Signal to "count as a beat" and which to ignore.
 
int k;
float avg;
int sum=0;
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

 

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold);   
 
  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !"); 
  }
  
}

void spect()
{
  // Check 10 readings and take average
  for(int i=0 ; i<=10;i++){
     
     int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                                   // "myBPM" hold this BPM value now. 
     
      if (pulseSensor.sawStartOfBeat()) {  
        Serial.println("In");// Constantly test to see if "a beat happened". 
       Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
       Serial.print("BPM: ");                        // Print phrase "BPM: " 
       Serial.println(myBPM);
       // Print the value inside of myBPM.  
       sum=sum+myBPM;
       k += 1;
      }  
      delay(20); 
      
  }
  // Calculate and print average after every 10 readings.
  if((k % 10) == 0){
      avg = sum / 10;
      sum = 0;
      Serial.print("Avg: ");
      Serial.println(avg);
      Blynk.virtualWrite(V0, avg);
      if(avg>90){
        digitalWrite(LED13,HIGH);
        delay(500);
       
      }
      else{
        digitalWrite(LED13,LOW);
        
      }
      delay(20);
      
   }
  
  
  
  
}



void loop()
{ // Main Loop
  Blynk.run();
  // Run above created function
  spect();
}
