// Projected Pin Layout
/*
  LED Pins:      1, 2, 3, 4
  Button Pins:   5, 6, 7, 8
  Undo Button:   9
  Reset Button:  10
*/

#include <Wire.h>

#define SLAVE_ADDR 9
#define ANSWER_SIZE 5
#define MESSAGE_SIZE 32

// Game Variables
int lvl1[5];           // Sequence array for level 1
int lvl2[8];           // Sequence array for level 2
int lvl3[12];          // Sequence array for level 3
int lvl4[15];          // Sequence array for level 4
int lvl5[20];          // Sequence array for level 5
int userInput[20];     // To store user inputs for comparison
int sequenceSize = 5;  // Current sequence size (starting at level 1)
const unsigned long inputTimeLimit = 10000; // 10 seconds for input time limit
int userInputIndex = 1;
int difficultyLevel = 1;

// Function to generate a random number (10-13)
int flash(int previousNumber) 
{
  int randomInt = random(10, 14);

  while (randomInt == previousNumber)
  {
    randomInt = random(10, 14);
  }

  return randomInt;
}

// Fill the sequence arrays for each level
void level1() 
{
  lvl1[0] = flash(-1);

  for (int i = 1; i < 5; i++) 
  {
    lvl1[i] = flash(lvl1[i-1]);
  }
}

void level2() 
{
  lvl2[0] = flash(-1);
  
  for (int i = 1; i < 8; i++) 
  {
    lvl2[i] = flash(lvl2[i-1]);
  }
}

void level3() 
{
  lvl3[0] = flash(-1);

  for (int i = 1; i < 12; i++) 
  {
    lvl3[i] = flash(lvl3[i-1]);
  }
}

void level4() 
{
  lvl4[0] = flash(-1);

  for (int i = 1; i < 15; i++) 
  {
    lvl4[i] = flash(lvl4[i-1]);
  }
}

void level5() 
{
  lvl5[0] = flash(-1);

  for (int i = 1; i < 20; i++) 
  {
    lvl5[i] = flash(lvl5[i-1]);
  }
}

// Function to send a signal to a light (turn it on and off)
void sendSignal(int light) 
{
  digitalWrite(light, HIGH); 
  delay(500);                
  digitalWrite(light, LOW);  
  delay(600);                
}

void makeLEDPinsOutput()
{
  pinMode(10, OUTPUT); // Light 1
  pinMode(11, OUTPUT); // Light 2
  pinMode(12, OUTPUT); // Light 3
  pinMode(13, OUTPUT); // Light 4
}

void makeLEDPinsInput()
{
  pinMode(10, INPUT); // Light 1
  pinMode(11, INPUT); // Light 2
  pinMode(12, INPUT); // Light 3
  pinMode(13, INPUT); // Light 4
}

// Function to activate lights based on the array (flash each LED in sequence)
void activateLights(int arr[], int size) 
{
  delay(300);

  for (int i = 0; i < size; i++) 
  {
    sendSignal(arr[i]); // Flash the light corresponding to the value in the array
  }
}

// Function to get user input
bool getUserInput(int arr[], int size, int correctSequence[]) 
{
  makeLEDPinsInput();

  int time = inputTimeLimit/1000;
  String message = "Time=" + String(time);  


  // Debug print before sending
  Serial.print("About to send message: ");
  Serial.println(message);
  
  Wire.beginTransmission(SLAVE_ADDR);
  
  // Print each byte as it's being sent
  for (int i = 0; i < message.length(); i++) 
  {
      byte b = message.charAt(i);
      Wire.write(b);
      
      // // Debug print in hex and ASCII
      // Serial.print("Sending byte ");
      // Serial.print(i);
      // Serial.print(": 0x");
      // Serial.print(b, HEX);
      // Serial.print(" (");
      // Serial.print((char)b);
      // Serial.println(")");
  }
  
  Wire.write('\0');  // Null terminator
  byte error = Wire.endTransmission();

  arr[0] = 0;
  userInputIndex = 1; // Tracks the number of inputs received
  unsigned long startTime = millis(); // Record the start time

  while (millis() - startTime <= inputTimeLimit) 
  {
    if (digitalRead(10) == HIGH && arr[userInputIndex-1] != 10) 
    {
      Serial.println("10 High");
      arr[userInputIndex] = 10;
      // sendSignal(1); 
      userInputIndex++;
    } 
    else if (digitalRead(11) == HIGH && arr[userInputIndex-1] != 11) 
    {
      Serial.println("11 High");
      arr[userInputIndex] = 11;
      // sendSignal(2); 
      userInputIndex++;
    } 
    else if (digitalRead(12) == HIGH && arr[userInputIndex-1] != 12) 
    {
      Serial.println("12 High");
      arr[userInputIndex] = 12;
      // sendSignal(3); 
      userInputIndex++;
    } 
    else if (digitalRead(13) == HIGH && arr[userInputIndex-1] != 13) 
    {
      Serial.println("13 High");
      arr[userInputIndex] = 13;
      // sendSignal(4); 
      userInputIndex++;
    }

    // Reset switch
    else if (digitalRead(8) == HIGH)
    {
      Serial.println("RESET HIT");

      resetResponse();
    }

    // Ensure buttons are released before accepting the next input
    // while (digitalRead(10) == HIGH || digitalRead(11) == HIGH || digitalRead(12) == HIGH ||
    //        digitalRead(13) == HIGH )
    //       //  || digitalRead(9) == LOW || digitalRead(10) == LOW) 
    // {
    //   delay(5); // Short delay to avoid detecting multiple presses
    // }
  }

  // Check if the user's input matches the sequence
  for (int i = 1; i < size; i++) 
  {
    if (arr[i] != correctSequence[i-1]) 
    {
      difficultyLevel = 1;
      return false; // Incorrect sequence
    }
  }

  difficultyLevel++;
  return true; // Correct sequence
}

void flashCorrectAnswerLED()
{
  delay(500);

  unsigned long startTime = millis();
  
  while (millis() - startTime <= 2500) 
  {
    digitalWrite(7, HIGH);
    delay(100);
    digitalWrite(7, LOW);
    delay(100);
  }

  return;
}

void flashIncorrectAnswerLED()
{
  delay(500);

  unsigned long startTime = millis();

  while (millis() - startTime <= 2500) 
  {
    digitalWrite(6, HIGH);
    delay(100);
    digitalWrite(6, LOW);
    delay(100);
  }

  return;
}

void resetResponse()
{
  userInputIndex = 1;

  unsigned long startTime = millis();
  while (millis() - startTime <= 300) 
  {
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    delay(50);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    delay(50);
  }
}

void setup() 
{
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  Serial.begin(9600);
  Wire.begin();


  // pinMode(9, INPUT_PULLUP); // Undo button
  // pinMode(10, INPUT_PULLUP); // Reset button

  // Seed the random number generator for randomness
  randomSeed(analogRead(A0));

  // Generate the initial sequence for Level 1
  level1();
}

void loop() 
{
  String message = "Level=" + String(difficultyLevel);

  // Display the sequence
  if (sequenceSize == 5) 
  {    
    Wire.beginTransmission(SLAVE_ADDR);
    
    for (int i = 0; i < message.length(); i++) 
    {
        byte b = message.charAt(i);
        Wire.write(b);
    }
    
    Wire.write('\0');  // Null terminator
    byte error = Wire.endTransmission();
    
    Serial.println("Generating random sequence of 5");
    for (int i = 0; i < sizeof(lvl1); i++)
      Serial.println(lvl1[i]);

    makeLEDPinsOutput();
    activateLights(lvl1, sequenceSize);

    if (getUserInput(userInput, sequenceSize, lvl1)) 
    {
      Serial.println("CORRECT!!!!");
      flashCorrectAnswerLED();

      delay(2000); // Pause before next level
      sequenceSize++; // Increase the sequence size for the next level
      if (sequenceSize > 5) 
      {
        sequenceSize = 5; // Limit to 5 levels
      }
      level1(); // Generate a new sequence for the next level
    } 
    
    else 
    {
      Serial.println("INCORRECT!!!");
      flashIncorrectAnswerLED();

      delay(2000);

      for (int i = 0; i < sizeof(userInput); i++)
        if (userInput[i] == 0)
          break;
        else
          Serial.println(userInput[i]); 

      sequenceSize = 5; // Reset the sequence size
      level1(); // Restart the sequence from level 1
    }
  } 
  else if (sequenceSize == 8) 
  {
    Serial.println("Generating random sequence of 8");
    
    Wire.beginTransmission(SLAVE_ADDR);
    
    for (int i = 0; i < message.length(); i++) 
    {
        byte b = message.charAt(i);
        Wire.write(b);
    }
    
    Wire.write('\0');  // Null terminator
    byte error = Wire.endTransmission();

    makeLEDPinsOutput();
    activateLights(lvl2, sequenceSize);

    if (getUserInput(userInput, sequenceSize, lvl2)) 
    {
      Serial.println("CORRECT!!!!");
      flashCorrectAnswerLED();

      // If correct, move to the next level
      delay(2000); // Pause before next level
      sequenceSize++; // Increase the sequence size for the next level
      if (sequenceSize > 5) {
        sequenceSize = 5; // Limit to 5 levels
      }
      level1(); // Generate a new sequence for the next level
    } 
    else 
    {
      Serial.println("INCORRECT!!!");
      flashIncorrectAnswerLED();

      delay(2000);

      for (int i = 0; i < sizeof(userInput); i++)
        if (userInput[i] == 0)
          break;
        else
          Serial.println(userInput[i]); 
      sequenceSize = 5; // Reset the sequence size
      level1(); // Restart the sequence from level 1
    }
  } 
  else if (sequenceSize == 12) 
  {
    Serial.println("Generating random sequence of 12");
    
    Wire.beginTransmission(SLAVE_ADDR);
    
    for (int i = 0; i < message.length(); i++) 
    {
        byte b = message.charAt(i);
        Wire.write(b);
    }
    
    Wire.write('\0');  // Null terminator
    byte error = Wire.endTransmission();


    makeLEDPinsOutput();
    activateLights(lvl3, sequenceSize);

    if (getUserInput(userInput, sequenceSize, lvl3)) 
    {
      Serial.println("CORRECT!!!!");
      flashCorrectAnswerLED();
      
      // If correct, move to the next level
      delay(2000); // Pause before next level
      sequenceSize++; // Increase the sequence size for the next level
      if (sequenceSize > 5) {
        sequenceSize = 5; // Limit to 5 levels
      }
      level1(); // Generate a new sequence for the next level
    } 
    else 
    {
      Serial.println("INCORRECT!!!");
      flashIncorrectAnswerLED();

      delay(2000);

      for (int i = 0; i < sizeof(userInput); i++)
        if (userInput[i] == 0)
          break;
        else
          Serial.println(userInput[i]); 
      sequenceSize = 5; // Reset the sequence size
      level1(); // Restart the sequence from level 1
    }
  } 
  else if (sequenceSize == 15) 
  {
    Serial.println("Generating random sequence of 15");
    
    Wire.beginTransmission(SLAVE_ADDR);
    
    for (int i = 0; i < message.length(); i++) 
    {
        byte b = message.charAt(i);
        Wire.write(b);
    }
    
    Wire.write('\0');  // Null terminator
    byte error = Wire.endTransmission();

    makeLEDPinsOutput();
    activateLights(lvl4, sequenceSize);

    if (getUserInput(userInput, sequenceSize, lvl4)) 
    {
      Serial.println("CORRECT!!!!");
      flashCorrectAnswerLED();
      
      // If correct, move to the next level
      delay(2000); // Pause before next level
      sequenceSize++; // Increase the sequence size for the next level
      if (sequenceSize > 5) {
        sequenceSize = 5; // Limit to 5 levels
      }
      level1(); // Generate a new sequence for the next level
    } 
    else 
    {
      Serial.println("INCORRECT!!!");
      flashIncorrectAnswerLED();

      delay(2000);

      for (int i = 0; i < sizeof(userInput); i++)
        if (userInput[i] == 0)
          break;
        else
          Serial.println(userInput[i]); 
      sequenceSize = 5; // Reset the sequence size
      level1(); // Restart the sequence from level 1
    }
  } 
  else if (sequenceSize == 20) 
  {
    makeLEDPinsOutput();
        
    Wire.beginTransmission(SLAVE_ADDR);
    
    for (int i = 0; i < message.length(); i++) 
    {
        byte b = message.charAt(i);
        Wire.write(b);
    }
    
    Wire.write('\0');  // Null terminator
    byte error = Wire.endTransmission();

    activateLights(lvl5, sequenceSize);

    if (getUserInput(userInput, sequenceSize, lvl5)) 
    {
      Serial.println("CORRECT!!!!");
      flashCorrectAnswerLED();      
      
      // If correct, move to the next level
      delay(2000); // Pause before next level
      sequenceSize++; // Increase the sequence size for the next level
      if (sequenceSize > 5) 
      {
        sequenceSize = 5; // Limit to 5 levels
      }
      level1(); // Generate a new sequence for the next level
    } 
    else 
    {
      Serial.println("INCORRECT!!!");
      flashIncorrectAnswerLED();

      delay(2000);

      for (int i = 0; i < sizeof(userInput); i++)
        if (userInput[i] == 0)
          break;
        else
          Serial.println(userInput[i]); 
      sequenceSize = 5; // Reset the sequence size
      level1(); // Restart the sequence from level 1
    }
  }


  delay(2000); // Pause before showing the sequence again
}