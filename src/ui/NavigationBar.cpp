#include "ui/NavigationBar.hpp"

#include <SFML/Graphics/CircleShape.hpp>

namespace {
constexpr float NavIconSize = 18.f;
constexpr float NavIconGap = 10.f;
constexpr float NavItemGap = 46.f;
constexpr float NavStartX = 452.f;
constexpr float ControlSize = 38.f;
constexpr float ControlSpacing = 50.f;
}

NavigationBar::NavigationBar(const ResourceManager& resources) {
    setPosition(sf::Vector2f(0.f, 0.f));
    setSize(sf::Vector2f(Theme::Layout::WindowWidth, Theme::Layout::NavBarHeight));

    m_title.setFont(resources.getFont(Theme::Fonts::Display));
    m_title.setString("ARCADE HUB");
    m_title.setCharacterSize(Theme::FontSizes::Logo);
    m_title.setFillColor(Theme::Colors::TextWhite());
    m_title.setPosition(92.f, 22.f);

    m_subtitle.setFont(resources.getFont(Theme::Fonts::Bold));
    m_subtitle.setString("TETRIS & BREAKOUT");
    m_subtitle.setCharacterSize(Theme::FontSizes::LogoSub);
    m_subtitle.setLetterSpacing(1.3f);
    m_subtitle.setFillColor(Theme::Colors::GreenBright());
    // Centered under the title
    const float titleCenter = m_title.getPosition().x + m_title.getLocalBounds().width / 2.f;
    m_subtitle.setPosition(titleCenter - m_subtitle.getLocalBounds().width / 2.f, 50.f);

    const struct {
        DrawingUtilities::Icon icon;
        const char* label;
    } items[] = {
        {DrawingUtilities::Icon::Home, "HOME"},
        {DrawingUtilities::Icon::Gamepad, "GAMES"},
        {DrawingUtilities::Icon::Stats, "STATS"},
        {DrawingUtilities::Icon::Trophy, "ACHIEVEMENTS"},
    };

    float x = NavStartX;
    const float centerY = Theme::Layout::NavBarHeight / 2.f + 1.f;
    for (const auto& item : items) {
        NavItem nav{item.icon, sf::Text(item.label, resources.getFont(Theme::Fonts::SemiBold), Theme::FontSizes::NavItem), {}};
        const sf::FloatRect bounds = nav.label.getLocalBounds();
        nav.label.setOrigin(bounds.left, bounds.top + bounds.height / 2.f);
        nav.label.setPosition(x + NavIconSize + NavIconGap, centerY);
        const float right = x + NavIconSize + NavIconGap + bounds.width;
        nav.hitBox = sf::FloatRect(x - 8.f, 14.f, right - x + 16.f, Theme::Layout::NavBarHeight - 28.f);
        m_navItems.push_back(nav);
        x = right + NavItemGap;
    }

    const DrawingUtilities::Icon controls[] = {
        DrawingUtilities::Icon::Gear, DrawingUtilities::Icon::Music, DrawingUtilities::Icon::Speaker
    };
    float controlX = Theme::Layout::WindowWidth - 32.f - ControlSize - ControlSpacing * 2.f;
    for (const auto icon : controls) {
        m_controlButtons.emplace_back(
            sf::Vector2f(controlX, centerY - ControlSize / 2.f),
            sf::Vector2f(ControlSize, ControlSize),
            "",
            resources.getFont(Theme::Fonts::Bold),
            AnimatedButton::ButtonStyle::Round,
            icon
        );
        controlX += ControlSpacing;
    }
}

bool NavigationBar::handleEvent(const sf::Event& event) {
    bool handled = false;
    for (auto& button : m_controlButtons) {
        handled = button.handleEvent(event) || handled;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f point(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        for (std::size_t i = 0; i < m_navItems.size(); ++i) {
            if (m_navItems[i].hitBox.contains(point)) {
                m_clickedNavItem = static_cast<int>(i);
                handled = true;
            }
        }
    }
    return handled;
}

void NavigationBar::update(float deltaTime, const sf::Vector2f& mousePosition) {
    m_clickedNavItem = -1;
    for (auto& button : m_controlButtons) {
        button.update(deltaTime, mousePosition);
    }
    for (auto& item : m_navItems) {
        item.hovered = item.hitBox.contains(mousePosition);
    }
}

void NavigationBar::drawLogo(sf::RenderTarget& target) {
    const sf::Vector2f center(50.f, 41.f);
    const sf::Color green = Theme::Colors::GreenBright();

    // Halo behind the controller
    for (int i = 4; i >= 1; --i) {
        sf::CircleShape halo(16.f + i * 5.f);
        halo.setOrigin(halo.getRadius(), halo.getRadius());
        halo.setPosition(center);
        halo.setFillColor(Theme::withAlpha(green, static_cast<sf::Uint8>(10 + (4 - i) * 6)));
        target.draw(halo);
    }
    DrawingUtilities::drawIcon(target, DrawingUtilities::Icon::Gamepad, center, 52.f, green);

    // Leaf sprouting from the top of the controller
    sf::CircleShape leaf(7.f, 24);
    leaf.setOrigin(7.f, 7.f);
    leaf.setScale(1.f, 0.5f);
    leaf.setRotation(-40.f);
    leaf.setPosition(center + sf::Vector2f(9.f, -20.f));
    leaf.setFillColor(green);
    target.draw(leaf);
    DrawingUtilities::drawLine(target, center + sf::Vector2f(2.f, -13.f), center + sf::Vector2f(5.f, -18.f), 2.f, green);

    target.draw(m_title);
    target.draw(m_subtitle);
}

void NavigationBar::draw(sf::RenderTarget& target) {
    DrawingUtilities::drawGradientRoundedRectangle(
        target, m_position, m_size, 0.f,
        Theme::Colors::NavBg(), Theme::withAlpha(Theme::Colors::NavBg(), 0xD0)
    );
    DrawingUtilities::drawLine(
        target,
        sf::Vector2f(0.f, m_size.y - 0.5f),
        sf::Vector2f(m_size.x, m_size.y - 0.5f),
        1.f,
        Theme::withAlpha(Theme::Colors::BlueBright(), 70)
    );

    drawLogo(target);

    for (std::size_t i = 0; i < m_navItems.size(); ++i) {
        NavItem& item = m_navItems[i];
        const bool active = static_cast<int>(i) == m_activeNavItem;
        const sf::Color color = item.hovered ? Theme::Colors::GreenBright() : Theme::Colors::TextWhite();
        const sf::Vector2f labelPos = item.label.getPosition();
        DrawingUtilities::drawIcon(
            target,
            item.icon,
            sf::Vector2f(labelPos.x - NavIconGap - NavIconSize / 2.f, labelPos.y),
            NavIconSize,
            color
        );
        item.label.setFillColor(color);
        target.draw(item.label);

        if (active) {
            const float width = item.label.getLocalBounds().width;
            DrawingUtilities::drawRoundedRectangle(
                target,
                sf::Vector2f(labelPos.x, labelPos.y + 14.f),
                sf::Vector2f(width, 3.f),
                1.5f,
                Theme::Colors::GreenBright()
            );
        }
    }

    for (auto& button : m_controlButtons) {
        button.draw(target);
    }
}
