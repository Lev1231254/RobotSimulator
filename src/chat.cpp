#include <chat.hpp>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {
const char* fontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
constexpr float margin = 10.f;
constexpr float messageAreaVerticalPadding = 16.f;
}

ChatLog::ChatLog(const sf::Vector2f& topLeft, const sf::Vector2f& size)
    : text(font, "", 16),
      toggleHint(font, "M - hide/show hud", 16) {
    if (!font.openFromFile(fontPath)) {
        throw std::runtime_error(std::string("Failed to load chat font: ") + fontPath);
    }
    background.setSize(size);
    background.setPosition({topLeft.x, topLeft.y + margin});
    background.setFillColor(sf::Color(0, 0, 0, 180));
    background.setOutlineColor(sf::Color(180, 180, 180, 220));
    background.setOutlineThickness(1.f);
    text.setFillColor(sf::Color::White);
    text.setPosition({topLeft.x + margin, topLeft.y + margin + 8.f});
    toggleHint.setFillColor(sf::Color::White);
    toggleHint.setPosition({
        topLeft.x + margin,
        topLeft.y + margin + size.y + 4.f
    });
}


void ChatLog::addMessage(const std::string& message) {
    counter++;
    messages.push_back(message + "  (" + std::to_string(counter) + ")");

    if (scrollOffset > 0) {
        ++scrollOffset;
    }
    const std::size_t maxScrollOffset =
        messages.size() > visibleMessageCount()
            ? messages.size() - visibleMessageCount()
            : 0;
    scrollOffset = std::min(scrollOffset, maxScrollOffset);
    updateDisplayedMessages();
}

std::size_t ChatLog::visibleMessageCount() const {
    const float availableHeight =
        std::max(0.f, background.getSize().y - messageAreaVerticalPadding);
    const float lineSpacing = std::max(
        font.getLineSpacing(text.getCharacterSize()),
        1.f);
    return std::max<std::size_t>(
        1,
        static_cast<std::size_t>(availableHeight / lineSpacing));
}

void ChatLog::updateDisplayedMessages() {
    const std::size_t endIndex = messages.size() - scrollOffset;
    const std::size_t firstIndex =
        endIndex > visibleMessageCount()
            ? endIndex - visibleMessageCount()
            : 0;
    std::string displayedMessages;
    for (std::size_t index = firstIndex; index < endIndex; ++index) {
        displayedMessages += messages[index] + '\n';
    }
    text.setString(displayedMessages);
}

bool ChatLog::isMouseOver(const sf::Vector2i& mousePosition) const {
    const sf::Vector2f panelPosition = background.getPosition();
    const sf::Vector2f panelSize = background.getSize();
    return mousePosition.x >= panelPosition.x &&
        mousePosition.x <= panelPosition.x + panelSize.x &&
        mousePosition.y >= panelPosition.y &&
        mousePosition.y <= panelPosition.y + panelSize.y;
}

void ChatLog::scroll(float delta) {
    if (delta == 0.f) return;
    const std::size_t visibleCount = visibleMessageCount();
    const std::size_t maxScrollOffset =
        messages.size() > visibleCount
            ? messages.size() - visibleCount
            : 0;
    const std::size_t scrollSteps = static_cast<std::size_t>(
        std::ceil(std::abs(delta)));

    if (delta > 0.f) {
        scrollOffset += std::min(scrollSteps, maxScrollOffset - scrollOffset);
    } else {
        scrollOffset = scrollSteps >= scrollOffset
            ? 0
            : scrollOffset - scrollSteps;
    }
    updateDisplayedMessages();
}

void ChatLog::drawChat(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(text);
    window.draw(toggleHint);
}