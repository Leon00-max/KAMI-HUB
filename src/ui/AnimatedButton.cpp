#include "ui/AnimatedButton.hpp"

#include <algorithm>
#include <SFML/Graphics/CircleShape.hpp>

namespace {
constexpr float IconSize = 16.f;
constexpr float IconGap = 9.f;
constexpr float HorizontalPadding = 22.f;
}

AnimatedButton::AnimatedButton(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const std::string& label,
    const sf::Font& font,
    ButtonStyle style,
    DrawingUtilities::Icon icon,
    std::function<void()> onClick
)
    : m_icon(icon), m_onClick(std::move(onClick)) {
    setPosition(position);
    setSize(size);
    m_text.setFont(font);
    m_text.setCharacterSize(Theme::FontSizes::Button);
    m_text.setFillColor(Theme::Colors::TextWhite());
    setLabel(label);
    setStyle(style);
}

void AnimatedButton::setLabel(const std::string& label) {
    m_text.setString(sf::String::fromUtf8(label.begin(), label.end()));
}

void AnimatedButton::setStyle(ButtonStyle style) {
    m_style = style;
    switch (style) {
        case ButtonStyle::Green:
            m_palette = {sf::Color(0x6C, 0xC2, 0x3A), sf::Color(0x2E, 0x7D, 0x22), sf::Color(0xA5, 0xE8, 0x6A)};
            break;
        case ButtonStyle::Orange:
            m_palette = {sf::Color(0xF9, 0xA8, 0x25), sf::Color(0xC2, 0x5E, 0x00), sf::Color(0xFF, 0xD5, 0x80)};
            break;
        case ButtonStyle::Red:
            m_palette = {sf::Color(0xE5, 0x3E, 0x35), sf::Color(0x9E, 0x12, 0x12), sf::Color(0xFF, 0x8A, 0x80)};
            break;
        case ButtonStyle::Purple:
            m_palette = {sf::Color(0x9B, 0x4D, 0xD6), sf::Color(0x55, 0x1C, 0x88), sf::Color(0xCE, 0x93, 0xF8)};
            break;
        case ButtonStyle::Blue:
            m_palette = {sf::Color(0x2A, 0x7F, 0xD4), sf::Color(0x0D, 0x3D, 0x80), sf::Color(0x7F, 0xC4, 0xFF)};
            break;
        case ButtonStyle::Round:
            m_palette = {sf::Color(0x0E, 0x1A, 0x33, 0xF0), sf::Color(0x06, 0x0D, 0x1F, 0xF0), sf::Color(0x7F, 0xC4, 0xFF, 0xE0)};
            break;
    }
}

void AnimatedButton::setEnabled(bool enabled) {
    m_enabled = enabled;
    if (!enabled) {
        m_hovered = false;
        m_pressed = false;
    }
}

bool AnimatedButton::handleEvent(const sf::Event& event) {
    if (!m_enabled) {
        return false;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f point(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        m_pressed = contains(point);
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f point(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        const bool clicked = m_pressed && contains(point);
        m_pressed = false;
        if (clicked) {
            if (m_onClick) {
                m_onClick();
            }
            return true;
        }
    }

    return false;
}

void AnimatedButton::update(float deltaTime, const sf::Vector2f& mousePosition) {
    m_hovered = m_enabled && contains(mousePosition);

    const float hoverStep = deltaTime / Theme::Effects::HoverAnimationDuration;
    m_hoverAmount = std::clamp(m_hoverAmount + (m_hovered ? hoverStep : -hoverStep), 0.f, 1.f);

    const float pressStep = deltaTime / Theme::Effects::ButtonPressAnimationDuration;
    m_pressAmount = std::clamp(m_pressAmount + (m_pressed ? pressStep : -pressStep), 0.f, 1.f);
}

float AnimatedButton::preferredWidth() const {
    float width = m_text.getLocalBounds().width + HorizontalPadding * 2.f;
    if (m_icon != DrawingUtilities::Icon::None) {
        width += IconSize + IconGap;
    }
    return width;
}

AnimatedButton::Palette AnimatedButton::currentPalette() const {
    Palette palette = m_palette;
    if (!m_enabled) {
        return {Theme::muted(palette.top), Theme::muted(palette.bottom), Theme::muted(palette.border)};
    }
    if (m_pressAmount > 0.f) {
        const float factor = 1.f - 0.18f * m_pressAmount;
        palette.top = Theme::darken(palette.top, factor);
        palette.bottom = Theme::darken(palette.bottom, factor);
    } else if (m_hoverAmount > 0.f) {
        palette.top = Theme::mix(palette.top, Theme::lighten(palette.top, 1.15f), m_hoverAmount);
    }
    return palette;
}

void AnimatedButton::draw(sf::RenderTarget& target) {
    const Palette palette = currentPalette();
    const float scale = 1.f + (Theme::Effects::HoverScale - 1.f) * m_hoverAmount - 0.03f * m_pressAmount;
    const sf::Vector2f center = m_position + m_size / 2.f;
    const sf::Vector2f size = m_size * scale;
    const sf::Vector2f pos = center - size / 2.f;

    sf::Color glow = palette.border;
    glow.a = static_cast<sf::Uint8>(glow.a * (m_enabled ? 0.55f + 0.45f * m_hoverAmount : 0.f));

    if (m_style == ButtonStyle::Round) {
        const float radius = size.x / 2.f;
        DrawingUtilities::drawGlow(target, pos, size, radius, glow, 8.f);
        sf::CircleShape circle(radius, 40);
        circle.setOrigin(radius, radius);
        circle.setPosition(center);
        circle.setFillColor(palette.top);
        circle.setOutlineThickness(2.f);
        circle.setOutlineColor(palette.border);
        target.draw(circle);
        DrawingUtilities::drawIcon(target, m_icon, center, size.x * 0.48f, Theme::Colors::TextWhite(), palette.top);
        return;
    }

    const float radius = Theme::Effects::CornerRadius * scale;
    DrawingUtilities::drawGlow(target, pos, size, radius, glow, 9.f);
    DrawingUtilities::drawGradientRoundedRectangle(target, pos, size, radius, palette.top, palette.bottom);

    // Gloss across the upper half
    const sf::Vector2f inset(2.f, 2.f);
    DrawingUtilities::drawGradientRoundedRectangle(
        target, pos + inset, sf::Vector2f(size.x - 4.f, size.y * 0.5f), radius - 2.f,
        sf::Color(255, 255, 255, m_enabled ? 70 : 25), sf::Color(255, 255, 255, 0)
    );
    DrawingUtilities::drawRoundedRectangle(target, pos, size, radius, sf::Color::Transparent, palette.border, 1.5f);

    // Icon + label, centered as a group
    const sf::Color content = m_enabled ? Theme::Colors::TextWhite() : Theme::withAlpha(Theme::Colors::TextGray(), 150);
    m_text.setScale(scale, scale);
    m_text.setFillColor(content);
    const sf::FloatRect bounds = m_text.getLocalBounds();
    const float iconSize = IconSize * scale;
    const bool hasIcon = m_icon != DrawingUtilities::Icon::None;
    const float textWidth = bounds.width * scale;
    const float groupWidth = textWidth + (hasIcon ? iconSize + IconGap * scale : 0.f);
    float x = center.x - groupWidth / 2.f;

    if (hasIcon) {
        DrawingUtilities::drawIcon(target, m_icon, sf::Vector2f(x + iconSize / 2.f, center.y), iconSize, content, palette.bottom);
        x += iconSize + IconGap * scale;
    }

    m_text.setOrigin(bounds.left, bounds.top + bounds.height / 2.f);
    m_text.setPosition(x, center.y + 1.f);
    sf::Text shadow = m_text;
    shadow.setFillColor(sf::Color(0, 0, 0, m_enabled ? 90 : 0));
    shadow.move(0.f, 1.f);
    target.draw(shadow);
    target.draw(m_text);
}
