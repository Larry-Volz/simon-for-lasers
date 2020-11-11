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

//################################## I/O VARIABLES ##################################
const int buttonPin[] = {2,3,4,5,6};     // the number of the pushbutton pin
const int ledPin[] =  {13,12,8,A5,A4};      // the number of the LED pin
const int numberOfPins = 5;

const int RESET_PIN = A2;

const int KEYPAD_CONTROLLER_PIN = 9;

// variables will change:
int buttonState[] = {0,0,0,0,0};         // variable for reading the pushbutton status

//################################## MUSIC VARIABLES ################################
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
//Close Encounters melody (beginning sequence)
#define NOTE_AS4 466
#define NOTE_C5  523
#define NOTE_GS4 415
#define NOTE_GS3 208 
#define NOTE_DS4 311

//Win melody sequence

     
 
#define NO_SOUND 0 // used as a rest

#define SPEAKER A0  

int pace = 1450;

int note[5] = {NOTE_AS4, NOTE_C5, NOTE_GS4, NOTE_GS3, NOTE_DS4};

//bool restarting = 0;

//###################################### GAME VARIABLES ##################################

int scoreKeeper = 0; //for how many rounds won within a game
int sequence[7]; //for the sequence to win this round - randomize after each loss/game restart 
int listeningTimer = 750; //how long the system will listen for a response 
bool newGame = 1; //if game is lost reset to 1 - if still in game = 0 
bool newRound = 1; //if starting a game or starting a new round = 1
int lastPressed = -99; //holds number of last button pressed
int nextButton = 0; //next button expected in the series
int nextNumber = 0;

// Game Specific #definitionss
#define RAND             25001  // Random number for the seed
#define GAME_LENGTH      6     // 1 less than max number of flashes per game
#define DEBOUNCE_NUMBER  5      // number of passes that a button is pushed
                                // before registering the push or release.

// The following #defines are used for debugging
#define COMPUTER_LOGIC_OUTPUT   FALSE  //output sequence generating logic
#define PLAYER_TURN_OUTPUT      FALSE  //output player turn logic
#define MULTIPLE_BUTTONS_PUSHED FALSE  //output the multiple button push logic
#define CHECK_BUTTON_PRESS      FALSE  //output button press check logic

// Delays
#define ON_DELAY         200    // LED will stay on for 200 miliseconds
#define OFF_DELAY        200    // LED will go off for 200 miliseconds
#define ROUND_PAUSE      1000   // 1 second pause between rounds

// Setup #defines
#define MAX_NUM_LEDS     5      //num of LEDS and switches that are in the pattern
#define NUM_WORKING_LEDS 5      //if < 4, then some badly soldered switches
#define MELODY_LENGTH    4      //how long a tone should play
#define NUMBER_OF_TONES  8      //how many tones there are to choose between

//debugging
char print_output[256] ;         // used for debugging

// Typedefs
typedef enum
{
   TRUE,
   FALSE,
   ERROR
} eBoolean ;


//################################### END GAME VARIABLES #################################

//################################### FUNCTION DEFINITIONS ##############################

/**
 * TurnOffLEDs
 * - turns all 5 off
 */
void TurnOffLEDs() 
{
   for (int i = 0; i <5; i++)
     {
      digitalWrite(ledPin[i], LOW);
     }
}


/**
 * TurnONLEDs
 * - turns all 5 off
 */
void TurnOnLEDs() 
{
   for (int i = 0; i <5; i++)
     {
      digitalWrite(ledPin[i], HIGH);
     }
}
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
//listens for button press with one cycle through the buttons

/* ---------------------------------------------------------------------------
 - eventListener
 - Return values:
 -  -99 = player error (most likely multiple keys pressed)
 -  0-4 number of the button pressed.
 -  -1 = no key pressed
 -
 - This routine processes loops until a keypress passes the debounce period.
 - If there are more than one pushbutton pressed, a -1 return is given.  
 - Otherwise a return between 1 annd 4 inclusive is given to signal the button
 - pressed.
   ---------------------------------------------------------------------------*/
int eventListener( void )
{
   int debounce = 0 ;
   int last_button = -1 ;
   int current_button = -1 ;
   int button_1 = LOW ;
   int button_2 = LOW ;
   int button_3 = LOW ;
   int button_4 = LOW ;
   int button_5 = LOW ;
   int multi_press = FALSE ;
   
//   sprintf( print_output, "Entering eventListener()\r\n" ) ;
//   Serial.print(print_output) ;
   while (debounce != DEBOUNCE_NUMBER)
   {
      button_1 = digitalRead( buttonPin[0] ) ;
      button_2 = digitalRead( buttonPin[1] ) ;
      button_3 = digitalRead( buttonPin[2] ) ;
      button_4 = digitalRead( buttonPin[3] ) ;
      button_5 = digitalRead( buttonPin[4] ) ;
     
/*
      multi_press = CheckForMultipleButtonsPressed( button_1,
                                                    button_2,
                                                    button_3,
                                                    button_4,
                                                    button_5) ;

      if ( multi_press == 1)
      {
         sprintf( print_output, "Exiting GetKeyPress. Multi-press\r\n" ) ;
         Serial.print(print_output) ;
         Serial.print("button_1: ");
         Serial.println(button_1);
         Serial.print("button_2: ");
         Serial.println(button_2); 
         Serial.print("button_3: ");
         Serial.println(button_3);
         Serial.print("button_4: ");
         Serial.println(button_4);
         Serial.print("button_5: ");
         Serial.println(button_5);
       

         return -99 ;
      }
*/

      //set current_button to the single HIGH button.
      if (HIGH == button_1)
      {
         current_button = 0 ;
      }
      else if (HIGH == button_2)
      {
         current_button = 1 ;
      }
      else if (HIGH == button_3)
      {
         current_button = 2 ;
      }
      else if (HIGH == button_4)
      {
         current_button = 3 ;
      }
      else if (HIGH == button_5)
      {
         current_button = 4 ;
      }
      else // no button pressed
      {
         current_button = -1 ;
      }

      if ( (last_button == current_button) && (current_button > -1 ) )
      {
         debounce++ ;
      }
      else
      {
         last_button = current_button ;
         debounce = 0 ;
      }
   } // end of while loop -> stays until debounce # reached (5 ms pushed)

    tone(SPEAKER, note[last_button]);

    Serial.println("\tKey " + String(last_button) + " Pressed");
    
  // Now debounce the key release---------------------------------------------------

    //Serial.println("\tin KeyRelease part of eventListener");
   debounce = 0 ;

    multi_press = FALSE ;
    int button_read = 0 ;

   while ( debounce != DEBOUNCE_NUMBER )
   {

 
      if (digitalRead( buttonPin[last_button]) == LOW)
        {
           debounce++ ;
        }
        
   } //end getKeyRelease loop

   noTone(SPEAKER);

   Serial.println("\tKey " + String(last_button) + " Released");
      
   Serial.println("Exiting eventListener()\n") ;
   
   return last_button ;   
}


// --------------------------------------------------------------------------------------------------------------------
/**
 * WAITING STATE
 * - turns on all led's
 * - waits for player input
 */
//WAITING STATE PRE-GAME - all lights on waiting until someone pushes one
int waitingState() 
  {
    Serial.println("in waitingState()\n");
    for (int i = 0; i < 5; i++) 
      {
        digitalWrite(ledPin[i], HIGH);
      }
    int buttonPress = -99;
    Serial.println("entering eventListener()");  
    
    while (buttonPress < 0) 
    {
      
      buttonPress = eventListener();
      
    }
    
    Serial.println("\teventListener = " + String(buttonPress));

    Serial.println("exited waitingState()");
    Serial.println();
    return buttonPress;
  }


// --------------------------------------------------------------------------------------------------------------------
//creates a random sequence of 7 array elements valued at 0-4 to indicate order to press buttons

void createSequence(){
      Serial.println("Entering createSequence()");
      Serial.print("Sequence = ");
      for (int i = 0; i < 7; i++) {
       sequence[i] = round(random(0,5));
       Serial.print(sequence[i]);
       Serial.print("  ");
      }
    Serial.println();
    Serial.println();
    newGame = 0;  //to start a new game after getting it wrong reset newGame to 1
  }

// --------------------------------------------------------------------------------------------------------------------
// plays the start of game display 

    void startOfGameDisplay()
      {
        Serial.println("in startOfGameDisplay()");
        //play close encounters fast
        for (int i=0; i<5; i++)
           {
            digitalWrite(ledPin[i], HIGH);
            tone(SPEAKER, note[i]);
            delay(50);
            //Serial.println(note[i]);
            noTone(SPEAKER);
            digitalWrite(ledPin[i], LOW);
            delay(50);
          }
        
        //Flash all quickly 2x 
        for (int ii=0; ii<2; ii++)
        {
          for (int i=0; i<5; i++)
            {
            digitalWrite(ledPin[i], HIGH);
            //tone(SPEAKER, note[i]);
            delay(50);
            //Serial.println(note[i]);
            //noTone(SPEAKER);
            digitalWrite(ledPin[i], LOW);
            delay(25);
            }
        }
        
        // Turn all on
        for (int i=0; i<5; i++)
        {
        digitalWrite(ledPin[i], HIGH);
        }
      delay(1000);
      Serial.println();
      //restarting = 1;
      }


// --------------------------------------------------------------------------------------------------------------------
/**
 * plays the sequence-to-date (really to scoreKeeper) for the players to repeat back to Simon
 * outputs sequence to-date to monitor
 * returns nothing
 */

void playCurrentSeq(int scoreKeeper) 
  {
      //scoreKeeper is the round they are on: 0 = starting round.  i > 6 -> game is won

      Serial.println();
      Serial.print("Entering playCurrentSeq()");
      Serial.println();
      
      Serial.println("\tscoreKeeper (current round) = " + String(scoreKeeper));
      for (int i = 0; i <= scoreKeeper; i++)    
        { 
          int x = sequence[i];  //represents which button - value from 0 to 4
          //delay(250);
          digitalWrite(ledPin[x], LOW);
          tone(SPEAKER, note[x]); //the note for that button
              Serial.print("\tsequence: ");
              Serial.print(i);
              Serial.print(" button#: ");
              Serial.println(sequence[i]);
          delay(400);
          noTone(SPEAKER);
          delay(40);
          digitalWrite(ledPin[x], HIGH);
        }
      newRound = 0;
    Serial.println("leaving playCurrentSeq()");
    Serial.println();
      
  } //end of playCurrentSequence


// --------------------------------------------------------------------------------------------------------------------
//
/**
 * FAIL SEQUENCE
 * - buzzes
 */
void fail(){
  
  Serial.println("FAIL");
  
  for (int i = 0; i <5; i++)
    {
      digitalWrite(ledPin[i], LOW);
    }
  tone(SPEAKER, 75); //the note for that button
  delay(1000);
  noTone(SPEAKER);
  delay(2000);
    for (int i = 0; i <5; i++)
    {
      digitalWrite(ledPin[i], HIGH);
    }
  

  //CHEAT!  JUST RE-SETTING THE RST PIN!
  pinMode(RESET_PIN, OUTPUT);        //This "Activates" pin 12,
  delay(250);                       //To allow each mode to change
  digitalWrite(RESET_PIN, LOW);      //Puts the Reset pin LOW (as it needs),

}

// --------------------------------------------------------------------------------------------------------------------
/**
 * NEXT NUMBER CHECKER:
 * - loops from 0 to scoreKeeper (the current round)
 * - retunr -1 if fail
 */
int nextNumberChecker(int scoreKeeper)
  {
    Serial.println("Entering nextNumberChecker()");
    nextNumber = 0;
    
      while (nextNumber <= scoreKeeper ) 
        {
          Serial.print("\n\tin While loop: nextNumber= "+String(nextNumber));
          Serial.println(" scoreKeeper = " + String(scoreKeeper));
          
          int response = eventListener();
          Serial.println("\tresponse from eventListener = " + String(response));
          
          if (response > -1) 
            {  //if it has been pushed and it's not a multi-press
              
              if (response == sequence[nextNumber])  //and it is correct
                {  
                  Serial.println("\tresponse = button "+String(response)+" = sequence[nextNumber] where nextNumber = " + String(nextNumber) + "  CORRECT!");
                  
                  nextNumber++; //can advance to next button input
                  
                  Serial.println("\tnextNumber is now = "+String(nextNumber));
                  
                  response = -99; //????
                } 
                else 
                {
                  fail();   //if it was pushed and was not correct
                  newGame = 1;
                  scoreKeeper = 0;
                  Serial.println("Exiting nextNumberChecker() on a FAIL ");
                  return -1;
                }
            } // end of positive response loop
        }  //end of inputting while
        
     //scoreKeeper++; //on to the next round
     Serial.println("\tscoreKeeper is now = "+String(scoreKeeper));
     Serial.println("POSITIVELY exiting nextNumberChecker()");
     delay(500);
     return 1;      //successful round
     
  } // end of nextNumberChecker()

  /* ---------------------------------------------------------------------------
   CheckForMultipleButtonsPressed
 - Return Values:
 -   TRUE  = Multiple buttons were pressed
 -   FALSE = one or 0 buttons were pressed.
 -
 - This routine checks to see if multiple pushbuttons are pressed at the same
 - time.  This is done by counting the number of pushbuttons that are sending a
 - LOW signal to the Arduino's digital I/O pins
   ---------------------------------------------------------------------------*/
int CheckForMultipleButtonsPressed( int button_1_state,
                                    int button_2_state,
                                    int button_3_state,
                                    int button_4_state,
                                    int button_5_state)
{
   int return_value = TRUE ;
   int number_of_buttons_held = 0 ;

   if ( TRUE == MULTIPLE_BUTTONS_PUSHED )
   {
//      sprintf( print_output, "Entering CheckForMultipleButtonsPressed\r\n" ) ;
//      Serial.print(print_output) ;
   }

   // count the number of buttons pressed
   if ( button_1_state == LOW )
   {
      number_of_buttons_held++ ;
   }
   if ( button_2_state == LOW )
   {
      number_of_buttons_held++ ;
   }
   if ( button_3_state == LOW )
   {
      number_of_buttons_held++ ;
   }
   if ( button_4_state == LOW )
   {
      number_of_buttons_held++ ;
   }
   if ( button_5_state == LOW )
   {
      number_of_buttons_held++ ;
   }
   
   // if more than 1 button is pressed return TRUE.
   if ( number_of_buttons_held > 1 )
   {
      return_value = TRUE ;
   }
   
   // if 1 or no buttons are pressed return false.
   else
   {
      return_value = FALSE ;
   }

   if ( TRUE == MULTIPLE_BUTTONS_PUSHED )
   {
//      sprintf( print_output, 
//               "Exiting CheckForMultipleButtonsPressed. Return = %d\r\n",
//               return_value ) ;
      Serial.print(print_output) ;
   }
      
   return return_value ;
}


//###########################################################################################
//######################################### SETUP()  LOOP####################################
//###########################################################################################


void setup() {

  //Prep reset pin by sending a HIGH signal.  Sending a LOW signal will reset the board/game
  //pinMode(RESET_PIN, INPUT);
  //digitalWrite(RESET_PIN, HIGH);

  //initialize LED's as output and buttons as input
  for (int i = 0; i < numberOfPins; i++) 
    {
      pinMode(ledPin[i], OUTPUT);
      pinMode(buttonPin[i], INPUT);
    }
  //initialize speaker
  pinMode(SPEAKER, OUTPUT);

  //initialize KEYPAD_CONTROLLER_PIN
  pinMode(KEYPAD_CONTROLLER_PIN, OUTPUT);
  digitalWrite(KEYPAD_CONTROLLER_PIN, HIGH);

  //initialize randomization elements
  pinMode(A1, INPUT); // for randomSeed()
  randomSeed(analogRead(A1));

  //start monitor
  Serial.begin(9600);
  sprintf( print_output, "\r\n Freshly Loaded Program\r\n") ;
  Serial.print(print_output) ; 
  Serial.println();
  
}

// --------------------------------------------------------------------------------------------------------------------

/**
* PlayWinSequence
* plays the r2d2 sounds
*/
       
void PlayWinSequence()
{
    tone(SPEAKER, NOTE_A7); //A
    TurnOffLEDs();
    delay(100);
    tone(SPEAKER, NOTE_G7,200); //G
    TurnOnLEDs();
    delay(100);
    tone(SPEAKER, NOTE_E7,200); //E
    TurnOffLEDs();
    delay(100);
    tone(SPEAKER, NOTE_C7,200); //C
    TurnOnLEDs();
    delay(100);
    tone(SPEAKER, NOTE_D7,200); //D
    TurnOffLEDs();
    delay(100);
    tone(SPEAKER, NOTE_B7,200); //B
    TurnOnLEDs();
    delay(100);
    tone(SPEAKER, NOTE_F7,200); //F
    TurnOffLEDs();
    delay(100);
    tone(SPEAKER, NOTE_C8,200); //C
    TurnOnLEDs();
    delay(100);
    tone(SPEAKER, NOTE_A7,200); //A
    TurnOffLEDs();
    delay(100);
    tone(SPEAKER, NOTE_G7,200); //G
    TurnOnLEDs();
    delay(100);
    tone(SPEAKER, NOTE_E7,200); //E
    TurnOffLEDs();
    delay(100);
    tone(SPEAKER, NOTE_C7,200); //C
    TurnOnLEDs();
    delay(100);
    tone(SPEAKER, NOTE_D7,200); //D
    TurnOffLEDs();
    delay(100);
    tone(SPEAKER, NOTE_B7,200); //B
    TurnOnLEDs();
    delay(100);
    tone(SPEAKER, NOTE_F7,200); //F
    TurnOffLEDs();
    delay(100);
    tone(SPEAKER, NOTE_C8,200); //C
    TurnOnLEDs();
    delay(100);
    noTone(SPEAKER);
}

//#####################################################################################################
//########################################## MAIN LOOP ################################################
//#####################################################################################################


void loop() {


//NEW GAME START:  
//start with lights all on
//play lights and sounds & create game sequence when a player pushes a button

//DO:  SET UP WAITING MODE

  
  
  if (newGame)
  {   

    waitingState();
    
    createSequence();

    startOfGameDisplay();

    newGame = 0;
    
    newRound = 1;
    scoreKeeper = 0;
    nextNumber = 0;
    lastPressed = 99; //holds number of last button pressed
    nextButton = 0;

    
  } 


//MAIN GAME LOOP
  Serial.println("MAIN GAME LOOP");
  
  do
    { 
      Serial.println("\tnewRound = " + String(newRound));
      
      if (newRound > -1)
        {
          Serial.println("\tNEW ROUND!");
          
          playCurrentSeq(scoreKeeper);
          
          newRound = nextNumberChecker(scoreKeeper); //check for player input - returns 1 if player is proceeding

          scoreKeeper++;
        } 
        else 
        {
          Serial.print("\tnextNumberChecker() returned" + String(newRound)); 
          scoreKeeper = 0;
          newGame = 1;
          newRound = 0;
          nextNumber = 0;
          lastPressed = -99; //holds number of last button pressed
          nextButton = 0;
          break;
        }
    
    }
    
  while (scoreKeeper <= GAME_LENGTH); //end of do-while

    
    // WIN TESTER
    
      Serial.println("Entering Win Tester");
      Serial.println("\tscoreKeeper = " + String(scoreKeeper) + "GAME_LENGTH" + String(GAME_LENGTH));
      
      if (scoreKeeper > GAME_LENGTH) 
      {
        
          PlayWinSequence();

          //turn off lasers via communication with Pat's electronics
          digitalWrite(KEYPAD_CONTROLLER_PIN, LOW);
          Serial.println("WROTE LOW");
          delay(3000);
          digitalWrite(KEYPAD_CONTROLLER_PIN, HIGH);
          Serial.println("WROTE HIGH");

          Serial.println("WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!WIN!!!");          
          
          delay(3000);
          newGame = 1;
        
    
       
      }


}// end of void loop()
