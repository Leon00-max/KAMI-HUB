#include "core/Application.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include "scenes/HomeScene.hpp"

Application::Application()
    : m_window(sf::VideoMode(1280, 720), "KAMI Hub", sf::Style::Default) {
    m_window.setVerticalSyncEnabled(true);
    m_window.setFramerateLimit(60);
    m_window.setTitle("KAMI Hub");

    const std::filesystem::path assetRoot{"assets"};
    const std::string iconPath = (assetRoot / "icons" / "app_icon.png").string();
    std::ifstream iconFile(iconPath.c_str());
    if (iconFile.good()) {
        sf::Image icon;
        if (icon.loadFromFile(iconPath)) {
            m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        }
    }

    if (!m_resources.loadFont("main", (assetRoot / "fonts" / "NotoSans-Regular.ttf").string())) {
        std::cerr << "Failed to load main font from assets/fonts/NotoSans-Regular.ttf\n";
    }

    if (!m_resources.loadTexture("placeholder", (assetRoot / "images" / "placeholder.png").string())) {
        std::cerr << "Using generated placeholder texture because assets/images/placeholder.png could not be loaded\n";
    }

    m_scene = std::make_unique<HomeScene>(m_resources);
}

void Application::run(const unsigned int maxFrames) {
    sf::Clock clock;
    unsigned int renderedFrames = 0;

    while (m_window.isOpen()) {
        processEvents();

        const float deltaTime = clock.restart().asSeconds();
        const sf::Vector2f pointerPosition = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
        update(deltaTime, pointerPosition);
        render();

        ++renderedFrames;
        if (maxFrames > 0 && renderedFrames >= maxFrames) {
            m_window.close();
        }
    }
}

void Application::processEvents() {
    sf::Event event{};
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        } else if (event.type == sf::Event::Resized) {
            m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
        }

        if (m_scene) {
            m_scene->handleEvent(event);
        }
    }
}

void Application::update(float deltaTime, const sf::Vector2f& pointerPosition) {
    if (m_scene) {
        m_scene->update(deltaTime, pointerPosition);
    }
}

void Application::render() {
    m_window.clear(sf::Color(10, 12, 24));
    if (m_scene) {
        m_scene->draw(m_window);
    }
    m_window.display();
}
