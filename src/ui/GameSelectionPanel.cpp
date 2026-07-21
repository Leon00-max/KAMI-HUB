#include "ui/GameSelectionPanel.hpp"

#include "ui/DrawingUtilities.hpp"

GameSelectionPanel::GameSelectionPanel(const sf::Font& font)
    : m_font(font) {
    setPosition(sf::Vector2f(20.f, 90.f));
    setSize(sf::Vector2f(Theme::Sizes::SidePanelWidth, Theme::Sizes::SidePanelHeight));
    initializeGameCards();
    m_gameCards[0].setSelected(true); // Default to Tetris
}

void GameSelectionPanel::initializeGameCards() {
    m_gameCards.clear();

    // Tetris card
    m_gameCards.emplace_back(
        m_position + sf::Vector2f(Theme::Spacing::Medium, Theme::Spacing::Large),
        sf::Vector2f(Theme::Sizes::GameCardWidth, Theme::Sizes::GameCardHeight),
        "TETRIS",
        "🧱",
        m_font
    );

    // Breakout card
    m_gameCards.emplace_back(
        m_position + sf::Vector2f(Theme::Spacing::Medium, Theme::Spacing::Large + Theme::Sizes::GameCardHeight + Theme::Spacing::Large),
        sf::Vector2f(Theme::Sizes::GameCardWidth, Theme::Sizes::GameCardHeight),
        "BREAKOUT",
        "🎾",
        m_font
    );
}

bool GameSelectionPanel::handleEvent(const sf::Event& event) {
    bool handled = false;

    for (size_t i = 0; i < m_gameCards.size(); ++i) {
        if (m_gameCards[i].handleEvent(event)) {
            if (m_gameCards[i].wasClickedThisFrame()) {
                // Deselect all others
                for (size_t j = 0; j < m_gameCards.size(); ++j) {
                    if (i != j) {
                        m_gameCards[j].setSelected(false);
                    }
                }
                m_selectedGameIndex = static_cast<int>(i);
            }
            handled = true;
        }
    }

    return handled;
}

void GameSelectionPanel::update(float deltaTime, const sf::Vector2f& mousePosition) {
    for (auto& card : m_gameCards) {
        card.update(deltaTime, mousePosition);
    }
}

void GameSelectionPanel::draw(sf::RenderTarget& target) {
    // Draw panel background
    DrawingUtilities::drawGlassPanel(
        target,
        m_position,
        m_size
    );

    // Draw game mode title
    sf::Text title;
    title.setFont(m_font);
    title.setString("GAME MODE");
    title.setCharacterSize(Theme::Typography::FontSizes::BodySmall);
    title.setFillColor(Theme::Colors::TextGray);
    title.setPosition(m_position.x + Theme::Spacing::Medium, m_position.y + Theme::Spacing::Small);
    target.draw(title);

    // Draw game cards
    for (auto& card : m_gameCards) {
        card.draw(target);
    }
}
