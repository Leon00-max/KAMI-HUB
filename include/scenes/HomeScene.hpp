#pragma once

#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

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
 * @brief Hub screen
 *
 * Layout:
 * - Top: navigation bar (logo, nav items, settings/music/sound)
 * - Left: game mode panel (Tetris, Breakout)
 * - Center: frosted stage with the board, next piece, stats and pause
 * - Bottom: action buttons (Play, Pause, Restart, Menu, Settings)
 */
class HomeScene : public Scene {
public:
    // preview fills the board with a sample mid-game state for design review
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

    void initializeBackground();
    void loadPreviewState();
    void setGameRunning(bool running);
};
