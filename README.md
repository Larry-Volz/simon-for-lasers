# simon-for-lasers
Simon game made for giant buttons and arduino that communicates with our laser maze.  Most recent stable build.
/*
The circuit:
  For big buttons assume that if the led is facing upwards then:
  
  - YELLOWthe lead facing you is the line that attaches to 
    buttonPin[], 10k resister and ground - use yellow
    
  - RED the lead on the bottom goes to +5v - use red
  
  - BLACKthe one on the side to the right goes to common ground - use blk
  
  - GREEN the one on the left goes to the LED pin - use blue or green

  
  - LED attached from pin 13 to ground
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.  That will make button 0 flicker during uploads to board - once at start once when it's been uploaded

  - blue attach to pin 9 to go to positive of the key-switch on the keypad (in Pat's system)
  - black attach to ground rail - these are to turn off the lasers and turn on the keypad for entry

  - Attach RESET wire to A0 on one end and the RST pin on the other for game resets

FLOW
- define score keeper
- loop 7 times as i
- define a sequence of i tones
- listen for correct response
- if yes -> was that 7 correct responses? -yes-> play win sequence and unlock key-box
- if no -> start game over

DO:
- fix the need to push a button 2x to exit the waiting state

*/
