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

    // Each corner arc includes both endpoints
    unsigned int pointCount = (cornerSegments + 1) * 4;
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
        float angle = 1.5f * PI + (i * PI / 2.f / cornerSegments);
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

    return shape;
}

void DrawingUtilities::drawIcon(
    sf::RenderTarget& target,
    Icon icon,
    const sf::Vector2f& center,
    float size,
    const sf::Color& color
) {
    const float half = size / 2.f;
    const sf::Vector2f origin = center - sf::Vector2f(half, half);

    auto fillRect = [&](float x, float y, float w, float h) {
        sf::RectangleShape rect(sf::Vector2f(w * size, h * size));
        rect.setPosition(origin + sf::Vector2f(x * size, y * size));
        rect.setFillColor(color);
        target.draw(rect);
    };

    switch (icon) {
        case Icon::None:
            break;

        case Icon::Blocks: {
            // T tetromino on a 3x2 grid, vertically centered
            const float cell = 1.f / 3.f;
            const float gap = 0.03f;
            const float top = 0.5f - cell;
            for (int col = 0; col < 3; ++col) {
                fillRect(col * cell + gap, top + gap, cell - 2 * gap, cell - 2 * gap);
            }
            fillRect(cell + gap, top + cell + gap, cell - 2 * gap, cell - 2 * gap);
            break;
        }

        case Icon::Paddle: {
            // Brick row, ball, paddle
            for (int col = 0; col < 4; ++col) {
                fillRect(col * 0.25f + 0.02f, 0.08f, 0.21f, 0.12f);
            }
            sf::CircleShape ball(size * 0.08f);
            ball.setOrigin(ball.getRadius(), ball.getRadius());
            ball.setPosition(origin + sf::Vector2f(size * 0.6f, size * 0.55f));
            ball.setFillColor(color);
            target.draw(ball);
            fillRect(0.25f, 0.82f, 0.5f, 0.1f);
            break;
        }

        case Icon::Gear: {
            const int teeth = 6;
            for (int i = 0; i < teeth; ++i) {
                sf::RectangleShape tooth(sf::Vector2f(size * 0.2f, size * 1.f));
                tooth.setOrigin(tooth.getSize() / 2.f);
                tooth.setPosition(center);
                tooth.setRotation(i * 180.f / teeth);
                tooth.setFillColor(color);
                target.draw(tooth);
            }
            sf::CircleShape ring(size * 0.34f);
            ring.setOrigin(ring.getRadius(), ring.getRadius());
            ring.setPosition(center);
            ring.setFillColor(color);
            target.draw(ring);
            sf::CircleShape hole(size * 0.15f);
            hole.setOrigin(hole.getRadius(), hole.getRadius());
            hole.setPosition(center);
            hole.setFillColor(Theme::Colors::LightBg());
            target.draw(hole);
            break;
        }

        case Icon::Music: {
            // Two beamed eighth notes
            for (float x : {0.18f, 0.62f}) {
                sf::CircleShape head(size * 0.13f);
                head.setOrigin(head.getRadius(), head.getRadius());
                head.setScale(1.25f, 1.f);
                head.setPosition(origin + sf::Vector2f((x + 0.02f) * size, 0.8f * size));
                head.setFillColor(color);
                target.draw(head);
                fillRect(x + 0.14f, 0.15f, 0.07f, 0.65f);
            }
            fillRect(0.32f, 0.12f, 0.46f, 0.12f);
            break;
        }

        case Icon::Speaker: {
            fillRect(0.08f, 0.36f, 0.18f, 0.28f);
            sf::ConvexShape cone(4);
            cone.setPoint(0, origin + sf::Vector2f(0.26f * size, 0.36f * size));
            cone.setPoint(1, origin + sf::Vector2f(0.52f * size, 0.12f * size));
            cone.setPoint(2, origin + sf::Vector2f(0.52f * size, 0.88f * size));
            cone.setPoint(3, origin + sf::Vector2f(0.26f * size, 0.64f * size));
            cone.setFillColor(color);
            target.draw(cone);
            // Sound waves
            fillRect(0.64f, 0.38f, 0.07f, 0.24f);
            fillRect(0.82f, 0.24f, 0.07f, 0.52f);
            break;
        }
    }
}
