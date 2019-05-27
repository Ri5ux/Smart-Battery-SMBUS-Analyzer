#define SDA_PIN 2
#define SDA_PORT PORTD
#define SCL_PIN 3
#define SCL_PORT PORTD
#define I2C_SLOWMODE 1

#include <SoftI2CMaster.h>

#define MODE 0x03
#define VOLTAGE 0x09
#define TEMPERATURE 0x08
#define CURRENT 0x0a 
#define CAPACITY 0x10
#define TIME_TO_FULL 0x13
#define CHARGE 0x0d
#define CHARGE_VOLTAGE 0x15
#define CHARGE_CURRENT 0x14
#define TIME_TO_EMPTY 0x12
#define STATUS 0x16
#define CYCLE_COUNT 0x17
#define DESIGN_CAPACITY 0x18
#define DESIGN_VOLTAGE 0x19
#define MANUFACTURE_DATE 0x1b
#define SERIAL_NUMBER 0x1c
#define DEVICE_NAME 0x21
#define DEVICE_CHEMISTRY 0x22
#define MANUFACTURER_NAME 0x20
#define MANUFACTURER_DATA 0x23
#define MANUFACTURER_INFO 0x25
#define SPECIFICATION_INFO 0x1a
#define CELL4_VOLTAGE 0x3C
#define CELL3_VOLTAGE 0x3D
#define CELL2_VOLTAGE 0x3E
#define CELL1_VOLTAGE 0x3F

#define bufferLen 32

const int PIN_ENABLE = 5;
const int PIN_LED = 13;
const int PIN_LED_CHRG = A1;
const int PIN_LED_DISCHRG = A4;
const int PIN_LED_ALARM = A6;

byte address = 0;
uint8_t i2cBuffer[bufferLen];

long lastCheck = 0;
boolean canDisplay = true;
boolean onPlug = true;
int serial = -1;

void setup()
{
  Serial.begin(115200);
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_LED_CHRG, OUTPUT);
  pinMode(PIN_LED_DISCHRG, OUTPUT);
  pinMode(PIN_LED_ALARM, OUTPUT);
  digitalWrite(PIN_ENABLE, HIGH);

  Serial.println("Smart Battery Analyzer");
  Serial.println("Designed by Dustin Christensen");
  printSeparator();
  
  if (!i2c_init())
  {
    Serial.println("I2C init failed.");
  }
  else
  {
    setAddress(11);
    Serial.println("Listening...");
  }

  lastCheck = millis();
}

void loop()
{
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil(' ');
    
    if (command == "mode") {
      String arg1 = Serial.readStringUntil(' ');

      if (arg1 == "continuous") {
        onPlug = false;
        Serial.println("Mode: continuous");
      } else if (arg1 == "onplug") {
        onPlug = true;
        Serial.println("Mode: onplug");
      }
    } else {
      Serial.println("mode <onplug/continuous>");
    }
  }

  if (onPlug)
  {
    if (millis() - lastCheck >= 250)
    {
      lastCheck = millis();
  
      serial = fetchWord(SERIAL_NUMBER);
      
      if (serial != -1)
      {
        digitalWrite(PIN_LED, HIGH);
        if (canDisplay)
        {
          displayBatteryInfo();
          canDisplay = false;
          delay(500);
          digitalWrite(PIN_ENABLE, LOW);
        }
      }
      else
      {
        digitalWrite(PIN_ENABLE, HIGH);
        digitalWrite(PIN_LED, LOW);
        digitalWrite(PIN_LED_CHRG, LOW);
        digitalWrite(PIN_LED_DISCHRG, LOW);
        digitalWrite(PIN_LED_ALARM, LOW);
        canDisplay = true;
      }
    }
  }
  else
  {
    if (millis() - lastCheck >= 2500)
    {
      lastCheck = millis();
  
      serial = fetchWord(SERIAL_NUMBER);
      
      if (serial != -1)
      {
        displayBatteryInfo();
      }
    }
  }
}

void setAddress(byte addr)
{
  address = addr;
  Serial.print("Address Set: ");
  Serial.println(address);
}

float mvToV(int mv)
{
  return mv / 1000.0;
}

String convertDateCode(int dateCode)
{
  String date = "";
  
  int mday = dateCode & B00011111;
  int mmonth = dateCode >> 5 & B00001111;
  int myear = 1980 + (dateCode >> 9 & B01111111);
  
  date += mmonth;
  date += "-";
  date += mday;
  date += "-";
  date += myear;

  return date;
}

void printActiveStatusFlags(uint8_t flags)
{
    if (flags & 1 << 4) Serial.print("DISCHARGED ");
    if (flags & 1 << 5) {
      Serial.print("CHARGED ");
      digitalWrite(PIN_LED_CHRG, HIGH);
    } else {
      digitalWrite(PIN_LED_CHRG, LOW);
    }
    if (flags & 1 << 6) {
      Serial.print("DISCHARGING ");
      digitalWrite(PIN_LED_DISCHRG, HIGH);
    } else {
      digitalWrite(PIN_LED_DISCHRG, LOW);
    }
    if (flags & 1 << 7) Serial.print("INIT ");
    if (flags & 1 << 8) Serial.print("REM_TIME_ALARM ");
    if (flags & 1 << 9) Serial.print("REM_CAPACITY_ALARM ");
    if (flags & 1 << 11) {
      Serial.print("TERMINATE_DISCHARGE_ALARM ");
      digitalWrite(PIN_LED_ALARM, HIGH);
    } else {
      digitalWrite(PIN_LED_ALARM, LOW);
    }
    if (flags & 1 << 12) {
      Serial.print("OVERTEMP_ALARM ");
      digitalWrite(PIN_LED_ALARM, HIGH);
    } else {
      digitalWrite(PIN_LED_ALARM, LOW);
    }
    if (flags & 1 << 14) {
      Serial.print("TERMINATE_CHARGE_ALARM ");
      digitalWrite(PIN_LED_ALARM, HIGH);
    } else {
      digitalWrite(PIN_LED_ALARM, LOW);
    }
    if (flags & 1 << 15) {
      Serial.print("OVERCHARGE_ALARM ");
      digitalWrite(PIN_LED_ALARM, HIGH);
    } else {
      digitalWrite(PIN_LED_ALARM, LOW);
    }
    
    Serial.println();
}

void printSeparator()
{
  for (byte b = 32; b > 0; b--)
  {
    Serial.print('=');
  }
  Serial.println();
}

void printValue(float value, char* unit)
{
  Serial.print(value);
  Serial.print(' ');
  Serial.println(unit);
}

void displayBatteryInfo()
{
    uint8_t length_read = 0;

    printSeparator();

    if (onPlug)
    {
      delay(100);
    }

    Serial.println("[General]");
    Serial.print("Manufacturer: ");
    length_read = readBlock(MANUFACTURER_NAME, i2cBuffer, bufferLen);
    Serial.write(i2cBuffer, length_read);
    Serial.println();
    Serial.print("Device: ");
    length_read = readBlock(DEVICE_NAME, i2cBuffer, bufferLen);
    Serial.write(i2cBuffer, length_read);
    Serial.println();
    Serial.print("S/N: ");
    Serial.println(serial);
    Serial.print("Manufactured: ");
    int dateCode = fetchWord(MANUFACTURE_DATE);
    Serial.println(convertDateCode(dateCode));
    Serial.print("Type: ");
    length_read = readBlock(DEVICE_CHEMISTRY, i2cBuffer, bufferLen);
    Serial.write(i2cBuffer, length_read);
    Serial.println();
    Serial.print("Design Voltage: ");
    printValue(mvToV(fetchWord(DESIGN_VOLTAGE)), "V");
    Serial.print("Design Capacity: ");
    printValue(fetchWord(DESIGN_CAPACITY), "mAh");

    Serial.println("\n[Readings]");
    Serial.print("Status: ");
    printActiveStatusFlags(fetchWord(STATUS));
    Serial.print("Voltage: ");
    printValue(mvToV(fetchWord(VOLTAGE)), "V");
    Serial.print("Current: ");
    printValue(fetchWord(CURRENT), "ma");
    Serial.print("Capacity: ");
    printValue(fetchWord(CAPACITY), "mAh");
    Serial.print("C1: ");
    printValue(mvToV(fetchWord(CELL1_VOLTAGE)), "V");
    Serial.print("C2: ");
    printValue(mvToV(fetchWord(CELL2_VOLTAGE)), "V");
    Serial.print("C3: ");
    printValue(mvToV(fetchWord(CELL3_VOLTAGE)), "V");
    Serial.print("C4: ");
    printValue(mvToV(fetchWord(CELL4_VOLTAGE)), "V");
    Serial.print("Temp: ");
    printValue(((float) fetchWord(TEMPERATURE)) / 10.0 - 273.15, "C");
    
    Serial.println("\n[Charge Information]");
    
    Serial.print("Charge: ");
    printValue(fetchWord(CHARGE), "%");
    Serial.print("Charge Current: ");
    printValue(fetchWord(CHARGE_CURRENT), "ma");
    Serial.print("Charge Voltage: ");
    printValue(fetchWord(CHARGE_VOLTAGE), "V");
    Serial.print("Cycles: ");
    printValue(fetchWord(CYCLE_COUNT), "");
    Serial.print("Time To FULL: ");
    printValue(fetchWord(TIME_TO_FULL), "minutes");
    Serial.print("Time To EMPTY: ");
    printValue(fetchWord(TIME_TO_EMPTY), "minutes");
}

int fetchWord(byte func)
{
  i2c_start(address << 1 | I2C_WRITE);
  i2c_write(func);
  i2c_rep_start(address << 1 | I2C_READ);
  
  byte b1 = i2c_read(false);
  byte b2 = i2c_read(true);
  i2c_stop();
  
  return (int) b1| (((int) b2) << 8);
}

uint8_t readBlock(uint8_t command, uint8_t* blockBuffer, uint8_t blockBufferLen)
{
    uint8_t x;
    uint8_t num_bytes;
    
    i2c_start((address << 1) + I2C_WRITE);
    i2c_write(command);
    i2c_rep_start((address << 1) + I2C_READ);
    
    num_bytes = i2c_read(false);
    num_bytes = constrain(num_bytes, 0, blockBufferLen - 2);
    
    for (x = 0; x < num_bytes - 1; x++)
    {
      blockBuffer[x] = i2c_read(false);
    }
    
    blockBuffer[x++] = i2c_read(true);
    blockBuffer[x] = 0;
    
    i2c_stop();
    
    return num_bytes;
}

