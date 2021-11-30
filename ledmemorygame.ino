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

volatile bool mistake = false;
int run;

volatile int ROUNDS_TO_WIN = 5; //how many rounds it takes for the game to reach the end

int rand_Array[5]; //array to store the sequence to remember
int user_input[5]; //array to store the sequence user guesses

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

  Serial.begin(9600);
  run = 0;

  interrupts();
}

void loop() {

  //prompt the user to press startbutton
  Serial.println("Press white button to start");

  //if the game is not already on, start the game when startbutton is pressed
  if (digitalRead(startButton) == LOW){
    if (run == 0){
      run = 255;
    }else{
      run = 0;
     }
    }
    
  if(run > 0){
    game();
    delay(1000);
  }
}

//function for the game loop
void game(){
  int led_Pins[4]={40, 41, 42, 43}; //array to store the led pins
  int level = -1; // -1 because we want to start the level from 0

     do{
      level++; //next level
      int next_led = rand() % 4; //draws the next led for the sequence randomly
      rand_Array[level] = next_led; //input the randomized next_led to rand_Array in the place of current level

      //iterate over the sequence
      //shows the users what leds to remember
      for (int i = 0; i <= level; i++){
        digitalWrite(led_Pins[rand_Array[i]], HIGH);
        delay(1000);
        digitalWrite(led_Pins[rand_Array[i]], LOW);
        delay(1000);
       }
       
      //checks the user input
      mistake = input(level);
        
     } while(!mistake && level < ROUNDS_TO_WIN); //do while the user doesn't make a mistake and the max number of rounds is not passed
  
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
        delay(1000);
        digitalWrite(ledPin1, LOW);
        user_input[x] = 0;
        Serial.println(user_input[x]);
        //check the user_input against the rand_Array
        //if they don't match, returns true to game function
        //mistake becomes true -> game over
        if (user_input[x] != rand_Array[x]){
          return true;
        }
        x++;
      }
      
      if(buttonState == LOW && button_Pins[y] == 19){
        digitalWrite(ledPin2, HIGH);
        delay(1000);
        digitalWrite(ledPin2, LOW);
        user_input[x] = 1;
        Serial.println(user_input[x]);
        if (user_input[x] != rand_Array[x]){
          return true;
        }
        x++;
      }
      
      if(buttonState == LOW && button_Pins[y] == 20){
        digitalWrite(ledPin3, HIGH);
        delay(1000);
        digitalWrite(ledPin3, LOW);
        user_input[x] = 2;
        Serial.println(user_input[x]);
        if (user_input[x] != rand_Array[x]){
          return true;
        }
        x++;
      }
      
      if(buttonState == LOW && button_Pins[y] == 21){
        digitalWrite(ledPin4, HIGH);
        delay(1000);
        digitalWrite(ledPin4, LOW);
        user_input[x] = 3;
        Serial.println(user_input[x]);
        if (user_input[x] != rand_Array[x]){
          return true;
        }
        x++;
      }
      
    }
  }
  
  delay(500);
  return false; //return false when the user passes the level without mistake
  
}
