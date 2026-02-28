// Correct the branch speed - updated for SFML 3
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
// SFML 3: sf::Sprite has no default constructor, so we use a vector
// and a global texture so updateBranches() can still access branches[]
Texture textureBranch;
std::vector<Sprite> branches;
enum class side
{
    LEFT,
    RIGHT,
    NONE
};
side branchPositions[NUM_BRANCHES];
int main()
{
    // Low resolution code
    // SFML 3: VideoMode takes a Vector2u instead of two separate unsigned ints
    VideoMode vm({960u, 540u});
    // Create and open a window for the game
    RenderWindow window(vm, "Timber!!!");
    // SFML 3: FloatRect takes two Vector2f (position, size) instead of four floats
    View view(FloatRect({0.f, 0.f}, {1920.f, 1080.f}));
    window.setView(view);

    // Background
    Texture textureBackground;
    if (!textureBackground.loadFromFile("../assets/graphics/background.png"))
        return -1;
    // SFML 3: Sprite must be constructed with a Texture; no default constructor
    Sprite spriteBackground(textureBackground);
    // SFML 3: setPosition takes a single Vector2f, not two separate floats
    spriteBackground.setPosition({0.f, 0.f});

    // Tree
    Texture textureTree;
    if (!textureTree.loadFromFile("../assets/graphics/tree.png"))
        return -1;
    Sprite spriteTree1(textureTree);
    spriteTree1.setPosition({810.f, 0.f});

    // Bee
    Texture textureBee;
    if (!textureBee.loadFromFile("../assets/graphics/bee.png"))
        return -1;
    Sprite spriteBee(textureBee);
    spriteBee.setPosition({0.f, 2000.f});
    // Is the bee currently moving?
    bool beeActive = false;
    // How fast can the bee fly
    float beeSpeed = 0.0f;

    // Clouds
    Texture textureCloud;
    if (!textureCloud.loadFromFile("../assets/graphics/cloud.png"))
        return -1;
    Sprite spriteCloud1(textureCloud);
    Sprite spriteCloud2(textureCloud);
    Sprite spriteCloud3(textureCloud);

    // Start clouds off-screen
    spriteCloud1.setPosition({2000.f, 0.f});
    spriteCloud2.setPosition({2000.f, 0.f});
    spriteCloud3.setPosition({2000.f, 0.f});

    // Are the clouds currently on screen
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    Clock clock;

    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    // SFML 3: setPosition takes Vector2f
    timeBar.setPosition({(1920.f / 2) - timeBarStartWidth / 2, 980.f});

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    // Track whether the game is running
    bool paused = true;
    int score = 0;

    // SFML 3: Font::openFromFile replaces loadFromFile; must load font BEFORE
    // constructing Text objects since Text now requires a Font in its constructor
    Font font;
    if (!font.openFromFile("../assets/fonts/KOMIKAP_.ttf"))
        return -1;
    // SFML 3: Text must be constructed with a Font; no default constructor
    Text messageText(font);
    Text scoreText(font);
    // Assign messages
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);
    // Position the text
    FloatRect textRect = messageText.getLocalBounds();
    // SFML 3: FloatRect members renamed: left->position.x, top->position.y,
    //         width->size.x, height->size.y; setOrigin takes a single Vector2f
    messageText.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                           textRect.position.y + textRect.size.y / 2.0f});
    messageText.setPosition({1920.f / 2.0f, 1080.f / 2.0f});
    scoreText.setPosition({20.f, 20.f});

    // Branches - textureBranch is global so updateBranches() sprite vector persists
    if (!textureBranch.loadFromFile("../assets/graphics/branch.png"))
        return -1;
    branches.reserve(NUM_BRANCHES);
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches.emplace_back(textureBranch);
        branches[i].setPosition({-2000.f, -2000.f});
        branches[i].setOrigin({220.f, 20.f});
    }

    // Player
    Texture texturePlayer;
    if (!texturePlayer.loadFromFile("../assets/graphics/player.png"))
        return -1;
    Sprite spritePlayer(texturePlayer);
    spritePlayer.setPosition({580.f, 720.f}); // left side ratio

    // The player starts on the left
    side playerSide = side::LEFT;

    // Gravestone
    Texture textureRIP;
    if (!textureRIP.loadFromFile("../assets/graphics/rip.png"))
        return -1;
    Sprite spriteRIP(textureRIP);
    spriteRIP.setPosition({675.f, 2000.f});

    // Axe
    Texture textureAxe;
    if (!textureAxe.loadFromFile("../assets/graphics/axe.png"))
        return -1;
    Sprite spriteAxe(textureAxe);
    // Initial position of the Axe before the game starts (off-screen)
    spriteAxe.setPosition({2000.f, 830.f});

    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Flying log
    Texture textureLog;
    if (!textureLog.loadFromFile("../assets/graphics/log.png"))
        return -1;
    Sprite spriteLog(textureLog);
    spriteLog.setPosition({810.f, 720.f});

    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // Control the player input
    bool acceptInput = false;

    // Sounds
    // SFML 3: Sound must be constructed with a SoundBuffer; no default constructor
    SoundBuffer chopBuffer;
    if (!chopBuffer.loadFromFile("../assets/sound/chop.wav"))
        return -1;
    Sound chop(chopBuffer);

    SoundBuffer deathBuffer;
    if (!deathBuffer.loadFromFile("../assets/sound/death.wav"))
        return -1;
    Sound death(deathBuffer);

    SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("../assets/sound/out_of_time.wav"))
        return -1;
    Sound outOfTime(soundBuffer);

    while (window.isOpen())
    {
        // SFML 3: Event handling uses std::optional; pollEvent returns optional<Event>
        while (const std::optional<Event> event = window.pollEvent())
        {
            if (const auto *keyReleased = event->getIf<Event::KeyReleased>())
            {
                if (!paused)
                {
                    // Listen for key presses again
                    acceptInput = true;
                    // Hide the axe
                    spriteAxe.setPosition({2000.f, spriteAxe.getPosition().y});
                }
            }
        }

        // SFML 3: Key constants moved under Keyboard::Key::
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            window.close();
        }
        // Start the game
        // SFML 3: Keyboard::Return renamed to Keyboard::Key::Enter
        if (Keyboard::isKeyPressed(Keyboard::Key::Enter))
        {
            paused = false;
            // Reset time and score
            score = 0;
            timeRemaining = 6;
            // Make all branches disappear starting from second position
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }
            // Make sure the gravestone is hidden
            spriteRIP.setPosition({675.f, 2000.f});
            // Set the player into position
            spritePlayer.setPosition({580.f, 720.f});
            acceptInput = true;
        }
        // Make sure we are accepting input
        if (acceptInput)
        {
            // First handle pressing the right cursor key
            if (Keyboard::isKeyPressed(Keyboard::Key::Right))
            {
                playerSide = side::RIGHT;
                score++;
                timeRemaining += (2 / score) + .15;
                spriteAxe.setPosition({AXE_POSITION_RIGHT, spriteAxe.getPosition().y});
                spritePlayer.setPosition({1200.f, 720.f});
                updateBranches(score);
                spriteLog.setPosition({810.f, 720.f});
                logSpeedX = -5000;
                logActive = true;
                acceptInput = false;
                chop.play();
            }
            // Handle the left cursor key
            if (Keyboard::isKeyPressed(Keyboard::Key::Left))
            {
                playerSide = side::LEFT;
                score++;
                timeRemaining += (2 / score) + .15;
                spriteAxe.setPosition({AXE_POSITION_LEFT, spriteAxe.getPosition().y});
                spritePlayer.setPosition({580.f, 720.f});
                updateBranches(score);
                spriteLog.setPosition({810.f, 720.f});
                logSpeedX = 5000;
                logActive = true;
                acceptInput = false;
                chop.play();
            }
        }

        if (!paused)
        {
            // Measure time
            Time dt = clock.restart();

            // Subtract from the amount of time remaining
            timeRemaining -= dt.asSeconds();
            // Size up the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
            if (timeRemaining <= 0.0f)
            {
                paused = true;
                messageText.setString("Out of time!");
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                                       textRect.position.y + textRect.size.y / 2.0f});
                messageText.setPosition({1920.f / 2.0f, 1080.f / 2.0f});
                outOfTime.play();
            }

            // Setup the bee
            if (!beeActive)
            {
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition({2000.f, height});
                beeActive = true;
            }
            else
            {
                // Move the bee
                spriteBee.setPosition({spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                                       spriteBee.getPosition().y});
                // Has the bee reached the left-hand edge of the screen?
                if (spriteBee.getPosition().x < -100)
                {
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
                spriteCloud1.setPosition({-200.f, height});
                cloud1Active = true;
            }
            else
            {
                spriteCloud1.setPosition({spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
                                          spriteCloud1.getPosition().y});
                if (spriteCloud1.getPosition().x > 1920)
                {
                    cloud1Active = false;
                }
            }

            // Cloud 2
            if (!cloud2Active)
            {
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition({-200.f, height});
                cloud2Active = true;
            }
            else
            {
                spriteCloud2.setPosition({spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
                                          spriteCloud2.getPosition().y});
                if (spriteCloud2.getPosition().x > 1920)
                {
                    cloud2Active = false;
                }
            }

            // Cloud 3
            if (!cloud3Active)
            {
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition({-200.f, height});
                cloud3Active = true;
            }
            else
            {
                spriteCloud3.setPosition({spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
                                          spriteCloud3.getPosition().y});
                if (spriteCloud3.getPosition().x > 1920)
                {
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
                    branches[i].setPosition({610.f, height});
                    // SFML 3: setRotation takes sf::Angle; use degrees() helper
                    branches[i].setRotation(degrees(180.f));
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition({1330.f, height});
                    branches[i].setRotation(degrees(0.f));
                }
                else
                {
                    // Hide the branch
                    branches[i].setPosition({3000.f, height});
                }
            }

            // Handle a flying log
            if (logActive)
            {
                spriteLog.setPosition({spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                                       spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())});

                if (spriteLog.getPosition().x < -100 ||
                    spriteLog.getPosition().x > 2000)
                {
                    logActive = false;
                    spriteLog.setPosition({810.f, 720.f});
                }
            }

            // Has the player been squished by a branch?
            if (branchPositions[5] == playerSide)
            {
                paused = true;
                acceptInput = false;

                // Gravestone appears where the player was hit
                spriteRIP.setPosition({spritePlayer.getPosition().x,
                                       spritePlayer.getPosition().y + 30.f});
                // Hide the player
                spritePlayer.setPosition({2000.f, 660.f});
                // Hide the axe
                spriteAxe.setPosition({2000.f, 820.f});
                messageText.setString("Game Over!");

                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                                       textRect.position.y + textRect.size.y / 2.0f});
                messageText.setPosition({1920.f / 2.0f, 1080.f / 2.0f});
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
