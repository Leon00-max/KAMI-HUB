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
 * @brief Glossy button with icon, label, glow and hover/press animation
 */
class AnimatedButton : public UIComponent {
public:
    enum class ButtonStyle {
        Green,
        Orange,
        Red,
        Purple,
        Blue,
        Round   // Circular outlined icon button (navigation bar)
    };

    AnimatedButton(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const std::string& label,
        const sf::Font& font,
        ButtonStyle style = ButtonStyle::Blue,
        DrawingUtilities::Icon icon = DrawingUtilities::Icon::None,
        std::function<void()> onClick = nullptr
    );

    // Configuration
    void setLabel(const std::string& label);
    void setStyle(ButtonStyle style);
    void setIcon(DrawingUtilities::Icon icon) { m_icon = icon; }
    void setEnabled(bool enabled);
    void setCallback(std::function<void()> onClick) { m_onClick = std::move(onClick); }

    // Returns true when a click completes on this button
    bool handleEvent(const sf::Event& event);

    void update(float deltaTime, const sf::Vector2f& mousePosition);
    void draw(sf::RenderTarget& target) override;

    // Width that fits the icon and label with standard padding
    float preferredWidth() const;

    bool isHovered() const { return m_hovered; }
    bool isPressed() const { return m_pressed; }
    bool isEnabled() const { return m_enabled; }

private:
    struct Palette {
        sf::Color top;
        sf::Color bottom;
        sf::Color border;
    };

    Palette currentPalette() const;

    sf::Text m_text;
    ButtonStyle m_style = ButtonStyle::Blue;
    DrawingUtilities::Icon m_icon = DrawingUtilities::Icon::None;
    Palette m_palette;

    bool m_hovered = false;
    bool m_pressed = false;
    bool m_enabled = true;
    float m_hoverAmount = 0.f; // 0..1 eased hover progress
    float m_pressAmount = 0.f; // 0..1 eased press progress

    std::function<void()> m_onClick;
};
