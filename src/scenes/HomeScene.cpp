#include "scenes/HomeScene.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "ui/Theme.hpp"

HomeScene::HomeScene(ResourceManager& resources, bool preview)
    : m_resources(resources) {
    initializeBackground();

    m_navBar = std::make_unique<NavigationBar>(m_resources);
    m_gameSelection = std::make_unique<GameSelectionPanel>(m_resources);
    m_stage = std::make_unique<GlassPanel>(
        sf::Vector2f(Theme::Layout::StageX, Theme::Layout::StageY),
        sf::Vector2f(Theme::Layout::StageWidth, Theme::Layout::StageHeight)
    );
    m_board = std::make_unique<GameBoard>(
        sf::Vector2f(Theme::Layout::BoardX, Theme::Layout::BoardY),
        Theme::Layout::BoardColumns,
        Theme::Layout::BoardRows,
        Theme::Layout::CellSize
    );
    m_infoPanel = std::make_unique<InfoPanel>(m_resources);
    m_actionBar = std::make_unique<ActionButtonBar>(m_resources);

    if (preview) {
        loadPreviewState();
    }
    setGameRunning(preview);
}

void HomeScene::initializeBackground() {
    const sf::Texture& texture = m_resources.getTexture("background");
    m_background.setTexture(texture, true);
    const sf::Vector2u size = texture.getSize();
    if (size.x > 0 && size.y > 0) {
        m_background.setScale(
            Theme::Layout::WindowWidth / static_cast<float>(size.x),
            Theme::Layout::WindowHeight / static_cast<float>(size.y)
        );
    }
}

void HomeScene::setGameRunning(bool running) {
    m_actionBar->setButtonEnabled(ActionButtonBar::ButtonType::Pause, running);
    m_actionBar->setButtonEnabled(ActionButtonBar::ButtonType::Restart, running);
    m_infoPanel->setPauseEnabled(running);
}

void HomeScene::loadPreviewState() {
    const sf::Color O = Theme::Colors::Orange();
    const sf::Color Y = Theme::Colors::Yellow();
    const sf::Color R = Theme::Colors::Red();
    const sf::Color B = Theme::Colors::Blue();
    const sf::Color G = sf::Color(0x6F, 0xBF, 0x2C);
    const sf::Color P = Theme::Colors::Purple();
    const sf::Color _ = sf::Color::Transparent;

    // Rows 10-19 of the concept art; row 15 is mid-clear
    const sf::Color stack[10][10] = {
        {O, _, _, _, _, _, _, _, G, _},
        {O, _, O, O, _, _, _, _, B, B},
        {O, O, O, Y, _, _, _, R, R, B},
        {R, R, O, Y, _, _, Y, R, P, B},
        {R, R, R, B, _, Y, Y, Y, Y, B},
        {B, G, B, G, G, R, R, P, B, B},
        {B, _, B, _, G, R, R, P, B, B},
        {B, B, B, _, G, R, R, P, P, B},
        {B, Y, Y, R, G, R, B, B, B, B},
        {B, Y, Y, R, G, G, B, B, B, B},
    };
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            m_board->setCell(col, row + 10, stack[row][col]);
        }
    }
    m_board->setFlashRow(15);

    // Falling T piece
    for (int col = 3; col <= 5; ++col) {
        m_board->setCell(col, 4, B);
    }
    m_board->setCell(4, 5, B);

    m_infoPanel->setNextPiece({{1, 0}, {0, 1}, {1, 1}, {2, 1}}, P);
    m_infoPanel->setScore(12450);
    m_infoPanel->setLevel(5);
    m_infoPanel->setLines(42);
    m_infoPanel->setHighScore(98750);
}

void HomeScene::handleEvent(const sf::Event& event) {
    m_navBar->handleEvent(event);
    m_gameSelection->handleEvent(event);
    m_infoPanel->handleEvent(event);
    m_actionBar->handleEvent(event);
}

void HomeScene::update(float deltaTime, const sf::Vector2f& pointerPosition) {
    m_navBar->update(deltaTime, pointerPosition);
    m_gameSelection->update(deltaTime, pointerPosition);
    m_infoPanel->update(deltaTime, pointerPosition);
    m_actionBar->update(deltaTime, pointerPosition);
    // Button actions are wired up with the scene stack (next milestone)
}

void HomeScene::draw(sf::RenderTarget& target) {
    target.draw(m_background);
    m_stage->draw(target);
    m_board->draw(target);
    m_infoPanel->draw(target);
    m_gameSelection->draw(target);
    m_actionBar->draw(target);
    m_navBar->draw(target);
}
