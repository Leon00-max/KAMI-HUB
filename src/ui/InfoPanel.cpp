#include "ui/InfoPanel.hpp"

#include <algorithm>
#include <cstdio>
#include <string>

#include "ui/DrawingUtilities.hpp"

namespace {
constexpr float StatRowHeight = Theme::Layout::StatsHeight / 4.f;

std::string withThousandsSeparators(int value) {
    std::string digits = std::to_string(value < 0 ? -value : value);
    for (int i = static_cast<int>(digits.size()) - 3; i > 0; i -= 3) {
        digits.insert(static_cast<std::size_t>(i), ",");
    }
    return value < 0 ? "-" + digits : digits;
}

void centerText(sf::Text& text, float centerX, float centerY) {
    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    text.setPosition(centerX, centerY);
}
}

InfoPanel::InfoPanel(const ResourceManager& resources)
    : m_pauseButton(
          sf::Vector2f(Theme::Layout::SideX, Theme::Layout::SidePauseY),
          sf::Vector2f(Theme::Layout::SideWidth, Theme::Layout::SidePauseHeight),
          "PAUSE",
          resources.getFont(Theme::Fonts::Bold),
          AnimatedButton::ButtonStyle::Blue,
          DrawingUtilities::Icon::Pause
      ) {
    setPosition(sf::Vector2f(Theme::Layout::SideX, Theme::Layout::NextY));
    setSize(sf::Vector2f(Theme::Layout::SideWidth, Theme::Layout::SidePauseY + Theme::Layout::SidePauseHeight - Theme::Layout::NextY));

    const float centerX = Theme::Layout::SideX + Theme::Layout::SideWidth / 2.f;
    const sf::Font& labelFont = resources.getFont(Theme::Fonts::SemiBold);
    const sf::Font& valueFont = resources.getFont(Theme::Fonts::Bold);

    m_nextLabel = sf::Text("NEXT", labelFont, Theme::FontSizes::StatLabel);
    m_nextLabel.setFillColor(Theme::Colors::TextWhite());
    centerText(m_nextLabel, centerX, Theme::Layout::NextY + 18.f);

    const struct {
        const char* label;
        sf::Color color;
    } stats[] = {
        {"SCORE", Theme::Colors::GreenBright()},
        {"LEVEL", Theme::Colors::BlueBright()},
        {"LINES", Theme::Colors::Amber()},
        {"HIGH SCORE", Theme::Colors::PurpleBright()},
    };

    for (std::size_t i = 0; i < 4; ++i) {
        const float rowTop = Theme::Layout::StatsY + i * StatRowHeight;
        Stat stat{sf::Text(stats[i].label, labelFont, Theme::FontSizes::StatLabel),
                  sf::Text("", valueFont, Theme::FontSizes::StatValue)};
        stat.label.setFillColor(Theme::Colors::TextWhite());
        centerText(stat.label, centerX, rowTop + 19.f);
        stat.value.setFillColor(stats[i].color);
        stat.value.setPosition(centerX, rowTop + 42.f);
        m_stats.push_back(stat);
    }

    setScore(0);
    setLevel(1);
    setLines(0);
    setHighScore(0);
}

void InfoPanel::setStatValue(Stat& stat, const std::string& value) {
    stat.value.setString(value);
    centerText(stat.value, stat.value.getPosition().x, stat.value.getPosition().y);
}

void InfoPanel::setScore(int score) { setStatValue(m_stats[0], withThousandsSeparators(score)); }

void InfoPanel::setLevel(int level) {
    char buffer[8];
    std::snprintf(buffer, sizeof(buffer), "%02d", level);
    setStatValue(m_stats[1], buffer);
}

void InfoPanel::setLines(int lines) { setStatValue(m_stats[2], std::to_string(lines)); }

void InfoPanel::setHighScore(int highScore) { setStatValue(m_stats[3], withThousandsSeparators(highScore)); }

void InfoPanel::setNextPiece(const std::vector<sf::Vector2i>& cells, const sf::Color& color) {
    m_nextCells = cells;
    m_nextColor = color;
}

bool InfoPanel::handleEvent(const sf::Event& event) {
    m_pauseClicked = m_pauseButton.handleEvent(event) || m_pauseClicked;
    return m_pauseClicked;
}

void InfoPanel::update(float deltaTime, const sf::Vector2f& mousePosition) {
    m_pauseClicked = false;
    m_pauseButton.update(deltaTime, mousePosition);
}

void InfoPanel::draw(sf::RenderTarget& target) {
    const float x = Theme::Layout::SideX;
    const float width = Theme::Layout::SideWidth;

    // Next piece box
    DrawingUtilities::drawRoundedRectangle(
        target, sf::Vector2f(x, Theme::Layout::NextY), sf::Vector2f(width, Theme::Layout::NextHeight),
        Theme::Effects::CornerRadius, Theme::Colors::PanelBg(), Theme::Colors::PanelBorder(), 1.f
    );
    target.draw(m_nextLabel);

    if (!m_nextCells.empty()) {
        int minX = m_nextCells[0].x, maxX = minX, minY = m_nextCells[0].y, maxY = minY;
        for (const auto& cell : m_nextCells) {
            minX = std::min(minX, cell.x);
            maxX = std::max(maxX, cell.x);
            minY = std::min(minY, cell.y);
            maxY = std::max(maxY, cell.y);
        }
        const float cell = Theme::Layout::CellSize;
        const sf::Vector2f pieceSize((maxX - minX + 1) * cell, (maxY - minY + 1) * cell);
        const sf::Vector2f origin(
            x + (width - pieceSize.x) / 2.f,
            Theme::Layout::NextY + 34.f + (Theme::Layout::NextHeight - 34.f - pieceSize.y) / 2.f
        );
        for (const auto& c : m_nextCells) {
            DrawingUtilities::drawBlock(
                target, origin + sf::Vector2f((c.x - minX) * cell, (c.y - minY) * cell), cell, m_nextColor
            );
        }
    }

    // Stats box with dividers between rows
    DrawingUtilities::drawRoundedRectangle(
        target, sf::Vector2f(x, Theme::Layout::StatsY), sf::Vector2f(width, Theme::Layout::StatsHeight),
        Theme::Effects::CornerRadius, Theme::Colors::PanelBg(), Theme::Colors::PanelBorder(), 1.f
    );
    for (int i = 1; i < 4; ++i) {
        const float y = Theme::Layout::StatsY + i * StatRowHeight;
        DrawingUtilities::drawLine(
            target, sf::Vector2f(x + 8.f, y), sf::Vector2f(x + width - 8.f, y), 1.f, Theme::Colors::Divider()
        );
    }
    for (const auto& stat : m_stats) {
        target.draw(stat.label);
        target.draw(stat.value);
    }

    m_pauseButton.draw(target);
}
