#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

class Scene {
public:
    virtual ~Scene() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float deltaTime, const sf::Vector2f& pointerPosition) = 0;
    virtual void draw(sf::RenderTarget& target) = 0;
};
