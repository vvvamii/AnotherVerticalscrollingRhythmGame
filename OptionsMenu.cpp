#include "OptionsMenu.h"
#include <iostream>
#include <fstream>

OptionsMenu::OptionsMenu(SDL_Renderer* renderer, TTF_Font* font, std::map<int, SDL_Keycode>& keyBindings, int& volume)
    : renderer(renderer), font(font), keyBindings(keyBindings), selectedLane(0), waitingForKey(false), volume(volume) {
    loadConfig();
}

void OptionsMenu::handleEvent(SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (waitingForKey) {
            keyBindings[selectedLane] = event.key.key;
            std::cout << "Lane " << selectedLane + 1 << " bound to " << SDL_GetKeyName(event.key.key) << "\n";
            waitingForKey = false;
            saveConfig();
        }
        else {
            if (event.key.key == SDLK_UP) selectedLane = (selectedLane - 1 + 4) % 4;
            if (event.key.key == SDLK_DOWN) selectedLane = (selectedLane + 1) % 4;
            if (event.key.key == SDLK_LEFT) volume = std::max(0, volume - 5);
            if (event.key.key == SDLK_RIGHT) volume = std::min(100, volume + 5);
            if (event.key.key == SDLK_RETURN) {
                std::cout << "Press a key for Lane " << selectedLane + 1 << "\n";
                waitingForKey = true;
            }
            if (event.key.key == SDLK_ESCAPE) {
                std::cout << "Returning to Main Menu\n";
                returnToMainMenu();
            }
            saveConfig();
        }
    }
}

void OptionsMenu::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = { 255, 255, 255 };
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_FRect rect;

    for (int i = 0; i < 4; i++) {
        std::string keyText = "Lane " + std::to_string(i + 1) + ": " + SDL_GetKeyName(keyBindings[i]);
        surface = TTF_RenderText_Solid(font, keyText.c_str(), keyText.size(), (selectedLane == i) ? SDL_Color{ 255, 0, 0 } : white);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect = { 100, 100 + static_cast<float>(i) * 40, static_cast<float>(surface->w), static_cast<float>(surface->h) };
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);
    }

    std::string volumeText = "Volume: " + std::to_string(volume) + "%";
    surface = TTF_RenderText_Solid(font, volumeText.c_str(), volumeText.size(), white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect = { 100, 300, static_cast<float>(surface->w), static_cast<float>(surface->h) };
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);

    SDL_RenderPresent(renderer);
}

void OptionsMenu::saveConfig() {
    std::ofstream file("config.txt");
    if (file) {
        for (int i = 0; i < 4; i++) file << keyBindings[i] << "\n";
        file << volume << "\n";
    }
}

void OptionsMenu::loadConfig() {
    std::ifstream file("config.txt");
    if (file) {
        for (int i = 0; i < 4; i++) file >> keyBindings[i];
        file >> volume;
    }
}

void OptionsMenu::returnToMainMenu() {
   // stateMachine.goToMainMenu();
}