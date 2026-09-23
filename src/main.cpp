#include <SFML/Graphics.hpp>
#include <numbers>
#include <optional>
#include <robot.hpp>
#include <draw.hpp>

float const pi = 3.141592653f;
float const movement_speed = 2.0f;
float const turning_speed = pi / 90;

int main()
{

    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "Robot simulator",
        sf::Style::Close
    );
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);


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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)){
            robot.moveRobot(1, movement_speed);
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)){
            robot.moveRobot(0, movement_speed);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)){
            robot.setAngleRad(robot.getAngleRad() + turning_speed);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)){
            robot.setAngleRad(robot.getAngleRad() - turning_speed);
        }

        window.clear(sf::Color::Black);

        draw_robot(window, robot);

        window.display();
    }

    return 0;
}