#pragma once

#include <functional>
#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "DrawingUtilities.hpp"
#include "UIComponent.hpp"
#include "Theme.hpp"

/**
 * @class AnimatedButton
 * @brief Professional button with smooth hover/press animations and state feedback
 */
class AnimatedButton : public UIComponent {
public:
    enum class ButtonStyle {
        Primary,   // Bright accent (green, yellow, red)
        Secondary, // Muted blue
        Tertiary   // Dark background
    };

    AnimatedButton();

    AnimatedButton(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const std::string& label,
        const sf::Font& font,
        ButtonStyle style = ButtonStyle::Secondary,
        std::function<void()> onClick = nullptr
    );

    // Configuration
    void setLabel(const std::string& label);
    void setFont(const sf::Font& font);
    void setStyle(ButtonStyle style);
    void setColors(const sf::Color& idle, const sf::Color& hover, const sf::Color& pressed);
    void setEnabled(bool enabled);
    void setIcon(DrawingUtilities::Icon icon) { m_icon = icon; }
    void setCallback(std::function<void()> onClick) { m_onClick = onClick; }

    // Input handling
    bool handleEvent(const sf::Event& event);

    // Update with mouse position for hover detection
    void update(float deltaTime, const sf::Vector2f& mousePosition);

    // Drawing
    void draw(sf::RenderTarget& target) override;

    // State queries
    bool isHovered() const { return m_hovered; }
    bool isPressed() const { return m_pressed; }
    bool isEnabled() const { return m_enabled; }

private:
    void updateColors();

    // Text
    sf::Text m_text;
    DrawingUtilities::Icon m_icon = DrawingUtilities::Icon::None;

    // Colors
    sf::Color m_idleColor;
    sf::Color m_hoverColor;
    sf::Color m_pressedColor;

    // State
    bool m_hovered = false;
    bool m_pressed = false;
    bool m_enabled = true;
    float m_hoverScale = 1.f; // Current scale during hover animation
    float m_pressScale = 1.f; // Current scale during press animation
    float m_animationTime = 0.f;

    // Callbacks
    std::function<void()> m_onClick;
};
