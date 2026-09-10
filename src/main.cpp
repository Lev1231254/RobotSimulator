#include <SFML/Graphics.hpp>
#include <cmath>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1000, 700),
        "2D Robot Simulator"
    );

    window.setFramerateLimit(60);

    sf::CircleShape robotBody(40.0f);
    robotBody.setOrigin(40.0f, 40.0f);
    robotBody.setPosition(500.0f, 350.0f);
    robotBody.setFillColor(sf::Color(80, 160, 220));
    robotBody.setOutlineThickness(3.0f);
    robotBody.setOutlineColor(sf::Color::White);

    sf::RectangleShape frontSensor(sf::Vector2f(20.0f, 20.0f));
    frontSensor.setOrigin(10.0f, 10.0f);
    frontSensor.setFillColor(sf::Color::Red);

    float robotSpeed = 200.0f;
    float robotRotationSpeed = 120.0f;

    while (window.isOpen())
    {
        sf::Event event{};

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }

        float deltaTime = 1.0f / 60.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            float angle = robotBody.getRotation() * 3.14159265f / 180.0f;
            robotBody.move(
                std::cos(angle) * robotSpeed * deltaTime,
                std::sin(angle) * robotSpeed * deltaTime
            );
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            float angle = robotBody.getRotation() * 3.14159265f / 180.0f;
            robotBody.move(
                -std::cos(angle) * robotSpeed * deltaTime,
                -std::sin(angle) * robotSpeed * deltaTime
            );
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            robotBody.rotate(-robotRotationSpeed * deltaTime);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            robotBody.rotate(robotRotationSpeed * deltaTime);
        }

        float angle = robotBody.getRotation() * 3.14159265f / 180.0f;

        sf::Vector2f robotPosition = robotBody.getPosition();

        frontSensor.setPosition(
            robotPosition.x + std::cos(angle) * 50.0f,
            robotPosition.y + std::sin(angle) * 50.0f
        );

        frontSensor.setRotation(robotBody.getRotation());

        window.clear(sf::Color(30, 30, 30));

        window.draw(robotBody);
        window.draw(frontSensor);

        window.display();
    }

    return 0;
}