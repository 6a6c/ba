#ifndef JAKE_ENGINE
#define JAKE_ENGINE

#ifdef __EMSCRIPTEN__
#include "emscripten/emscripten.h"
#endif
#include "SDL3/SDL.h"

#include <geometry.hpp>

class engine {
    public:
        engine();
        ~engine();

        int run(); 

    private:
        bool poll();
        bool update();
        void render(uint64_t );
        void loop();

        int* gFrameBuffer;
        SDL_Window* gSDLWindow;
        SDL_Renderer* gSDLRenderer;
        SDL_Texture* gSDLTexture;
        int gDone;
        const int WINDOW_WIDTH = 1920 / 2;
        const int WINDOW_HEIGHT = 1080 / 2;

        mat4* baseTransform;
        mat4* cameraTransform;
        mat4 resultingMatrix;

        float x, y, z;
        float yaw, pitch, roll;

        object tmp;
};
#endif
