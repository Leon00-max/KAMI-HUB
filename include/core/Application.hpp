#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

#include "managers/ResourceManager.hpp"
#include "scenes/Scene.hpp"

class Application {
public:
    Application();
    ~Application() = default;

    void run(unsigned int maxFrames = 0);

private:
    void processEvents();
    void update(float deltaTime, const sf::Vector2f& pointerPosition);
    void render();

    sf::RenderWindow m_window;
    ResourceManager m_resources;
    std::unique_ptr<Scene> m_scene;
};
