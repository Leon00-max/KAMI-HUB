#include "ui/DrawingUtilities.hpp"

#include <cmath>

void DrawingUtilities::drawRoundedRectangle(
    sf::RenderTarget& target,
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    float radius,
    const sf::Color& fillColor,
    const sf::Color& outlineColor,
    float outlineThickness
) {
    auto shape = createRoundedRectangle(position, size, radius);
    shape.setFillColor(fillColor);
    if (outlineThickness > 0.f && outlineColor.a > 0) {
        shape.setOutlineThickness(outlineThickness);
        shape.setOutlineColor(outlineColor);
    }
    target.draw(shape);
}

void DrawingUtilities::drawGlassPanel(
    sf::RenderTarget& target,
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Color& glassColor,
    const sf::Color& glowColor
) {
    // Draw shadow
    drawShadow(target, position, size);

    // Draw main glass panel
    drawRoundedRectangle(target, position, size, Theme::Effects::CornerRadius, glassColor,
                        glowColor, Theme::Effects::GlowThickness);
}

void DrawingUtilities::drawShadow(
    sf::RenderTarget& target,
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Vector2f& offset,
    float blur,
    const sf::Color& shadowColor
) {
    // Simple shadow approximation: draw a slightly offset, darker rectangle
    sf::Vector2f shadowPos = position + offset;
    auto shadowShape = createRoundedRectangle(shadowPos, size, Theme::Effects::CornerRadius);
    shadowShape.setFillColor(shadowColor);
    target.draw(shadowShape);
}

sf::ConvexShape DrawingUtilities::createRoundedRectangle(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    float radius,
    unsigned int cornerSegments
) {
    sf::ConvexShape shape;
    std::vector<sf::Vector2f> points;

    float w = size.x;
    float h = size.y;
    float r = std::min(radius, std::min(w, h) / 2.f);

    // Limit radius to prevent invalid shapes
    if (r < 0.1f) r = 0.1f;

    unsigned int pointCount = cornerSegments * 4;
    shape.setPointCount(pointCount);

    unsigned int pointIdx = 0;

    // Top-left arc
    for (unsigned int i = 0; i <= cornerSegments; ++i) {
        float angle = PI + (i * PI / 2.f / cornerSegments);
        float px = r + r * std::cos(angle);
        float py = r + r * std::sin(angle);
        shape.setPoint(pointIdx++, position + sf::Vector2f(px, py));
    }

    // Top-right arc
    for (unsigned int i = 0; i <= cornerSegments; ++i) {
        float angle = PI / 2.f - (i * PI / 2.f / cornerSegments);
        float px = w - r + r * std::cos(angle);
        float py = r + r * std::sin(angle);
        shape.setPoint(pointIdx++, position + sf::Vector2f(px, py));
    }

    // Bottom-right arc
    for (unsigned int i = 0; i <= cornerSegments; ++i) {
        float angle = (i * PI / 2.f / cornerSegments);
        float px = w - r + r * std::cos(angle);
        float py = h - r + r * std::sin(angle);
        shape.setPoint(pointIdx++, position + sf::Vector2f(px, py));
    }

    // Bottom-left arc
    for (unsigned int i = 0; i <= cornerSegments; ++i) {
        float angle = PI / 2.f + (i * PI / 2.f / cornerSegments);
        float px = r + r * std::cos(angle);
        float py = h - r + r * std::sin(angle);
        shape.setPoint(pointIdx++, position + sf::Vector2f(px, py));
    }

    // Remove duplicate corners (we added one extra at each junction)
    while (shape.getPointCount() > pointIdx) {
        // Shape size was allocated, just reduce logical count via point manipulation
        break;
    }

    return shape;
}
