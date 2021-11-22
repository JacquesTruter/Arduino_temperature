
//Libraries
#include <DHT.h>
#include <Wire.h> 
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>

//Constants
#define DHTPIN 4     // what pin DHT22 temp sensor is connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
DS3231 rtc(SDA, SCL); // Init the DS3232 using the hardware interface DS3232 and DS3231 uses the same code. DS3232 has less time loss. 
Time t;
const long interval = 1000;

// temperature and time to switch on/off
const int onHour = 18;
const int offHour = 8;
const int onTemp = 25; // in celsius 
const int offTemp = 35; // in celsius

//pin assignments #note that LCD uses same pin declaration but close over I2C so theres no conflict
const int relay_Temperature = 7;
const int relay_UV = 6;
const int button_Backlight = 2;
const int buttonTT = 3;

//ALWAYS USE THIS WITH LCD I2C and Addres 0x3F
#define I2C_ADDR 0x3F
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

//Variables
int chk;
float humidity;  //Stores humidity value
float temperature; //Stores temperature value
int lcdState = LOW; // lcd state set
unsigned long refresh = 0;
unsigned long previousMillis = 0;
int backlight_State = 0;
int tempOrHumDisplayState = 0;


void setup()
{
    Serial.begin(9600);
    dht.begin();
    rtc.begin();
    lcd.begin(16,2); //character line display 16 characters with 2 lines.
    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    lcd.setBacklight(HIGH);
    pinMode(button_Backlight, INPUT);
    pinMode(buttonTT, INPUT);
    pinMode(relay_Temperature, OUTPUT);
    pinMode(relay_UV, OUTPUT);
    digitalWrite(relay_Temperature, LOW); //relays need to Initialize in low state if solidstate and high if standard coil.
    digitalWrite(relay_UV, LOW); //relays need to Initialize in low state if solidstate and high if standard coil. 
}

void loop()
{
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    t = rtc.getTime();

    //anything related to serial is code for debugging on the serial interface on the arduino board. Baudrate of 9600
    //Serial.print(t.hour);
    //Serial.print(" hour(s), ");
    //Serial.print(t.min);
    //Serial.print(" minute(s)");
    //Serial.println(" ");
    if ((t.hour >= OnHour) && (t.hour <= OffHour))
    {
        //Serial.println("UV LIGHT ON");
        digitalWrite(relay_UV, LOW);}
    else
    {
        //Serial.println("UV LIGHT OFF");
        digitalWrite(relay_UV,HIGH);
    }

    if (temperature <= offTemp)
    {
        //Serial.println("HEAT LIGHT ON");
        digitalWrite(relay_Temperature, LOW);}
    }
    else if (temperature >= onTemp)
    {
        //Serial.println("HEAT LIGHT OFF");
        digitalWrite(relay_Temperature,HIGH);
    }

{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        //save the last time you blinked the LED
        previousMillis = currentMillis;
    }
    if (lcdState == LOW) 
    {
        (Temp_Or_Hum_Display_State = digitalRead(buttonTT));
    }
    //Read data and store it to variables hum and temp
    if (tempOrHumDisplayState == HIGH) 
    {
    //Print temp and humidity values to serial monitor
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Temp: ");
        lcd.print(temperature);
        lcd.print(" ");
        lcd.print((char)223);
        lcd.print(" C");
        lcd.setCursor(0,1);
        lcd.print("Hum:  ");
        lcd.print(t.hour);
        lcd.print(" %");
    } 
    else
    {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Date: ");
        lcd.print(rtc.getDateStr());
        lcd.print(" ");
        lcd.print(refresh);
        lcd.setCursor(0,1);
        lcd.print("Time:  ");
        lcd.print(rtc.getTimeStr());
    }
    else
    {
    lcdState = LOW;
    }
}
    (backlight_State = digitalRead(button_Backlight));
    if (backlight_State == HIGH)
    {
    lcd.setBacklight(HIGH);
    }
    else
    {
    lcd.setBacklight(LOW);
    }
}
}

 
