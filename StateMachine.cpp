#include "StateMachine.h"
#include <iostream>

StateMachine::StateMachine(SDL_Renderer* renderer) : renderer(renderer), currentState(GameState::MAIN_MENU), menuSelection(0), selectedLane(-1) {
    font = TTF_OpenFont("arial.ttf", 24);
    if (!font) std::cerr << "Failed to load font: " << SDL_GetError() << std::endl;
    charts = { "Chart 1", "Chart 2", "Chart 3" };
    menuItems = { "Play", "Options", "About", "Exit" };

    // Default keybindings
    keyBindings = {
        {0, SDLK_D}, // Lane 1
        {1, SDLK_F}, // Lane 2
        {2, SDLK_J}, // Lane 3
        {3, SDLK_K}  // Lane 4
    };
}

StateMachine::~StateMachine() {
    TTF_CloseFont(font);
}

void StateMachine::handleEvent(SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (currentState == GameState::MAIN_MENU) {
            if (event.key.key == SDLK_UP) menuSelection = (menuSelection - 1 + 4) % 4;
            if (event.key.key == SDLK_DOWN) menuSelection = (menuSelection + 1) % 4;
            if (event.key.key == SDLK_RETURN) {
                if (menuSelection == 0) goToChartSelection();
                if (menuSelection == 1) goToOptions();
                if (menuSelection == 2) std::cout << "About selected.\n";
                if (menuSelection == 3) exit(0);
            }
        }
        else if (currentState == GameState::OPTIONS) {
            if (event.key.key == SDLK_UP) selectedLane = (selectedLane - 1 + 4) % 4;
            if (event.key.key == SDLK_DOWN) selectedLane = (selectedLane + 1) % 4;
            if (event.key.key == SDLK_RETURN) std::cout << "Press a new key for Lane " << selectedLane + 1 << std::endl;
        }
        else if (currentState == GameState::CHART_SELECTION) {
            if (event.key.key == SDLK_UP) menuSelection = (menuSelection - 1 + charts.size()) % charts.size();
            if (event.key.key == SDLK_DOWN) menuSelection = (menuSelection + 1) % charts.size();
            if (event.key.key == SDLK_RETURN) std::cout << "Starting game with " << charts[menuSelection] << std::endl;
        }
    }
    else if (event.type == SDL_EVENT_KEY_DOWN && currentState == GameState::OPTIONS) {
        if (selectedLane != -1) {
            remapKey(selectedLane, event.key.key);
            selectedLane = -1;
        }
    }
}

void StateMachine::goToMainMenu() { currentState = GameState::MAIN_MENU; menuSelection = 0; }
void StateMachine::goToChartSelection() { currentState = GameState::CHART_SELECTION; menuSelection = 0; }
void StateMachine::goToOptions() { currentState = GameState::OPTIONS; selectedLane = -1; }

void StateMachine::remapKey(int lane, SDL_Keycode newKey) {
    keyBindings[lane] = newKey;
    std::cout << "Lane " << lane + 1 << " now uses key: " << SDL_GetKeyName(newKey) << std::endl;
}

void StateMachine::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = { 255, 255, 255 };
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_FRect rect;

    if (currentState == GameState::MAIN_MENU) {
        for (size_t i = 0; i < menuItems.size(); i++) {
            surface = TTF_RenderText_Solid(font, menuItems[i].c_str(), menuItems[i].size(), (menuSelection == i) ? SDL_Color{ 255, 0, 0 } : white);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            rect = { 100, 100 + static_cast<float>(i) * 40, static_cast<float>(surface->w), static_cast<float>(surface->h) };
            SDL_RenderTexture(renderer, texture, nullptr, &rect);
            SDL_DestroyTexture(texture);
            SDL_DestroySurface(surface);
        }
    }
    else if (currentState == GameState::OPTIONS) {
        std::string optionText = "Change Keybindings (Use Arrow Keys, Enter to Select)";
        surface = TTF_RenderText_Solid(font, optionText.c_str(), optionText.size(), white);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect = { 100, 50, static_cast<float>(surface->w), static_cast<float>(surface->h) };
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);

        for (int i = 0; i < 4; i++) {
            std::string keyText = "Lane " + std::to_string(i + 1) + ": " + SDL_GetKeyName(keyBindings[i]);
            surface = TTF_RenderText_Solid(font, keyText.c_str(), keyText.size(), (selectedLane == i) ? SDL_Color{ 255, 0, 0 } : white);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            rect = { 100, 100 + static_cast<float>(i) * 40, static_cast<float>(surface->w), static_cast<float>(surface->h) };
            SDL_RenderTexture(renderer, texture, nullptr, &rect);
            SDL_DestroyTexture(texture);
            SDL_DestroySurface(surface);
        }
    }

    SDL_RenderPresent(renderer);
}
