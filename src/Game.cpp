#include "Game.h"

//Initialises a SFML window
void Game::initWindow()
{
    win.create(sf::VideoMode(600, 700), "Snake Game");
    win.setFramerateLimit(6);
}

//Initialises textures
void Game::initTextures()
{
    //Initialising line, snake and apple texture
    initRectangleShape(line, sf::Vector2f(600, 1), sf::Vector2f(0, 97.5), sf::Color::White, 0, sf::Color::White);
    initRectangleShape(snakeBlock, sf::Vector2f(50, 50), sf::Vector2f(0, 0), sf::Color::White, -3, sf::Color::Black);
    initRectangleShape(appleBlock, sf::Vector2f(50, 50), sf::Vector2f(0, 0), sf::Color::Red, -3, sf::Color::Black);

    //Initialising font
    font.loadFromFile("Tiny5-Regular.ttf");

    //Initialising score board
    initText(scoreText, 48, sf::Vector2f(10, -10), sf::Color::White, "");
    initText(highScoreText, 48, sf::Vector2f(10, 38), sf::Color::White, "");

    //Initialising lost menu
    initText(lostText, 128, sf::Vector2f(300, 400), sf::Color::Red, "You Lost");
    centerAlign(lostText);

    //Initialising restart button
    initText(restartButtonText, 48, sf::Vector2f(300, 500), sf::Color::White, "Restart");
    centerAlign(restartButtonText);
    initRectangleShape(restartButtonShape, sf::Vector2f(restartButtonText.getLocalBounds().width + 20, restartButtonText.getLocalBounds().height + 20), sf::Vector2f(300, 500), sf::Color::Black, -3, sf::Color::White);
    centerAlign(restartButtonShape, sf::Vector2f(0, 6));
}

//Initialises rectangle shapes
void Game::initRectangleShape(sf::RectangleShape& rectangleShape, const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& fillColor, const float& outlineThickness, const sf::Color& outlineColor)
{
    rectangleShape.setSize(size);
    rectangleShape.setPosition(position);
    rectangleShape.setFillColor(fillColor);
    rectangleShape.setOutlineColor(outlineColor);
    rectangleShape.setOutlineThickness(outlineThickness);
}

//Initialises text
void Game::initText(sf::Text &text, const int &characterSize, const sf::Vector2f &position, const sf::Color &color, const std::string& label)
{
    text.setFont(font);
    text.setCharacterSize(characterSize);
    text.setPosition(position);
    text.setFillColor(color);
    text.setString(label);
}

//Sets origin of text to the center
void Game::centerAlign(sf::Text &text)
{
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2 + text.getCharacterSize() / 2);
}

//Sets origin of rectangles to the center
void Game::centerAlign(sf::RectangleShape& rectangleShape, const sf::Vector2f &offset)
{
    rectangleShape.setOrigin(rectangleShape.getLocalBounds().width / 2 + offset.x, rectangleShape.getLocalBounds().height / 2 + offset.y);
}

//Handles key presses and exit
void Game::handleEvents()
{
    while (win.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			win.close();
		}
        else if (e.type == sf::Event::KeyPressed && !lost)
        {
            if (e.key.scancode == sf::Keyboard::Scan::W)
            {
                lastButtonPressed = 'W';
            }
            else if (e.key.scancode == sf::Keyboard::Scan::A)
            {
                lastButtonPressed = 'A';
            }
            else if (e.key.scancode == sf::Keyboard::Scan::S)
            {
                lastButtonPressed = 'S';
            }
            else if (e.key.scancode == sf::Keyboard::Scan::D)
            {
                lastButtonPressed = 'D';
            }
        }
        else if (e.type == sf::Event::MouseButtonPressed && lost)
        {
            if (e.mouseButton.button == sf::Mouse::Left)
            {
                mousePos = sf::Mouse::getPosition(win);
                if (restartButtonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    std::cout << "Executed\n";
                    startGame();
                }
            }
        }
	}
}

//Updates game
void Game::update()
{
    //Update snake
    updateSnakeDirection();
    updateSnakePosition();

    for (auto it=snake.begin();it!=snake.end();it++)
    {
        keepSnakeInFrame(it);
        checkForCollision(it);
    }

    //Check when snake eats the apple
    if (snake.front() == apple)
    {
        growSnake();
        spawnApple();
        score++;
        updateScoreString();
    }
}

//Renders game
void Game::render()
{
    win.clear();

    if (lost)
    {
        win.draw(lostText);
        win.draw(restartButtonShape);
        win.draw(restartButtonText);
    }
    else
    {
        appleBlock.setPosition(apple.x * 50, apple.y * 50 + 100);
        win.draw(appleBlock);
        for (sf::Vector2i element : snake)
        {
            snakeBlock.setPosition(element.x * 50, element.y * 50 + 100);
            win.draw(snakeBlock);
        }
    }

    win.draw(line);
    win.draw(scoreText);
    win.draw(highScoreText);

    win.display();
}

void Game::startGame()
{
    lost = false;
    score = 0;
    updateScoreString();
    updateHighScoreString();
    lastButtonPressed = 'D';
    snake.clear();
    spawnSnake();
    spawnApple();
}

//Spawns a snake
void Game::spawnSnake()
{
    snake.emplace_back(3, 1);
    snake.emplace_back(2, 1);
    snake.emplace_back(1, 1);
    dir = sf::Vector2i(1, 0);
}

//Spawns an apple
void Game::spawnApple()
{
    bool validPosition;
    do
    {
        apple = sf::Vector2i(rand() % 12, rand() % 12);
        validPosition = std::none_of(snake.begin(), snake.end(), [this](sf::Vector2i segment)
        {
            return segment == apple;
        });
    } while (!validPosition);
}

//Updates snake direction
void Game::updateSnakeDirection()
{
    //Some checks to make sure snake can't turn 180 degrees
    switch (lastButtonPressed)
    {
        case 'W':
            if (dir.y != 1)
            {
                dir.x = 0;
                dir.y = -1;
            }
            break;
        case 'A':
            if (dir.x != 1)
            {
                dir.x = -1;
                dir.y = 0;
            }
            break;
        case 'S':
            if (dir.y != -1)
            {
                dir.x = 0;
                dir.y = 1;
            }
            break;
        case 'D':
            if (dir.x != -1)
            {
                dir.x = 1;
                dir.y = 0;
            }
            break;
    }
}

//Updates snake position
void Game::updateSnakePosition()
{
    snake.pop_back();
    snake.emplace_front(snake.front() + dir);
}

//Keeps snake within the frame
void Game::keepSnakeInFrame(const std::deque<sf::Vector2i>::iterator& snakeSegment)
{
    snakeSegment->x = (snakeSegment->x + 12) % 12;
    snakeSegment->y = (snakeSegment->y + 12) % 12;
}

//Checks if snake head collides with other snake segments
void Game::checkForCollision(const std::deque<sf::Vector2i>::iterator& snakeSegment)
{
    if (snake.begin() != snakeSegment && *snake.begin() == *snakeSegment)
    {
        lost = true;
        updateHighScoreString();
    }
}

//Increase snake length by one
void Game::growSnake()
{
    snake.push_back(snake.back());
}

void Game::updateScoreString()
{
    scoreText.setString("Score: " + std::to_string(score));
}

void Game::updateHighScoreString()
{
    if (score > highScore)
    {
        highScore = score;

        std::fstream file("score.conf");
        file << highScore << std::endl;
        file.close();
    }

    highScoreText.setString("Highscore: " + std::to_string(highScore));
}

Game::Game()
    : score(0)
{
    srand(time(0));
    initWindow();

    //Creates a score file if not already present
    std::fstream file("score.conf");
    if (!file)
    {
        std::ofstream newFile("score.conf");
        newFile << 0 << std::endl;
        newFile.close();
    }
    else
    {
        std::string highScoreString;
        std::getline(file, highScoreString);
        file.close();
        highScore = std::stoi(highScoreString);
    }

    initTextures();
    startGame();
}

Game::~Game()
{}

//Main game loop
void Game::run()
{
    while (win.isOpen())
    {
        handleEvents();
        if (!lost)
        {
            update();
        }
        render();
    }
}
