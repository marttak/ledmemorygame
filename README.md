# Led memory game
This is a group project for a course about embedded system programming and microcontrollers.
### Components:
- Arduino Mega 2560
- 5 pushbuttons
- 4 leds
- buzzer
- lcd display

### The goals for this project are to:
- use four AVR blocks, which are: watchdog timer, EEPROM, Timer 2 library (tune) and TWI block (I2C-adapter)
- create a two-way interface
- practice all the things learned in this course
## What does it do?
The game starts when the player presses the white button. The objective is to remember the correct order in which the leds are being displayed. Starting from one led being displayed, the game gets harder on each level when a new randomly picked led is added to the sequence. To help memorize, the buzzer plays a sound for each different led.

After the system has shown the player the sequence, the player must press the pushbuttons (each led has its own pushbutton) in correct order. If the player guesses right, next level begins. If the player guesses wrong, it's game over.