#pragma once

#include <vector>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include "Theme.hpp"

/**
 * @class DrawingUtilities
 * @brief Helper functions for drawing common UI shapes with visual polish
 */
class DrawingUtilities {
public:
    /**
     * Draw a rounded rectangle
     * @param target Render target
     * @param position Top-left corner
     * @param size Width and height
     * @param radius Corner radius
     * @param fillColor Fill color
     * @param outlineColor Outline color
     * @param outlineThickness Outline thickness
     */
    static void drawRoundedRectangle(
        sf::RenderTarget& target,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        float radius,
        const sf::Color& fillColor,
        const sf::Color& outlineColor = sf::Color::Transparent,
        float outlineThickness = 0.f
    );

    /**
     * Draw a glass panel with glow effect
     * @param target Render target
     * @param position Top-left corner
     * @param size Width and height
     * @param glassColor Semi-transparent base color
     * @param glowColor Border glow color
     */
    static void drawGlassPanel(
        sf::RenderTarget& target,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const sf::Color& glassColor = Theme::Colors::GlassBase,
        const sf::Color& glowColor = Theme::Colors::GlassBorder
    );

    /**
     * Draw a shadow underneath a shape
     * @param target Render target
     * @param position Top-left corner of the shape
     * @param size Size of the shape
     * @param offset Shadow offset (typically positive for bottom-right)
     * @param blur Shadow blur radius
     */
    static void drawShadow(
        sf::RenderTarget& target,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const sf::Vector2f& offset = sf::Vector2f(4.f, 4.f),
        float blur = 2.f,
        const sf::Color& shadowColor = sf::Color(0, 0, 0, 80)
    );

    /**
     * Create a rounded rectangle shape
     * (Approximate using convex shape with enough segments)
     */
    static sf::ConvexShape createRoundedRectangle(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        float radius,
        unsigned int cornerSegments = 8
    );

private:
    static constexpr float PI = 3.14159265359f;
};
