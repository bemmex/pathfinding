#include <iostream>
#include <queue>
#include <iomanip>
#include <chrono>
#include "Grid.h"

Grid::Grid()
{}

Grid::Grid(int width, int height):
    width_(width), 
    height_(height)
{}

Grid::~Grid()
{}

void Grid::init()
{}

void Grid::initNumbers(const sf::Font& font)
{
    font_       = font;
    int numRows = height_ / step_;
    int numCols = width_ / step_;
    cellNumbers_.clear();

    for (int r = 0; r < numRows; ++r)
    {
        for (int c = 0; c < numCols; ++c)
        {
            int cellNumber = r * numCols + c;
            sf::Text text;
            text.setFont(font);
            text.setString(std::to_string(cellNumber));
            text.setCharacterSize(12);
            text.setFillColor(sf::Color::White);
            text.setPosition(c * step_ + 5, r * step_ + 5);
            cellNumbers_.push_back(text);

            Cell cinfo;
            cinfo.value  = -1;
            cinfo.target = "";
            cellInfo_.push_back(cinfo);
        }
    }
}

void Grid::getDimension()
{
    std::cout << "Grid: (" << width_ << "," << height_ << ")\n";
}

void Grid::setDimension(int width, int height)
{
    width_  = width;
    height_ = height;
}

void Grid::draw(sf::RenderWindow& window, bool showNumbers)
{
    for (int i = 0; i <= width_ / step_; ++i)
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(i * step_, 0)),
            sf::Vertex(sf::Vector2f(i * step_, height_))
        };
        window.draw(line, 2, sf::Lines);
    }

    for (int i = 0; i <= height_ / step_; ++i)
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(0, i * step_)),
            sf::Vertex(sf::Vector2f(width_, i * step_))
        };
        window.draw(line, 2, sf::Lines);
    }

    if (showNumbers)
    {
        for (const auto& text : cellNumbers_)
        {
            window.draw(text);
        }

        int numCols    = width_ / step_;
        int cellNumber = 0;
        for (const auto& cell : cellInfo_)
        {
            int row = cellNumber / numCols;
            int col = cellNumber % numCols;
            sf::Text text;
            text.setFont(font_);
            text.setCharacterSize(12);
            text.setFillColor(sf::Color::White);
            text.setPosition(col * step_ + (step_-20), row * step_ + 5);
            text.setString(std::to_string(cell.value));

            sf::Text target;
            target.setFont(font_);
            target.setCharacterSize(14);
            target.setFillColor(sf::Color::White);
            target.setPosition(col * step_ + 5, row * step_ + 50);
            target.setString(cell.target);

            window.draw(text);
            window.draw(target);

            cellNumber++;
        }
    }
}

void Grid::setCellInfo(int cellNumber, int value, std::string target)
{
    if (value != -99){
        cellInfo_.at(cellNumber).value  = value;
    }
    //cellInfo_.at(cellNumber).value  = value;
    cellInfo_.at(cellNumber).target = target;
}

void Grid::drawRectangle(sf::RenderWindow& window, int row, int col, sf::Color color)
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(step_-1, step_-1));
    rectangle.setPosition(col * step_, row * step_);
    rectangle.setFillColor(color);
    window.draw(rectangle);
}

void Grid::drawRectangle(sf::RenderWindow& window, int cellNumber, sf::Color color, int value, std::string target)
{
    setCellInfo(cellNumber, value, target);
    int numCols = width_ / step_;
    int row = cellNumber / numCols;
    int col = cellNumber % numCols;
    drawRectangle(window, row, col, color );
}

void Grid::distanceMap()
{
    auto startTime = std::chrono::high_resolution_clock::now();

    std::queue<int> q;

    //reset all values
    /*for (auto& cell : cellInfo_)
    {
        if (cell.target != "Enemy")
            cell.value = -1;
    }*/

    //find END index
    int endIndex = -1;
    for (int i = 0; i < cellInfo_.size(); ++i)
    {
        cellInfo_[i].value = -1;
        if (cellInfo_[i].target == "End")
        {
            endIndex = i;
            //std::cout << "End found! " << endIndex << "\n";
            //break;
        }
    }
    auto endTime  = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "DistanceMap Clear time: " << duration.count() << " microseconds\n";

    if (endIndex == -1){
        std::cout << "End not found! \n";
        return;
    }

    //init BFS
    cellInfo_[endIndex].value = 0;
    q.push(endIndex);

    const int numCols = width_ / step_;
    const int numRows = height_ / step_;

    //core BFS
    while (!q.empty())
    {
        int current = q.front();
        //std::cout << "Current: " << current << "\n";
        q.pop();

        int x = current % numCols;
        int y = current / numCols;

        int currentValue = cellInfo_[current].value;

        //std::cout << "Current Value: " << currentValue << "(x,y) (" <<  x << "," << y <<  ")\n";

        //set four directions
        const int dx[4] = {1, -1, 0, 0};
        const int dy[4] = {0, 0, 1, -1};

        for (int i = 0; i < 4; ++i)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];

            //check border
            if (nx < 0 || nx >= numCols || ny < 0 || ny >= numRows)
                continue;

            int neighborIndex = ny * numCols + nx;
            //std::cout << "neighborIndex " <<  neighborIndex << "\n";
            //if ( neighborIndex > cellInfo_.size()-1 ){
            //    std::cout << "neighborIndex out of range" << "\n";
            //}

            if (cellInfo_[neighborIndex].value == -1 &&
                cellInfo_[neighborIndex].target != "Enemy" &&
                cellInfo_[neighborIndex].target != "Mountain"
            ){
                cellInfo_[neighborIndex].value = currentValue + 1;
                q.push(neighborIndex);
            }
        }
    }

    endTime  = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "DistanceMap time: " << duration.count() << " microseconds\n";
}

void Grid::pathfindingMove(sf::RenderWindow& window, int startIndex, int endIndex)
{
    if (startIndex < 0 || startIndex >= static_cast<int>(cellInfo_.size())) return;
    if (cellInfo_[startIndex].value == -1) return;

    int current = startIndex;
    int numCols = width_ / step_;
    int numRows = height_ / step_;

    //direction: right, left, down, up
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    int maxSteps = cellInfo_.size(); //safety break
    int steps    = 0;

    while (current != endIndex && steps < maxSteps)
    {
        int row = current / numCols;
        int col = current % numCols;

        drawCircle(window, sf::Color::Yellow, col, row);

        int currentValue = cellInfo_[current].value;
        int next = -1;

        //search neighbor with value (currentValue - 1)
        for (int i = 0; i < 4; ++i)
        {
            int nx = col + dx[i];
            int ny = row + dy[i];

            if (nx >= 0 && nx < numCols && ny >= 0 && ny < numRows)
            {
                int neighborIdx = ny * numCols + nx;
                if (cellInfo_[neighborIdx].value == currentValue - 1)
                {
                    next = neighborIdx;
                    break; 
                }
            }
        }

        if (next != -1) current = next;
        else break;

        steps++;
    }

    //draw last point (End)
    if (current == endIndex)
    {
        int row = current / numCols;
        int col = current % numCols;
        drawCircle(window, sf::Color::Yellow, col, row);
    }
}

void Grid::drawCircle(sf::RenderWindow& window, sf::Color color, int col, int row)
{
    sf::CircleShape circle(step_ / 5.0f);
    circle.setFillColor(color);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(col * step_ + step_ / 2.0f, row * step_ + step_ / 2.0f);
    window.draw(circle);
}

void Grid::influenceMap(){}