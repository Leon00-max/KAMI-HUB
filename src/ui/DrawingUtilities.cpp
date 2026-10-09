#include "ui/DrawingUtilities.hpp"

#include <algorithm>
#include <cmath>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

std::vector<sf::Vector2f> DrawingUtilities::roundedRectanglePoints(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    float radius,
    unsigned int cornerSegments
) {
    const float r = std::clamp(radius, 0.f, std::min(size.x, size.y) / 2.f);
    if (r < 0.5f || cornerSegments == 0) {
        return {
            position,
            position + sf::Vector2f(size.x, 0.f),
            position + size,
            position + sf::Vector2f(0.f, size.y)
        };
    }

    // Corner centers in clockwise order with the angle each arc starts at
    const sf::Vector2f centers[4] = {
        position + sf::Vector2f(r, r),
        position + sf::Vector2f(size.x - r, r),
        position + sf::Vector2f(size.x - r, size.y - r),
        position + sf::Vector2f(r, size.y - r)
    };
    const float startAngles[4] = {PI, 1.5f * PI, 0.f, 0.5f * PI};

    std::vector<sf::Vector2f> points;
    points.reserve((cornerSegments + 1) * 4);
    for (int corner = 0; corner < 4; ++corner) {
        for (unsigned int i = 0; i <= cornerSegments; ++i) {
            const float angle = startAngles[corner] + (PI / 2.f) * static_cast<float>(i) / cornerSegments;
            points.push_back(centers[corner] + sf::Vector2f(r * std::cos(angle), r * std::sin(angle)));
        }
    }
    return points;
}

sf::ConvexShape DrawingUtilities::createRoundedRectangle(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    float radius,
    unsigned int cornerSegments
) {
    const auto points = roundedRectanglePoints(position, size, radius, cornerSegments);
    sf::ConvexShape shape(points.size());
    for (std::size_t i = 0; i < points.size(); ++i) {
        shape.setPoint(i, points[i]);
    }
    return shape;
}

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

void DrawingUtilities::drawGradientRoundedRectangle(
    sf::RenderTarget& target,
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    float radius,
    const sf::Color& topColor,
    const sf::Color& bottomColor
) {
    const auto points = roundedRectanglePoints(position, size, radius);
    sf::VertexArray fan(sf::TriangleFan, points.size() + 2);
    fan[0] = sf::Vertex(position + size / 2.f, Theme::mix(topColor, bottomColor, 0.5f));
    for (std::size_t i = 0; i <= points.size(); ++i) {
        const sf::Vector2f& p = points[i % points.size()];
        const float t = size.y > 0.f ? (p.y - position.y) / size.y : 0.f;
        fan[i + 1] = sf::Vertex(p, Theme::mix(topColor, bottomColor, t));
    }
    target.draw(fan);
}

void DrawingUtilities::drawGlow(
    sf::RenderTarget& target,
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    float radius,
    const sf::Color& color,
    float spread
) {
    const int layers = 6;
    const float step = spread / layers;
    for (int i = 1; i <= layers; ++i) {
        const float grow = step * (i - 0.5f);
        const float falloff = 1.f - static_cast<float>(i) / (layers + 1);
        auto ring = createRoundedRectangle(
            position - sf::Vector2f(grow, grow),
            size + sf::Vector2f(grow * 2.f, grow * 2.f),
            radius + grow
        );
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineThickness(step);
        ring.setOutlineColor(Theme::withAlpha(color, static_cast<sf::Uint8>(color.a * falloff * falloff * 0.45f)));
        target.draw(ring);
    }
}

void DrawingUtilities::drawShadow(
    sf::RenderTarget& target,
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    float radius,
    const sf::Vector2f& offset,
    const sf::Color& shadowColor
) {
    // Stacked translucent layers read as a soft shadow without a blur shader
    for (int i = 3; i >= 1; --i) {
        const float grow = 3.f * i;
        drawRoundedRectangle(
            target,
            position + offset - sf::Vector2f(grow, grow) / 2.f,
            size + sf::Vector2f(grow, grow),
            radius + grow / 2.f,
            Theme::withAlpha(shadowColor, static_cast<sf::Uint8>(shadowColor.a / (i + 1)))
        );
    }
}

void DrawingUtilities::drawGlassPanel(
    sf::RenderTarget& target,
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    float radius,
    const sf::Color& fillColor,
    const sf::Color& borderColor
) {
    drawShadow(target, position, size, radius);
    drawRoundedRectangle(target, position, size, radius, fillColor, borderColor, 1.f);
}

void DrawingUtilities::drawFrostedPanel(
    sf::RenderTarget& target,
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    float radius
) {
    drawShadow(target, position, size, radius, sf::Vector2f(0.f, 8.f), sf::Color(0, 0, 0, 90));
    drawGradientRoundedRectangle(
        target, position, size, radius, Theme::Colors::FrostTop(), Theme::Colors::FrostBottom()
    );
    drawRoundedRectangle(
        target, position, size, radius, sf::Color::Transparent, Theme::Colors::FrostBorder(), 1.5f
    );
    // Specular edge along the top
    drawLine(
        target,
        position + sf::Vector2f(radius, 1.5f),
        position + sf::Vector2f(size.x - radius, 1.5f),
        1.f,
        sf::Color(255, 255, 255, 110)
    );
}

void DrawingUtilities::drawBlock(
    sf::RenderTarget& target,
    const sf::Vector2f& position,
    float size,
    const sf::Color& color
) {
    const float gap = 1.f;
    const float x0 = position.x + gap;
    const float y0 = position.y + gap;
    const float x1 = position.x + size - gap;
    const float y1 = position.y + size - gap;
    const float b = size * 0.16f;

    sf::VertexArray quads(sf::Quads);
    auto quad = [&quads](sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Color col) {
        quads.append(sf::Vertex(p0, col));
        quads.append(sf::Vertex(p1, col));
        quads.append(sf::Vertex(p2, col));
        quads.append(sf::Vertex(p3, col));
    };

    // Bevel edges, lit from the top-left
    quad({x0, y0}, {x1, y0}, {x1 - b, y0 + b}, {x0 + b, y0 + b}, Theme::lighten(color, 1.45f));
    quad({x0, y0}, {x0 + b, y0 + b}, {x0 + b, y1 - b}, {x0, y1}, Theme::lighten(color, 1.15f));
    quad({x1, y0}, {x1, y1}, {x1 - b, y1 - b}, {x1 - b, y0 + b}, Theme::darken(color, 0.75f));
    quad({x0, y1}, {x0 + b, y1 - b}, {x1 - b, y1 - b}, {x1, y1}, Theme::darken(color, 0.55f));

    // Face with a soft vertical gradient
    const sf::Color faceTop = Theme::lighten(color, 1.12f);
    quads.append(sf::Vertex({x0 + b, y0 + b}, faceTop));
    quads.append(sf::Vertex({x1 - b, y0 + b}, faceTop));
    quads.append(sf::Vertex({x1 - b, y1 - b}, color));
    quads.append(sf::Vertex({x0 + b, y1 - b}, color));

    // Gloss on the upper half of the face
    const float midY = (y0 + y1) / 2.f;
    quads.append(sf::Vertex({x0 + b, y0 + b}, sf::Color(255, 255, 255, 55)));
    quads.append(sf::Vertex({x1 - b, y0 + b}, sf::Color(255, 255, 255, 55)));
    quads.append(sf::Vertex({x1 - b, midY}, sf::Color(255, 255, 255, 0)));
    quads.append(sf::Vertex({x0 + b, midY}, sf::Color(255, 255, 255, 0)));

    target.draw(quads);
}

void DrawingUtilities::drawLine(
    sf::RenderTarget& target,
    const sf::Vector2f& from,
    const sf::Vector2f& to,
    float thickness,
    const sf::Color& color
) {
    const sf::Vector2f delta = to - from;
    const float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    sf::RectangleShape line(sf::Vector2f(length, thickness));
    line.setOrigin(0.f, thickness / 2.f);
    line.setPosition(from);
    line.setRotation(std::atan2(delta.y, delta.x) * 180.f / PI);
    line.setFillColor(color);
    target.draw(line);

    // Round caps hide seams where polyline segments meet
    if (thickness >= 2.f) {
        sf::CircleShape cap(thickness / 2.f);
        cap.setOrigin(thickness / 2.f, thickness / 2.f);
        cap.setFillColor(color);
        cap.setPosition(from);
        target.draw(cap);
        cap.setPosition(to);
        target.draw(cap);
    }
}

void DrawingUtilities::drawArc(
    sf::RenderTarget& target,
    const sf::Vector2f& center,
    float radius,
    float startDegrees,
    float endDegrees,
    float thickness,
    const sf::Color& color
) {
    const int segments = 24;
    sf::VertexArray strip(sf::TriangleStrip, (segments + 1) * 2);
    for (int i = 0; i <= segments; ++i) {
        const float degrees = startDegrees + (endDegrees - startDegrees) * i / segments;
        const sf::Vector2f dir(std::cos(degrees * PI / 180.f), std::sin(degrees * PI / 180.f));
        strip[i * 2] = sf::Vertex(center + dir * (radius - thickness / 2.f), color);
        strip[i * 2 + 1] = sf::Vertex(center + dir * (radius + thickness / 2.f), color);
    }
    target.draw(strip);
}

void DrawingUtilities::drawIcon(
    sf::RenderTarget& target,
    Icon icon,
    const sf::Vector2f& center,
    float size,
    const sf::Color& color,
    const sf::Color& cutoutColor
) {
    const sf::Vector2f origin = center - sf::Vector2f(size / 2.f, size / 2.f);
    const float stroke = std::max(1.5f, size * 0.1f);

    auto at = [&](float x, float y) { return origin + sf::Vector2f(x * size, y * size); };
    auto line = [&](float ax, float ay, float bx, float by) {
        drawLine(target, at(ax, ay), at(bx, by), stroke, color);
    };
    auto fillRect = [&](float x, float y, float w, float h) {
        sf::RectangleShape rect(sf::Vector2f(w * size, h * size));
        rect.setPosition(at(x, y));
        rect.setFillColor(color);
        target.draw(rect);
    };
    auto fillCircle = [&](float x, float y, float r, const sf::Color& fill) {
        sf::CircleShape circle(r * size);
        circle.setOrigin(r * size, r * size);
        circle.setPosition(at(x, y));
        circle.setFillColor(fill);
        target.draw(circle);
    };

    switch (icon) {
        case Icon::None:
            break;

        case Icon::Home:
            line(0.08f, 0.5f, 0.5f, 0.12f);
            line(0.5f, 0.12f, 0.92f, 0.5f);
            line(0.2f, 0.4f, 0.2f, 0.88f);
            line(0.2f, 0.88f, 0.8f, 0.88f);
            line(0.8f, 0.88f, 0.8f, 0.4f);
            line(0.42f, 0.88f, 0.42f, 0.64f);
            line(0.42f, 0.64f, 0.58f, 0.64f);
            line(0.58f, 0.64f, 0.58f, 0.88f);
            break;

        case Icon::Gamepad: {
            auto body = createRoundedRectangle(at(0.04f, 0.26f), sf::Vector2f(0.92f, 0.5f) * size, 0.22f * size);
            body.setFillColor(sf::Color::Transparent);
            body.setOutlineThickness(stroke);
            body.setOutlineColor(color);
            target.draw(body);
            line(0.19f, 0.51f, 0.41f, 0.51f);
            line(0.3f, 0.4f, 0.3f, 0.62f);
            fillCircle(0.65f, 0.45f, 0.06f, color);
            fillCircle(0.77f, 0.57f, 0.06f, color);
            break;
        }

        case Icon::Stats:
            fillRect(0.14f, 0.5f, 0.14f, 0.4f);
            fillRect(0.43f, 0.18f, 0.14f, 0.72f);
            fillRect(0.72f, 0.38f, 0.14f, 0.52f);
            break;

        case Icon::Trophy:
            line(0.28f, 0.14f, 0.72f, 0.14f);
            line(0.72f, 0.14f, 0.68f, 0.42f);
            line(0.68f, 0.42f, 0.5f, 0.58f);
            line(0.5f, 0.58f, 0.32f, 0.42f);
            line(0.32f, 0.42f, 0.28f, 0.14f);
            drawArc(target, at(0.27f, 0.3f), 0.12f * size, 90.f, 270.f, stroke, color);
            drawArc(target, at(0.73f, 0.3f), 0.12f * size, -90.f, 90.f, stroke, color);
            line(0.5f, 0.58f, 0.5f, 0.76f);
            line(0.36f, 0.78f, 0.64f, 0.78f);
            line(0.3f, 0.9f, 0.7f, 0.9f);
            break;

        case Icon::Gear: {
            // Each bar spans the gear, giving two opposite teeth
            const int bars = 4;
            for (int i = 0; i < bars; ++i) {
                sf::RectangleShape tooth(sf::Vector2f(size * 0.2f, size * 0.98f));
                tooth.setOrigin(tooth.getSize() / 2.f);
                tooth.setPosition(center);
                tooth.setRotation(i * 180.f / bars);
                tooth.setFillColor(color);
                target.draw(tooth);
            }
            fillCircle(0.5f, 0.5f, 0.3f, color);
            fillCircle(0.5f, 0.5f, 0.13f, cutoutColor);
            break;
        }

        case Icon::Music:
            // Two beamed eighth notes
            for (float x : {0.18f, 0.62f}) {
                sf::CircleShape head(size * 0.13f);
                head.setOrigin(head.getRadius(), head.getRadius());
                head.setScale(1.25f, 1.f);
                head.setPosition(at(x + 0.02f, 0.8f));
                head.setFillColor(color);
                target.draw(head);
                fillRect(x + 0.14f, 0.15f, 0.07f, 0.65f);
            }
            fillRect(0.32f, 0.12f, 0.46f, 0.12f);
            break;

        case Icon::Speaker: {
            fillRect(0.06f, 0.36f, 0.18f, 0.28f);
            sf::ConvexShape cone(4);
            cone.setPoint(0, at(0.24f, 0.36f));
            cone.setPoint(1, at(0.48f, 0.12f));
            cone.setPoint(2, at(0.48f, 0.88f));
            cone.setPoint(3, at(0.24f, 0.64f));
            cone.setFillColor(color);
            target.draw(cone);
            drawArc(target, at(0.5f, 0.5f), 0.2f * size, -50.f, 50.f, stroke, color);
            drawArc(target, at(0.5f, 0.5f), 0.38f * size, -50.f, 50.f, stroke, color);
            break;
        }

        case Icon::Play: {
            sf::ConvexShape triangle(3);
            triangle.setPoint(0, at(0.22f, 0.12f));
            triangle.setPoint(1, at(0.88f, 0.5f));
            triangle.setPoint(2, at(0.22f, 0.88f));
            triangle.setFillColor(color);
            target.draw(triangle);
            break;
        }

        case Icon::Pause:
            fillRect(0.2f, 0.12f, 0.2f, 0.76f);
            fillRect(0.6f, 0.12f, 0.2f, 0.76f);
            break;

        case Icon::Restart: {
            const float startDegrees = -50.f;
            drawArc(target, center, 0.34f * size, startDegrees, 250.f, stroke * 1.2f, color);
            // Arrowhead at the open end, pointing counter-clockwise
            const float theta = startDegrees * PI / 180.f;
            const sf::Vector2f radial(std::cos(theta), std::sin(theta));
            const sf::Vector2f tangent(std::sin(theta), -std::cos(theta));
            const sf::Vector2f tip = center + radial * (0.34f * size);
            sf::ConvexShape head(3);
            head.setPoint(0, tip + tangent * (0.2f * size));
            head.setPoint(1, tip + radial * (0.17f * size));
            head.setPoint(2, tip - radial * (0.17f * size));
            head.setFillColor(color);
            target.draw(head);
            break;
        }

        case Icon::Menu:
            for (float y : {0.16f, 0.44f, 0.72f}) {
                fillRect(0.08f, y, 0.16f, 0.14f);
                fillRect(0.34f, y, 0.58f, 0.14f);
            }
            break;

        case Icon::Blocks: {
            // Ascending staircase of blocks: column heights 1, 2, 3
            const float cell = size / 3.f;
            for (int col = 0; col < 3; ++col) {
                for (int row = 0; row <= col; ++row) {
                    drawBlock(target, origin + sf::Vector2f(col * cell, size - (row + 1) * cell), cell, color);
                }
            }
            break;
        }

        case Icon::Paddle: {
            // Faint trajectory, ball, paddle
            const sf::Color trail = Theme::withAlpha(Theme::lighten(color, 1.4f), 120);
            drawLine(target, at(0.34f, 0.7f), at(0.7f, 0.3f), std::max(1.f, size * 0.05f), trail);
            fillCircle(0.72f, 0.28f, 0.14f, Theme::lighten(color, 1.4f));
            fillCircle(0.68f, 0.24f, 0.05f, sf::Color(255, 255, 255, 170));
            drawGradientRoundedRectangle(
                target, at(0.06f, 0.72f), sf::Vector2f(0.88f, 0.16f) * size, 0.08f * size,
                Theme::lighten(color, 1.3f), Theme::darken(color, 0.7f)
            );
            break;
        }
    }
}
