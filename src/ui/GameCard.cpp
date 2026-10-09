#include "ui/GameCard.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include "ui/DrawingUtilities.hpp"

GameCard::GameCard() {}

GameCard::GameCard(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const std::string& gameName,
    DrawingUtilities::Icon gameIcon,
    const sf::Font& font
) {
    setPosition(position);
    setSize(size);
    setFont(font);
    setGameName(gameName);
    setGameIcon(gameIcon);
}

void GameCard::setGameName(const std::string& name) {
    m_gameName.setString(name);
    m_gameName.setCharacterSize(Theme::Typography::FontSizes::BodyLarge);
    m_gameName.setFillColor(Theme::Colors::TextWhite());
}

void GameCard::setFont(const sf::Font& font) {
    m_gameName.setFont(font);
}

bool GameCard::handleEvent(const sf::Event& event) {
    m_clickedThisFrame = false;

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left &&
            contains(sf::Vector2f(static_cast<float>(event.mouseButton.x),
                                static_cast<float>(event.mouseButton.y)))) {
            m_selected = true;
            m_clickedThisFrame = true;
            return true;
        }
    }

    return false;
}

void GameCard::update(float deltaTime, const sf::Vector2f& mousePosition) {
    m_hovered = contains(mousePosition);

    // Smooth hover animation
    if (m_hovered) {
        m_hoverAnimation = std::min(1.f, m_hoverAnimation + deltaTime / Theme::Effects::HoverAnimationDuration);
    } else {
        m_hoverAnimation = std::max(0.f, m_hoverAnimation - deltaTime / Theme::Effects::HoverAnimationDuration);
    }
}

void GameCard::draw(sf::RenderTarget& target) {
    // Background color based on state
    sf::Color bgColor = Theme::Colors::LightBg();
    sf::Color borderColor = sf::Color::Transparent;
    float borderWidth = 0.f;

    if (m_selected) {
        bgColor = Theme::Colors::MediumBg();
        borderColor = Theme::Colors::PrimaryGreen();
        borderWidth = 2.f;
    } else if (m_hovered) {
        bgColor = Theme::lighten(Theme::Colors::LightBg());
        borderColor = Theme::Colors::AccentYellow();
        borderWidth = 1.5f;
    }

    // Draw card background
    DrawingUtilities::drawRoundedRectangle(
        target,
        m_position,
        m_size,
        Theme::Effects::CornerRadius,
        bgColor,
        borderColor,
        borderWidth
    );

    // Draw icon (centered)
    DrawingUtilities::drawIcon(
        target,
        m_gameIcon,
        sf::Vector2f(m_position.x + m_size.x / 2.f, m_position.y + m_size.y / 3.f + 4.f),
        48.f,
        Theme::Colors::PrimaryGreen()
    );

    // Draw name (below icon)
    auto nameBounds = m_gameName.getLocalBounds();
    m_gameName.setOrigin(nameBounds.width / 2.f, nameBounds.height / 2.f);
    m_gameName.setPosition(
        m_position.x + m_size.x / 2.f,
        m_position.y + m_size.y - 35.f
    );
    target.draw(m_gameName);
}
