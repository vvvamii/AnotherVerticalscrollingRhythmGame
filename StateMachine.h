#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>
#include <map>

enum class GameState {
    MAIN_MENU,
    CHART_SELECTION,
    OPTIONS,
    RUNNING,
    PAUSED
};

class StateMachine {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    GameState currentState;
    std::vector<std::string> charts;
    std::vector<std::string> menuItems;
    int menuSelection;

    std::map<int, SDL_Keycode> keyBindings; // Lane to Key mapping
    int selectedLane; // Used in key remapping

public:
    StateMachine(SDL_Renderer* renderer);
    ~StateMachine();

    void handleEvent(SDL_Event& event);
    void render(SDL_Renderer* renderer);

    void goToMainMenu();
    void goToChartSelection();
    void goToOptions();
    void remapKey(int lane, SDL_Keycode newKey);
};

#endif
