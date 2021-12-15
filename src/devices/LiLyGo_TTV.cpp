
/*

#include <Arduino.h>
#include <HardwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "LiLyGo_TTV_pin_setup.h"
#include "Button2.h"
#include <EEPROM.h>

//#define _DEBUG

#define BUTTON_UP     12
#define BUTTON_ENTER  13
#define BUTTON_DOWN   17
#define EEPROM_SIZE   1
#define MENU_POSITIONS 4

char* menu_items[] = {
  "Vertical speed",
  "Horizontal speed",
  "Total speed",
  "Heading"
};


const uint8_t vbatPin = 34;

*/

//U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0,
        /* clock=*/// OLED_SCLK,
        /* data=*/ // OLED_MOSI,
        /* cs=*/   // OLED_CS,
        /* dc=*/   // OLED_DC,
        /* reset=*/// OLED_RST);
/*
TinyGPS gps;
HardwareSerial SerialGPS ( 1 );
Button2 button_up(BUTTON_UP);
Button2 button_enter(BUTTON_ENTER);
Button2 button_down(BUTTON_DOWN);

bool commit=false;
byte cursor_position=1;

float VBAT= 0; // battery voltage from ESP32 ADC read

volatile int random_speed = 140;

void move_cursor(int direction){
  // up = 1
  // down = 2

  if (direction==2){
    Serial.println(cursor_position);    
    if (cursor_position < MENU_POSITIONS){
      cursor_position++;
    } else {
      cursor_position=1;
    }
  } else {
    if (cursor_position != 1){
      cursor_position--;
    } else {
      cursor_position=MENU_POSITIONS;
    }
  }
}

void button_init()
{
    button_up.setTapHandler([](Button2 & b) {
        Serial.println("Up"); 
        move_cursor(1);
    });


    button_enter.setLongClickTime(4000);
    button_enter.setLongClickDetectedHandler([](Button2 & b) {
        Serial.println("Enter long hold");
        menu_loop();
    });

    button_enter.setTapHandler([](Button2 & b) {
        Serial.println("Enter");
        move_cursor(2);
        //menu_loop();
    });

    
    button_down.setTapHandler([](Button2 & b) {
        Serial.println("Down");
        //move_cursor(2);
    });
}


void setup()
{
  Serial.begin(115200);
  Serial.println("Starting up");
  SerialGPS.begin ( 9600 , SERIAL_8N1, 21 , 22 );


  pinMode(PWR_ON, OUTPUT);
  digitalWrite(PWR_ON, HIGH);

  pinMode(OLED_PWR, OUTPUT);
  digitalWrite(OLED_PWR, HIGH);

  pinMode(vbatPin, INPUT);


  u8g2.begin();

  u8g2.enableUTF8Print(); // enable UTF8 support for the Arduino print() function
 
  //EEPROM.begin(EEPROM_SIZE);
  //clickCounter = EEPROM.read(0);


  

  button_init();
 
  Serial.println("initialization done.");
  Serial.println("Ready");

  //displaySpeed(0);
}

void button_loop()
{
    button_up.loop();
    button_enter.loop();
    button_down.loop();
}

//For future EEPOM writes
void float2Bytes(float val,byte* bytes_array){
  // Create union of shared memory space
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  // Overite bytes of union with float variable
  u.float_variable = val;
  // Assign bytes to input array
  memcpy(bytes_array, u.temp_array, 4);
}

void menu_loop(){
  Serial.write(0x0C);
  for (int i=0; i< MENU_POSITIONS; i++){
    char buffer[25];
    if (i+1==cursor_position){
      sprintf(buffer, "* %s", menu_items[i]);
    }  else {
      sprintf(buffer, "  %s", menu_items[i]);
    }    
    Serial.println(buffer);
  }
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  float flat;
  float flon;
  float altit;
  unsigned long age;
  int YY;
  byte MM;
  byte DD;
  byte hrs;
  byte mins;
  byte secs;
  byte hunds;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 250;)
  {
    while (SerialGPS.available())
    {
      char c = SerialGPS.read();
      //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;        
    }
  }

  if (newData)
  {
    gps.f_get_position(&flat, &flon, &age);
    gps.crack_datetime(&YY, &MM, &DD, &hrs, &mins, &secs, &hunds, &age);
    //altit = gps.altitude();



#ifdef _DEBUG    
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    Serial.println();
#endif
    //displaySpeed(gps.f_speed_kmph());

//    displayCourse(gps.f_course());
  } else {
    //displayStats();
    //displayRandomSpeed();
  }

  gps.stats(&chars, &sentences, &failed);
  if (chars == 0) {
    Serial.println("** No characters received from GPS: check wiring **");    
  }

  button_loop();
  displayMenu();
}


void displayStats() {

  VBAT = (float)(analogRead(vbatPin)) * 3600 / 4095 * 2 / 1000;

  u8g2.setFont(u8g2_font_t0_11_mf   );  
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.println("Vbat = ");
  u8g2.print(VBAT,2);
  u8g2.println(" Volts");

  u8g2.setCursor(0, 40);
  u8g2.print(" SAT=");
  u8g2.println(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());

  u8g2.sendBuffer();
  delay(250);
}


void displayMiniStats() {
  VBAT = (float)(analogRead(vbatPin)) * 3600 / 4095 * 2 / 1000;
  u8g2.setFont(u8g2_font_profont11_mf  );  
  u8g2.setFontDirection(3);
  u8g2.setCursor(125, 55);
  u8g2.print(VBAT,1);
  u8g2.print("v ");
  u8g2.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
  u8g2.println("¤");  
}


void displayRandomSpeed(){
    int randNumber = random(3);
    if (random_speed < randNumber){
        random_speed = random_speed + randNumber;
    } else {
      int direction =  random(4);
      if (direction > 1){
        random_speed = random_speed + randNumber;        
      } else {
        random_speed = random_speed - randNumber;
      }
    }
    
    displaySpeed(random_speed);
}


void displaySpeed(float spd) {
  //spd = 172.1;
  int ispd = int(spd);

  u8g2.setFont(u8g2_font_inb46_mn);  
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 50);
  u8g2.print(ispd);
  //u8g2.setFont(u8g2_font_unifont_t_chinese2); 
  //u8g2.setCursor(95, 40);
  //u8g2.print("km/h");
  displayMiniStats();
  u8g2.sendBuffer();
  delay(250);
}


void displayCourse(float course) {
  //course = 270.8;
  int icourse = int(course);
  u8g2.setFont(u8g2_font_inb46_mn);  
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 50);
  u8g2.print(icourse);
  u8g2.sendBuffer();
  delay(250);
}


void displayMenu(){

  u8g2.setFont(u8g2_font_t0_11_mf   );  
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();  
  

  for (int i=0; i< MENU_POSITIONS; i++){
    char buffer[25];
    if (i+1==cursor_position){
      sprintf(buffer, "* %s", menu_items[i]);
    }  else {
      sprintf(buffer, "  %s", menu_items[i]);
    }    
    u8g2.setCursor(0, (i+1)*10);
    u8g2.println(buffer);
  }
  u8g2.sendBuffer();
  delay(250);
}
*/