#include "ui/GameSelectionPanel.hpp"

#include "ui/DrawingUtilities.hpp"

namespace {
constexpr float CardInset = 10.f;
constexpr float CardHeight = 66.f;
constexpr float FirstCardOffset = 48.f;
constexpr float CardGap = 10.f;
}

GameSelectionPanel::GameSelectionPanel(const ResourceManager& resources) {
    setPosition(sf::Vector2f(Theme::Layout::ModePanelX, Theme::Layout::ModePanelY));
    setSize(sf::Vector2f(Theme::Layout::ModePanelWidth, Theme::Layout::ModePanelHeight));

    m_title.setFont(resources.getFont(Theme::Fonts::SemiBold));
    m_title.setString("GAME MODE");
    m_title.setCharacterSize(Theme::FontSizes::PanelTitle);
    m_title.setFillColor(Theme::Colors::TextWhite());
    const sf::FloatRect bounds = m_title.getLocalBounds();
    m_title.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    m_title.setPosition(m_position.x + m_size.x / 2.f, m_position.y + 27.f);

    const sf::Font& cardFont = resources.getFont(Theme::Fonts::Bold);
    const sf::Vector2f cardSize(m_size.x - CardInset * 2.f, CardHeight);
    m_gameCards.emplace_back(
        m_position + sf::Vector2f(CardInset, FirstCardOffset),
        cardSize, "TETRIS", DrawingUtilities::Icon::Blocks, Theme::Colors::Green(), cardFont
    );
    m_gameCards.emplace_back(
        m_position + sf::Vector2f(CardInset, FirstCardOffset + CardHeight + CardGap),
        cardSize, "BREAKOUT", DrawingUtilities::Icon::Paddle, Theme::Colors::Blue(), cardFont
    );
    m_gameCards[0].setSelected(true);
}

bool GameSelectionPanel::handleEvent(const sf::Event& event) {
    for (std::size_t i = 0; i < m_gameCards.size(); ++i) {
        if (m_gameCards[i].handleEvent(event)) {
            m_selectedGameIndex = static_cast<int>(i);
            for (std::size_t j = 0; j < m_gameCards.size(); ++j) {
                m_gameCards[j].setSelected(i == j);
            }
            return true;
        }
    }
    return false;
}

void GameSelectionPanel::update(float deltaTime, const sf::Vector2f& mousePosition) {
    for (auto& card : m_gameCards) {
        card.update(deltaTime, mousePosition);
    }
}

void GameSelectionPanel::draw(sf::RenderTarget& target) {
    DrawingUtilities::drawGlassPanel(target, m_position, m_size);
    target.draw(m_title);
    for (auto& card : m_gameCards) {
        card.draw(target);
    }
}
