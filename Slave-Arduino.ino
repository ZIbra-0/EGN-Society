#include <Wire.h>
#include <LiquidCrystal.h>

#define SLAVE_ADDR 9
#define ANSWER_SIZE 5
#define MESSAGE_SIZE 32

String answer = "hello";

char receivedMessage[MESSAGE_SIZE];
volatile byte messageIndex = 0;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Level: ");

  // put your setup code here, to run once:
  Wire.begin(SLAVE_ADDR);

  // Wire.onRequest(requestEvent);

  Wire.onReceive(receiveEvent);

  Serial.begin(9600);
}

void setLevel(String level)
{
  lcd.setCursor(7, 0);
  lcd.print("  ");
  lcd.setCursor(7, 0);
  lcd.print(level);
}

void runTimer(int time)
{
  lcd.setCursor(0, 1);

  // Print time remaining
  lcd.print("Time: ");

  int timeRemaining = time;

  // Countdown
  while (timeRemaining > 9)
  {
    lcd.setCursor(6, 1);

    lcd.print(timeRemaining);

    delay(1000000/5);
    timeRemaining--;
  }

  lcd.setCursor(7, 1);
  lcd.print(" ");

  while (timeRemaining >= 0)
  {
    lcd.setCursor(6, 1);

    lcd.print(timeRemaining);

    delay(1000000/5);
    timeRemaining--;
  }

  if (timeRemaining <= 0)
  {
    tone(8, 50, 3000);
    delay(3000);
  
    return;
  }
}

void receiveEvent()
{
  messageIndex = 0;

  while (Wire.available() && messageIndex < MESSAGE_SIZE - 1)
  {
    char c = Wire.read();

    if (c == '\0')
    {
      receivedMessage[messageIndex] = '\0';

      Serial.print("Received: ");
      Serial.println(receivedMessage);
      messageIndex = 0;

      String receivedMessageString = String(receivedMessage);
      
      if (receivedMessageString.indexOf("Level") != -1)
      {
        String level = receivedMessageString.substring(receivedMessageString.indexOf("=")+1);

        setLevel(level);
      }

      else if (receivedMessageString.indexOf("Time") != -1)
      {
        int time = receivedMessageString.substring(receivedMessageString.indexOf("=")+1).toInt();
        runTimer(time);
      }

      return;
    }

    receivedMessage[messageIndex] = c;
    messageIndex++;
  }

  receivedMessage[messageIndex] = '\0';
}

void loop() 
{
  delay(50);
}
