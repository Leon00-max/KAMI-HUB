#include "ui/GlassPanel.hpp"

#include "ui/DrawingUtilities.hpp"

GlassPanel::GlassPanel(const sf::Vector2f& position, const sf::Vector2f& size) {
    setPosition(position);
    setSize(size);
}

void GlassPanel::draw(sf::RenderTarget& target) {
    DrawingUtilities::drawFrostedPanel(target, m_position, m_size);
}
