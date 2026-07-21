#include "ui/NavigationBar.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include "ui/DrawingUtilities.hpp"

NavigationBar::NavigationBar(const sf::Font& font)
    : m_font(font) {
    setPosition(sf::Vector2f(0.f, 0.f));
    setSize(sf::Vector2f(1280.f, Theme::Sizes::NavBarHeight));
    initializeNavItems();
    initializeControlButtons();
}

void NavigationBar::setWindowWidth(float width) {
    m_windowWidth = width;
    setSize(sf::Vector2f(width, Theme::Sizes::NavBarHeight));
    initializeNavItems();
    initializeControlButtons();
}

void NavigationBar::initializeNavItems() {
    m_navItems.clear();

    const std::vector<std::string> navLabels = {"HOME", "GAMES", "STATISTICS", "ACHIEVEMENTS"};
    const float startX = 200.f; // After logo
    const float spacing = 120.f;

    for (size_t i = 0; i < navLabels.size(); ++i) {
        sf::Text item;
        item.setFont(m_font);
        item.setString(navLabels[i]);
        item.setCharacterSize(Theme::Typography::FontSizes::NavItem);
        item.setFillColor(Theme::Colors::TextGray);
        item.setPosition(startX + i * spacing, Theme::Sizes::NavBarHeight / 2.f - 10.f);
        m_navItems.push_back(item);
    }
}

void NavigationBar::initializeControlButtons() {
    m_controlButtons.clear();

    const float buttonSize = 40.f;
    const float startX = m_windowWidth - 200.f;
    const float spacing = 60.f;

    // Settings button
    m_controlButtons.emplace_back(
        sf::Vector2f(startX, Theme::Sizes::NavBarHeight / 2.f - buttonSize / 2.f),
        sf::Vector2f(buttonSize, buttonSize),
        "⚙",
        m_font,
        AnimatedButton::ButtonStyle::Tertiary
    );

    // Music button
    m_controlButtons.emplace_back(
        sf::Vector2f(startX + spacing, Theme::Sizes::NavBarHeight / 2.f - buttonSize / 2.f),
        sf::Vector2f(buttonSize, buttonSize),
        "♪",
        m_font,
        AnimatedButton::ButtonStyle::Tertiary
    );

    // Sound button
    m_controlButtons.emplace_back(
        sf::Vector2f(startX + spacing * 2, Theme::Sizes::NavBarHeight / 2.f - buttonSize / 2.f),
        sf::Vector2f(buttonSize, buttonSize),
        "🔊",
        m_font,
        AnimatedButton::ButtonStyle::Tertiary
    );
}

bool NavigationBar::handleEvent(const sf::Event& event) {
    bool handled = false;

    // Check control buttons
    for (auto& btn : m_controlButtons) {
        if (btn.handleEvent(event)) {
            handled = true;
        }
    }

    // Check nav items
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f clickPos(static_cast<float>(event.mouseButton.x),
                            static_cast<float>(event.mouseButton.y));

        for (size_t i = 0; i < m_navItems.size(); ++i) {
            auto bounds = m_navItems[i].getGlobalBounds();
            bounds.width += 80.f; // Make clickable area wider
            bounds.height += 20.f;

            if (bounds.contains(clickPos)) {
                m_clickedNavItem = static_cast<int>(i);
                handled = true;
                break;
            }
        }
    }

    return handled;
}

void NavigationBar::update(float deltaTime, const sf::Vector2f& mousePosition) {
    m_clickedNavItem = -1;

    for (auto& btn : m_controlButtons) {
        btn.update(deltaTime, mousePosition);
    }

    // Update nav item hover effects
    for (auto& item : m_navItems) {
        auto bounds = item.getGlobalBounds();
        bounds.width += 80.f;
        bounds.height += 20.f;

        if (bounds.contains(mousePosition)) {
            item.setFillColor(Theme::Colors::PrimaryGreen());
        } else {
            item.setFillColor(Theme::Colors::TextGray());
        }
    }
}

void NavigationBar::draw(sf::RenderTarget& target) {
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

    // Draw logo (placeholder "ARCADE HUB")
    sf::Text logo;
    logo.setFont(m_font);
    logo.setString("ARCADE HUB");
    logo.setCharacterSize(24);
    logo.setFillColor(Theme::Colors::PrimaryGreen());
    logo.setPosition(20.f, Theme::Sizes::NavBarHeight / 2.f - 12.f);
    target.draw(logo);

    // Draw nav items
    for (const auto& item : m_navItems) {
        target.draw(item);
    }

    // Draw control buttons
    for (auto& btn : m_controlButtons) {
        btn.draw(target);
    }
}
