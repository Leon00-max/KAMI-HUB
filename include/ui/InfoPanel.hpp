#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "UIComponent.hpp"
#include "Theme.hpp"

/**
 * @class InfoPanel
 * @brief Right-side panel displaying game information
 * Shows: Next Piece (placeholder), Score, Level, Lines, High Score, Pause button
 */
class InfoPanel : public UIComponent {
public:
    InfoPanel(const sf::Font& font);

    // Update values
    void setScore(int score);
    void setLevel(int level);
    void setLines(int lines);
    void setHighScore(int highScore);

    // Drawing
    void draw(sf::RenderTarget& target) override;

private:
    const sf::Font& m_font;

    // Info texts
    sf::Text m_nextLabel;
    sf::Text m_scoreLabel;
    sf::Text m_scoreValue;
    sf::Text m_levelLabel;
    sf::Text m_levelValue;
    sf::Text m_linesLabel;
    sf::Text m_linesValue;
    sf::Text m_highScoreLabel;
    sf::Text m_highScoreValue;

    void initializeTexts();
};
