#pragma once

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "managers/ResourceManager.hpp"
#include "GameCard.hpp"
#include "Theme.hpp"
#include "UIComponent.hpp"

/**
 * @class GameSelectionPanel
 * @brief "Game mode" panel with one card per game (Tetris, Breakout)
 */
class GameSelectionPanel : public UIComponent {
public:
    explicit GameSelectionPanel(const ResourceManager& resources);

    bool handleEvent(const sf::Event& event);
    void update(float deltaTime, const sf::Vector2f& mousePosition);
    void draw(sf::RenderTarget& target) override;

    int getSelectedGameIndex() const { return m_selectedGameIndex; }

private:
    sf::Text m_title;
    std::vector<GameCard> m_gameCards;
    int m_selectedGameIndex = 0;
};
