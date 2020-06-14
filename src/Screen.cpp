#include "Screen.h"

Screen::Screen() :
        m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer1(NULL), m_buffer2(NULL) {

}


bool Screen::init() {
    const int SCREEN_WIDTH = 300;
    const int SCREEN_HEIGHT = 512;

    if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        return false;
    }

    m_window = SDL_CreateWindow("Live Voltage Plot (/2)",
            940, 40, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(m_window == NULL) {
        SDL_Quit();
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

    if(m_renderer == NULL) {
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    if(m_texture == NULL) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    m_buffer1  = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
    m_buffer2  = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
    memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
    memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

    m_buffer1[30000] = 0xFFFFFFFF;

    for(int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        m_buffer1[i] = 0x000000FF;
    }

    return true;
}

void Screen::update() {
    SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
}

void Screen::setPixle(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {

    if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return;
    }

    unsigned int color = 0;

    color += red;
    color <<= 8;
    color += green;
    color <<= 8;
    color += blue;
    color <<= 8;
    color += 0xFF;

    // RGBA

    m_buffer1[(y * SCREEN_WIDTH) + x] = color;
}

void Screen::setPixleHex(int x, int y, Uint32 color) {
    if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return;
    }

    m_buffer1[(y * SCREEN_WIDTH) + x] = color;
}

Uint32 Screen::getPixle(int x, int y) {
    if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return 0;
    }
    return m_buffer1[(y * SCREEN_WIDTH) + x];
}

void Screen::drawLine(int x1, int y1, int x2, int y2, Uint32 color) {
    if(y2 == y1) {
        for(int k = x1; k <= x2; k++) {
            setPixleHex(k, y1, color);
        }
    } else {
        // Bresenham's line algorithm
        const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
        if(steep) {
            std::swap(x1, y1);
            std::swap(x2, y2);
        }

        if(x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        const float dx = x2 - x1;
        const float dy = fabs(y2 - y1);

        float error = dx / 2.0f;
        const int ystep = (y1 < y2) ? 1 : -1;
        int y = (int)y1;

        const int maxX = (int)x2;

        for(int x=(int)x1; x<maxX; x++) {
            if(steep) {
                setPixleHex(y,x, color);
            }
            else {
                setPixleHex(x,y, color);
            }

            error -= dy;
            if(error < 0) {
                y += ystep;
                error += dx;
            }
        }
    }
}

bool Screen::processEvents() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}

void Screen::close() {
    delete [] m_buffer1;
    delete [] m_buffer2;

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyTexture(m_texture);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}


