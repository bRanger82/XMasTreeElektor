// Blinking Christmas tree for Arduino Uno, Elektor SKU: 18348
// 
// Changed original code, replaced each define pin to a pin-array and added randomSeed

// Connected output pins connected to the electronic christmas tree
byte pins [] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };  

// Define the min- and max-brightness for the LED tree (except the top one, which is always set to full brightness)
#define MinBrigthness    5
#define MaxBrightness  255

// If pin is connected to GND, no fading is used. If it is floating or HIGH, fading is used.
#define FadingPin       A0

// Putpose: checks, if a pin is set to low.
bool CheckInputPinLow (int pin);
// Purpose: turns all lights off, including top light
void AllLightsOff();
// Purpose: Changes the light brightness of the tree leds
void UpdateLedBrightness(bool Fading = true);

void setup()
{
  // define the pins as outputs
  for (byte pos = 0; pos < (sizeof(pins) / sizeof(pins[0])); pos++)
  {
    pinMode(pins[pos], OUTPUT);
  }
}

void loop() 
{
  // Have a "realistic" feeling
  randomSeed(millis());

  // If fading pin is thight to GND, do not use the fading
  if (CheckInputPinLow(FadingPin))
  {
    // Do not use fading
    UpdateLedBrightness(false);  
  } else
  {
    // Use fading
    UpdateLedBrightness(true);  
  }
  
  // Wait a random time for chaning the brightness again
  delay(random(100,500));
}

/*
 * Putpose: checks, if a pin is set to low.
 * 
 * Parameter(s): pin, the pin number
 * 
 * Returns: true if the PIN is low, otherwise false
*/
bool CheckInputPinLow (int pin)
{
  /*  If nothing is connected to the pin, it is floating.
   *  To prevent a reading of a misleading GND value, two values are read.
   *  These values are compared and must be 0 (GND) -> returns true.
   */
  int data [2];
  data [0] = analogRead(pin); // first read
  delayMicroseconds(20);      // delay a moment
  data [1] = analogRead(pin); // second read
  return (data[0] == data[1] && data[0] == 0);
}

/*
 * Purpose: turns all lights off, including top light
 * 
 * Paramter(s): nothing
 * Returns: nothing
*/
void AllLightsOff()
{
  for (byte pos = 0; pos < (sizeof(pins) / sizeof(pins[0])); pos++)
  {
    analogWrite(pins[pos], 0);
  }
}

/*
 * Purpose: Changes the light brightness of the tree leds
 * 
 * Parameters: Fading (bool), default true, if true the LEDs are faded from the current value to the new value
 * 
 * Returns: nothing
*/
void UpdateLedBrightness(bool Fading = true)
{
  // The one on the top is always the brightest
  analogWrite(pins[0], 255);
  
  // Tree-lights should light up randomly
  for (byte pos = 1; pos < (sizeof(pins) / sizeof(pins[0])); pos++)
  {
    // Get actual value 
    int temp = analogRead(pins[pos]);
    // Map the actual value as analogRead returns a value from 0 to 1023, analogWrite supports only 0 to 255.
    byte CurrentValue = map(temp, 0, 1024, MinBrigthness, MaxBrightness);
    
    // Get new value
    byte NewToValue = random(MinBrigthness,MaxBrightness);

    if (Fading)
    {
      // Gading from the actual value to the new value
      while (NewToValue != CurrentValue)
      {
        // If the current brightness is lower than the NewValue increase the value
        if (NewToValue > CurrentValue)
        {
          CurrentValue++;
        } else
        {
          CurrentValue--;
        }
        analogWrite(pins[pos], CurrentValue);  
        delayMicroseconds(20);
      }  
    } else // Use _no_ fading
    {
      analogWrite(pins[pos], CurrentValue);  
    }
  }
}
