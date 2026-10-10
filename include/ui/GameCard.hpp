#pragma once

#include <string>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "DrawingUtilities.hpp"
#include "Theme.hpp"
#include "UIComponent.hpp"

/**
 * @class GameCard
 * @brief Selectable game tile: icon on the left, name on the right
 */
class GameCard : public UIComponent {
public:
    GameCard(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const std::string& gameName,
        DrawingUtilities::Icon icon,
        const sf::Color& iconColor,
        const sf::Font& font
    );

    void setSelected(bool selected) { m_selected = selected; }

    // Returns true when the card was clicked
    bool handleEvent(const sf::Event& event);
    void update(float deltaTime, const sf::Vector2f& mousePosition);
    void draw(sf::RenderTarget& target) override;

    bool isSelected() const { return m_selected; }
    bool isHovered() const { return m_hovered; }

private:
    sf::Text m_name;
    DrawingUtilities::Icon m_icon;
    sf::Color m_iconColor;

    bool m_selected = false;
    bool m_hovered = false;
    float m_hoverAmount = 0.f;
};
