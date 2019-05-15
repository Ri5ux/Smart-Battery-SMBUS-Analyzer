#define SDA_PIN 2
#define SDA_PORT PORTD
#define SCL_PIN 3
#define SCL_PORT PORTD
#define I2C_SLOWMODE 1

#include <SoftI2CMaster.h>

#define VOLTAGE 0x09
#define TEMPERATURE 0x08
#define CURRENT 0x0a
#define CAPACITY 0x10
#define TIME_TO_FULL 0x13
#define CHARGE 0x0d

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
#define CELL3_VOLTAGE 0x3D
#define CELL2_VOLTAGE 0x3E
#define CELL1_VOLTAGE 0x3F

#define bufferLen 32

byte address = 0;
uint8_t i2cBuffer[bufferLen];

long lastCheck = 0;
boolean canDisplay = true;
boolean onPlug = true;
int serial = -1;

void setup()
{
  Serial.begin(115200);

  Serial.println("Smart Battery SMBUS Analyzer");
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
        if (canDisplay)
        {
          displayBatteryInfo();
          canDisplay = false;
        }
      }
      else
      {
        canDisplay = true;
      }
    }
  }
  else
  {
    if (millis() - lastCheck >= 2500)
    {
      lastCheck = millis();
      displayBatteryInfo();
    }
  }
}

void setAddress(byte addr)
{
  address = addr;
  Serial.print("Address Set: ");
  Serial.print(address);
  Serial.println();
}

float mvToV(int mv)
{
  return mv / 1000.0;
}

String convertDateCode(int dateCode)
{
  String date = "";
  
  int mday = B00011111 & dateCode;
  int mmonth = dateCode >> 5 & B00001111;
  int myear = 1980 + (dateCode >> 9 & B01111111);
  
  date += mmonth;
  date += "-";
  date += mday;
  date += "-";
  date += myear;

  return date;
}

void printSeparator()
{
  Serial.println("");
  for (byte b = 30; b > 0; b--)
  {
    Serial.print('=');
  }
  Serial.println("");
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
    Serial.println("");
    
    Serial.print("Device: ");
    length_read = readBlock(DEVICE_NAME, i2cBuffer, bufferLen);
    Serial.write(i2cBuffer, length_read);
    Serial.println("");
    
    Serial.print("S/N: ");
    Serial.print(serial);
    Serial.println("");
    
    Serial.print("Manufacture Date: ");
    int dateCode = fetchWord(MANUFACTURE_DATE);
    Serial.println(convertDateCode(dateCode));
    
    Serial.print("Spec: ");
    Serial.print(fetchWord(SPECIFICATION_INFO));
    Serial.println("");
    
    Serial.print("Type: ");
    length_read = readBlock(DEVICE_CHEMISTRY, i2cBuffer, bufferLen);
    Serial.write(i2cBuffer, length_read);
    Serial.println("");
    
    Serial.print("Design Voltage: ");
    float designVoltage = mvToV(fetchWord(DESIGN_VOLTAGE));
    Serial.print(designVoltage);
    Serial.println("V");
    
    Serial.print("Design Capacity: ");
    Serial.print(fetchWord(DESIGN_CAPACITY));
    Serial.println("mAh");
    
    Serial.print("Status: ");
    Serial.print(String(fetchWord(STATUS), HEX));
    Serial.println("");

    Serial.println("\n[Readings]");
    
    Serial.print("Voltage: ");
    float v = mvToV(fetchWord(VOLTAGE));
    Serial.print(v);
    Serial.println("V");
    
    Serial.print("Current: ");
    Serial.print(fetchWord(CURRENT));
    Serial.println("ma");
    
    Serial.print("C1: ");
    int cV1 = fetchWord(CELL1_VOLTAGE);
    Serial.print(mvToV(cV1));
    Serial.println("V");
    
    Serial.print("C2: ");
    int cV2 = fetchWord(CELL2_VOLTAGE);
    Serial.print(mvToV(cV2));
    Serial.println("V");
    
    Serial.print("C3: ");
    int cV3 = fetchWord(CELL3_VOLTAGE);
    Serial.print(mvToV(cV3));
    Serial.println("V");
    
    Serial.print("Capacity: ");
    Serial.print(fetchWord(CAPACITY));
    Serial.println("mAh");
    
    Serial.println("\n[Charge Information]");
    
    Serial.print("Charge: ");
    Serial.print(fetchWord(CHARGE));
    Serial.println("%");
    
    Serial.print("Cycles: ");
    Serial.print(fetchWord(CYCLE_COUNT));
    Serial.println("");
    
    Serial.print("Time To FULL: ");
    Serial.print(fetchWord(TIME_TO_FULL));
    Serial.println(" minute(s)");
    
    Serial.print("Time To EMPTY: ");
    Serial.print(fetchWord(TIME_TO_EMPTY));
    Serial.println(" minute(s)");
    
    Serial.println("\n[ETC]");
    
    Serial.print("Temp: ");
    float temp = ((float) fetchWord(TEMPERATURE)) / 10.0 - 273.15;
    Serial.print(temp);
    Serial.println("C");
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

