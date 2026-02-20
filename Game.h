#pragma once

#include <SFML/Graphics.hpp>
#include "Grid.h"

struct Player
{
    int speed;
    float posX;
    float posY;
    float velX;
    float velY;
    sf::Keyboard::Key code;
};

class Game
{
private:

    sf::RenderWindow   window_;
    sf::Clock          deltaClock_;
    sf::Font           font_;                                  // load a font
    sf::Text           text_;                                  // for text
    sf::Texture        textureTiles_;
    sf::Sprite         tiles_;
    Player             player_;
    Grid               grid_;
    std::string        title_           = "Path Finding Demo!";
    std::string        fontPath_        = "fonts/arial.ttf";
    int                isFullscreen_    = 0;
    int                fps_             = 60;
    int                windowWidth_     = 900;
	int                windowHeight_    = 600;
    bool               pause_           = false;
    bool               running_         = true;
    float              currentFps_      = 0.0f;
    int                currentFrame_    = 0;
    bool               showGridNumbers_ = false;
    int                lastEnd_         = -1;

    void init();
    void toggleFullscreen(int isFullscreen);
    void setPause(bool pause);
    void sfmlVersion();
    void input();
    void render();
    void move();
    void wasd(sf::Event event);

public:

    Game();
    void run();
};