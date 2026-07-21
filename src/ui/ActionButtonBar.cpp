#include "ui/ActionButtonBar.hpp"

#include "ui/DrawingUtilities.hpp"

ActionButtonBar::ActionButtonBar(const sf::Font& font)
    : m_font(font) {
    setPosition(sf::Vector2f(0.f, 620.f));
    setSize(sf::Vector2f(1280.f, 100.f));
    initializeButtons();
}

void ActionButtonBar::setWindowWidth(float width) {
    m_windowWidth = width;
    setSize(sf::Vector2f(width, 100.f));
    initializeButtons();
}

void ActionButtonBar::initializeButtons() {
    m_buttons.clear();
    m_buttonTypes.clear();

    const std::vector<std::string> labels = {"PLAY", "PAUSE", "RESTART", "MENU", "SETTINGS"};
    const std::vector<ButtonType> types = {ButtonType::Play, ButtonType::Pause, ButtonType::Restart, ButtonType::Menu, ButtonType::Settings};
    const std::vector<AnimatedButton::ButtonStyle> styles = {
        AnimatedButton::ButtonStyle::Primary,  // Play - Green
        AnimatedButton::ButtonStyle::Primary,  // Pause - Yellow
        AnimatedButton::ButtonStyle::Primary,  // Restart - Red
        AnimatedButton::ButtonStyle::Secondary, // Menu - Blue
        AnimatedButton::ButtonStyle::Tertiary   // Settings - Dark
    };

    const float centerX = m_windowWidth / 2.f;
    const float totalWidth = 5 * Theme::Sizes::ButtonWidth + 4 * Theme::Spacing::Large;
    const float startX = (centerX - totalWidth / 2.f);
    const float buttonY = m_position.y + 20.f;

    for (size_t i = 0; i < labels.size(); ++i) {
        m_buttons.emplace_back(
            sf::Vector2f(startX + i * (Theme::Sizes::ButtonWidth + Theme::Spacing::Large), buttonY),
            sf::Vector2f(Theme::Sizes::ButtonWidth, Theme::Sizes::ButtonHeight),
            labels[i],
            m_font,
            styles[i]
        );
        m_buttonTypes.push_back(types[i]);
    }
}

bool ActionButtonBar::handleEvent(const sf::Event& event) {
    m_clickedButtonIndex = -1;
    bool handled = false;

    for (size_t i = 0; i < m_buttons.size(); ++i) {
        if (m_buttons[i].handleEvent(event)) {
            m_clickedButtonIndex = static_cast<int>(i);
            handled = true;
        }
    }

    return handled;
}

void ActionButtonBar::update(float deltaTime, const sf::Vector2f& mousePosition) {
    for (auto& btn : m_buttons) {
        btn.update(deltaTime, mousePosition);
    }
}

void ActionButtonBar::draw(sf::RenderTarget& target) {
    // Draw background
    DrawingUtilities::drawRoundedRectangle(
        target,
        m_position,
        sf::Vector2f(m_windowWidth, m_size.y),
        0.f,
        Theme::Colors::MediumBg(),
        Theme::Colors::GlassBorder(),
        1.f
    );

    // Draw buttons
    for (auto& btn : m_buttons) {
        btn.draw(target);
    }
}
