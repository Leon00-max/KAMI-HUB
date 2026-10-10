#include "ui/GameBoard.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "ui/DrawingUtilities.hpp"

namespace {
constexpr float Pi = 3.14159265f;
}

GameBoard::GameBoard(const sf::Vector2f& position, int columns, int rows, float cellSize)
    : m_columns(columns), m_rows(rows), m_cellSize(cellSize),
      m_cells(static_cast<std::size_t>(columns * rows), sf::Color::Transparent) {
    setPosition(position);
    const float padding = Theme::Layout::BoardPadding * 2.f;
    setSize(sf::Vector2f(columns * cellSize + padding, rows * cellSize + padding));
}

void GameBoard::clear() {
    std::fill(m_cells.begin(), m_cells.end(), sf::Color::Transparent);
    m_activeCells.clear();
    m_ghostCells.clear();
    m_clearingRows.clear();
    m_clearProgress = 0.f;
}

void GameBoard::setCell(int column, int row, const sf::Color& color) {
    if (column < 0 || column >= m_columns || row < 0 || row >= m_rows) {
        return;
    }
    m_cells[static_cast<std::size_t>(row * m_columns + column)] = color;
}

void GameBoard::setActivePiece(const std::vector<sf::Vector2f>& cells, const sf::Color& color) {
    m_activeCells = cells;
    m_activeColor = color;
}

void GameBoard::setGhost(const std::vector<sf::Vector2i>& cells, const sf::Color& color) {
    m_ghostCells = cells;
    m_ghostColor = color;
}

void GameBoard::setClearing(const std::vector<int>& rows, float progress) {
    m_clearingRows = rows;
    m_clearProgress = std::clamp(progress, 0.f, 1.f);
}

sf::Vector2f GameBoard::gridOrigin() const {
    return m_position + sf::Vector2f(Theme::Layout::BoardPadding, Theme::Layout::BoardPadding);
}

sf::Vector2f GameBoard::gridSize() const {
    return sf::Vector2f(m_columns * m_cellSize, m_rows * m_cellSize);
}

bool GameBoard::isClearing(int row) const {
    return std::find(m_clearingRows.begin(), m_clearingRows.end(), row) != m_clearingRows.end();
}

void GameBoard::drawClearingRow(sf::RenderTarget& target, int row) {
    // Blocks brighten toward white, then the row collapses into its center
    const sf::Vector2f origin = gridOrigin();
    const float p = m_clearProgress;
    const float whiten = std::min(1.f, p * 2.5f) * 0.75f;
    const float collapse = p < 0.45f ? 1.f : 1.f - std::pow((p - 0.45f) / 0.55f, 2.f);
    const float centerX = origin.x + m_columns * m_cellSize / 2.f;
    const float size = m_cellSize * std::max(0.f, collapse);

    for (int col = 0; col < m_columns; ++col) {
        const sf::Color& color = m_cells[static_cast<std::size_t>(row * m_columns + col)];
        if (color.a == 0 || size < 1.f) {
            continue;
        }
        const float cellCenterX = origin.x + (col + 0.5f) * m_cellSize;
        const float x = centerX + (cellCenterX - centerX) * collapse - size / 2.f;
        const float y = origin.y + (row + 0.5f) * m_cellSize - size / 2.f;
        DrawingUtilities::drawBlock(target, sf::Vector2f(x, y), size, Theme::mix(color, sf::Color::White, whiten));
    }
}

void GameBoard::drawFlash(sf::RenderTarget& target, int row) {
    const sf::Vector2f origin = gridOrigin();
    const float p = m_clearProgress;
    const float intensity = std::sin(Pi * std::min(1.f, p * 1.2f)); // Peaks early, fades out
    const float y = origin.y + (row + 0.5f) * m_cellSize;
    const float overhang = 34.f * (0.5f + 0.5f * intensity);
    const float left = origin.x - overhang;
    const float right = origin.x + m_columns * m_cellSize + overhang;
    const sf::Color green = Theme::Colors::GreenBright();
    auto fade = [intensity](int alpha) { return static_cast<sf::Uint8>(std::clamp(alpha * intensity, 0.f, 255.f)); };

    // Wash over the row, then a glowing beam through it
    DrawingUtilities::drawRoundedRectangle(
        target, sf::Vector2f(origin.x, y - m_cellSize / 2.f), sf::Vector2f(m_columns * m_cellSize, m_cellSize),
        0.f, Theme::withAlpha(green, fade(70))
    );
    for (int i = 6; i >= 1; --i) {
        DrawingUtilities::drawLine(
            target, sf::Vector2f(left, y), sf::Vector2f(right, y), i * 3.f,
            Theme::withAlpha(green, fade(18 + (6 - i) * 12))
        );
    }
    DrawingUtilities::drawLine(
        target, sf::Vector2f(left, y), sf::Vector2f(right, y), 2.f, sf::Color(0xF0, 0xFF, 0xB0, fade(255))
    );

    // Leaf-like particles drifting outward from the beam; seeded per row so
    // each particle keeps its identity across frames
    std::uint32_t seed = 0x9E3779B9u ^ static_cast<std::uint32_t>(row * 7919);
    auto next = [&seed]() {
        seed = seed * 1664525u + 1013904223u;
        return static_cast<float>(seed >> 8) / static_cast<float>(1u << 24);
    };
    const float drift = 0.5f + p * 1.6f;
    for (int i = 0; i < 46; ++i) {
        const float px = left + next() * (right - left);
        const float spread = (4.f + next() * 22.f) * drift;
        const float py = y + (next() < 0.5f ? -spread : spread) * next();
        sf::CircleShape leaf(1.5f + next() * 2.5f, 12);
        leaf.setOrigin(leaf.getRadius(), leaf.getRadius());
        leaf.setScale(1.f, 0.5f);
        leaf.setRotation(next() * 180.f + p * 120.f);
        leaf.setPosition(px, py);
        leaf.setFillColor(Theme::withAlpha(Theme::mix(green, sf::Color(0xE6, 0xFF, 0x8A), next()), fade(230)));
        target.draw(leaf);
    }
}

void GameBoard::draw(sf::RenderTarget& target) {
    DrawingUtilities::drawRoundedRectangle(
        target, m_position, m_size, Theme::Effects::CornerRadius,
        Theme::Colors::BoardBg(), Theme::Colors::PanelBorder(), 1.5f
    );

    const sf::Vector2f origin = gridOrigin();
    const sf::Vector2f size = gridSize();
    const sf::Color gridColor = Theme::Colors::BoardGrid();

    sf::VertexArray grid(sf::Lines);
    for (int col = 0; col <= m_columns; ++col) {
        const float x = origin.x + col * m_cellSize;
        grid.append(sf::Vertex(sf::Vector2f(x, origin.y), gridColor));
        grid.append(sf::Vertex(sf::Vector2f(x, origin.y + size.y), gridColor));
    }
    for (int row = 0; row <= m_rows; ++row) {
        const float y = origin.y + row * m_cellSize;
        grid.append(sf::Vertex(sf::Vector2f(origin.x, y), gridColor));
        grid.append(sf::Vertex(sf::Vector2f(origin.x + size.x, y), gridColor));
    }
    target.draw(grid);

    for (int row = 0; row < m_rows; ++row) {
        if (isClearing(row)) {
            drawClearingRow(target, row);
            continue;
        }
        for (int col = 0; col < m_columns; ++col) {
            const sf::Color& color = m_cells[static_cast<std::size_t>(row * m_columns + col)];
            if (color.a > 0) {
                DrawingUtilities::drawBlock(
                    target, origin + sf::Vector2f(col * m_cellSize, row * m_cellSize), m_cellSize, color
                );
            }
        }
    }

    // Ghost: tinted fill with a bright outline where the piece will land
    for (const auto& cell : m_ghostCells) {
        if (cell.y < 0) {
            continue;
        }
        const sf::Vector2f pos = origin + sf::Vector2f(cell.x * m_cellSize + 2.f, cell.y * m_cellSize + 2.f);
        DrawingUtilities::drawRoundedRectangle(
            target, pos, sf::Vector2f(m_cellSize - 4.f, m_cellSize - 4.f), 3.f,
            Theme::withAlpha(m_ghostColor, 38), Theme::withAlpha(Theme::lighten(m_ghostColor, 1.3f), 150), 1.5f
        );
    }

    // Falling piece; cells still above the visible well are skipped
    for (const auto& cell : m_activeCells) {
        if (cell.y < -0.5f) {
            continue;
        }
        DrawingUtilities::drawBlock(
            target, origin + sf::Vector2f(cell.x * m_cellSize, cell.y * m_cellSize), m_cellSize, m_activeColor
        );
    }

    for (int row : m_clearingRows) {
        drawFlash(target, row);
    }
}
