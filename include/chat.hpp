#ifndef CHAT_HPP
#define CHAT_HPP

#include <SFML/Graphics.hpp>
#include <deque>
#include <string>

class ChatLog {
    sf::Font font;
    sf::RectangleShape background;
    sf::Text text;
    std::deque<std::string> messages;
    sf::Vector2f topLeft;

public:
    explicit ChatLog(const sf::Vector2f& topLeft, const sf::Vector2f& size);
    void addMessage(const std::string& message);
    void drawChat(sf::RenderWindow& window);
};

#endif
