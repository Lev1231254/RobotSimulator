#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>
#include <robot.hpp>
#include <draw.hpp>

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
    robot.setRadius(100.0f);


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