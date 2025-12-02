#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace sf;

int main()
{
    // Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Time Bar with Countdown");

    // Total countdown time in seconds
    const float totalTime = 6.0f;

    // Time bar background
    sf::RectangleShape timeBarBack(sf::Vector2f(340, 30));
    timeBarBack.setFillColor(sf::Color(50, 50, 50)); // dark gray
    timeBarBack.setOrigin(timeBarBack.getLocalBounds().width / 2, timeBarBack.getLocalBounds().height / 2);
    timeBarBack.setPosition(400, 300);

    // Time bar foreground
    sf::RectangleShape timeBar(sf::Vector2f(340, 30));
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setOrigin(timeBar.getLocalBounds().width / 2, timeBar.getLocalBounds().height / 2);
    timeBar.setPosition(400, 300);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("fonts/KOMIKAP_.ttf")) // Make sure arial.ttf is available
    {
        std::cerr << "Could not load font.\n";
        return -1;
    }

    // Countdown text
    sf::Text countdownText;
    countdownText.setFont(font);
    countdownText.setCharacterSize(24);
    countdownText.setFillColor(Color::White);
    countdownText.setStyle(Text::Bold);
    countdownText.setOrigin(countdownText.getLocalBounds().width / 2, countdownText.getLocalBounds().height / 2);
    countdownText.setPosition(400, 300); // Adjust to center on the bar

    // Clock to track time
    sf::Clock clock;

    while (window.isOpen())
    {
        // Handle window events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        // Get elapsed time
        float elapsedTime = clock.getElapsedTime().asSeconds();

        // Calculate width of time bar
        float width = 340 * (1.0f - elapsedTime / totalTime);
        if (width < 0)
            width = 0;

        timeBar.setSize(sf::Vector2f(width, 30));

        // Calculate remaining time for countdown
        int timeLeft = static_cast<int>(totalTime - elapsedTime);
        if (timeLeft < 1)
            timeLeft = 0;

        // Convert to string
        countdownText.setString(std::to_string(timeLeft));
        // Optionally center text based on character width
        sf::FloatRect textBounds = countdownText.getLocalBounds();
        countdownText.setOrigin(textBounds.width / 2, textBounds.height / 2);
        countdownText.setPosition(400, 300); // Centered on time bar

        // Draw
        window.clear(sf::Color::Black);
        window.draw(timeBarBack);
        window.draw(timeBar);
        window.draw(countdownText);
        window.display();
    }

    return 0;
}
