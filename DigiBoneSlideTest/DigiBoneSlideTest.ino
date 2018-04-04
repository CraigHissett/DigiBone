
// Navigation button variables
int SlidePosition;
int readKey;
int pot1;
int pot2;
int potTotal;
int savedDistance = 0;;
int button = 0;
int rangevalue = 0;
int Play = 0;

#include <Wire.h>
#include <LiquidCrystal.h>
#include <NewPing.h>


// Setting the LCD shields pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Initialize the Ping Sensor for the slide
NewPing SlidePing(23, 22, 100);

void setup() {

  // Initializes serial communication
  Serial.begin(115200);

  // Initializes and clears the LCD screen
  lcd.begin(16, 2);
  lcd.clear();

  delay(100);
  lcd.setCursor(0, 0);
  lcd.print("Slide Test");
  delay(1000);

}

void loop() {
CheckButton();
delay(100);

}

// This function will generate the 2 menu items that can fit on the screen. They will change as you scroll through your menu. Up and down arrows will indicate your current menu position.

void PotDraw() {
  pot1 = analogRead(14);
  pot2 = analogRead(15);
  potTotal = pot1 + pot2;
  lcd.setCursor(12, 1);
  lcd.print(potTotal);
}

void ReadSlide() {
  
  lcd.setCursor(0, 1);
  lcd.write("Pos: ");
  lcd.setCursor(6, 1);
  lcd.print(SlidePosition);
}


void CheckRange(){
  //SlidePosition = map(SlidePing.ping_cm(),0,100,0,7);
  String Notes[7];
  switch (rangevalue) {
    case 0:
      Notes[0]="A#/Bb";
      Notes[1]="A";
      Notes[2]="G#/Ab";
      Notes[3]="G";
      Notes[4]="F#/Gb";
      Notes[5]="F";
      Notes[6]="E";      
    case 1:
      Notes[0]="A#/Bb";
      Notes[1]="A";
      Notes[2]="G#/Ab";
      Notes[3]="G";
      Notes[4]="F#/Gb";
      Notes[5]="F";
      Notes[6]="E";  
    case 2:
      Notes[0]="A#/Bb";
      Notes[1]="A";
      Notes[2]="G#/Ab";
      Notes[3]="G";
      Notes[4]="F#/Gb";
      Notes[5]="F";
      Notes[6]="E";  
    case 3:
      Notes[0]="F";
      Notes[1]="E";
      Notes[2]="D#/Eb";
      Notes[3]="D";
      Notes[4]="C#/Db";
      Notes[5]="C";
      Notes[6]="B";  
    case 4:
      Notes[0]="A#/Bb";
      Notes[1]="A";
      Notes[2]="G#/Ab";
      Notes[3]="G";
      Notes[4]="F#/Gb";
      Notes[5]="F";
      Notes[6]="E";  
  }
  while (Play == 1) {
    Serial.println(Notes[map(SlidePing.ping_cm(),0,100,0,7)]);
  }
}

void CheckButton() {
  int activeButton = 0;
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 0: // When button returns as 0 there is no action taken
        break;
      case 1:  // This case will execute if the "forward" button is pressed
        break;
      case 2:
        //Serial.println("Range Up");
        rangevalue = rangevalue + 1;
        button = 0;
        activeButton = 1;
        break;
      case 3:
        //Serial.println("Range Down");
        rangevalue = rangevalue - 1;
        button = 0;
        activeButton = 1;
        break;
      case 4:   
        break;      
      case 5:
        if (Play == 0) {
          Play = 1;}
        else if (Play == 0) {
          Play = 1;}
    }
  }
}


// This function is called whenever a button press is evaluated. The LCD shield works by observing a voltage drop across the buttons all hooked up to A0.
int evaluateButton(int x) {
  int result = 0;
  if (x < 50) {
    result = 1; // right
  } else if (x < 200) { //195
    result = 2; // up
  } else if (x < 400) { //380
    result = 3; // down
  } else if (x < 600) { //was 790
    result = 4; // left
  } else if (x < 800) {
    result = 5; // Select
  }
  return result;
}
