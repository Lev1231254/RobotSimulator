#include <SFML/Graphics.hpp>
#include <numbers>
#include <optional>
#include <robot.hpp>
#include <draw.hpp>

float const pi = 3.141592653f;

int main()
{

    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "Robot simulator",
        sf::Style::Close
    );
    window.setFramerateLimit(60);


    Robot robot;
    robot.setPosition({500.0f, 500.0f});
    robot.setRadius(50.0f);
    robot.setAngleRad(pi / 6);


    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){

            if (event->is<sf::Event::Closed>()){
                window.close();
            }



        }

        window.clear(sf::Color::Black);

        draw_robot(window, robot);

        window.display();
    }

    return 0;
}