#include "scenes/HomeScene.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include "ui/Theme.hpp"

HomeScene::HomeScene(ResourceManager& resources)
    : m_resources(resources) {
    initializeBackground();

    const sf::Font& font = m_resources.getFont("main");

    // Create UI components
    m_navBar = std::make_unique<NavigationBar>(font);
    m_gameSelection = std::make_unique<GameSelectionPanel>(font);
    m_gameBoard = std::make_unique<GlassPanel>(
        sf::Vector2f((1280.f - Theme::Sizes::GameBoardWidth) / 2.f, 95.f),
        sf::Vector2f(Theme::Sizes::GameBoardWidth, Theme::Sizes::GameBoardHeight)
    );
    m_infoPanel = std::make_unique<InfoPanel>(font);
    m_actionBar = std::make_unique<ActionButtonBar>(font);
    // Nothing is running on the hub screen yet
    m_actionBar->setButtonEnabled(ActionButtonBar::ButtonType::Pause, false);
    m_actionBar->setButtonEnabled(ActionButtonBar::ButtonType::Restart, false);
}

void HomeScene::initializeBackground() {
    // Full-window diagonal gradient between the theme's background tones
    const sf::Color top = Theme::Colors::DarkBg();
    const sf::Color bottom = Theme::Colors::LightBg();
    m_background = sf::VertexArray(sf::Quads, 4);
    m_background[0] = sf::Vertex(sf::Vector2f(0.f, 0.f), top);
    m_background[1] = sf::Vertex(sf::Vector2f(1280.f, 0.f), top);
    m_background[2] = sf::Vertex(sf::Vector2f(1280.f, 720.f), bottom);
    m_background[3] = sf::Vertex(sf::Vector2f(0.f, 720.f), bottom);
}

void HomeScene::handleEvent(const sf::Event& event) {
    if (m_navBar) {
        m_navBar->handleEvent(event);
    }
    if (m_gameSelection) {
        m_gameSelection->handleEvent(event);
    }
    if (m_actionBar) {
        m_actionBar->handleEvent(event);
    }
}

void HomeScene::update(float deltaTime, const sf::Vector2f& pointerPosition) {
    if (m_navBar) {
        m_navBar->update(deltaTime, pointerPosition);
    }
    if (m_gameSelection) {
        m_gameSelection->update(deltaTime, pointerPosition);
    }
    if (m_actionBar) {
        m_actionBar->update(deltaTime, pointerPosition);
    }

    handleUIInteraction();
}

void HomeScene::handleUIInteraction() {
    // Handle game selection changes
    if (m_gameSelection) {
        int selectedGame = m_gameSelection->getSelectedGameIndex();
        // Future: Switch game logic based on selection
        // 0 = Tetris, 1 = Breakout
    }

    // Handle action buttons
    if (m_actionBar) {
        int clickedButton = m_actionBar->getClickedButtonIndex();
        // Future: Handle button actions
        // 0 = Play, 1 = Pause, 2 = Restart, 3 = Menu, 4 = Settings
    }
}

void HomeScene::draw(sf::RenderTarget& target) {
    // Draw background
    target.draw(m_background);

    // Draw main UI components in order
    if (m_gameBoard) {
        m_gameBoard->draw(target);
    }
    if (m_gameSelection) {
        m_gameSelection->draw(target);
    }
    if (m_infoPanel) {
        m_infoPanel->draw(target);
    }
    if (m_navBar) {
        m_navBar->draw(target);
    }
    if (m_actionBar) {
        m_actionBar->draw(target);
    }
}

