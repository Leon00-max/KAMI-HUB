#pragma once

#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Theme.hpp"

/**
 * @class DrawingUtilities
 * @brief Immediate-mode helpers for the shapes, effects and icons the UI uses.
 */
class DrawingUtilities {
public:
    // Shape-drawn icons; the bundled fonts have no emoji or symbol glyphs
    enum class Icon {
        None,
        Home,
        Gamepad,
        Stats,
        Trophy,
        Gear,
        Music,
        Speaker,
        Play,
        Pause,
        Restart,
        Menu,
        Blocks,  // Tetris
        Paddle   // Breakout
    };

    static void drawRoundedRectangle(
        sf::RenderTarget& target,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        float radius,
        const sf::Color& fillColor,
        const sf::Color& outlineColor = sf::Color::Transparent,
        float outlineThickness = 0.f
    );

    // Rounded rectangle whose fill fades from top to bottom
    static void drawGradientRoundedRectangle(
        sf::RenderTarget& target,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        float radius,
        const sf::Color& topColor,
        const sf::Color& bottomColor
    );

    // Soft halo approximated with concentric outlines of falling alpha
    static void drawGlow(
        sf::RenderTarget& target,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        float radius,
        const sf::Color& color,
        float spread = 10.f
    );

    // Dark glass panel with drop shadow and hairline border
    static void drawGlassPanel(
        sf::RenderTarget& target,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        float radius = Theme::Effects::PanelRadius,
        const sf::Color& fillColor = Theme::Colors::PanelBg(),
        const sf::Color& borderColor = Theme::Colors::PanelBorder()
    );

    // Light frosted panel with top highlight
    static void drawFrostedPanel(
        sf::RenderTarget& target,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        float radius = Theme::Effects::StageRadius
    );

    static void drawShadow(
        sf::RenderTarget& target,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        float radius,
        const sf::Vector2f& offset = sf::Vector2f(0.f, 6.f),
        const sf::Color& shadowColor = sf::Color(0, 0, 0, 70)
    );

    // Beveled, glossy Tetris cell
    static void drawBlock(
        sf::RenderTarget& target,
        const sf::Vector2f& position,
        float size,
        const sf::Color& color
    );

    static void drawLine(
        sf::RenderTarget& target,
        const sf::Vector2f& from,
        const sf::Vector2f& to,
        float thickness,
        const sf::Color& color
    );

    static void drawArc(
        sf::RenderTarget& target,
        const sf::Vector2f& center,
        float radius,
        float startDegrees,
        float endDegrees,
        float thickness,
        const sf::Color& color
    );

    /**
     * Draw an icon centered on a point
     * @param size Edge length of the square the icon fits in
     * @param cutoutColor Fill for holes in solid icons (gear center)
     */
    static void drawIcon(
        sf::RenderTarget& target,
        Icon icon,
        const sf::Vector2f& center,
        float size,
        const sf::Color& color,
        const sf::Color& cutoutColor = Theme::Colors::NavBg()
    );

    static std::vector<sf::Vector2f> roundedRectanglePoints(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        float radius,
        unsigned int cornerSegments = 8
    );

    static sf::ConvexShape createRoundedRectangle(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        float radius,
        unsigned int cornerSegments = 8
    );

private:
    static constexpr float PI = 3.14159265359f;
};
