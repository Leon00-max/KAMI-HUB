#pragma once

#include <memory>
#include <string>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "games/tetris/Tetris.hpp"
#include "managers/ResourceManager.hpp"
#include "scenes/Scene.hpp"
#include "ui/ActionButtonBar.hpp"
#include "ui/GameBoard.hpp"
#include "ui/GameSelectionPanel.hpp"
#include "ui/GlassPanel.hpp"
#include "ui/InfoPanel.hpp"
#include "ui/NavigationBar.hpp"

/**
 * @class HomeScene
 * @brief Hub screen; Tetris plays directly on the center board.
 *
 * Layout:
 * - Top: navigation bar (logo, nav items, settings/music/sound)
 * - Left: game mode panel (Tetris, Breakout)
 * - Center: frosted stage with the board, next piece, stats and pause
 * - Bottom: action buttons (Play, Pause, Restart, Menu, Settings)
 */
class HomeScene : public Scene {
public:
    // preview shows a sample mid-game state until Play is pressed
    HomeScene(ResourceManager& resources, bool preview = false);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime, const sf::Vector2f& pointerPosition) override;
    void draw(sf::RenderTarget& target) override;

private:
    ResourceManager& m_resources;
    sf::Sprite m_background;

    std::unique_ptr<NavigationBar> m_navBar;
    std::unique_ptr<GameSelectionPanel> m_gameSelection;
    std::unique_ptr<GlassPanel> m_stage;
    std::unique_ptr<GameBoard> m_board;
    std::unique_ptr<InfoPanel> m_infoPanel;
    std::unique_ptr<ActionButtonBar> m_actionBar;

    tetris::Game m_game;
    bool m_showingPreview = false;
    int m_highScore = 0;

    // Rendered piece position eases toward the logical one
    sf::Vector2f m_displayPosition;
    unsigned int m_displayedSerial = 0;

    sf::Text m_overlayTitle;
    sf::Text m_overlayBody;
    sf::Text m_toastText;
    float m_toastTimer = 0.f;

    void initializeBackground();
    void loadPreviewState();

    void onPlay();
    void onPauseToggle();
    void onRestart();
    void showToast(const std::string& message);
    void handleKey(const sf::Event::KeyEvent& key, bool pressed);

    bool gameActive() const;
    void syncGameView(float deltaTime);
    void syncControls();
    void drawOverlay(sf::RenderTarget& target);
    void drawToast(sf::RenderTarget& target);
};
