#pragma once

#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Theme.hpp"
#include "UIComponent.hpp"

/**
 * @class GameBoard
 * @brief Draws the Tetris well: framed grid plus colored cells.
 *
 * Purely a view; game rules own the cell contents and push them here.
 */
class GameBoard : public UIComponent {
public:
    GameBoard(const sf::Vector2f& position, int columns, int rows, float cellSize);

    void clear();
    void setCell(int column, int row, const sf::Color& color);

    // Highlight a row being cleared (-1 for none)
    void setFlashRow(int row) { m_flashRow = row; }

    void draw(sf::RenderTarget& target) override;

private:
    int m_columns;
    int m_rows;
    float m_cellSize;
    std::vector<sf::Color> m_cells; // Transparent = empty
    int m_flashRow = -1;

    void drawFlash(sf::RenderTarget& target, const sf::Vector2f& origin);
};
