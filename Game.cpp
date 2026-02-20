#include <iostream>
#include <cmath>
#include "Game.h"

Game::Game()
{
    init();
}

void Game::init()
{
    sfmlVersion();
    if ( !font_.loadFromFile(fontPath_.c_str()) )
    {
    	//if we can't load the font, print an error to the error console and exit
    	std::cerr << "Could not load font!\n";
    	exit(-1);
    }

    toggleFullscreen(!isFullscreen_);
    window_.setFramerateLimit(fps_);

    text_.setFont(font_);
    text_.setString("FPS:");
    text_.setCharacterSize(24);
	text_.setFillColor(sf::Color::White);
    text_.setPosition(window_.getSize().x - text_.getCharacterSize()*4 , 0 + text_.getCharacterSize());
    //std::cout << "window size x " << window_.getSize().x << "\n";

    textureTiles_.loadFromFile("images/tiles.png");

    tiles_.setTexture(textureTiles_);
    tiles_.setTextureRect(sf::IntRect(36,0,18,18));
    tiles_.setPosition(window_.getSize().x/2, window_.getSize().y/2);

    player_.speed = 2;
    player_.posX  = window_.getSize().x/2;
    player_.posY  = window_.getSize().y/2;
    player_.code  = sf::Keyboard::End;

    grid_.setDimension(window_.getSize().x, window_.getSize().y);
    grid_.initNumbers(font_);
    grid_.getDimension();
}

void Game::toggleFullscreen(int isFullscreen)
{
    //change state
    isFullscreen_ = !isFullscreen;

    if (isFullscreen_)
    {
        //full screen
        sf::VideoMode fullscreen_mode = sf::VideoMode::getDesktopMode();
        window_.create(fullscreen_mode, title_, sf::Style::Fullscreen);
    }
    else
    {
        //normal window
        window_.create(sf::VideoMode(windowWidth_, windowHeight_), title_, sf::Style::Default);
        
        //center windows
        //window.setPosition(sf::Vector2i(100, 100)); 
    }
    
    //set frame rate after create window
    window_.setFramerateLimit(fps_);
}

void Game::setPause(bool pause)
{
    pause_ = pause;
}

void Game::sfmlVersion()
{
    std::cout << "SFML Version: " 
              << SFML_VERSION_MAJOR << "." 
              << SFML_VERSION_MINOR << "." 
              << SFML_VERSION_PATCH << std::endl;
}

void Game::run()
{
    while (running_)
    {
        if (!pause_)
        {
            move();
            //todo
            //collision()
        }
        input();
        render();

        currentFrame_++; 

        sf::Time frameTime = deltaClock_.restart();
        currentFps_ = 1.0f / frameTime.asSeconds();
    }

    window_.close();
}

void Game::input()
{
    sf::Event event;
    while (window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            running_ = false;

        if (event.type == sf::Event::LostFocus)
            pause_ = true;

        if (event.type == sf::Event::GainedFocus)
            pause_ = false;

        // this event is triggered when a key is pressed
        if( event.type == sf::Event::KeyPressed )
        {
            //print the key that was pressed to the console
            std::cout << "Key pressed with code = " << event.key.code << "\n";

            if ( event.key.code == sf::Keyboard::Escape ){
                running_ = false;
            }

            if ( event.key.code == sf::Keyboard::P ){
                pause_ = !pause_;
            }

            if ( event.key.code == sf::Keyboard::G ){
                showGridNumbers_ = !showGridNumbers_;
            }

            if ( event.key.code == sf::Keyboard::F11 ){
                toggleFullscreen(isFullscreen_);
            }

            if ( event.key.code == sf::Keyboard::F3 ){
                //todo imgui on/off
            }

            wasd(event);
        }

        if (event.type == sf::Event::KeyReleased)
        {
            player_.code = sf::Keyboard::End;
        }
    }
}

void Game::wasd(sf::Event event)
{
    if ( event.key.code == sf::Keyboard::W ){
        player_.code = sf::Keyboard::W;
    }
    if ( event.key.code == sf::Keyboard::A ){
        player_.code = sf::Keyboard::A;
    }
    if ( event.key.code == sf::Keyboard::S ){
        player_.code = sf::Keyboard::S;
    }
    if ( event.key.code == sf::Keyboard::D ){
        player_.code = sf::Keyboard::D;
    }

    if ( event.key.code == sf::Keyboard::Up ){
        player_.code = sf::Keyboard::Up;
    }
    if ( event.key.code == sf::Keyboard::Down ){
        player_.code = sf::Keyboard::Down;
    }
    if ( event.key.code == sf::Keyboard::Left ){
        player_.code = sf::Keyboard::Left;
    }
    if ( event.key.code == sf::Keyboard::Right ){
        player_.code = sf::Keyboard::Right;
    }
}

void Game::render()
{
    text_.setString("FPS "+std::to_string((int)std::roundf(currentFps_)));
    text_.setPosition(window_.getSize().x - text_.getCharacterSize()*4 , 0 + text_.getCharacterSize());
    //std::cout << "window size x " << window_.getSize().x << "\n";

    window_.clear(sf::Color::Blue);

    int step = grid_.getStep();
    const int numCols = window_.getSize().x / step;
    const int col     = player_.posX / step;
    const int row     = player_.posY / step;
    int endIndex      = row*numCols + col;

    grid_.drawRectangle(window_, 13, sf::Color::Green, -99, "Start");
    grid_.drawRectangle(window_, 41, sf::Color::Red, 50, "Enemy");

    sf::Color brown(139, 69, 19); // Marrone
    grid_.drawRectangle(window_, 19, brown, 25, "Mountain");
    grid_.drawRectangle(window_, 20, brown, 25, "Mountain");
    grid_.drawRectangle(window_, 31, brown, 25, "Mountain");
    grid_.drawRectangle(window_, 32, brown, 25, "Mountain");

    if ( lastEnd_ > -1 ){
        grid_.drawRectangle(window_, lastEnd_, sf::Color::Blue, -1, "");
    }
    grid_.drawRectangle(window_, endIndex, sf::Color::Cyan, 0, "End");

    if (lastEnd_ != endIndex){
        grid_.distanceMap();
        lastEnd_  = endIndex;
    }
    
    grid_.draw(window_, showGridNumbers_);
    grid_.pathfindingMove(window_, 13, endIndex);
    window_.draw(text_);
    window_.draw(tiles_);

    window_.display();
}

void Game::move()
{
    player_.velX = 0.0f;
    player_.velY = 0.0f;

    if ( player_.code == sf::Keyboard::W || player_.code == sf::Keyboard::Up ){
        player_.velY -= player_.speed;
    }
    if ( player_.code == sf::Keyboard::S || player_.code == sf::Keyboard::Down ){
        player_.velY += player_.speed;
    }

    if ( player_.code == sf::Keyboard::A || player_.code == sf::Keyboard::Left ){
        player_.velX -= player_.speed;
    }
    if ( player_.code == sf::Keyboard::D || player_.code == sf::Keyboard::Right ){
        player_.velX += player_.speed;
    }

    //std::cout << "code: " << player_.code << " ; " << player_.velX << " " << player_.velY << "\n";

    player_.posX += player_.velX;
    player_.posY += player_.velY;

    tiles_.setPosition(player_.posX, player_.posY);
}
