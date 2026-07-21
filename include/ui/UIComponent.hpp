#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

/**
 * @class UIComponent
 * @brief Base class for all UI components.
 * Provides common functionality for positioning, sizing, and drawing.
 */
class UIComponent {
public:
    virtual ~UIComponent() = default;

    // Positioning and sizing
    void setPosition(const sf::Vector2f& position) { m_position = position; }
    void setSize(const sf::Vector2f& size) { m_size = size; }
    
    const sf::Vector2f& getPosition() const { return m_position; }
    const sf::Vector2f& getSize() const { return m_size; }

    // Drawing
    virtual void draw(sf::RenderTarget& target) = 0;

    // Point-in-bounds testing
    bool contains(const sf::Vector2f& point) const {
        return point.x >= m_position.x && point.x <= m_position.x + m_size.x &&
               point.y >= m_position.y && point.y <= m_position.y + m_size.y;
    }

protected:
    sf::Vector2f m_position{0.f, 0.f};
    sf::Vector2f m_size{0.f, 0.f};
};
