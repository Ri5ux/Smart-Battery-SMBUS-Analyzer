Smart Battery SMBUS Analyzer

Development Board: Arduino Pro Mini
Baud Rate: 115200
Smart Battery System specification datasheet: http://sbs-forum.org/specs/sbdat110.pdf

Commands: mode
mode [onplug/continuous] - Sets the analyzer in one of two modes. OnPlug shows battery info only when the battery is initially plugged in. Continuous shows battery info in 2.5s intervals.

Known SMBUS Device Addresses:
[11] Acer AL12X32, Acer AL12A32

Used Parameters:
VOLTAGE 0x09
TEMPERATURE 0x08
CURRENT 0x0a
CAPACITY 0x10
TIME_TO_FULL 0x13
CHARGE 0x0d
TIME_TO_EMPTY 0x12
STATUS 0x16
CYCLE_COUNT 0x17
DESIGN_CAPACITY 0x18
DESIGN_VOLTAGE 0x19
MANUFACTURE_DATE 0x1b
SERIAL_NUMBER 0x1c
DEVICE_NAME 0x21
DEVICE_CHEMISTRY 0x22
MANUFACTURER_NAME 0x20
MANUFACTURER_DATA 0x23
MANUFACTURER_INFO 0x25
SPECIFICATION_INFO 0x1a
CELL3_VOLTAGE 0x3D
CELL2_VOLTAGE 0x3E
CELL1_VOLTAGE 0x3F