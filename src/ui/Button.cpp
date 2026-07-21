#include "ui/Button.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Mouse.hpp>

Button::Button(const sf::Vector2f& position,
               const sf::Vector2f& size,
               const std::string& label,
               const sf::Font& font,
               std::function<void()> onClick)
    : m_onClick(std::move(onClick)) {
    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(m_idleColor);
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color::White);

    m_label.setFont(font);
    m_label.setString(label);
    m_label.setCharacterSize(24);
    m_label.setFillColor(sf::Color::White);
    m_label.setPosition(position + sf::Vector2f(12.f, size.y * 0.5f - 12.f));
}

void Button::setPosition(const sf::Vector2f& position) {
    m_shape.setPosition(position);
    m_label.setPosition(position + sf::Vector2f(12.f, m_shape.getSize().y * 0.5f - 12.f));
}

void Button::setSize(const sf::Vector2f& size) {
    m_shape.setSize(size);
    m_label.setPosition(m_shape.getPosition() + sf::Vector2f(12.f, size.y * 0.5f - 12.f));
}

void Button::setColors(const sf::Color& idle, const sf::Color& hover, const sf::Color& pressed) {
    m_idleColor = idle;
    m_hoverColor = hover;
    m_pressedColor = pressed;
    m_shape.setFillColor(m_idleColor);
}

bool Button::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f mouse = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        if (m_shape.getGlobalBounds().contains(mouse)) {
            m_pressed = true;
            m_shape.setFillColor(m_pressedColor);
            return true;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f mouse = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        if (m_pressed && m_shape.getGlobalBounds().contains(mouse) && m_onClick) {
            m_onClick();
        }
        m_pressed = false;
        m_shape.setFillColor(m_hovered ? m_hoverColor : m_idleColor);
        return true;
    }

    return false;
}

void Button::update(const sf::Vector2f& mousePosition) {
    const bool hovered = m_shape.getGlobalBounds().contains(mousePosition);
    if (hovered != m_hovered) {
        m_hovered = hovered;
        m_shape.setFillColor(hovered ? m_hoverColor : m_idleColor);
    }
}

void Button::draw(sf::RenderTarget& target) const {
    target.draw(m_shape);
    target.draw(m_label);
}
