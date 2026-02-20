#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct Cell {
    int value;
    std::string target;
};

class Grid
{
private:
    int                   width_;
    int                   height_;
    int                   step_            = 75;        //grid step
    std::vector<sf::Text> cellNumbers_;
    std::vector<Cell> cellInfo_;
    sf::Font              font_;
    
    void init();
    void influenceMap();
    void setCellInfo(int cellNumber, int value, std::string target);
    void drawCircle(sf::RenderWindow& window, sf::Color color, int col, int row);

public:
    Grid();
    Grid(int width, int height);
    ~Grid();
    int getStep(){ return step_; };
    void initNumbers(const sf::Font& font);
    void getDimension();
    void setDimension(int width, int height);
    void draw(sf::RenderWindow& window, bool showNumbers);
    void drawRectangle(sf::RenderWindow& window, int row, int col, sf::Color color);
    void drawRectangle(sf::RenderWindow& window, int cellNumber, sf::Color color, int value, std::string target);
    void pathfindingMove(sf::RenderWindow& window, int startIndex, int endIndex);
    void distanceMap();
};


