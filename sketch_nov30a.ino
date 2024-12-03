// Projected Pin Layout
/*
  LED Pins:      1, 2, 3, 4
  Button Pins:   5, 6, 7, 8
  Undo Button:   9
  Reset Button:  10
*/

// Game Variables
int lvl1[5];           // Sequence array for level 1
int lvl2[8];           // Sequence array for level 2
int lvl3[12];          // Sequence array for level 3
int lvl4[15];          // Sequence array for level 4
int lvl5[20];          // Sequence array for level 5
int userInput[20];     // To store user inputs for comparison
int sequenceSize = 5;  // Current sequence size (starting at level 1)
const unsigned long inputTimeLimit = 20000; // 20 seconds for input time limit

// Function to generate a random number (1-4)
int flash() {
  return random(1, 5);
}

// Fill the sequence arrays for each level
void level1() {
  for (int i = 0; i < 5; i++) {
    lvl1[i] = flash(); // Generate random light sequence
  }
}

void level2() {
  for (int i = 0; i < 8; i++) {
    lvl2[i] = flash();
  }
}

void level3() {
  for (int i = 0; i < 12; i++) {
    lvl3[i] = flash();
  }
}

void level4() {
  for (int i = 0; i < 15; i++) {
    lvl4[i] = flash();
  }
}

void level5() {
  for (int i = 0; i < 20; i++) {
    lvl5[i] = flash();
  }
}

// Function to send a signal to a light (turn it on and off)
void sendSignal(int light) {
  digitalWrite(light, HIGH); 
  delay(500);                
  digitalWrite(light, LOW);  
  delay(250);                
}

// Function to activate lights based on the array (flash each LED in sequence)
void activateLights(int arr[], int size) {
  for (int i = 0; i < size; i++) {
    sendSignal(arr[i]); // Flash the light corresponding to the value in the array
  }
}

// Function to get user input
bool getUserInput(int arr[], int size) {
  int userInputIndex = 0; // Tracks the number of inputs received
  unsigned long startTime = millis(); // Record the start time

  while (userInputIndex < size) {
    // Check if time limit is exceeded
    if (millis() - startTime > inputTimeLimit) {
      return false; // 
    }

    // Check for button presses and record inputs
    if (digitalRead(5) == LOW) { // Button 1 pressed
      arr[userInputIndex] = 1;
      sendSignal(1); 
      userInputIndex++;
      delay(200);
    } else if (digitalRead(6) == LOW) { // Button 2 pressed
      arr[userInputIndex] = 2;
      sendSignal(2); 
      userInputIndex++;
      delay(200);
    } else if (digitalRead(7) == LOW) { // Button 3 pressed
      arr[userInputIndex] = 3;
      sendSignal(3); 
      userInputIndex++;
      delay(200);
    } else if (digitalRead(8) == LOW) { // Button 4 pressed
      arr[userInputIndex] = 4;
      sendSignal(4); 
      userInputIndex++;
      delay(200);
    }

    // Undo button (pin 9) pressed
    if (digitalRead(9) == LOW) {
      if (userInputIndex > 0) {
        userInputIndex--; // Remove the last input
      }
      delay(200); 
    }

    // Reset button (pin 10) pressed
    if (digitalRead(10) == LOW) {
      userInputIndex = 0; // Clear all inputs
      delay(200); 
    }

    // Ensure buttons are released before accepting the next input
    while (digitalRead(5) == LOW || digitalRead(6) == LOW || digitalRead(7) == LOW ||
           digitalRead(8) == LOW || digitalRead(9) == LOW || digitalRead(10) == LOW) {
      delay(10); // Short delay to avoid detecting multiple presses
    }
  }

  // Check if the user's input matches the sequence
  for (int i = 0; i < size; i++) {
    if (arr[i] != lvl1[i] && arr[i] != lvl2[i] && arr[i] != lvl3[i] && arr[i] != lvl4[i] && arr[i] != lvl5[i]) {
      return false; // Incorrect sequence
    }
  }
  return true; // Correct sequence
}

void setup() {
  // Initialize LED pins as OUTPUT
  pinMode(1, OUTPUT); // Light 1
  pinMode(2, OUTPUT); // Light 2
  pinMode(3, OUTPUT); // Light 3
  pinMode(4, OUTPUT); // Light 4

  // Initialize button pins as INPUT with internal pull-up resistors
  pinMode(5, INPUT_PULLUP); // Button 1
  pinMode(6, INPUT_PULLUP); // Button 2
  pinMode(7, INPUT_PULLUP); // Button 3
  pinMode(8, INPUT_PULLUP); // Button 4
  pinMode(9, INPUT_PULLUP); // Undo button
  pinMode(10, INPUT_PULLUP); // Reset button

  // Seed the random number generator for randomness
  randomSeed(analogRead(A0));

  // Generate the initial sequence for Level 1
  level1();
}

void loop() {
  // Display the sequence
  if (sequenceSize == 5) {
    activateLights(lvl1, sequenceSize);
  } else if (sequenceSize == 8) {
    activateLights(lvl2, sequenceSize);
  } else if (sequenceSize == 12) {
    activateLights(lvl3, sequenceSize);
  } else if (sequenceSize == 15) {
    activateLights(lvl4, sequenceSize);
  } else if (sequenceSize == 20) {
    activateLights(lvl5, sequenceSize);
  }

  // Wait for user input and check if it's correct
  if (getUserInput(userInput, sequenceSize)) {
    // If correct, move to the next level
    delay(1000); // Pause before next level
    sequenceSize++; // Increase the sequence size for the next level
    if (sequenceSize > 5) {
      sequenceSize = 5; // Limit to 5 levels
    }
    level1(); // Generate a new sequence for the next level
  } else {
    // If incorrect, flash all LEDs to indicate failure
    for (int i = 0; i < 3; i++) {
      digitalWrite(1, HIGH);
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      delay(300);
      digitalWrite(1, LOW);
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      delay(300);
    }
    sequenceSize = 5; // Reset the sequence size
    level1(); // Restart the sequence from level 1
  }

  delay(2000); // Pause before showing the sequence again
}


