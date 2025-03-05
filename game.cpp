#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "StateMachine.h"

int main() {

    // Initialize SDL and SDL_ttf
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() < 0) {
        printf("TTF_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("Rhythm Game", 800, 600, NULL);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize StateMachine
    StateMachine stateMachine(renderer);

    bool running = true;
    SDL_Event event;

    // Main loop
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            stateMachine.handleEvent(event);
        }

        SDL_RenderClear(renderer);
        stateMachine.render(renderer);
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
