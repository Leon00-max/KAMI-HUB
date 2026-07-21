#include "ui/InfoPanel.hpp"

#include <string>
#include "ui/DrawingUtilities.hpp"

InfoPanel::InfoPanel(const sf::Font& font)
    : m_font(font) {
    // Default position/size - will be set by HomeScene
    setPosition(sf::Vector2f(950.f, 90.f));
    setSize(sf::Vector2f(250.f, 500.f));
    initializeTexts();
}

void InfoPanel::initializeTexts() {
    // Next Piece label
    m_nextLabel.setFont(m_font);
    m_nextLabel.setString("NEXT PIECE");
    m_nextLabel.setCharacterSize(Theme::Typography::FontSizes::BodySmall);
    m_nextLabel.setFillColor(Theme::Colors::TextGray());
    m_nextLabel.setPosition(m_position.x + Theme::Spacing::Medium, m_position.y + 20.f);

    // Score
    m_scoreLabel.setFont(m_font);
    m_scoreLabel.setString("SCORE");
    m_scoreLabel.setCharacterSize(Theme::Typography::FontSizes::Label);
    m_scoreLabel.setFillColor(Theme::Colors::TextGray());
    m_scoreLabel.setPosition(m_position.x + Theme::Spacing::Medium, m_position.y + 130.f);

    m_scoreValue.setFont(m_font);
    m_scoreValue.setString("12,450");
    m_scoreValue.setCharacterSize(Theme::Typography::FontSizes::BodyLarge);
    m_scoreValue.setFillColor(Theme::Colors::AccentYellow());
    m_scoreValue.setPosition(m_position.x + Theme::Spacing::Medium, m_position.y + 155.f);

    // Level
    m_levelLabel.setFont(m_font);
    m_levelLabel.setString("LEVEL");
    m_levelLabel.setCharacterSize(Theme::Typography::FontSizes::Label);
    m_levelLabel.setFillColor(Theme::Colors::TextGray());
    m_levelLabel.setPosition(m_position.x + Theme::Spacing::Medium, m_position.y + 210.f);

    m_levelValue.setFont(m_font);
    m_levelValue.setString("05");
    m_levelValue.setCharacterSize(Theme::Typography::FontSizes::BodyLarge);
    m_levelValue.setFillColor(Theme::Colors::PrimaryGreen());
    m_levelValue.setPosition(m_position.x + Theme::Spacing::Medium, m_position.y + 235.f);

    // Lines
    m_linesLabel.setFont(m_font);
    m_linesLabel.setString("LINES");
    m_linesLabel.setCharacterSize(Theme::Typography::FontSizes::Label);
    m_linesLabel.setFillColor(Theme::Colors::TextGray());
    m_linesLabel.setPosition(m_position.x + Theme::Spacing::Medium, m_position.y + 290.f);

    m_linesValue.setFont(m_font);
    m_linesValue.setString("42");
    m_linesValue.setCharacterSize(Theme::Typography::FontSizes::BodyLarge);
    m_linesValue.setFillColor(Theme::Colors::AccentYellow());
    m_linesValue.setPosition(m_position.x + Theme::Spacing::Medium, m_position.y + 315.f);

    // High Score
    m_highScoreLabel.setFont(m_font);
    m_highScoreLabel.setString("HIGH SCORE");
    m_highScoreLabel.setCharacterSize(Theme::Typography::FontSizes::Label);
    m_highScoreLabel.setFillColor(Theme::Colors::TextGray());
    m_highScoreLabel.setPosition(m_position.x + Theme::Spacing::Medium, m_position.y + 370.f);

    m_highScoreValue.setFont(m_font);
    m_highScoreValue.setString("98,750");
    m_highScoreValue.setCharacterSize(Theme::Typography::FontSizes::BodyLarge);
    m_highScoreValue.setFillColor(Theme::Colors::AccentPurple());
    m_highScoreValue.setPosition(m_position.x + Theme::Spacing::Medium, m_position.y + 395.f);
}

void InfoPanel::setScore(int score) {
    m_scoreValue.setString(std::to_string(score));
}

void InfoPanel::setLevel(int level) {
    std::string levelStr = (level < 10 ? "0" : "") + std::to_string(level);
    m_levelValue.setString(levelStr);
}

void InfoPanel::setLines(int lines) {
    m_linesValue.setString(std::to_string(lines));
}

void InfoPanel::setHighScore(int highScore) {
    m_highScoreValue.setString(std::to_string(highScore));
}

void InfoPanel::draw(sf::RenderTarget& target) {
    // Draw panel background
    DrawingUtilities::drawGlassPanel(
        target,
        m_position,
        m_size
    );

    // Draw Next Piece placeholder (small glass area)
    DrawingUtilities::drawRoundedRectangle(
        target,
        m_position + sf::Vector2f(Theme::Spacing::Medium, 45.f),
        sf::Vector2f(m_size.x - Theme::Spacing::Medium * 2, 70.f),
        Theme::Effects::CornerRadius,
        Theme::Colors::DarkBg(),
        Theme::Colors::GlassBorder(),
        1.f
    );

    // Draw "Next Piece" placeholder text
    sf::Text placeholder;
    placeholder.setFont(m_font);
    placeholder.setString("[ Next ]");
    placeholder.setCharacterSize(Theme::Typography::FontSizes::BodySmall);
    placeholder.setFillColor(Theme::Colors::TextDarkGray());
    placeholder.setPosition(m_position.x + Theme::Spacing::Medium + 30.f, m_position.y + 55.f);
    target.draw(placeholder);

    // Draw info
    target.draw(m_scoreLabel);
    target.draw(m_scoreValue);
    target.draw(m_levelLabel);
    target.draw(m_levelValue);
    target.draw(m_linesLabel);
    target.draw(m_linesValue);
    target.draw(m_highScoreLabel);
    target.draw(m_highScoreValue);
}
