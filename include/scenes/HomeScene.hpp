#pragma once

#include <vector>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "managers/ResourceManager.hpp"
#include "scenes/Scene.hpp"
#include "ui/Button.hpp"

class HomeScene : public Scene {
public:
    explicit HomeScene(ResourceManager& resources);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime, const sf::Vector2f& pointerPosition) override;
    void draw(sf::RenderTarget& target) override;

private:
    ResourceManager& m_resources;
    sf::Sprite m_background;
    sf::RectangleShape m_panel;
    sf::Text m_title;
    sf::Text m_subtitle;
    sf::Text m_hint;
    std::vector<Button> m_buttons;
};
