unsigned char PadNote[5] = {88,100,50,70,92};         // MIDI notes from 0-127
 
int PadCutOff[5] = {60,50,70,80,90};          //minimum value to trigger the sensor
 
int MaxPlayTime[5] = {30,75,50,90,100};               // Cycles to pass before the second beat is triggered .
 
#define  midichannel    0;                              //  Midi port
 
boolean VelocityFlag  = true;                // If the sound is according to the intensity of the beat .
 
 
 
 
//*******************************************************************************************************************
// Internal use of variables                 
//*******************************************************************************************************************
 
boolean activePad[5] = {0,0,0,0,0};                   // See if the pads are active or not .
int PinPlayTime[5] = {0,0,0,0,0};                     // Counter of cycles since the pad was triggered.
 
unsigned char status;
 
int pin = 0;    
int hitavg = 0;
 
//*******************************************************************************************************************
// Setup                       
//*******************************************************************************************************************
 
void setup()
{
  Serial.begin(57600);                              
}
 
//*******************************************************************************************************************
// Main Program                
//*******************************************************************************************************************
 
void loop()
{
  for(int pin=0; pin < 5; pin++) 
  {
    hitavg = analogRead(pin);  // Value of the sensor                           
 
    if((hitavg > PadCutOff[pin]))  // Verifies that the value picked up by the sensor is greater than the minimum value to cause the Sound
    {
      if((activePad[pin] == false))  // Verifies that the Pad is already running .
      {
        if(VelocityFlag == true)  // Check if the sound is according to the intensity of the beat, to generate the Midi signal .
        {
        //hitavg = 127 / ((1023 - PadCutOff[pin]) / (hitavg - PadCutOff[pin]));    // With full range (Too sensitive ?)
        //for(int j=3;j<9;j++)
          hitavg = (hitavg / 8) -1 ;  
                    // Upper range
        }
        else
        {
          //for(int i=50;i<127;i++)
          hitavg = 127;
        }
 
        MIDI_TX(144,PadNote[pin],hitavg); // Play the MIDI signal
        PinPlayTime[pin] = 0;  // Set the cycle for '0'
        activePad[pin] = true;  // Changes the active pad .
      }
      else
      {
        PinPlayTime[pin] = PinPlayTime[pin] + 1;  // If the Pad is already active , 1 cycle increments .
      } 
    }
    else if((activePad[pin] == true)) // THIS PART IS ONLY RESPONSIBLE FOR INCREASING THE CYCLES AND ON / OFF THE PADS .
    {
      PinPlayTime[pin] = PinPlayTime[pin] + 1;
     
      if(PinPlayTime[pin] > MaxPlayTime[pin])
      {
        activePad[pin] = false;
        MIDI_TX(144,PadNote[pin],125);
      }
    }
  }
}
 
 
//*******************************************************************************************************************
// Function that transmits MIDI                       
//*******************************************************************************************************************
void MIDI_TX(unsigned char MESSAGE, unsigned char PITCH, unsigned char VELOCITY)
{
  status = MESSAGE + midichannel;
  Serial.write(status);
  Serial.write(PITCH);
  Serial.write(VELOCITY);
}
