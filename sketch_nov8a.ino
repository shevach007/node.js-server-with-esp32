/*A simple sketch that shows how to use both cores for different tasks.

I connected a Led on pin 18 and a potentiometer on pin 34.

The reading of the analog input and the print statement with the led status is done in the void loop section
that runs on Core 1. the Led control(turn on, turn of and blinking) is done on Core 0. notice that the global variable 'ledStatus' is availible 
on both cores. 
*/

#define ledPin 18
#define analogInput 34
String ledStatus = ""; 
TaskHandle_t Task1; // name of the Core's task

//function that blinks the led perioducly for a duration set by the user
int blink(byte pin, int duration) {
  while(){
    ledStatus = "Led is On!";
    digitalWrite(pin, HIGH);
    Serial.println(ledStatus);
    delay(duration);
    ledStatus = "Led is Off!";
    digitalWrite(pin, LOW);
    Serial.println(ledStatus);
    delay(duration);
    if(Serial.available())
      {
      int state = Serial.parseInt();
      if(state == 1)
        return 1;
      if(state == 2)
        return 2;
      }
    }
}

void codeForTask1( void * parameter )
{
  for (;;) { //does same as the void loop 
    if(Serial.available())
      {
      int state = Serial.parseInt();
    // Rescale to potentiometer's voltage (from 0V to 3.3V):
      if(state == 3)
      {
        state = blink(ledPin, 500);
      }
      if(state == 1){
        ledStatus = "Led is On!";
        digitalWrite(ledPin, HIGH);
        Serial.println(ledStatus);
      }
      if(state == 2){
        ledStatus = "Led is Off!";
        digitalWrite(ledPin, LOW);
        Serial.println(ledStatus);
      }
      
    }
  }
}
//calculate the voltage of the analog input
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
} 
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
    xTaskCreatePinnedToCore( 
    codeForTask1, /* Task function. */
    "led1Task", /* name of task. */
    1000,/* Stack size of task */
    NULL,/* parameter of the task */
    1,/* priority of the task */
    &Task1,/* Task handle to keep track of created task */
    0);/* Core (Core 0) */


  if(digitalRead(ledPin) == 0)
    ledStatus = "Led is Off!";
  else
    ledStatus = "Led is On!";
}

// the loop function runs over and over again forever
void loop() { // loop runs by default on Core 1
  int analogValue = analogRead(analogInput);
  float voltage = floatMap(analogValue, 0, 4095, 0, 3.3);  
  char buffer[80];
  sprintf(buffer, "Analog: %d, Voltage: %f, percent: %f%, %s",analogValue, voltage, voltage*100/3.3, ledStatus);
  Serial.println(buffer);

  delay(100);
  
}