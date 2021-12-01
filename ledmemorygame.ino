#include <LiquidCrystal.h>
#include <avr/wdt.h>

//define game over notes
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

//buttons
const int buttonPin1 = 18;
const int buttonPin2 = 19;
const int buttonPin3 = 20;
const int buttonPin4 = 21;
const int startButton = 12;

//leds
const int ledPin1 = 40;
const int ledPin2 = 41;
const int ledPin3 = 42;
const int ledPin4 = 43;

//lcd display pins
const int rs = 24;
const int en = 22;
const int d4 = 37;
const int d5 = 35;
const int d6 = 33;
const int d7 = 31;

//piezo
const int buzzer = 53;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

volatile bool mistake = false;
int run;
volatile int score = 0; //user score

volatile int ROUNDS_TO_WIN = 250; //how many rounds it takes for the game to reach the end
                                 //first level is 0

int rand_Array[249]; //array to store the sequence to remember
int user_input[249]; //array to store the sequence user guesses

//Underwolrd tempo
int underworld_tempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};

//Underworld melody
int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};

void setup() {
  noInterrupts();
  //initialize LED pins as outputs:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  //initialize button pins as inputs:
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);
  
  lcd.begin(16,2);
  Serial.begin(9600);
  run = 0;

  wdt_enable(WDTO_8S);

  interrupts();
}

void loop() {

  //prompt the user to press startbutton
  lcd.setCursor(0,0);
  lcd.print("Press White Button");
  lcd.setCursor(0,1);
  lcd.print("to Start");
  Serial.println("Press White Button to Start");

  //if the game is not already on, start the game when startbutton is pressed
  if (digitalRead(startButton) == LOW){
    if (run == 0){
      run = 255;
    }else{
      run = 0;
     }
    }
    
  if(run > 0){
    wdt_reset();
    delay(1000);
    game();
  }
}

//function for the game loop
void game(){
  lcd.clear();
  score = 0;
  int led_Pins[4]={40, 41, 42, 43}; //array to store the led pins
  int buzzer_Array[4]={800, 1000, 1200, 1400};
  int level = -1; // -1 because we want to start the level from 0

     do{
      wdt_reset();
      level++; //next level
      int next_led = rand() % 4; //draws the next led for the sequence randomly
      rand_Array[level] = next_led; //input the randomized next_led to rand_Array in the place of current level
      lcd.setCursor(0,0);
      lcd.print("Current Level: ");
      lcd.print(level + 1);
 
      //iterate over the sequence
      //shows the users what leds to remember
      for (int i = 0; i <= level; i++){
        wdt_reset();
        digitalWrite(led_Pins[rand_Array[i]], HIGH);
        tone(buzzer, buzzer_Array[rand_Array[i]], 500);
        delay(500);
        digitalWrite(led_Pins[rand_Array[i]], LOW);
        delay(500);
        wdt_reset();
       }
       
      //checks the user input
      mistake = input(level);
        
     } while(!mistake && level < ROUNDS_TO_WIN); //do while the user doesn't make a mistake and the max number of rounds is not passed

  wdt_reset();
  Serial.println("Your score: ");
  Serial.print(score);
  delay(1000);
  
  //after the game is finished (user makes a mistake or max numbers of rounds is passed) return to main loop
  run = 0;
}

//function to get user input and check the input
int input(int level){
  int button_Pins[4]={18,19,20,21}; //array to store button pins

  //statement controlled by level count
  for(int x = 0; x <= level;){
    
    for(int y = 0; y < 4; y++){
      
      int buttonState = digitalRead(button_Pins[y]); //read buttonstate of 

      //checking for button push
      if(buttonState == LOW && button_Pins[y] == 18){
        wdt_reset();
        digitalWrite(ledPin1, HIGH);
        tone(buzzer, 800, 500);
        delay(500);
        digitalWrite(ledPin1, LOW);
        user_input[x] = 0;
        wdt_reset();
        //check the user_input against the rand_Array
        //if they don't match, returns true to game function
        //mistake becomes true -> game over
        if (user_input[x] != rand_Array[x]){
          wdt_reset();
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Game Over");
          return true;
          wdt_reset();
        }
        x++;
      }
      
      if(buttonState == LOW && button_Pins[y] == 19){
        wdt_reset();
        digitalWrite(ledPin2, HIGH);
        tone(buzzer, 1000, 500);
        delay(500);
        digitalWrite(ledPin2, LOW);
        user_input[x] = 1;
        wdt_reset();
        if (user_input[x] != rand_Array[x]){
          wdt_reset();
          return true;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Game Over");
          wdt_reset();
        }
        x++;
      }
      
      if(buttonState == LOW && button_Pins[y] == 20){
        wdt_reset();
        digitalWrite(ledPin3, HIGH);
        tone(buzzer, 1200, 500);
        delay(500);
        digitalWrite(ledPin3, LOW);
        user_input[x] = 2;
        wdt_reset();
        if (user_input[x] != rand_Array[x]){
          wdt_reset();
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Game Over");
          return true;
          wdt_reset();
        }
        x++;
      }
      
      if(buttonState == LOW && button_Pins[y] == 21){
        wdt_reset();
        digitalWrite(ledPin4, HIGH);
        tone(buzzer, 1400, 500);
        delay(500);
        digitalWrite(ledPin4, LOW);
        user_input[x] = 3;
        wdt_reset();
        if (user_input[x] != rand_Array[x]){
          wdt_reset();
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Game Over");
          return true;
          wdt_reset();
        }
        x++;
      }
      
    }
  }

  score++;
  delay(500);
  return false; //return false when the user passes the level without mistake
  
}
