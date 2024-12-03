//Projected Pin layout
/*
  pinMode(1, OUTPUT);  //light
  pinMode(2, OUTPUT);  //light
  pinMode(3, OUTPUT);  //light
  pinMode(4, OUTPUT);  //light
  pinMode(X, OUTPUT);  //Piezo
  pinMode(X, OUTPUT);  //RGB Light ( idk how LCD pins gonna work)

  pinMode(X, INPUT);   //Button
  pinMode(X, INPUT);   //Button
  pinMode(X, INPUT);   //Button
  pinMode(X, INPUT);   //Button

*/
  int lvl1[3];
  int lvl2[5];
  int lvl3[8];
  int lvl4[10];
  int lvl5[12];
  int lvl6[15];
  int lvl7[20];

  int flash(){
    int randomLight = random(1, 5); //RNG using arduino random function
    return randomLight;
  }  
  
  void level1(){
    for (int i=0; i < 5; i++){
      lvl1[i] = flash();
    }
  }

void sendSignal(int light) {
  digitalWrite(light, HIGH); // Turn on the LED connected to the specified pin
  delay(500);                
  digitalWrite(light, LOW);  
  delay(250);                
}

// Function to activate lights based on the array
void activateLights(int arr[], int size) {
  for (int i = 0; i < size; i++) {
    sendSignal(arr[i]); // Send signal for the corresponding light
  }
}
  
void setup() { 
  pinMode(1, OUTPUT);  //light
  pinMode(2, OUTPUT);  //light
  pinMode(3, OUTPUT);  //light
  pinMode(4, OUTPUT);  //light
}

void loop() {
 level1();
 activateLights(lvl1, 5);
 delay(2000);
}
