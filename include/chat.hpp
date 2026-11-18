#ifndef CHAT_HPP
#define CHAT_HPP

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <deque>
#include <string>

class ChatLog {
    sf::Font font;
    sf::RectangleShape background;
    sf::Text text;
    sf::Text toggleHint;
    std::deque<std::string> messages;
    sf::Vector2f topLeft;
    std::size_t scrollOffset = 0;
    int counter = 0;

    std::size_t visibleMessageCount() const;
    void updateDisplayedMessages();

public:
    explicit ChatLog(const sf::Vector2f& topLeft, const sf::Vector2f& size);
    void addMessage(const std::string& message);
    bool isMouseOver(const sf::Vector2i& mousePosition) const;
    void scroll(float delta);
    void drawChat(sf::RenderWindow& window);
};

#endif
