#pragma once

#include <functional>
#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

class Button {
public:
    Button() = default;
    Button(const sf::Vector2f& position,
           const sf::Vector2f& size,
           const std::string& label,
           const sf::Font& font,
           std::function<void()> onClick);

    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    void setColors(const sf::Color& idle, const sf::Color& hover, const sf::Color& pressed);
    bool handleEvent(const sf::Event& event);
    void update(const sf::Vector2f& mousePosition);
    void draw(sf::RenderTarget& target) const;

private:
    sf::RectangleShape m_shape;
    sf::Text m_label;
    sf::Color m_idleColor{sf::Color(53, 97, 162, 230)};
    sf::Color m_hoverColor{sf::Color(83, 137, 226, 240)};
    sf::Color m_pressedColor{sf::Color(18, 58, 112, 255)};
    std::function<void()> m_onClick;
    bool m_hovered = false;
    bool m_pressed = false;
};
