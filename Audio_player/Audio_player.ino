/* By Miaad.K
 * 9/25/2020
 */
#define DEBUG
 
#include "SD.h"
#include "TMRpcm.h"
#include "SPI.h"

#define SD_ChipSelectPin 4
#define USE_TIMER2
#define audioVolume 5 

// =========== VARIABLES ===========
const byte PROGMEM SpeakerPin = 9;
const int PROGMEM buttonsPins [] = {0, 1, 2, 3, 5, 6, 7, 8, 10, A2, A2, A3, A4, A5};
const int PROGMEM numberOfButtons = sizeof(buttonsPins) / sizeof(buttonsPins[0]);

bool buttonsStates [numberOfButtons];      // Current state
bool buttonsPrvStates [numberOfButtons];   // Previous state

// =========== MAIN ===========
TMRpcm audio;

void setup()
{
  #ifdef DEBUG
  Serial.begin(9600);
  while(!Serial){}
  Serial.println(F("Serial communication accomplished."));
  Serial.println("Number of buttons:" + String(numberOfButtons));
  
  if (!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    tone(SpeakerPin, 1000, 2000);
    return;
  }
  #endif

  // Set pin modes
  
  for(int i = 0; i < numberOfButtons; i++)      
  {
     pinMode((int)pgm_read_word(&(buttonsPins[i])), INPUT_PULLUP);
     buttonsStates[i] = 1;          
     buttonsPrvStates[i] = 1;       // No buttons are pressed at first
     delay(500);
  }
  
  pinMode(SpeakerPin, OUTPUT);
  pinMode(A0, INPUT);              // For generating random numbers
  
  audio.speakerPin = SpeakerPin;
  audio.setVolume(audioVolume);
  
  randomSeed(analogRead(A0));
  int randNum = random(2, 4);    

  // play intro audio
  Serial.println("playing");
  audio.play("1.wav");      
  Serial.println("Done!");                              
  audio.play(sprintf("%i.wmv", randNum));   
}

void loop()
{
  for(int i = 0; i< numberOfButtons; i++)              // Iterate through buttons array.
  {
    buttonsStates[i] = digitalRead((int)pgm_read_word(&(buttonsPins[i])));
    
    if (buttonsPrvStates[i] && !buttonsStates[i])      // Compare the button state to its previous state.
    {
      #ifdef DEBUG
      Serial.println("Button " + String(i) + " is pressed.");
      Serial.println("Playing" + String(i + 5) + ".wmv");
      #endif
      audio.play(sprintf("%i.wmv", i + 5));            // Play the audio
    }

    buttonsPrvStates[i] = buttonsStates[i];            // Save the curent state as previous state.
    delay(35);  // Debounce
  }
}

// END
