#pragma once
#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <string>

class OptionsMenu {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::map<int, SDL_Keycode>& keyBindings;
    int selectedLane;
    bool waitingForKey;
    int volume;

public:
    OptionsMenu(SDL_Renderer* renderer, TTF_Font* font, std::map<int, SDL_Keycode>& keyBindings, int& volume);
    void handleEvent(SDL_Event& event);
    void render();
    void saveConfig();
    void loadConfig();
    void returnToMainMenu();
};

#endif