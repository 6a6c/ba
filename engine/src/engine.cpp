#include "engine.hpp"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

engine::engine() {
    this->tmp.readFromFile((char *)"assets/utah.tri");

    this->baseTransform = basePerspectiveMatrix(((float) WINDOW_HEIGHT / WINDOW_WIDTH), 1.57, 1, 100);
    this->cameraTransform = cameraTransformMatrix(0, 0, 0, 0, -1, 4);
    
    this->x = &(this->cameraTransform->matrix[0][3]);
    this->y = &(this->cameraTransform->matrix[1][3]);
    this->z = &(this->cameraTransform->matrix[2][3]);
}

engine::~engine() {
    delete[] this->baseTransform;
    delete[] this->cameraTransform;
}

/* SDL initialization and starts main loop */
int engine::run() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        return -1;
    }

    this->gFrameBuffer = new int[WINDOW_WIDTH * WINDOW_HEIGHT];
    this->gSDLWindow = SDL_CreateWindow("SDL3 window", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    this->gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL);
    this->gSDLTexture = SDL_CreateTexture(gSDLRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!gFrameBuffer || !gSDLWindow || !gSDLRenderer || !gSDLTexture)
        return -1;

    gDone = 0;
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(loop, 0, 1);
    #else
    while (gDone != 1)
    {
        this->loop();
    }
    #endif

    SDL_DestroyTexture(this->gSDLTexture);
    SDL_DestroyRenderer(this->gSDLRenderer);
    SDL_DestroyWindow(this->gSDLWindow);
    SDL_Quit();

    return 0;
}

/* Polls IO. Returns false if we should stop updating and leave */
bool engine::poll() {
    /* Handle events */
    SDL_Event e; 
    
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            return false;
        }
        if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE) {
            return false;
        }
        if(e.type == SDL_EVENT_KEY_DOWN) {
            switch(e.key.key){
                case SDLK_W:
                    *this->z -= .1;
                    break;
                case SDLK_S:
                    *this->z += .1;
                    break;
                case SDLK_A:
                    *this->x += .1;
                    break;
                case SDLK_D:
                    *this->x -= .1;
                    break;
                case SDLK_SPACE:
                    *this->y -= .1;
                    break;
                case SDLK_LCTRL:
                    *this->y += .1;
                    break;
            }
        }
    }

    return true;
}

/* Updates the SDL Renderer with frame buffer */
bool engine::update() {
    char* pix;
    int pitch;

    if(!this->poll()){
        return false;
    }

    /* Clear renderer */
    SDL_RenderClear(this->gSDLRenderer);

    /* Copy data from frame buffer into render texture */
    SDL_LockTexture(this->gSDLTexture, NULL, (void**)&pix, &pitch);
    for (int i = 0, sp = 0, dp = 0; i < WINDOW_HEIGHT; i++, dp += WINDOW_WIDTH, sp += pitch)
        memcpy(pix + sp, this->gFrameBuffer + dp, WINDOW_WIDTH * 4);

    SDL_UnlockTexture(this->gSDLTexture);  
    
    /* Render the texture */
    SDL_RenderTexture(this->gSDLRenderer, gSDLTexture, NULL, NULL);
    SDL_RenderPresent(this->gSDLRenderer);

    /* Delay between updates */
    SDL_Delay(1);

    return true;
}

/* Renders the world into frame buffer */
void engine::render(Uint64 aTicks) {
    this->resultingMatrix = *this->baseTransform * *this->cameraTransform;

    this->tmp.project(&(this->resultingMatrix));
    
    this->tmp.rasterize(WINDOW_WIDTH, WINDOW_HEIGHT, this->gFrameBuffer);
}

/* Main loop */
void engine::loop() {
    /* Update the SDL stuff */
    if (!update()) {
        /* If false, exit the program */
        gDone = 1;
        #ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
        #endif
    }else {
        /* If true, clear frame buffer and render next frame */
        memset(this->gFrameBuffer, 0x0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));
        this->render(SDL_GetTicks());
    }
}
