#pragma once

#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "managers/ResourceManager.hpp"
#include "scenes/Scene.hpp"
#include "ui/NavigationBar.hpp"
#include "ui/GameSelectionPanel.hpp"
#include "ui/GlassPanel.hpp"
#include "ui/InfoPanel.hpp"
#include "ui/ActionButtonBar.hpp"

/**
 * @class HomeScene
 * @brief Main home/menu scene with professional UI layout
 *
 * Layout:
 * - Top: Navigation bar (logo, nav items, controls)
 * - Left: Game selection panel (Tetris, Breakout)
 * - Center: Glass panel for game board
 * - Right: Info panel (score, level, etc.)
 * - Bottom: Action button bar (Play, Pause, Restart, Menu, Settings)
 */
class HomeScene : public Scene {
public:
    explicit HomeScene(ResourceManager& resources);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime, const sf::Vector2f& pointerPosition) override;
    void draw(sf::RenderTarget& target) override;

private:
    ResourceManager& m_resources;
    sf::VertexArray m_background;

    // UI Components
    std::unique_ptr<NavigationBar> m_navBar;
    std::unique_ptr<GameSelectionPanel> m_gameSelection;
    std::unique_ptr<GlassPanel> m_gameBoard;
    std::unique_ptr<InfoPanel> m_infoPanel;
    std::unique_ptr<ActionButtonBar> m_actionBar;

    void initializeBackground();
    void handleUIInteraction();
};
