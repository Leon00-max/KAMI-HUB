#pragma once

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "UIComponent.hpp"
#include "GameCard.hpp"
#include "Theme.hpp"

/**
 * @class GameSelectionPanel
 * @brief Left-side panel with game selection cards (Tetris, Breakout)
 */
class GameSelectionPanel : public UIComponent {
public:
    GameSelectionPanel(const sf::Font& font);

    // Input handling
    bool handleEvent(const sf::Event& event);

    // Update with mouse position
    void update(float deltaTime, const sf::Vector2f& mousePosition);

    // Drawing
    void draw(sf::RenderTarget& target) override;

    // Query selected game
    int getSelectedGameIndex() const { return m_selectedGameIndex; }

private:
    const sf::Font& m_font;
    std::vector<GameCard> m_gameCards;
    int m_selectedGameIndex = 0; // Default to Tetris

    void initializeGameCards();
};
