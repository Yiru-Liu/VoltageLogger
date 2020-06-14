#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

#define _WIN32_WINNT 0x0601
#include <windows.h>

#include "Screen.h"
#include "Serial.h"     // for interfacing with Arduino via Serial Port

#define FIRST_LINE 116

#define PLOT_STEP 1

#define arduino_com_port "\\\\.\\COM7"

using namespace std;

int main(int argv, char** args) {
//    ShowWindow(GetConsoleWindow(), SW_HIDE);    // Hide the console window
    char path[] = R"(C:\Users\Biotree\Desktop\VoltageLog.csv)";  // path of the .csv file

    /// Check to see if VoltageLog.csv already exists: ----------------/
    ifstream toCheckIfExists(path);
    if(toCheckIfExists.good()) {
        if(MessageBox(NULL, "VoltageLog.csv already exists. Would you like to overwrite VoltageLog.csv?", "Confirm Overwrite", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDNO) {
            return 0;
        }
    }
    toCheckIfExists.close();
    remove(path);
    /**---------------------------------------------------------------*/

    /// Setup Serial port: --------------------------------------------/
	std::cout << "Connecting to Arduino...\n";
	Serial* SP = new Serial(arduino_com_port);     // COM port of Arduino

	if (SP->IsConnected()) {    // When we are connected:
		std::cout << "Connected.\n" << endl;  //We're connected!
	}
    /**---------------------------------------------------------------*/

    /// Initialize variables & other stuff: ---------------------------/
    string voltageVal;              // string of the value of the voltage

    int voltageValPos;              // where the plot should plot the next point
    int lastVoltageValPos = 372;    // the voltageValPos from the previous loop

////    size_t commaPos = 0;
////    size_t lastEolPos = 0;
////    size_t eolPos = 0;

    int counter = 0;                // counter that counts up every loop

    Screen screen;

	char incomingData[256] = "";    // char array to hold received data
	int dataLength = 255;           // amount of data we want to receive at once (bytes)
	int readResult = 0;             // the length of the serial read (bytes)

    std::string receivedData = "";  // string for storing the full lines of data
    std::string leftoverData = "";  // string for storing the data that wasn't completely sent (to save it for the next serial read)
	std::size_t lastNewline;        // variable for storing the position of the newline to separate complete reads

	std::ofstream csvFileWrite;     // ofstream for writing to the csv file
	csvFileWrite.open("VoltageLog.csv");
    /**---------------------------------------------------------------*/

    if(screen.init() == false) {
        cout << "Error initialising SDL." << endl;
    }

    for(int x = 0; x < Screen::SCREEN_WIDTH; x++) {
        for(int y = 0; y < Screen::SCREEN_HEIGHT; y++) {
            screen.setPixle(x, y, 0xFA, 0xFA, 0xFF);
        }
    }

    while(true) {
////        totalFile = "";
////        ifstream file;
////        file.open(path);
////        totalFile.assign( (std::istreambuf_iterator<char>(file) ),
////                (std::istreambuf_iterator<char>()    ) );
////        file.close();
////        totalFile = totalFile.substr(FIRST_LINE);
////
////        commaPos = totalFile.find(',', commaPos);
////        eolPos = totalFile.find('\n', lastEolPos + 1);
////
////        if(eolPos == string::npos) {
////            eolPos = lastEolPos;
////            commaPos = lastEolPos;
////            if(screen.processEvents() == false) { break; }
////            continue;
////        } else {
////            lastEolPos = eolPos;
////            ++commaPos;
////        }

        voltageVal = totalFile.substr(commaPos, eolPos - commaPos);
        ++counter;

        if(counter % 2 == 0) {
            for(int x = 0; x < Screen::SCREEN_WIDTH; x++) {
                for(int y = 0; y < Screen::SCREEN_HEIGHT; y++) {
                    Uint32 pixelColor = screen.getPixle(x + PLOT_STEP, y);
                    if(pixelColor == 0) {
                        pixelColor = 0xF5F5FAFF;
                    }
                    screen.setPixleHex(x, y, pixelColor);
                }
            }

            voltageValPos = stoi(voltageVal) / -2 + 512;
            screen.drawLine(Screen::SCREEN_WIDTH - PLOT_STEP, lastVoltageValPos, Screen::SCREEN_WIDTH - 1, voltageValPos, 0x000080FF);
            lastVoltageValPos = voltageValPos;

            screen.update();
        }

        if(screen.processEvents() == false) {
            break;
        }

    }

    screen.close();

    return 0;
}
