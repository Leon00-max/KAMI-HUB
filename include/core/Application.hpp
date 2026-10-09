#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "managers/ResourceManager.hpp"
#include "scenes/Scene.hpp"

class Application {
public:
    struct Options {
        bool preview = false; // Show sample game state for design review
    };

    Application() : Application(Options{}) {}
    explicit Application(const Options& options);
    ~Application() = default;

    void run(unsigned int maxFrames = 0);

private:
    void loadResources();
    void processEvents();
    void update(float deltaTime, const sf::Vector2f& pointerPosition);
    void render();

    // Keep the fixed logical view centered with black bars on resize
    void updateLetterbox(unsigned int width, unsigned int height);
    // Rewrite mouse coordinates from window pixels to logical view units
    sf::Event toViewCoordinates(const sf::Event& event) const;

    sf::RenderWindow m_window;
    ResourceManager m_resources;
    std::unique_ptr<Scene> m_scene;
};
