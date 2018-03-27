// You can have up to 10 menu items in the menuItems[] array below without having to change the base programming at all. Name them however you'd like. Beyond 10 items, you will have to add additional "cases" in the switch/case
// section of the operateMainMenu() function below. You will also have to add additional void functions (i.e. menuItem11, menuItem12, etc.) to the program.

String Notes[] = {"C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", "A", "A#/Bb", "B"};

// Navigation button variables
int readKey;
int pot1;
int pot2;
int potTotal;
int savedDistance = 0;
int SlidePosition;

// Menu control variables
int menuPage = 0;
int rangevalue = 0;
//int maxMenuPages = round(((sizeof(Notes) / sizeof(String)) / 2) + .5);
int maxMenuPages = round(sizeof(Notes)); 
int cursorPosition = 0;


#include <Wire.h>
#include <LiquidCrystal.h>
#include <NewPing.h>

// Setting the LCD shields pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Initialize the Ping Sensor for the slide
NewPing SlidePing(23, 22, 200);

void setup() {

  // Initializes serial communication
  Serial.begin(9600);

  // Initializes and clears the LCD screen
  lcd.begin(16, 2);
  lcd.clear();

}

void loop() {
  NoteDraw();
  operateMainMenu();
}

// This function will generate the 2 menu items that can fit on the screen. They will change as you scroll through your menu. Up and down arrows will indicate your current menu position.
void NoteDraw() {
  Serial.print(menuPage);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(Notes[menuPage]);
  if (menuPage == 0) {
    lcd.setCursor(15, 0);
    lcd.write(">");
  } else if (menuPage > 0 and menuPage < maxMenuPages) {
    lcd.setCursor(15, 0);
    lcd.write(">");
    lcd.setCursor(0, 0);
    lcd.write("<");
  } else if (menuPage == maxMenuPages) {
    lcd.setCursor(0, 0);
    lcd.write("<");
  }
  RangeDraw(); 
  ReadSlide();
}

void PotDraw() {
  pot1 = analogRead(14);
  pot2 = analogRead(15);
  potTotal = pot1 + pot2;
  lcd.setCursor(12, 1);
  lcd.print(potTotal);
}

void RangeDraw() {
  lcd.setCursor(0, 1);
  lcd.write("Range: ");
  lcd.setCursor(8, 1);
  lcd.print(rangevalue);  
}

void ReadSlide() {
  SlidePosition = map(SlidePing.ping_cm(),0,200,0,7);
  //Serial.println(SlidePosition);
  //Serial.print(SlidePing.ping_cm());
}

void PlayNote(){
Serial.println(Notes[menuPage]);
//Serial.println(rangevalue);
}

void operateMainMenu() {
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
        //Serial.println("Note Up");
        menuPage = menuPage + 1;
        menuPage = constrain(menuPage, 0, maxMenuPages);
        button = 0;
        activeButton = 1;
        NoteDraw();
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
        //Serial.println("Note Down");
        menuPage = menuPage - 1;
        menuPage = constrain(menuPage, 0, maxMenuPages);       
        button = 0;
        activeButton = 1;
        NoteDraw();
        break;      
      case 5:   
        PlayNote();
        button = 0;
        activeButton = 1;
        break;  
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
