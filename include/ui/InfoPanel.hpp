#pragma once

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "managers/ResourceManager.hpp"
#include "AnimatedButton.hpp"
#include "Theme.hpp"
#include "UIComponent.hpp"

/**
 * @class InfoPanel
 * @brief Column beside the board: next piece, score/level/lines/high score, pause
 */
class InfoPanel : public UIComponent {
public:
    explicit InfoPanel(const ResourceManager& resources);

    void setScore(int score);
    void setLevel(int level);
    void setLines(int lines);
    void setHighScore(int highScore);

    // Cells of the upcoming piece in its own grid coordinates
    void setNextPiece(const std::vector<sf::Vector2i>& cells, const sf::Color& color);
    void setPauseEnabled(bool enabled) { m_pauseButton.setEnabled(enabled); }
    void setPauseLabel(const std::string& label) { m_pauseButton.setLabel(label); }

    // Returns true when the pause button was clicked
    bool handleEvent(const sf::Event& event);
    void update(float deltaTime, const sf::Vector2f& mousePosition);
    void draw(sf::RenderTarget& target) override;

private:
    struct Stat {
        sf::Text label;
        sf::Text value;
    };

    void setStatValue(Stat& stat, const std::string& value);

    sf::Text m_nextLabel;
    std::vector<Stat> m_stats; // Score, level, lines, high score
    std::vector<sf::Vector2i> m_nextCells;
    sf::Color m_nextColor = sf::Color::Transparent;
    AnimatedButton m_pauseButton;
};
