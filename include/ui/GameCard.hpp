#pragma once

#include <string>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "UIComponent.hpp"
#include "Theme.hpp"

/**
 * @class GameCard
 * @brief Selectable game card showing game name and icon
 * Provides visual feedback on hover and selection
 */
class GameCard : public UIComponent {
public:
    GameCard();

    GameCard(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const std::string& gameName,
        const std::string& gameIcon,
        const sf::Font& font
    );

    // Configuration
    void setSelected(bool selected) { m_selected = selected; }
    void setGameName(const std::string& name);
    void setGameIcon(const std::string& icon);
    void setFont(const sf::Font& font);

    // Input handling
    bool handleEvent(const sf::Event& event);

    // Update with mouse position
    void update(float deltaTime, const sf::Vector2f& mousePosition);

    // Drawing
    void draw(sf::RenderTarget& target) override;

    // State queries
    bool isSelected() const { return m_selected; }
    bool isHovered() const { return m_hovered; }
    bool wasClickedThisFrame() const { return m_clickedThisFrame; }

private:
    sf::Text m_gameName;
    sf::Text m_gameIcon;

    bool m_selected = false;
    bool m_hovered = false;
    bool m_clickedThisFrame = false;
    float m_hoverAnimation = 0.f;
};
