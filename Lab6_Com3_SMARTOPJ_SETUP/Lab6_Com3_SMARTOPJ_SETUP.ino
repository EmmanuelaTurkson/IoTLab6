//Lab6_com3
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include <EasyUltrasonic.h>
#include "DHT.h"

#define DHTPIN 18  
#define TRIGPIN 22 // Digital pin connected to the trig pin of the ultrasonic sensor
#define ECHOPIN 23 // Digital pin connected to the echo pin of the ultrasonic sensor
#define DHTTYPE DHT22  

DHT dht(DHTPIN, DHTTYPE);
EasyUltrasonic ultrasonic; // Create the ultrasonic object
// define two tasks for Blink & AnalogRead
void TaskReadUltrasonic( void *pvParameters );
void TaskReadDHT22( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
    TaskReadDHT22
    ,  "TaskReadDHT22"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskReadUltrasonic
    ,  "TaskReadUltrasonic"
    ,  1024  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskReadDHT22(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
  dht.begin();
  for (;;)
  {
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Check if any reads failed and exit early (to try again).
    if (isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    Serial.print(F("Temperature: "));
    Serial.print(t);
    Serial.print(F("°C \n"));

    vTaskDelay(2000);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskReadUltrasonic(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
  ultrasonic.attach(TRIGPIN, ECHOPIN);
  for (;;)
  {
    float distanceIN = ultrasonic.getDistanceIN(); // Read the distance in inches

    float distanceCM = convertToCM(distanceIN); // Convert the distance to centimeters
  
    // Print the new distance value in Serial Monitor
    Serial.print(F("Distance: "));
    Serial.print(distanceCM);
    Serial.println(" cm\n");

    vTaskDelay(2500);  // one tick delay (15ms) in between reads for stability
  }
}
