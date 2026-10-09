#include "ui/GameBoard.hpp"

#include <algorithm>
#include <cstdint>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "ui/DrawingUtilities.hpp"

GameBoard::GameBoard(const sf::Vector2f& position, int columns, int rows, float cellSize)
    : m_columns(columns), m_rows(rows), m_cellSize(cellSize),
      m_cells(static_cast<std::size_t>(columns * rows), sf::Color::Transparent) {
    setPosition(position);
    const float padding = Theme::Layout::BoardPadding * 2.f;
    setSize(sf::Vector2f(columns * cellSize + padding, rows * cellSize + padding));
}

void GameBoard::clear() {
    std::fill(m_cells.begin(), m_cells.end(), sf::Color::Transparent);
}

void GameBoard::setCell(int column, int row, const sf::Color& color) {
    if (column < 0 || column >= m_columns || row < 0 || row >= m_rows) {
        return;
    }
    m_cells[static_cast<std::size_t>(row * m_columns + column)] = color;
}

void GameBoard::drawFlash(sf::RenderTarget& target, const sf::Vector2f& origin) {
    const float y = origin.y + (m_flashRow + 0.5f) * m_cellSize;
    const float overhang = 34.f;
    const float left = origin.x - overhang;
    const float right = origin.x + m_columns * m_cellSize + overhang;
    const sf::Color green = Theme::Colors::GreenBright();

    // Wash over the clearing row, then a glowing beam through it
    DrawingUtilities::drawRoundedRectangle(
        target, sf::Vector2f(origin.x, y - m_cellSize / 2.f), sf::Vector2f(m_columns * m_cellSize, m_cellSize),
        0.f, Theme::withAlpha(green, 70)
    );
    for (int i = 6; i >= 1; --i) {
        DrawingUtilities::drawLine(
            target, sf::Vector2f(left, y), sf::Vector2f(right, y), i * 3.f,
            Theme::withAlpha(green, static_cast<sf::Uint8>(18 + (6 - i) * 12))
        );
    }
    DrawingUtilities::drawLine(target, sf::Vector2f(left, y), sf::Vector2f(right, y), 2.f, sf::Color(0xF0, 0xFF, 0xB0));

    // Leaf-like particles scattered along the beam (fixed seed: stable frames)
    std::uint32_t seed = 0x9E3779B9u;
    auto next = [&seed]() {
        seed = seed * 1664525u + 1013904223u;
        return static_cast<float>(seed >> 8) / static_cast<float>(1u << 24);
    };
    for (int i = 0; i < 46; ++i) {
        const float px = left + next() * (right - left);
        const float spread = 4.f + next() * 22.f;
        const float py = y + (next() < 0.5f ? -spread : spread) * next();
        sf::CircleShape leaf(1.5f + next() * 2.5f, 12);
        leaf.setOrigin(leaf.getRadius(), leaf.getRadius());
        leaf.setScale(1.f, 0.5f);
        leaf.setRotation(next() * 180.f);
        leaf.setPosition(px, py);
        leaf.setFillColor(Theme::withAlpha(Theme::mix(green, sf::Color(0xE6, 0xFF, 0x8A), next()), 220));
        target.draw(leaf);
    }
}

void GameBoard::draw(sf::RenderTarget& target) {
    DrawingUtilities::drawRoundedRectangle(
        target, m_position, m_size, Theme::Effects::CornerRadius,
        Theme::Colors::BoardBg(), Theme::Colors::PanelBorder(), 1.5f
    );

    const sf::Vector2f origin = m_position + sf::Vector2f(Theme::Layout::BoardPadding, Theme::Layout::BoardPadding);
    const float width = m_columns * m_cellSize;
    const float height = m_rows * m_cellSize;
    const sf::Color gridColor = Theme::Colors::BoardGrid();

    sf::VertexArray grid(sf::Lines);
    for (int col = 0; col <= m_columns; ++col) {
        const float x = origin.x + col * m_cellSize;
        grid.append(sf::Vertex(sf::Vector2f(x, origin.y), gridColor));
        grid.append(sf::Vertex(sf::Vector2f(x, origin.y + height), gridColor));
    }
    for (int row = 0; row <= m_rows; ++row) {
        const float y = origin.y + row * m_cellSize;
        grid.append(sf::Vertex(sf::Vector2f(origin.x, y), gridColor));
        grid.append(sf::Vertex(sf::Vector2f(origin.x + width, y), gridColor));
    }
    target.draw(grid);

    for (int row = 0; row < m_rows; ++row) {
        for (int col = 0; col < m_columns; ++col) {
            const sf::Color& color = m_cells[static_cast<std::size_t>(row * m_columns + col)];
            if (color.a > 0) {
                DrawingUtilities::drawBlock(
                    target, origin + sf::Vector2f(col * m_cellSize, row * m_cellSize), m_cellSize, color
                );
            }
        }
    }

    if (m_flashRow >= 0 && m_flashRow < m_rows) {
        drawFlash(target, origin);
    }
}
