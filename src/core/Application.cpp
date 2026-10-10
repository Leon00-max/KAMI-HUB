#include "core/Application.hpp"

#include <filesystem>
#include <iostream>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Mouse.hpp>

#include "scenes/HomeScene.hpp"
#include "ui/Theme.hpp"

Application::Application(const Options& options)
    : m_assetRoot(options.assetRoot),
      m_window(
          sf::VideoMode(static_cast<unsigned int>(Theme::Layout::WindowWidth), static_cast<unsigned int>(Theme::Layout::WindowHeight)),
          "Arcade Hub",
          sf::Style::Default
      ) {
    // VSync alone paces frames; combining it with a frame limit makes them fight
    m_window.setVerticalSyncEnabled(true);
    updateLetterbox(m_window.getSize().x, m_window.getSize().y);

    sf::Image icon;
    if (icon.loadFromFile((m_assetRoot / "icons" / "app_icon.png").string())) {
        m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    loadResources();
    m_scene = std::make_unique<HomeScene>(m_resources, options.preview);
}

void Application::loadResources() {
    const struct {
        const char* key;
        const char* file;
    } fonts[] = {
        {Theme::Fonts::Display, "Orbitron-Bold.ttf"},
        {Theme::Fonts::Regular, "Exo2-Regular.ttf"},
        {Theme::Fonts::SemiBold, "Exo2-SemiBold.ttf"},
        {Theme::Fonts::Bold, "Exo2-Bold.ttf"},
    };
    for (const auto& font : fonts) {
        const std::string path = (m_assetRoot / "fonts" / font.file).string();
        if (!m_resources.loadFont(font.key, path)) {
            std::cerr << "Failed to load font " << path << "\n";
        }
    }

    const std::string backgroundPath = (m_assetRoot / "images" / "menu_background.png").string();
    if (!m_resources.loadTexture("background", backgroundPath)) {
        std::cerr << "Using generated background because " << backgroundPath << " could not be loaded\n";
    }
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

void Application::updateLetterbox(unsigned int width, unsigned int height) {
    const float windowRatio = static_cast<float>(width) / static_cast<float>(height);
    const float viewRatio = Theme::Layout::WindowWidth / Theme::Layout::WindowHeight;

    sf::FloatRect viewport(0.f, 0.f, 1.f, 1.f);
    if (windowRatio > viewRatio) {
        viewport.width = viewRatio / windowRatio;
        viewport.left = (1.f - viewport.width) / 2.f;
    } else {
        viewport.height = windowRatio / viewRatio;
        viewport.top = (1.f - viewport.height) / 2.f;
    }

    sf::View view(sf::FloatRect(0.f, 0.f, Theme::Layout::WindowWidth, Theme::Layout::WindowHeight));
    view.setViewport(viewport);
    m_window.setView(view);
}

sf::Event Application::toViewCoordinates(const sf::Event& event) const {
    sf::Event mapped = event;
    if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
        const sf::Vector2f point = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        mapped.mouseButton.x = static_cast<int>(point.x);
        mapped.mouseButton.y = static_cast<int>(point.y);
    } else if (event.type == sf::Event::MouseMoved) {
        const sf::Vector2f point = m_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        mapped.mouseMove.x = static_cast<int>(point.x);
        mapped.mouseMove.y = static_cast<int>(point.y);
    }
    return mapped;
}

void Application::processEvents() {
    sf::Event event{};
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        } else if (event.type == sf::Event::Resized) {
            updateLetterbox(event.size.width, event.size.height);
        }

        if (m_scene) {
            m_scene->handleEvent(toViewCoordinates(event));
        }
    }
}

void Application::update(float deltaTime, const sf::Vector2f& pointerPosition) {
    if (m_scene) {
        m_scene->update(deltaTime, pointerPosition);
    }
}

void Application::render() {
    m_window.clear(sf::Color::Black);
    if (m_scene) {
        m_scene->draw(m_window);
    }
    m_window.display();
}
