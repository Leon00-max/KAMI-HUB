#include "ui/GlassPanel.hpp"

#include "ui/DrawingUtilities.hpp"

GlassPanel::GlassPanel() {
    setPosition(sf::Vector2f(300.f, 110.f));
    setSize(sf::Vector2f(Theme::Sizes::GameBoardWidth, Theme::Sizes::GameBoardHeight));
}

GlassPanel::GlassPanel(
    const sf::Vector2f& position,
    const sf::Vector2f& size
) {
    setPosition(position);
    setSize(size);
}

void GlassPanel::setGlassColor(const sf::Color& color) {
    m_glassColor = color;
}

void GlassPanel::setGlowColor(const sf::Color& color) {
    m_glowColor = color;
}

void GlassPanel::draw(sf::RenderTarget& target) {
    DrawingUtilities::drawGlassPanel(
        target,
        m_position,
        m_size,
        m_glassColor,
        m_glowColor
    );

    // Draw a grid placeholder to indicate game board structure
    // This will be replaced with actual game content later
    sf::Color gridColor = Theme::Colors::GlassBorder();
    gridColor.a = 30; // Very subtle

    const float cellSize = 40.f;
    const float startX = m_position.x;
    const float startY = m_position.y;

    // Draw vertical grid lines
    for (float x = startX; x <= startX + m_size.x; x += cellSize) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, startY), gridColor),
            sf::Vertex(sf::Vector2f(x, startY + m_size.y), gridColor)
        };
        target.draw(line, 2, sf::Lines);
    }

    // Draw horizontal grid lines
    for (float y = startY; y <= startY + m_size.y; y += cellSize) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(startX, y), gridColor),
            sf::Vertex(sf::Vector2f(startX + m_size.x, y), gridColor)
        };
        target.draw(line, 2, sf::Lines);
    }
}
