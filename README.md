<h1>Smart Battery SMBUS Analyzer</h1>
Development Board: Arduino Pro Mini<br/>
Baud Rate: 115200<br/>
Smart Battery System specification datasheet: http://sbs-forum.org/specs/sbdat110.pdf
<br/><br/><br/>
Commands: mode<br/>
mode [onplug/continuous] - Sets the analyzer in one of two modes. OnPlug shows battery info only when the battery is initially plugged in. Continuous shows battery info in 2.5s intervals.
<br/><br/><br/>
Known SMBUS Device Addresses:<br/>
[11] Acer AL12X32, Acer AL12A32
<br/><br/><br/>
Used Parameters:
<ul>
<li/>VOLTAGE 0x09
<li/>TEMPERATURE 0x08
<li/>CURRENT 0x0a
<li/>CAPACITY 0x10
<li/>TIME_TO_FULL 0x13
<li/>CHARGE 0x0d
<li/>TIME_TO_EMPTY 0x12
<li/>STATUS 0x16
<li/>CYCLE_COUNT 0x17
<li/>DESIGN_CAPACITY 0x18
<li/>DESIGN_VOLTAGE 0x19
<li/>MANUFACTURE_DATE 0x1b
<li/>SERIAL_NUMBER 0x1c
<li/>DEVICE_NAME 0x21
<li/>DEVICE_CHEMISTRY 0x22
<li/>MANUFACTURER_NAME 0x20
<li/>MANUFACTURER_DATA 0x23
<li/>MANUFACTURER_INFO 0x25
<li/>SPECIFICATION_INFO 0x1a
<li/>CELL3_VOLTAGE 0x3D
<li/>CELL2_VOLTAGE 0x3E
<li/>CELL1_VOLTAGE 0x3F
</ul>