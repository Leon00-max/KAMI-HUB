#include "ui/GameCard.hpp"

#include <algorithm>

GameCard::GameCard(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const std::string& gameName,
    DrawingUtilities::Icon icon,
    const sf::Color& iconColor,
    const sf::Font& font
)
    : m_name(gameName, font, Theme::FontSizes::CardLabel), m_icon(icon), m_iconColor(iconColor) {
    setPosition(position);
    setSize(size);
}

bool GameCard::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f point(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        if (contains(point)) {
            m_selected = true;
            return true;
        }
    }
    return false;
}

void GameCard::update(float deltaTime, const sf::Vector2f& mousePosition) {
    m_hovered = contains(mousePosition);
    const float step = deltaTime / Theme::Effects::HoverAnimationDuration;
    m_hoverAmount = std::clamp(m_hoverAmount + (m_hovered ? step : -step), 0.f, 1.f);
}

void GameCard::draw(sf::RenderTarget& target) {
    const float radius = Theme::Effects::CornerRadius;
    const sf::Color green = Theme::Colors::GreenBright();

    if (m_selected) {
        DrawingUtilities::drawGlow(target, m_position, m_size, radius, Theme::withAlpha(green, 200), 8.f);
        DrawingUtilities::drawGradientRoundedRectangle(
            target, m_position, m_size, radius, sf::Color(0x22, 0x46, 0x1E, 0xE6), sf::Color(0x10, 0x26, 0x14, 0xE6)
        );
        DrawingUtilities::drawRoundedRectangle(target, m_position, m_size, radius, sf::Color::Transparent, green, 2.f);
    } else {
        const sf::Color border = Theme::mix(Theme::Colors::PanelBorder(), Theme::withAlpha(Theme::Colors::BlueBright(), 200), m_hoverAmount);
        DrawingUtilities::drawGradientRoundedRectangle(
            target, m_position, m_size, radius, sf::Color(0x16, 0x26, 0x44, 0xE6), sf::Color(0x0B, 0x15, 0x2C, 0xE6)
        );
        DrawingUtilities::drawRoundedRectangle(target, m_position, m_size, radius, sf::Color::Transparent, border, 1.f);
    }

    const float iconSize = 38.f;
    const sf::Vector2f iconCenter(m_position.x + 12.f + iconSize / 2.f, m_position.y + m_size.y / 2.f);
    DrawingUtilities::drawIcon(target, m_icon, iconCenter, iconSize, m_iconColor);

    const sf::FloatRect bounds = m_name.getLocalBounds();
    m_name.setOrigin(bounds.left, bounds.top + bounds.height / 2.f);
    m_name.setPosition(iconCenter.x + iconSize / 2.f + 10.f, iconCenter.y);
    m_name.setFillColor(m_selected ? sf::Color(0xC5, 0xF5, 0x9A) : Theme::Colors::TextWhite());
    target.draw(m_name);
}
