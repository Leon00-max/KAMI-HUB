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

    const int columns = Theme::Sizes::GameBoardColumns;
    const int rows = Theme::Sizes::GameBoardRows;
    const float cellWidth = m_size.x / static_cast<float>(columns);
    const float cellHeight = m_size.y / static_cast<float>(rows);

    // Draw vertical grid lines
    for (int col = 1; col < columns; ++col) {
        const float x = m_position.x + col * cellWidth;
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, m_position.y), gridColor),
            sf::Vertex(sf::Vector2f(x, m_position.y + m_size.y), gridColor)
        };
        target.draw(line, 2, sf::Lines);
    }

    // Draw horizontal grid lines
    for (int row = 1; row < rows; ++row) {
        const float y = m_position.y + row * cellHeight;
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(m_position.x, y), gridColor),
            sf::Vertex(sf::Vector2f(m_position.x + m_size.x, y), gridColor)
        };
        target.draw(line, 2, sf::Lines);
    }
}
