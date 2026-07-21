#pragma once

#include <vector>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "UIComponent.hpp"
#include "Theme.hpp"
#include "AnimatedButton.hpp"

/**
 * @class NavigationBar
 * @brief Top navigation bar with logo, nav items, and control buttons
 * Shows: ARCADE HUB logo | Home | Games | Statistics | Achievements | (...spacer...) | Settings | Music | Sound
 */
class NavigationBar : public UIComponent {
public:
    NavigationBar(const sf::Font& font);

    // Set window width for layout calculations
    void setWindowWidth(float width);

    // Input handling
    bool handleEvent(const sf::Event& event);

    // Update (for button animations)
    void update(float deltaTime, const sf::Vector2f& mousePosition);

    // Drawing
    void draw(sf::RenderTarget& target) override;

    // Query which nav item was clicked (returns index or -1)
    int getClickedNavItem() const { return m_clickedNavItem; }

private:
    const sf::Font& m_font;
    std::vector<sf::Text> m_navItems;
    std::vector<AnimatedButton> m_controlButtons; // Settings, Music, Sound icons

    float m_windowWidth = 1280.f;
    int m_clickedNavItem = -1;

    void initializeNavItems();
    void initializeControlButtons();
};
