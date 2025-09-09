// Correct the brach speed
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
void updateBranches(int seed);
// void updateRain(int seed);
const int NUM_BRANCHES = 6;
// const int NUM_RAIN = 10;
Sprite branches[NUM_BRANCHES];
// Sprite rain[NUM_RAIN];
enum class side
{
    LEFT,
    RIGHT,
    NONE
};
side branchPositions[NUM_BRANCHES];
int main()
{
    // create a video mode object
    // VideoMode vm(1920,1080);

    // Low resolution code
    VideoMode vm(960, 540);
    // Create and open a window for the game
    // RenderWindow window (vm,"Timber!!!",Style::Fullscreen);

    // Low resolution code
    RenderWindow window(vm, "Timber!!!");
    View view(FloatRect(0, 0, 1920, 1080));
    window.setView(view);
    
    // create a texture to hold a graphic on the GPU
    Texture textureBackground;
    // Load a graphics into the texture
    textureBackground.loadFromFile("graphics/background.png");
    // Create a sprite to hold the texture
    Sprite spriteBackground;
    // Attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);
    // Set the spriteBackground to cover the whole window
    spriteBackground.setPosition(0, 0);

    // Make a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree1;
    spriteTree1.setTexture(textureTree);
    spriteTree1.setPosition(810, 0);

    // Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 2000);
    // Is the bee currently moving ?
    bool beeActive = false;
    // how fast can the bee fly
    float beeSpeed = 0.0f;


    // Prepare the cloud
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);

    // Those values are given to show the clouds before the start of the game onn the screen
    //  spriteCloud1.setPosition(0, 0);
    //  spriteCloud2.setPosition(0, 250);
    //  spriteCloud3.setPosition(0, 500);

    // This value given for no cloud should show before start the game (Out of the screen )
    spriteCloud1.setPosition(2000, 0);
    spriteCloud2.setPosition(2000, 0);
    spriteCloud3.setPosition(2000, 0);

    // Are the clouds currently on screen
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // rain drop speed
    // float raindropSpeed = 500.0f; // Adjust this value for faster/slower rain

    Clock clock;

    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    // set the color for the timebar
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    // Track whether the game is running
    bool paused = true;
    // Draw some text
    int score = 0;

    Text messageText;
    Text scoreText;
    // We need to choose the font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    // Set the font to our text
    messageText.setFont(font);
    scoreText.setFont(font);
    // Assign the actual message
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");
    // Make it really big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    // Choose a color
    messageText.setFillColor(Color::White); // Changed to setFillColor
    scoreText.setFillColor(Color::White);   // Changed to setFillColor
    // Position the text
    FloatRect textRect = messageText.getLocalBounds();
    // FloatRect textRect = messageText.getLocalBounds();//Get co-ordinate of whole screen of mine
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

    scoreText.setPosition(20, 20);
    // For the branch
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }

    // Texture textureRain;
    // textureRain.loadFromFile("graphics/rain.png");
    // for (int i = 0; i < NUM_RAIN; i++)
    // {
    //     rain[i].setTexture(textureRain);
    //     rain[i].setPosition(-2000, -2000);
    //     rain[i].setOrigin(220, 20);
    // }

    // Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    // spritePlayer.setPosition(580, 720);
    spritePlayer.setPosition(580, 720); // left side ratio
    // spritePlayer.setPosition(1200, 700); //Right side ratio

    // The player starts on the left
    side playerSide = side::LEFT;

    // Prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(675, 2000);

    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    // Initial position of the Axe before start the game
    spriteAxe.setPosition(2000, 830);

    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // Some other useful log related variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // Control the player input
    bool acceptInput = false;

    // prepare the sound

    // chop sound
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    // death sound
    SoundBuffer deathBuffer;
    chopBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    // outofTime sound
    SoundBuffer soundBuffer;
    soundBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(soundBuffer);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                // Listen for key presses again
                acceptInput = true;

                // hide the axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        // Start the game
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;
            // Reset the time and the score
            score = 0;
            timeRemaining = 6;
            // Make all the branches disappear 
            // starting in the second position
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }
            // Make sure the gravestone is hidden
            spriteRIP.setPosition(675, 2000);
            // set the player into position
            spritePlayer.setPosition(580, 720);
            acceptInput = true;
        }
        // Wrap the player controls to
        // Make sure we are accepting input
        if (acceptInput)
        {
            // First handle pressing the right cursor key
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                // Make sure the player is on the right
                playerSide = side::RIGHT;
                score++;
                // Add to the amount of time remaining
                timeRemaining += (2 / score) + .15;
                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1200, 720);
                // Update the branches
                updateBranches(score);
                // Set the log flying to the left
                spriteLog.setPosition(810, 720);
                logSpeedX = -5000; // Fly the log upward but on the left side of the screen
                logActive = true;  // for active the log otherwise speed values will not working
                acceptInput = false;
                chop.play();
            }
            // Handle the left cursor key
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                // Make sure the player is on the left
                playerSide = side::LEFT;
                score++;
                // Add to the amount of time remaining
                timeRemaining += (2 / score) + .15;
                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);
                // update the branches
                updateBranches(score);
                // set the log flying right
                spriteLog.setPosition(810, 720);
                logSpeedX = 5000; // for the log flying upward but right side of the screen
                // logSpeedY = true;
                logActive = true; // Make log active otherwise no speed will be accepted or will work
                acceptInput = false;
                chop.play();
            }
        }
        if (!paused)
        {
            // Measure time
            Time dt = clock.restart();
            // Move the raindrops
            // for (int i = 0; i < NUM_RAIN; i++)
            // {
            //     rain[i].move(0, raindropSpeed * dt.asSeconds()); // Move downwards
            //     // Reset raindrop position when it goes off-screen
            //     if (rain[i].getPosition().y > 1080)
            //     {
            //         // Set raindrop (width of the screen)
            //         rain[i].setPosition(rand() % 2000, -rain[i].getGlobalBounds().height); // Random x-position, start from the top
            //     }
            // }

            // Subtract from the amount of time remaining
            timeRemaining -= dt.asSeconds();
            // Sizeup the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
            if (timeRemaining <= 0.0f)
            {
                // Pause the game
                paused = true;

                // Change the message shown to the player
                messageText.setString("Out of time!");

                // Reposition the text based on its new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                outOfTime.play();
            }

            // Setup the bee
            if (!beeActive)
            {
                // How fast is the bee
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;

                // How high is the bee
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else
            // Move the bee
            {
                spriteBee.setPosition(
                    spriteBee.getPosition().x -
                        (beeSpeed * dt.asSeconds()),
                    spriteBee.getPosition().y);

                // Has the bee reached the left-hand edge of the screen?
                if (spriteBee.getPosition().x < -100)
                {
                    // Set it up ready to be a whole new bee next frame
                    beeActive = false;
                }
            }

            // Cloud 1
            if (!cloud1Active)
            {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else
            {
                spriteCloud1.setPosition(
                    spriteCloud1.getPosition().x +
                        (cloud1Speed * dt.asSeconds()),
                    spriteCloud1.getPosition().y);

                if (spriteCloud1.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud next frame
                    cloud1Active = false;
                }
            }

            // Cloud 2
            if (!cloud2Active)
            {
                // How fast is the cloud
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);

                // How high is the cloud
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else
            {
                spriteCloud2.setPosition(
                    spriteCloud2.getPosition().x +
                        (cloud2Speed * dt.asSeconds()),
                    spriteCloud2.getPosition().y);

                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud2.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud next frame
                    cloud2Active = false;
                }
            }

            if (!cloud3Active)
            {
                // How fast is the cloud
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);

                // How high is the cloud
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else
            {
                spriteCloud3.setPosition(
                    spriteCloud3.getPosition().x +
                        (cloud3Speed * dt.asSeconds()),
                    spriteCloud3.getPosition().y);

                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud3.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud next frame
                    cloud3Active = false;
                }
            }

            // Update the score text
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            // Update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;

                if (branchPositions[i] == side::LEFT)
                {
                    // Move the sprite to the left side
                    branches[i].setPosition(610, height);

                    // Flip the sprite round the other way
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    // Move the sprite to the right side
                    branches[i].setPosition(1330, height);

                    // Set the sprite rotation to normal
                    branches[i].setRotation(0);
                }
                else
                {
                    // Hide the branch
                    branches[i].setPosition(3000, height);
                }
            }

            // Handle a flying log
            if (logActive)
            {
                spriteLog.setPosition(
                    spriteLog.getPosition().x +
                        (logSpeedX * dt.asSeconds()),
                    spriteLog.getPosition().y +
                        (logSpeedY * dt.asSeconds()));

                // Has the log reached the right hand edge?
                if (spriteLog.getPosition().x < -100 ||
                    spriteLog.getPosition().x > 2000)
                {
                    // Set it up ready to be a whole new log next frame
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }

            // Has the player been squished by a branch ?
            if (branchPositions[5] == playerSide)
            {
                // Death
                paused = true;
                acceptInput = false;

                // Draw the gravestone
                // spriteRIP.setPosition(525, 760);
                // spriteRIP.setPosition(580, 740); // It will appear only oe side of the screen

                // Grave Stone will appear where player is present
                spriteRIP.setPosition(spritePlayer.getPosition().x, (spritePlayer.getPosition().y + 30)); // Gravestone will appear where the player will hit the branch

                // Hide the player
                spritePlayer.setPosition(2000, 660);
                // Hide the Axe
                spriteAxe.setPosition(2000, 820);
                // Change the text of the message
                messageText.setString("Game Over!");

                // Center it on the screen
                FloatRect textRect = messageText.getLocalBounds();

                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f,
                                        1080 / 2.0f);
                death.play();
            }
        }

        window.clear();

        window.draw(spriteBackground);

        // Draw the clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        // Draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }
        // draw rain drops
        // for (int i = 0; i < NUM_RAIN; i++)
        // {
        //     window.draw(rain[i]);
        // }
        // Draw the trees
        window.draw(spriteTree1);
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP);
        window.draw(spriteBee);
        window.draw(scoreText);
        window.draw(timeBar);
        if (paused)
        {
            window.draw(messageText);
        }

        window.display();
    }

    return 0;
}

void updateBranches(int seed)
{

    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }

    srand((int)time(0) + seed);
    int r = (rand() % 5);

    switch (r)
    {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }
}
