#include "ui/ActionButtonBar.hpp"

#include <algorithm>

namespace {
constexpr float MinButtonWidth = 94.f;
}

ActionButtonBar::ActionButtonBar(const ResourceManager& resources) {
    const struct {
        const char* label;
        const char* widestLabel; // Longest label the button can switch to
        AnimatedButton::ButtonStyle style;
        DrawingUtilities::Icon icon;
    } specs[] = {
        {"PLAY", "PLAY", AnimatedButton::ButtonStyle::Green, DrawingUtilities::Icon::Play},
        {"PAUSE", "RESUME", AnimatedButton::ButtonStyle::Orange, DrawingUtilities::Icon::Pause},
        {"RESTART", "RESTART", AnimatedButton::ButtonStyle::Red, DrawingUtilities::Icon::Restart},
        {"MENU", "MENU", AnimatedButton::ButtonStyle::Purple, DrawingUtilities::Icon::Menu},
        {"SETTINGS", "SETTINGS", AnimatedButton::ButtonStyle::Blue, DrawingUtilities::Icon::Gear},
    };

    const sf::Font& font = resources.getFont(Theme::Fonts::Bold);
    const float y = Theme::Layout::ActionY;
    const float height = Theme::Layout::ActionHeight;

    // Build at the origin to measure, then lay out centered
    std::vector<float> widths;
    for (const auto& spec : specs) {
        m_buttons.emplace_back(sf::Vector2f(0.f, y), sf::Vector2f(MinButtonWidth, height), spec.widestLabel, font, spec.style, spec.icon);
        widths.push_back(std::max(MinButtonWidth, m_buttons.back().preferredWidth()));
        m_buttons.back().setLabel(spec.label);
    }

    float total = Theme::Layout::ActionGap * (widths.size() - 1);
    for (float width : widths) {
        total += width;
    }

    float x = (Theme::Layout::WindowWidth - total) / 2.f;
    for (std::size_t i = 0; i < m_buttons.size(); ++i) {
        m_buttons[i].setPosition(sf::Vector2f(x, y));
        m_buttons[i].setSize(sf::Vector2f(widths[i], height));
        x += widths[i] + Theme::Layout::ActionGap;
    }

    setPosition(m_buttons.front().getPosition());
    setSize(sf::Vector2f(total, height));
}

void ActionButtonBar::setButtonEnabled(ButtonType type, bool enabled) {
    m_buttons[static_cast<std::size_t>(type)].setEnabled(enabled);
}

void ActionButtonBar::setButtonLabel(ButtonType type, const std::string& label) {
    m_buttons[static_cast<std::size_t>(type)].setLabel(label);
}

bool ActionButtonBar::handleEvent(const sf::Event& event) {
    m_clickedButtonIndex = -1;
    bool handled = false;
    for (std::size_t i = 0; i < m_buttons.size(); ++i) {
        if (m_buttons[i].handleEvent(event)) {
            m_clickedButtonIndex = static_cast<int>(i);
            handled = true;
        }
    }
    return handled;
}

void ActionButtonBar::update(float deltaTime, const sf::Vector2f& mousePosition) {
    for (auto& button : m_buttons) {
        button.update(deltaTime, mousePosition);
    }
}

void ActionButtonBar::draw(sf::RenderTarget& target) {
    for (auto& button : m_buttons) {
        button.draw(target);
    }
}
