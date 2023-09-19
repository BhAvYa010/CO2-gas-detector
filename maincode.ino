#include <Wire.h> 
#include <Adafruit_GFX.h>                     //grafix library for OLED
#include <Adafruit_SSD1306.h>                
#define anInput     A0                        //analog feed from MQ135
#define digTrigger   2                        //digital feed from MQ135
#define co2Zero     60                        //calibrated CO2 0 level
#define led          9
#define led1          10
#define led2         11
#define led3        12                      
#define OLED_RESET   4                      
Adafruit_SSD1306 display(OLED_RESET);         //create instance of OLED called display
void setup() {
 
  pinMode(anInput,INPUT);                     //MQ135 analog feed set for input
  pinMode(digTrigger,INPUT);                  //MQ135 digital feed set for input
  pinMode(led,OUTPUT);                        
  Serial.begin(9600);                        
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.display();                          //show buffer
  display.clearDisplay();                  
 
}
void loop() {
 
int co2now[10];                               //int array for co2
int co2raw = 0;                            
int co2comp = 0;                              //int for compensated co2
int co2ppm = 0;                               //int for calculated ppm
int avg = 0;                                
                       


  display.clearDisplay();                     //clear display @ beginning of each loop

  for (int x = 0;x<10;x++){                   //samplpe co2 10x over 2 seconds
    co2now[x]=analogRead(A0);
    delay(200);
  }

/*
 * Atmospheric CO2 Level..............400ppm
 * Average indoor co2.............350-450ppm
 * Maxiumum acceptable co2...........1000ppm
 * Dangerous co2 levels.............>2000ppm
 */
for (int x = 0;x<10;x++){                     //add samples together
    avg=avg + co2now[x];
   
  }
  co2raw = avg/10;                            //divide samples by 10
  co2comp = co2raw - co2Zero;                 //get compensated value
  co2ppm = map(co2comp,0,1023,400,5000);      //map value for atmospheric levels

  display.setTextSize(2);                     //set text size
  display.setTextColor(WHITE);                //set text color
  display.setCursor(0,0);                     //set cursor
  display.println("CO2 Level");               //print title
  display.println(" ");                       //skip a line
  display.print(co2ppm);                      //print co2 ppm
  display.print(" PPM");                      //print units
  display.display();                          //show the buffer
  if(co2ppm<400){                            
    digitalWrite(led,HIGH);
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);                  
  }
  else if(350<co2ppm<450) {
    digitalWrite(led,HIGH);  
    digitalWrite(led1,HIGH);
     digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);//turn off led
  }
  else if(1000<co2ppm<2000){
    digitalWrite(led,HIGH);  
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);// three led on indicating maximum capacity
    digitalWrite(led3,LOW);
  }
  else if(co2ppm>2000){
    digitalWrite(led,HIGH);  
    digitalWrite(led1,HIGH);// all four leds are indicating danger zone
    digitalWrite(led2,HIGH);
    digitalWrite(led3,HIGH);
    }
  else { digitalWrite(led,LOW);  
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);// all four leds are off
    digitalWrite(led3,LOW);}  
}
