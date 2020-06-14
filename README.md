## Voltage Plotter and Logger

This project uses an Arduino Uno to continuously read an analog voltage signal between 0V and 5V and plots it in real-time, and saves the data recorded as a .csv file.

### Installation
#### Set up Arduino:
1. Use the [Arduino IDE](https://www.arduino.cc/en/main/software) to upload VoltageMeter.ino to the Arduino Uno.
2. Connect the voltage to be measured to Pin A1.

#### Set up computer:
1. Install [PuTTY](https://www.chiark.greenend.org.uk/~sgtatham/putty/) if not already installed.
2. In PuTTY:
	1. In the "Category" menu, select "Session".
	2. Under "Connection type", select "Serial".
	3. Under "Speed", enter 9600.
	4. Under "Serial line", enter the COM Port of the Arduino Uno.
	5. In the text field under "Saved Sessions", enter "ArduinoVoltageLogger".
	6. In the "Category" menu, select "Logging".
	7. Under "Session logging", select "All session output".
	8. Under "Log file name:", enter "C:\Users\<your username here>\Desktop\VoltageLog.csv", replacing "<your username here>" with your username.
	

### Example
![]("screenshot.png")
