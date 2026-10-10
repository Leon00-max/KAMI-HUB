#include "scenes/HomeScene.hpp"

#include <algorithm>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "ui/DrawingUtilities.hpp"
#include "ui/Theme.hpp"

namespace {

using tetris::Game;
using tetris::PieceType;

constexpr int TetrisIndex = 0;
constexpr float ToastDuration = 2.2f;
constexpr float FallSmoothing = 22.f;  // Higher = snappier
constexpr float SlideSmoothing = 32.f;

sf::Color pieceColor(PieceType type) {
    switch (type) {
        case PieceType::I: return Theme::Colors::BlueBright();
        case PieceType::O: return Theme::Colors::Yellow();
        case PieceType::T: return Theme::Colors::Purple();
        case PieceType::S: return sf::Color(0x6F, 0xBF, 0x2C);
        case PieceType::Z: return Theme::Colors::Red();
        case PieceType::J: return Theme::Colors::Blue();
        case PieceType::L: return Theme::Colors::Orange();
    }
    return Theme::Colors::TextGray();
}

sf::Color cellColor(std::uint8_t value) {
    return value == 0 ? sf::Color::Transparent : pieceColor(static_cast<PieceType>(value - 1));
}

void centerOn(sf::Text& text, const sf::Vector2f& center) {
    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    text.setPosition(center);
}

float approach(float current, float target, float rate, float deltaTime) {
    return current + (target - current) * std::min(1.f, rate * deltaTime);
}

} // namespace

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

    m_overlayTitle.setFont(m_resources.getFont(Theme::Fonts::Display));
    m_overlayTitle.setCharacterSize(24);
    m_overlayTitle.setFillColor(Theme::Colors::TextWhite());
    m_overlayBody.setFont(m_resources.getFont(Theme::Fonts::SemiBold));
    m_overlayBody.setCharacterSize(14);
    m_overlayBody.setLineSpacing(1.35f);
    m_overlayBody.setFillColor(Theme::Colors::TextGray());
    m_toastText.setFont(m_resources.getFont(Theme::Fonts::Bold));
    m_toastText.setCharacterSize(14);
    m_toastText.setFillColor(Theme::Colors::TextWhite());

    if (preview) {
        loadPreviewState();
    }
    syncControls();
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

void HomeScene::loadPreviewState() {
    m_showingPreview = true;

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
    m_board->setClearing({15}, 0.3f);
    m_board->setActivePiece({{3.f, 4.f}, {4.f, 4.f}, {5.f, 4.f}, {4.f, 5.f}}, B);

    m_infoPanel->setNextPiece({{1, 0}, {0, 1}, {1, 1}, {2, 1}}, P);
    m_infoPanel->setScore(12450);
    m_infoPanel->setLevel(5);
    m_infoPanel->setLines(42);
    m_infoPanel->setHighScore(98750);
}

bool HomeScene::gameActive() const {
    const auto state = m_game.state();
    return state == Game::State::Playing || state == Game::State::Clearing || state == Game::State::Paused;
}

void HomeScene::onPlay() {
    if (m_gameSelection->getSelectedGameIndex() != TetrisIndex) {
        showToast("BREAKOUT IS COMING SOON");
        return;
    }
    switch (m_game.state()) {
        case Game::State::Paused:
            m_game.togglePause();
            break;
        case Game::State::Ready:
        case Game::State::GameOver:
            m_showingPreview = false;
            m_game.start();
            break;
        default:
            break;
    }
}

void HomeScene::onPauseToggle() {
    m_game.togglePause();
}

void HomeScene::onRestart() {
    m_showingPreview = false;
    m_game.start();
}

void HomeScene::showToast(const std::string& message) {
    m_toastText.setString(message);
    m_toastTimer = ToastDuration;
}

void HomeScene::handleKey(const sf::Event::KeyEvent& key, bool pressed) {
    using Key = sf::Keyboard::Key;

    if (key.code == Key::Down || key.code == Key::S) {
        m_game.setSoftDrop(pressed);
        return;
    }
    if (!pressed) {
        return;
    }

    switch (key.code) {
        case Key::Left:
        case Key::A:
            m_game.moveLeft();
            break;
        case Key::Right:
        case Key::D:
            m_game.moveRight();
            break;
        case Key::Up:
        case Key::W:
        case Key::X:
            m_game.rotateClockwise();
            break;
        case Key::Z:
        case Key::LControl:
            m_game.rotateCounterClockwise();
            break;
        case Key::Space:
            m_game.hardDrop();
            break;
        case Key::P:
        case Key::Escape:
            m_game.togglePause();
            break;
        case Key::Enter:
            if (!gameActive()) {
                onPlay();
            }
            break;
        default:
            break;
    }
}

void HomeScene::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
        handleKey(event.key, event.type == sf::Event::KeyPressed);
        return;
    }
    if (event.type == sf::Event::LostFocus && m_game.state() != Game::State::Paused && gameActive()) {
        m_game.togglePause();
        m_game.setSoftDrop(false);
        return;
    }

    if (m_navBar->handleEvent(event)) {
        const int navItem = m_navBar->getClickedNavItem();
        if (navItem > 0 || m_navBar->getClickedControl() >= 0) {
            showToast("COMING SOON");
        }
    }

    m_gameSelection->handleEvent(event);

    if (m_infoPanel->handleEvent(event)) {
        onPauseToggle();
    }

    if (m_actionBar->handleEvent(event)) {
        switch (static_cast<ActionButtonBar::ButtonType>(m_actionBar->getClickedButtonIndex())) {
            case ActionButtonBar::ButtonType::Play:
                onPlay();
                break;
            case ActionButtonBar::ButtonType::Pause:
                onPauseToggle();
                break;
            case ActionButtonBar::ButtonType::Restart:
                onRestart();
                break;
            case ActionButtonBar::ButtonType::Menu:
            case ActionButtonBar::ButtonType::Settings:
                showToast("COMING SOON");
                break;
        }
    }
}

void HomeScene::update(float deltaTime, const sf::Vector2f& pointerPosition) {
    m_navBar->update(deltaTime, pointerPosition);
    m_gameSelection->update(deltaTime, pointerPosition);
    m_infoPanel->update(deltaTime, pointerPosition);
    m_actionBar->update(deltaTime, pointerPosition);

    m_game.update(deltaTime);
    m_highScore = std::max(m_highScore, m_game.score());
    m_toastTimer = std::max(0.f, m_toastTimer - deltaTime);

    if (!m_showingPreview) {
        syncGameView(deltaTime);
    }
    syncControls();
}

void HomeScene::syncGameView(float deltaTime) {
    m_board->clear();
    for (int row = 0; row < Game::VisibleHeight; ++row) {
        for (int col = 0; col < Game::Width; ++col) {
            m_board->setCell(col, row, cellColor(m_game.cellAt(col, row + Game::HiddenRows)));
        }
    }

    const auto state = m_game.state();
    const bool clearing = state == Game::State::Clearing
        || (state == Game::State::Paused && !m_game.clearingRows().empty());
    if (clearing) {
        std::vector<int> rows;
        for (int row : m_game.clearingRows()) {
            rows.push_back(row - Game::HiddenRows);
        }
        m_board->setClearing(rows, m_game.clearProgress());
    }

    if (state == Game::State::Playing || (state == Game::State::Paused && !clearing)) {
        const auto& piece = m_game.activePiece();
        const sf::Vector2f target(static_cast<float>(piece.x), static_cast<float>(piece.y - Game::HiddenRows));
        if (m_game.pieceSerial() != m_displayedSerial) {
            m_displayedSerial = m_game.pieceSerial();
            m_displayPosition = target;
        } else {
            m_displayPosition.x = approach(m_displayPosition.x, target.x, SlideSmoothing, deltaTime);
            m_displayPosition.y = approach(m_displayPosition.y, target.y, FallSmoothing, deltaTime);
        }

        const auto shape = tetris::shapeOf(piece.type, piece.rotation);
        const sf::Color color = pieceColor(piece.type);
        std::vector<sf::Vector2f> active;
        std::vector<sf::Vector2i> ghost;
        const int ghostRow = m_game.ghostY() - Game::HiddenRows;
        for (const auto& cell : shape) {
            active.emplace_back(m_displayPosition.x + cell.x, m_displayPosition.y + cell.y);
            ghost.emplace_back(piece.x + cell.x, ghostRow + cell.y);
        }
        m_board->setActivePiece(active, color);
        if (ghostRow > piece.y - Game::HiddenRows) {
            m_board->setGhost(ghost, color);
        }
    }

    std::vector<sf::Vector2i> next;
    for (const auto& cell : tetris::shapeOf(m_game.nextPiece(), 0)) {
        next.emplace_back(cell.x, cell.y);
    }
    if (state != Game::State::Ready) {
        m_infoPanel->setNextPiece(next, pieceColor(m_game.nextPiece()));
    }
    m_infoPanel->setScore(m_game.score());
    m_infoPanel->setLevel(m_game.level());
    m_infoPanel->setLines(m_game.lines());
    m_infoPanel->setHighScore(m_highScore);
}

void HomeScene::syncControls() {
    const bool active = gameActive() || m_showingPreview;
    const bool paused = m_game.state() == Game::State::Paused;
    m_actionBar->setButtonEnabled(ActionButtonBar::ButtonType::Pause, active);
    m_actionBar->setButtonEnabled(ActionButtonBar::ButtonType::Restart, active);
    m_actionBar->setButtonLabel(ActionButtonBar::ButtonType::Pause, paused ? "RESUME" : "PAUSE");
    m_infoPanel->setPauseEnabled(active);
    m_infoPanel->setPauseLabel(paused ? "RESUME" : "PAUSE");
}

void HomeScene::drawOverlay(sf::RenderTarget& target) {
    if (m_showingPreview) {
        return;
    }

    std::string title;
    std::string body;
    switch (m_game.state()) {
        case Game::State::Ready:
            title = "TETRIS";
            body = "PRESS PLAY OR ENTER\n\nLEFT / RIGHT   move\nUP   rotate\nDOWN   soft drop\nSPACE   hard drop\nP   pause";
            break;
        case Game::State::Paused:
            title = "PAUSED";
            body = "PRESS P OR RESUME";
            break;
        case Game::State::GameOver:
            title = "GAME OVER";
            body = "SCORE " + std::to_string(m_game.score()) + "\n\nPRESS PLAY TO TRY AGAIN";
            break;
        default:
            return;
    }

    const sf::Vector2f origin = m_board->gridOrigin();
    const sf::Vector2f size = m_board->gridSize();
    DrawingUtilities::drawRoundedRectangle(target, origin, size, 4.f, sf::Color(4, 8, 18, 185));

    const sf::Vector2f center = origin + size / 2.f;
    m_overlayTitle.setString(title);
    centerOn(m_overlayTitle, center - sf::Vector2f(0.f, 70.f));
    target.draw(m_overlayTitle);

    m_overlayBody.setString(body);
    const sf::FloatRect bounds = m_overlayBody.getLocalBounds();
    m_overlayBody.setOrigin(bounds.left + bounds.width / 2.f, bounds.top);
    m_overlayBody.setPosition(center.x, center.y - 34.f);
    target.draw(m_overlayBody);
}

void HomeScene::drawToast(sf::RenderTarget& target) {
    if (m_toastTimer <= 0.f) {
        return;
    }
    const float alpha = std::min(1.f, m_toastTimer / 0.3f); // Fade out at the end
    const sf::FloatRect bounds = m_toastText.getLocalBounds();
    const sf::Vector2f size(bounds.width + 40.f, 38.f);
    const sf::Vector2f center(Theme::Layout::WindowWidth / 2.f, Theme::Layout::ActionY - 34.f);

    DrawingUtilities::drawRoundedRectangle(
        target, center - size / 2.f, size, 19.f,
        sf::Color(6, 13, 31, static_cast<sf::Uint8>(230 * alpha)),
        Theme::withAlpha(Theme::Colors::GreenBright(), static_cast<sf::Uint8>(200 * alpha)), 1.5f
    );
    m_toastText.setFillColor(Theme::withAlpha(Theme::Colors::TextWhite(), static_cast<sf::Uint8>(255 * alpha)));
    centerOn(m_toastText, center);
    target.draw(m_toastText);
}

void HomeScene::draw(sf::RenderTarget& target) {
    target.draw(m_background);
    m_stage->draw(target);
    m_board->draw(target);
    drawOverlay(target);
    m_infoPanel->draw(target);
    m_gameSelection->draw(target);
    m_actionBar->draw(target);
    m_navBar->draw(target);
    drawToast(target);
}
