/* 
NOTE: This program borrows code from http://www.arduino.cc/en/Tutorial/PlayMelody.

Name: Party Button
Purpose: On button press, the speaker plays a song with accompanying flashes of
an RGB LED.
Created by: Aaron Hnatiw

Credit: 
- 2005 D. Cuartielles for K3. Thank you for the public use of your code,
I could not have made this program without it.
- Thomas Ouellet Fredericks. Thank you for creating the Bounce Arduino library,
it has saved me countless hours of frustration with buttons on Arduino.
 */

// Include bounce library for button debouncing
#include <Bounce.h>

#define button 2
#define red 3
#define blue 4
#define green 5

// Set up speaker on a PWM pin (digital 9, 10 or 11)
int speakerOut = 9;

// To monitor button state
int buttonPushCounter = 1;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

// TONES  ==========================================
// Start by defining the relationship between 
//       note, period, &  frequency. 
// period is in microsecond so P = 1/f * (1E6)

#define  c3    7634
#define  d3    6803
#define  e3    6061
#define  f3    5714
#define  g3    5102
#define  a3    4545
#define  b3    4049
#define  c4    3816    // 261 Hz 
#define  d4    3401    // 294 Hz 
#define  e4    3030    // 329 Hz 
#define  f4    2865    // 349 Hz 
#define  g4    2551    // 392 Hz 
#define  a4    2272    // 440 Hz 
#define  a4s   2146
#define  b4    2028    // 493 Hz 
#define  c5    1912    // 523 Hz
#define  d5    1706
#define  d5s   1608
#define  e5    1517
#define  f5    1433
#define  g5    1276
#define  a5    1136
#define  a5s   1073
#define  b5    1012
#define  c6    955
// Define a special note, 'R', to represent a rest
#define  R     0
// Do we want debugging on serial out? 1 for yes, 0 for no
int DEBUG = 1;
// Instantiate a Bounce object with a 5 millisecond debounce time
Bounce bouncer = Bounce( button,5 );

void setup() {
  pinMode(button,INPUT); 
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(speakerOut, OUTPUT);
  if (DEBUG) { 
    Serial.begin(9600); // Set serial out if we want debugging
  } 
}

// MELODY and TIMING  =======================================
//  melody[] is an array of notes, accompanied by beats[], 
//  which sets each note's relative length (higher #, longer note) 

// star wars theme
//int melody[] = {  f4,  f4, f4,  a4s,   f5,  d5s,  d5,  c5, b5, f5, d5s,  d5,  c5, b5, f5, d5s, d5, d5s,   c5};
//int beats[]  = {  21,  21, 21,  128,  128,   21,  21,  21, 128, 64,  21,  21,  21, 128, 64,  21, 21,  21, 128 };

//DaRude's Sandstorm
int melody[] = {  b3, b3, b3, b3, b3, b3, b3, b3, b3, b3, e4, e4, e4, e4, e4, e4, e4, e4, e4, e4, d4, d4, d4, d4, d4, d4, d4, d4, d4, d4, a3, b3, b3, b3, b3, b3 };
int beats[]  = {   9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9 };

//super mario theme (in progress)
//int melody[] = {e5, e5, R,  e5, R,  c5, e5, R,  g5, R,  R,  R,  g4, R,  R,  R,  c5, R,  R, g4, R, R,  e4};
//int beats[] = { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 8, 16, 8, 16, 16};

int MAX_COUNT = sizeof(melody) / 2; // Melody length, for looping.

// Set overall tempo
long tempo = 10000;
// Set length of pause between notes
int pause = 1000;
// Loop variable to increase Rest length
int rest_count = 50;

// Initialize core variables
int toneM = 0;
int beat = 0;
long duration  = 0;

// PLAY TONE  ==============================================
// Pulse the speaker to play a tone for a particular duration
void playTone() {
  long elapsed_time = 0;

  if (toneM > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {
      //play a random led from the rgb led
      int led = random(3,5);
      digitalWrite(led,HIGH);
      
      digitalWrite(speakerOut,HIGH);
      delayMicroseconds(toneM / 2);

      // DOWN
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(toneM / 2);

      // Keep track of how long we pulsed
      elapsed_time += (toneM);
      digitalWrite(led,LOW);
    } 
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) {
      delayMicroseconds(duration);  
    }                                
  }                                 
}

// MAIN PROGRAM LOOP
void loop() {
  // Update the debouncer
  bouncer.update ( );
  // Get the update value
  int value = bouncer.read();
  if (value == HIGH) { 
    // Set up a counter to pull from melody[] and beats[]
    for (int i=0; i<MAX_COUNT; i++) {
      toneM = melody[i];
      beat = beats[i];

      duration = beat * tempo; // Set up timing

      playTone();
      
      // A pause between notes...
      delayMicroseconds(pause);
    }
  }
}
