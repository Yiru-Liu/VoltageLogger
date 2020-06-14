#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <cstring>
#include <math.h>
#include <algorithm>

class Screen {
    public:
        const static int SCREEN_WIDTH = 300;
        const static int SCREEN_HEIGHT = 512;

    private:
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        SDL_Texture *m_texture;
        Uint32 *m_buffer1;
        Uint32 *m_buffer2;

    public:
        Screen();
        bool init();
        void update();
        void setPixle(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
        void setPixleHex(int x, int y, Uint32 color);
        Uint32 getPixle(int x, int y);
        void drawLine(int x1, int y1, int x2, int y2, Uint32 color);
        bool processEvents();
        void close();
        void boxBlur();

};

#endif // SCREEN_H

