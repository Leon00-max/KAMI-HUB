#pragma once

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "managers/ResourceManager.hpp"
#include "AnimatedButton.hpp"
#include "DrawingUtilities.hpp"
#include "Theme.hpp"
#include "UIComponent.hpp"

/**
 * @class NavigationBar
 * @brief Top bar: logo | Home, Games, Stats, Achievements | Settings, Music, Sound
 */
class NavigationBar : public UIComponent {
public:
    explicit NavigationBar(const ResourceManager& resources);

    bool handleEvent(const sf::Event& event);
    void update(float deltaTime, const sf::Vector2f& mousePosition);
    void draw(sf::RenderTarget& target) override;

    // Index of the nav item clicked this frame, or -1
    int getClickedNavItem() const { return m_clickedNavItem; }
    void setActiveNavItem(int index) { m_activeNavItem = index; }

private:
    struct NavItem {
        DrawingUtilities::Icon icon;
        sf::Text label;
        sf::FloatRect hitBox;
        bool hovered = false;
    };

    sf::Text m_title;
    sf::Text m_subtitle;
    std::vector<NavItem> m_navItems;
    std::vector<AnimatedButton> m_controlButtons; // Settings, Music, Sound

    int m_activeNavItem = 0;
    int m_clickedNavItem = -1;

    void drawLogo(sf::RenderTarget& target);
};
