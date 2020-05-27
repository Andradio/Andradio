#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <DS3231.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

int H,M,S, H1[2],M1[2],TP[4];
int Inten;
int pinCS = 13; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 1;

//вывода BMP-280
#define BMP_SCK  (9)
#define BMP_MISO (12)//sdo
#define BMP_MOSI (11)//sda
#define BMP_CS   (10)
//Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
float Pres, Temper;

DS3231 Clock;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String tape = "Arduino";
int wait = 50; // In milliseconds

int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels

void setup() {
   Serial.begin(9600);
 if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
   
// Start the I2C interface
  Wire.begin();
  matrix.setIntensity(1); // Use a value between 0 and 15 for brightness

// Adjust to your own needs
  matrix.setPosition(0, 3, 0); // The first display is at <0, 0>
  matrix.setPosition(1, 2, 0); // The second display is at <1, 0>
  matrix.setPosition(2, 1, 0); // The third display is at <2, 0>
  matrix.setPosition(3, 0, 0); // And the last display is at <3, 0> 
//  ...
  matrix.setRotation(0, 1);    // The first display is position upside down
  matrix.setRotation(1, 1);
  matrix.setRotation(2, 1);
  matrix.setRotation(3, 1);    // The same hold for the last display

 // Clock.setMinute(23);
 // Clock.setHour(22);

 /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
 
}

void loop() {
bool h12 =1;
bool PM=1;

  //Clock.getYear()
 // Clock.getYear();
 // Clock.getMonth(Century);
  //Clock.getDate();
	H= Clock.getHour(h12, PM);//String(Clock.getHour(h12, PM),DEC); //24-hr
	M= Clock.getMinute();//String(Clock.getMinute(),DEC);
   // Clock.getSecond();
	tape = H + ":" + M;
 while(1){

  H= Clock.getHour(h12, PM);//String(Clock.getHour(h12, PM),DEC); //24-hr
  M= Clock.getMinute();//String(Clock.getMinute(),DEC);
  S=Clock.getSecond();
H1[0]=H/10;
H1[1]=H-(10*H1[0]);
M1[0]=M/10;
M1[1]=M-(10*M1[0]);

//read BMP-280 
Temper = bmp.readTemperature();//in C

Pres = bmp.readPressure();//in Pa

    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
TP[0]=Temper/1000;
Temper=Temper-TP[0]*1000;
TP[1]=Temper/100;
Temper=Temper-TP[1]*100;
TP[2]=Temper/10;
Temper=Temper-TP[2]*10;
TP[3]=Temper;
matrix.fillScreen(LOW);
  int y = (matrix.height() - 8) / 2; // center the text vertically
  int x=1;
  for(int i=0;i<4;i++){
   matrix.drawChar(x+6*i, y, String(TP[i])[0], HIGH, LOW, 1);
  }
  matrix.write(); 
delay(1000);
Temper=Pres/100;
TP[0]=Temper/1000;
Temper=Temper-TP[0]*1000;
TP[1]=Temper/100;
Temper=Temper-TP[1]*100;
TP[2]=Temper/10;
Temper=Temper-TP[2]*10;
TP[3]=Temper;
matrix.fillScreen(LOW);
   y = (matrix.height() - 8) / 2; // center the text vertically
   x=1;
  for(int i=0;i<4;i++){
   matrix.drawChar(x+6*i, y, String(TP[i])[0], HIGH, LOW, 1);
  }
  matrix.write(); 
delay(1000);
  //An code
	matrix.fillScreen(LOW);
	 y = (matrix.height() - 8) / 2; // center the text vertically
	 x=1;
   matrix.drawChar(x+0, y, String(H1[0])[0], HIGH, LOW, 1);
   matrix.drawChar(x+6, y, String(H1[1])[0], HIGH, LOW, 1);
  if(S & 1){
   matrix.drawChar(x+12, y, (String(":"))[0], HIGH, LOW, 1);
   }else{
   matrix.drawChar(x+12, y, (String(" "))[0], HIGH, LOW, 1);
    }
   matrix.drawChar(x+18, y, String(M1[0])[0], HIGH, LOW, 1);
   matrix.drawChar(x+24, y, String(M1[1])[0], HIGH, LOW, 1);
   
  
   matrix.write(); 
 
 delay(1000);
 }
  
}
