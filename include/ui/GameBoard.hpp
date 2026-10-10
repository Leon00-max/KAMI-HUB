#pragma once

#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Theme.hpp"
#include "UIComponent.hpp"

/**
 * @class GameBoard
 * @brief Draws the Tetris well: framed grid, locked cells, ghost, falling piece.
 *
 * Purely a view; the game rules own the state and push it here each frame.
 * Coordinates are visible-cell units with (0, 0) at the top-left cell.
 */
class GameBoard : public UIComponent {
public:
    GameBoard(const sf::Vector2f& position, int columns, int rows, float cellSize);

    void clear();
    void setCell(int column, int row, const sf::Color& color);

    // Falling piece; fractional positions allow smooth motion
    void setActivePiece(const std::vector<sf::Vector2f>& cells, const sf::Color& color);
    // Landing preview for the falling piece
    void setGhost(const std::vector<sf::Vector2i>& cells, const sf::Color& color);
    // Rows being cleared and animation progress 0..1
    void setClearing(const std::vector<int>& rows, float progress);

    // Top-left of the cell grid in view coordinates
    sf::Vector2f gridOrigin() const;
    sf::Vector2f gridSize() const;

    void draw(sf::RenderTarget& target) override;

private:
    int m_columns;
    int m_rows;
    float m_cellSize;
    std::vector<sf::Color> m_cells; // Transparent = empty

    std::vector<sf::Vector2f> m_activeCells;
    sf::Color m_activeColor;
    std::vector<sf::Vector2i> m_ghostCells;
    sf::Color m_ghostColor;
    std::vector<int> m_clearingRows;
    float m_clearProgress = 0.f;

    bool isClearing(int row) const;
    void drawClearingRow(sf::RenderTarget& target, int row);
    void drawFlash(sf::RenderTarget& target, int row);
};
