#include <SFML/Graphics.hpp>
#include <numbers>
#include <optional>
#include <robot.hpp>
#include <draw.hpp>
#include <utils.hpp>

float const pi = 3.141592653f;
float const movement_speed = 2.f;
float const turning_speed = pi / 90.f;
float const dTime = 1.f / 90.f;


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

    sf::RectangleShape obstacle({100, 100});
    obstacle.setPosition({300, 300});
    obstacle.setFillColor(sf::Color::Blue);


    sf::Clock clock;
    float accumulator = 0.f;

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){

            if (event->is<sf::Event::Closed>()){
                window.close();
            }
            


        }
        
        // simulation
        float frameTime = clock.restart().asSeconds();
        accumulator += frameTime;
        
        while (accumulator >= dTime) {

            // while (circleIntersectsRect(robot.getBodyCircle(), obstacle)){
            //     robot.moveRobot(0, movement_speed / 2);
            // }
            scanAndMove(robot, obstacle, movement_speed, turning_speed);
            accumulator -= dTime;

        }

        // drawing
        window.clear(sf::Color::Black);

        draw_robot(window, robot);
        window.draw(obstacle);

        window.display();
    }

    return 0;
}