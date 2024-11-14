#pragma once

#include <cstdlib>
#include <ctime>
#include <deque>
#include <fstream>
#include <iostream>

#include "SFML/Graphics.hpp"

class Game
{
private:
    sf::RenderWindow win;
    sf::Event e;

    std::deque<sf::Vector2i> snake;
    sf::Vector2i apple;
    char lastButtonPressed;
    sf::Vector2i mousePos;
    bool lost;
    int score;
    int highScore;
    sf::Vector2i dir;
    sf::Font font;
    sf::Text scoreText;
    sf::Text highScoreText;
    sf::Text lostText;
    sf::Text restartButtonText;
    sf::RectangleShape line;
    sf::RectangleShape snakeBlock;
    sf::RectangleShape appleBlock;
    sf::RectangleShape restartButtonShape;

    void initWindow();
    void initTextures();
    void initRectangleShape(sf::RectangleShape& rectangleShape, const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& fillColor, const float& outlineThickness, const sf::Color& outlineColor);
    void initText(sf::Text& text, const int& characterSize, const sf::Vector2f& position, const sf::Color& color, const std::string& label);
    void centerAlign(sf::Text& text);
    void centerAlign(sf::RectangleShape& rectangleShape, const sf::Vector2f& offset);

    void handleEvents();
    void update();
    void render();

    void startGame();
    void spawnSnake();
    void spawnApple();
    void updateSnakeDirection();
    void updateSnakePosition();
    void keepSnakeInFrame(const std::deque<sf::Vector2i>::iterator& snakeSegment);
    void checkForCollision(const std::deque<sf::Vector2i>::iterator& snakeSegment);
    void growSnake();
    void updateScoreString();
    void updateHighScoreString();

public:
    Game();
    virtual ~Game();

    void run();
};


class Button
{
private:
    enum button_states{BTN_IDLE = 0, BTN_HOVER, BTN_PRESSED};
    short unsigned buttonState;

    sf::RectangleShape shape;
    sf::Font* font;
    sf::Text text;

    sf::Color textIdleColor;
    sf::Color textHoverColor;

public:
    Button(const float& x, const float& y, const std::string& text, const int& characterSize);
    ~Button();

    const bool isPressed();

    void update();
    void rander();
};