#include <controlsGuide.hpp>

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>

namespace {
const char* controlsFontPath = "../assets/fonts/Roboto/Roboto-VariableFont_wdth,wght.ttf";
}

ControlsGuide::ControlsGuide()
    : header(font, "CONTROLS GUIDE", 30),
      headerHint(font, "F1  -  show / hide controls guide", 20),
      leftText(
          font,
          "LSHIFT  -  switch modes\n"
          "ESC  -  exit program\n\n"

          "R  -  show / hide red zone\n"
          "P  -  show / hide path drawing\n"
          "M  -  show / hide map",
          24),
      rightText(
          font,
          "PATHFINDING MODE:\n"
          "  LMouse click  -  choose target\n\n"

          "MOVEMENT MODE:\n"
          "  WASD  -  move",
          24) {
    if (!font.openFromFile(controlsFontPath)) {
        throw std::runtime_error(std::string("Failed to load controls font: ") + controlsFontPath);
    }

    header.setFillColor(sf::Color::White);
    headerHint.setFillColor(sf::Color::White);
    leftText.setFillColor(sf::Color::White);
    rightText.setFillColor(sf::Color::White);
    leftText.setLineSpacing(1.35f);
    rightText.setLineSpacing(1.35f);

    background.setFillColor(sf::Color(20, 20, 20, 240));
    background.setOutlineColor(sf::Color(180, 180, 180));
    background.setOutlineThickness(2.f);
}

void ControlsGuide::draw(sf::RenderWindow& window) {
    const sf::Vector2f windowSize(window.getSize());
    sf::RectangleShape backdrop(windowSize);
    backdrop.setFillColor(sf::Color(0, 0, 0, 90));
    window.draw(backdrop);

    const sf::Vector2f panelSize{
        std::min(windowSize.x - 40.f, 800.f),
        std::min(windowSize.y - 40.f, 450.f)
    };
    background.setSize(panelSize);
    background.setPosition({
        (windowSize.x - panelSize.x) / 2.f,
        (windowSize.y - panelSize.y) / 2.f
    });

    const sf::Vector2f panelPosition = background.getPosition();
    const sf::Vector2f headerSize = header.getLocalBounds().size;
    const sf::Vector2f hintSize = headerHint.getLocalBounds().size;
    header.setPosition({
        std::round(panelPosition.x + (panelSize.x - headerSize.x) / 2.f),
        panelPosition.y + 32.f
    });
    headerHint.setPosition({
        std::round(panelPosition.x + (panelSize.x - hintSize.x) / 2.f),
        panelPosition.y + 80.f
    });

    leftText.setPosition({
        panelPosition.x + 40.f,
        panelPosition.y + 160.f
    });
    rightText.setPosition({
        panelPosition.x + panelSize.x / 2.f + 20.f,
        panelPosition.y + 160.f
    });

    window.draw(background);
    window.draw(header);
    window.draw(headerHint);
    window.draw(leftText);
    window.draw(rightText);
}
