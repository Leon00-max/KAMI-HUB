#include "scenes/HomeScene.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "ui/Theme.hpp"

HomeScene::HomeScene(ResourceManager& resources)
    : m_resources(resources) {
    initializeBackground();

    const sf::Font& font = m_resources.getFont("main");

    // Create UI components
    m_navBar = std::make_unique<NavigationBar>(font);
    m_gameSelection = std::make_unique<GameSelectionPanel>(font);
    m_gameBoard = std::make_unique<GlassPanel>(
        sf::Vector2f(300.f, 90.f),
        sf::Vector2f(Theme::Sizes::GameBoardWidth, Theme::Sizes::GameBoardHeight)
    );
    m_infoPanel = std::make_unique<InfoPanel>(font);
    m_actionBar = std::make_unique<ActionButtonBar>(font);
}

void HomeScene::initializeBackground() {
    // Try to load background image, fallback to generated texture
    const sf::Texture& texture = m_resources.getTexture("placeholder");
    m_background.setTexture(texture);
    m_background.setScale(1.6f, 1.6f);
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

