//Lab6_Com5
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include "DHT.h"

#define MSIGNAL 22
#define DHTPIN 18  
#define DHTTYPE DHT22  

DHT dht(DHTPIN, DHTTYPE);

// define two tasks for Blink & AnalogRead
void TaskMotor( void *pvParameters );
void TaskReadDHT22( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
    TaskMotor
    ,  "TaskMotor"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskReadDHT22
    ,  "ReadDHT22"
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

void TaskMotor(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(MSIGNAL, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(MSIGNAL, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay(10000);  // one tick delay (15ms) in between reads for stability
    digitalWrite(MSIGNAL, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay(10000);  // one tick delay (15ms) in between reads for stability
  }
}

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
