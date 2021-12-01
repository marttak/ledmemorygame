#include <LiquidCrystal.h>

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
      level++; //next level
      int next_led = rand() % 4; //draws the next led for the sequence randomly
      rand_Array[level] = next_led; //input the randomized next_led to rand_Array in the place of current level
      lcd.setCursor(0,0);
      lcd.print("Current Level: ");
      lcd.print(level + 1);
 
      //iterate over the sequence
      //shows the users what leds to remember
      for (int i = 0; i <= level; i++){
        digitalWrite(led_Pins[rand_Array[i]], HIGH);
        tone(buzzer, buzzer_Array[rand_Array[i]], 500);
        delay(500);
        digitalWrite(led_Pins[rand_Array[i]], LOW);
        delay(500);
       }
       
      //checks the user input
      mistake = input(level);
        
     } while(!mistake && level < ROUNDS_TO_WIN); //do while the user doesn't make a mistake and the max number of rounds is not passed

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
        digitalWrite(ledPin1, HIGH);
        tone(buzzer, 800, 500);
        delay(500);
        digitalWrite(ledPin1, LOW);
        user_input[x] = 0;
        //check the user_input against the rand_Array
        //if they don't match, returns true to game function
        //mistake becomes true -> game over
        if (user_input[x] != rand_Array[x]){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Game Over");
          return true;
        }
        x++;
      }
      
      if(buttonState == LOW && button_Pins[y] == 19){
        digitalWrite(ledPin2, HIGH);
        tone(buzzer, 1000, 500);
        delay(500);
        digitalWrite(ledPin2, LOW);
        user_input[x] = 1;
        if (user_input[x] != rand_Array[x]){
          return true;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Game Over");
        }
        x++;
      }
      
      if(buttonState == LOW && button_Pins[y] == 20){
        digitalWrite(ledPin3, HIGH);
        tone(buzzer, 1200, 500);
        delay(500);
        digitalWrite(ledPin3, LOW);
        user_input[x] = 2;
        if (user_input[x] != rand_Array[x]){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Game Over");
          return true;
        }
        x++;
      }
      
      if(buttonState == LOW && button_Pins[y] == 21){
        digitalWrite(ledPin4, HIGH);
        tone(buzzer, 1400, 500);
        delay(500);
        digitalWrite(ledPin4, LOW);
        user_input[x] = 3;
        if (user_input[x] != rand_Array[x]){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Game Over");
          return true;
        }
        x++;
      }
      
    }
  }

  score++;
  delay(500);
  return false; //return false when the user passes the level without mistake
  
}
