#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <ctime>
#include <algorithm>




// Game parameters
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;  
const float BirdMaxSpeed = -15.0f;
const float BirdMinSpeed = 15.0f;
const sf::Vector2<float> BirdDefaultPos = {250, 250};
const float gravity_acc = -1.0f;
const float PipeSpeed = 5.0f;
const float PipeWidth = 100.0f;
// const float PipeLength = 500.0f;
const float GapSize  = 250.0f; 
const sf::Time SPAWN_INTERVAL = sf::seconds(1.5f);


bool gameOver = false;

int main()
{
    

    // struct for two pipes with a gap in between
    struct PipePair {
        sf::RectangleShape topPipe;
        sf::RectangleShape bottomPipe;
    };
    

    // Create the window
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "FlappyBird");
    sf::Font font("C:\\Windows\\Fonts\\arial.ttf"); 

    // Bird
    sf::CircleShape bird(40.0f);
    bird.setFillColor(sf::Color::Yellow);
    bird.setPosition(BirdDefaultPos);

    // Bird vertical speed
    float speed = -10.0f;

    // Pipes
    std::vector<PipePair> pipes;
    sf::Clock spawnClock;
    //std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Pipe Score
    int score = 0;
    sf::Text Score_message(font);
    Score_message.setString(std::to_string(score));
    Score_message.setFillColor(sf::Color::Blue);
    Score_message.setCharacterSize(50);
    Score_message.setPosition({1200, 600});

    // Game Over Message
    sf::Text message_go(font);           // Set the font
    message_go.setString("GAME OVER!");   // Text to display
    message_go.setCharacterSize(60);        // Font size in pixels
    message_go.setFillColor(sf::Color::Red); // Text color
    message_go.setPosition({440, 300}); 

    // Restart Message
    sf::Text message_restart(font);           // Set the font
    message_restart.setString("press R to restart");   // Text to display
    message_restart.setCharacterSize(20);        // Font size in pixels
    message_restart.setFillColor(sf::Color::White); // Text color
    message_restart.setPosition({550, 400});


    

    window.setFramerateLimit(60);

    while (window.isOpen())
    {

        

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())  window.close();

            if (event->is<sf::Event::KeyPressed>())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                {
                    speed = BirdMaxSpeed;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) && gameOver)
                {
                    bird.setPosition(BirdDefaultPos);
                    speed = BirdMaxSpeed;
                    score = 0;
                    Score_message.setString(std::to_string(score));
                    pipes.clear();
                    gameOver = false;
                }
            }
        }

        if(!gameOver)
        {
            // Bird Vertical Movement
            speed -= gravity_acc;
            if(speed > BirdMinSpeed) speed = BirdMinSpeed;
            bird.move({0, speed});


            
            // Create new Pipes
            if (spawnClock.getElapsedTime() >= SPAWN_INTERVAL) 
            {
                float gapY = 100 + std::rand() % (WINDOW_HEIGHT -  static_cast<int>(GapSize) - 200);

                PipePair newPipe;
                
                newPipe.topPipe.setSize({PipeWidth, gapY});
                newPipe.topPipe.setFillColor(sf::Color::Green);
                newPipe.topPipe.setPosition({WINDOW_WIDTH, 0});

                newPipe.bottomPipe.setSize({PipeWidth, WINDOW_HEIGHT - gapY - GapSize});
                newPipe.bottomPipe.setFillColor(sf::Color::Green);
                newPipe.bottomPipe.setPosition({WINDOW_WIDTH, gapY + GapSize});

                pipes.push_back(newPipe);
                spawnClock.restart();
            }

            // Move Pipes
            for(auto& pair : pipes)
            {
                pair.topPipe.move({-PipeSpeed,  0});
                pair.bottomPipe.move({-PipeSpeed,  0});
            }

            // Remove Pipes off screen
            pipes.erase(
                std::remove_if(pipes.begin(), pipes.end(), [](const PipePair& pair) {
                    return pair.topPipe.getPosition().x + PipeWidth < 0;
                }),
                pipes.end()
            );

            // Add a pased pipe to a score
            for(const auto& pair : pipes)
            {
                if (bird.getPosition().x == pair.topPipe.getPosition().x)
                {
                    score++;
                    Score_message.setString(std::to_string(score));
                    break;
                }
            }
            

            // Detect collision with pipes
            for (const auto& pair : pipes)
            {
                if (bird.getGlobalBounds().findIntersection(pair.topPipe.getGlobalBounds()) ||
                    bird.getGlobalBounds().findIntersection(pair.bottomPipe.getGlobalBounds()))
                {
                    gameOver = true;
                    break;
                }
            }

            // Detect Collision with floor and ceiling
            if(bird.getPosition().y <= 0 || bird.getPosition().y + 80 >= WINDOW_HEIGHT)
            {
                gameOver = true;
            }
        }
        



        window.clear();
        if (!gameOver)
        {    
            window.draw(bird);
            for(const auto& pair : pipes)
            {
                window.draw(pair.topPipe);
                window.draw(pair.bottomPipe);
            }
            window.draw(Score_message);

        }
        else 
        {
            window.draw(message_go);
            window.draw(message_restart);
        }

        window.display();
    }

    return 0;
}