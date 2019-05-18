<h1>Smart Battery SMBUS Analyzer</h1>
Development Board: Arduino Pro Mini<br/>
Baud Rate: 115200<br/>
Smart Battery System specification datasheet: http://sbs-forum.org/specs/sbdat110.pdf
<br/><br/>
Commands: mode<br/>
mode [onplug/continuous] - Sets the analyzer in one of two modes. OnPlug shows battery info only when the battery is initially plugged in. Continuous shows battery info in 2.5s intervals.
<br/><br/>
Known SMBUS Device Addresses:<br/>
[11] Acer AL12X32, Acer AL12A32
<br/><br/>
Texas Instruments BMS Gas Guage IC Note: Acer Battery Model AL12X32 uses a TI BQ9000 IC, which is 'proprietary' according to TI; however, it appears to be a re-branded TI BQ4050.<br/>
Datasheet: http://www.ti.com/lit/ds/symlink/bq4050.pdf
<br/><br/>
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

<h2>Instructions (Microsoft Windows)</h2>
Requirements:
<ul>
<li/>Smart Battery SMBUS Analyzer
<li/>Putty or another serial terminal application
<li/>Smart battery
</ul>
<br/><br/>

1) Download and install Putty from here: https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html<br/>
2) Plug in the analyzer and then open Device Manager and look for "Silicon Labs CP210x USB to UART Bridge" under "Ports (COM &amp; LPT)". Take note of the COM port number in parenthesis at the end of the device name.<br/>
3) Open Putty. Select "Serial" under "Connection type:".<br/>
4) Change the "Serial line" value to the COM port you noted earlier. (Commonly "COM3")<br/>
5) Change the "Speed" value to 115200.<br/>
6) Click Open. This will open a new terminal connection to the analyzer. If you wish to see the startup output, press the reset button on the analyzer device.<br/>
7) At this point, you're using the analyzer, you can plug in your Smart Battery and the device will automatically output information from the batteries internal BMS to the terminal window.<br/><br/>

<h2>Diagnostics</h2>
Issue: Garbled output. Question marks, negative or missing values.<br/>
Fix: Try "jump starting" the battery by connecting the VCC(+12.6V) and GND wires. Reconnect the battery. If the output is still garbled, give it a bit of time and try again. If there is still no output after 30 seconds of "jumping", the internal BMS might be bad or the analyzer cable might be backwards.<br/><br/>
Issue: USB Device Not Recognized<br/>
Fix: USB cable the analyzer is attached to is bad, too long, or the USB port it is plugged into may be weak.