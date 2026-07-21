#pragma once

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "UIComponent.hpp"
#include "AnimatedButton.hpp"
#include "Theme.hpp"

/**
 * @class ActionButtonBar
 * @brief Bottom action button bar with Play, Pause, Restart, Menu, Settings
 * Supports different colored buttons for visual hierarchy
 */
class ActionButtonBar : public UIComponent {
public:
    enum class ButtonType {
        Play,
        Pause,
        Restart,
        Menu,
        Settings
    };

    ActionButtonBar(const sf::Font& font);

    // Set window width for centered layout
    void setWindowWidth(float width);

    // Input handling
    bool handleEvent(const sf::Event& event);

    // Update (for button animations)
    void update(float deltaTime, const sf::Vector2f& mousePosition);

    // Drawing
    void draw(sf::RenderTarget& target) override;

    // Query button clicks
    int getClickedButtonIndex() const { return m_clickedButtonIndex; }

private:
    const sf::Font& m_font;
    std::vector<AnimatedButton> m_buttons;
    std::vector<ButtonType> m_buttonTypes;
    int m_clickedButtonIndex = -1;
    float m_windowWidth = 1280.f;

    void initializeButtons();
};
