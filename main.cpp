#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

#define _WIN32_WINNT 0x0601
#include <windows.h>

#include "Screen.h"

#define FIRST_LINE 116

#define PLOT_STEP 1

using namespace std;

int main(int argv, char** args) {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    char path[] = R"(C:\Users\Biotree\Desktop\VoltageLog.csv)";  // path of the .csv file
    ifstream toCheckIfExists(path);
    if(toCheckIfExists.good()) {
        if(MessageBox(NULL, "VoltageLog.csv already exists. Would you like to overwrite VoltageLog.csv?", "Confirm Overwrite", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDNO) {
            return 0;
        }
    }
    toCheckIfExists.close();
    remove(path);

    system(R"(START "C:\Program Files\VoltagePlotter" VoltagePlotter_600sec.exe)");

    Sleep(150);

    system(R"(START "C:\Program Files\PuTTY" putty.exe @ArduinoVoltageLogger)");

    string totalFile = "";
    string voltageVal;

    int voltageValPos;
    int lastVoltageValPos = 372;

    size_t commaPos = 0;
    size_t lastEolPos = 0;
    size_t eolPos = 0;

    int counter = 0;

    Screen screen;

    if(screen.init() == false) {
        cout << "Error initialising SDL." << endl;
    }

    for(int x = 0; x < Screen::SCREEN_WIDTH; x++) {
        for(int y = 0; y < Screen::SCREEN_HEIGHT; y++) {
            screen.setPixle(x, y, 0xFA, 0xFA, 0xFF);
        }
    }

    Sleep(2400);

    while(true) {
        totalFile = "";
        ifstream file;
        file.open(path);
        totalFile.assign( (std::istreambuf_iterator<char>(file) ),
                (std::istreambuf_iterator<char>()    ) );
        file.close();
        totalFile = totalFile.substr(FIRST_LINE);

        commaPos = totalFile.find(',', commaPos);
        eolPos = totalFile.find('\n', lastEolPos + 1);

        if(eolPos == string::npos) {
            eolPos = lastEolPos;
            commaPos = lastEolPos;
            if(screen.processEvents() == false) { break; }
            continue;
        } else {
            lastEolPos = eolPos;
            ++commaPos;
        }

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
