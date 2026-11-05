#include <chat.hpp>

#include <stdexcept>

namespace {
const std::size_t maxMessages = 7;
const char* fontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
constexpr float margin = 10.f;
}

ChatLog::ChatLog(const sf::Vector2f& topLeft, const sf::Vector2f& size)
    : text(font, "", 16) {
    if (!font.openFromFile(fontPath)) {
        throw std::runtime_error(std::string("Failed to load chat font: ") + fontPath);
    }
    counter = 0;

    background.setSize(size);
    background.setPosition({topLeft.x, topLeft.y + margin});
    background.setFillColor(sf::Color(0, 0, 0, 180));
    background.setOutlineColor(sf::Color(180, 180, 180, 220));
    background.setOutlineThickness(1.f);
    text.setFillColor(sf::Color::White);
    text.setPosition({topLeft.x + margin, topLeft.y + margin + 8.f});
}


void ChatLog::addMessage(const std::string& message) {
    counter++;
    messages.push_back(message + "  (" + std::to_string(counter) + ")");
    if (messages.size() > maxMessages) {
        messages.pop_front();
    }

    std::string displayedMessages;
    for (const std::string& entry : messages) {
        displayedMessages += entry + '\n';
    }
    text.setString(displayedMessages);
}

void ChatLog::drawChat(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(text);
}