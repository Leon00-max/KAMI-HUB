#include "ui/AnimatedButton.hpp"

#include <cmath>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Mouse.hpp>

#include "ui/DrawingUtilities.hpp"

AnimatedButton::AnimatedButton()
    : m_idleColor(Theme::Colors::ButtonDefault()),
      m_hoverColor(Theme::Colors::ButtonHover()),
      m_pressedColor(Theme::Colors::ButtonPressed()) {}

AnimatedButton::AnimatedButton(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const std::string& label,
    const sf::Font& font,
    ButtonStyle style,
    std::function<void()> onClick
)
    : m_onClick(onClick) {
    setPosition(position);
    setSize(size);
    setFont(font);
    setLabel(label);
    setStyle(style);

    // Center text in button
    const sf::FloatRect bounds = m_text.getLocalBounds();
    m_text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    m_text.setPosition(
        position.x + size.x / 2.f,
        position.y + size.y / 2.f
    );
}

void AnimatedButton::setLabel(const std::string& label) {
    m_text.setString(sf::String::fromUtf8(label.begin(), label.end()));
    m_text.setCharacterSize(Theme::Typography::FontSizes::Button);
}

void AnimatedButton::setFont(const sf::Font& font) {
    m_text.setFont(font);
}

void AnimatedButton::setStyle(ButtonStyle style) {
    switch (style) {
        case ButtonStyle::Primary:
            // Bright primary accent - will cycle through accent colors
            setColors(Theme::Colors::AccentYellow(), Theme::lighten(Theme::Colors::AccentYellow()),
                     Theme::darken(Theme::Colors::AccentYellow()));
            break;

        case ButtonStyle::Secondary:
            // Muted blue (default)
            setColors(Theme::Colors::ButtonDefault(), Theme::Colors::ButtonHover(),
                     Theme::Colors::ButtonPressed());
            break;

        case ButtonStyle::Tertiary:
            // Dark background
            setColors(Theme::Colors::LightBg(), Theme::Colors::MediumBg(),
                     Theme::Colors::DarkBg());
            break;
    }
}

void AnimatedButton::setColors(const sf::Color& idle, const sf::Color& hover, const sf::Color& pressed) {
    m_idleColor = idle;
    m_hoverColor = hover;
    m_pressedColor = pressed;
    m_text.setFillColor(Theme::Colors::TextWhite());
    updateColors();
}

void AnimatedButton::setEnabled(bool enabled) {
    m_enabled = enabled;
    if (!enabled) {
        m_hovered = false;
        m_pressed = false;
        m_animationTime = 0.f;
    }
}

bool AnimatedButton::handleEvent(const sf::Event& event) {
    if (!m_enabled) {
        return false;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left && contains(sf::Vector2f(
                static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y)
            ))) {
            m_pressed = true;
            m_animationTime = 0.f;
            return true;
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left && m_pressed) {
            m_pressed = false;
            if (m_onClick) {
                m_onClick();
            }
            return true;
        }
    }

    return false;
}

void AnimatedButton::update(float deltaTime, const sf::Vector2f& mousePosition) {
    if (!m_enabled) {
        m_hoverScale = 1.f;
        m_pressScale = 1.f;
        return;
    }

    bool wasHovered = m_hovered;
    m_hovered = contains(mousePosition);

    // Hover animation
    if (m_hovered) {
        m_hoverScale = std::min(1.1f, m_hoverScale + deltaTime / Theme::Effects::HoverAnimationDuration);
    } else {
        m_hoverScale = std::max(1.f, m_hoverScale - deltaTime / Theme::Effects::HoverAnimationDuration);
    }

    // Press animation
    if (m_pressed) {
        m_pressScale = std::max(0.95f, m_pressScale - deltaTime / Theme::Effects::ButtonPressAnimationDuration);
    } else {
        m_pressScale = std::min(1.f, m_pressScale + deltaTime / Theme::Effects::ButtonPressAnimationDuration);
    }

    m_animationTime += deltaTime;
    updateColors();
}

void AnimatedButton::updateColors() {
    if (m_pressed) {
        m_text.setFillColor(Theme::Colors::TextWhite());
    } else if (m_hovered) {
        m_text.setFillColor(Theme::Colors::TextWhite());
    } else {
        m_text.setFillColor(Theme::Colors::TextWhite());
    }
}

void AnimatedButton::draw(sf::RenderTarget& target) {
    if (!m_enabled) {
        // Draw disabled state
        DrawingUtilities::drawRoundedRectangle(
            target, m_position, m_size, Theme::Effects::CornerRadius,
            Theme::Colors::ButtonDisabled()
        );
    } else if (m_pressed) {
        // Draw pressed state
        DrawingUtilities::drawRoundedRectangle(
            target, m_position, m_size, Theme::Effects::CornerRadius,
            m_pressedColor
        );
    } else if (m_hovered) {
        // Draw hover state
        DrawingUtilities::drawRoundedRectangle(
            target, m_position, m_size, Theme::Effects::CornerRadius,
            m_hoverColor, Theme::Colors::GlassBorder(), 1.f
        );
    } else {
        // Draw idle state
        DrawingUtilities::drawRoundedRectangle(
            target, m_position, m_size, Theme::Effects::CornerRadius,
            m_idleColor
        );
    }

    // Draw text (centered and scaled)
    sf::Transform transform;
    transform.translate(m_position.x + m_size.x / 2.f, m_position.y + m_size.y / 2.f);
    transform.scale(m_hoverScale * m_pressScale, m_hoverScale * m_pressScale);
    transform.translate(-m_position.x - m_size.x / 2.f, -m_position.y - m_size.y / 2.f);

    if (m_icon != DrawingUtilities::Icon::None) {
        DrawingUtilities::drawIcon(
            target, m_icon, m_position + m_size / 2.f, m_size.y * 0.5f, Theme::Colors::TextGray()
        );
        return;
    }

    target.draw(m_text, transform);
}
