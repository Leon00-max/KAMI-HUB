#include "scenes/HomeScene.hpp"

#include <cmath>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

HomeScene::HomeScene(ResourceManager& resources)
    : m_resources(resources) {
    const sf::Texture& texture = m_resources.getTexture("placeholder");
    m_background.setTexture(texture);
    m_background.setScale(1.6f, 1.6f);

    const sf::Font& font = m_resources.getFont("main");
    m_title.setFont(font);
    m_title.setString("KAMI Hub");
    m_title.setCharacterSize(64);
    m_title.setFillColor(sf::Color::White);
    m_title.setPosition(720.f, 280.f);

    m_subtitle.setFont(font);
    m_subtitle.setString("Framework Initialization");
    m_subtitle.setCharacterSize(28);
    m_subtitle.setFillColor(sf::Color(204, 216, 248));
    m_subtitle.setPosition(720.f, 360.f);

    m_hint.setFont(font);
    m_hint.setString("Hover and click the button below to confirm UI interaction layers.");
    m_hint.setCharacterSize(20);
    m_hint.setFillColor(sf::Color(140, 162, 188));
    m_hint.setPosition(640.f, 460.f);

    m_panel.setSize(sf::Vector2f(780.f, 420.f));
    m_panel.setPosition(570.f, 220.f);
    m_panel.setFillColor(sf::Color(20, 24, 48, 220));
    m_panel.setOutlineThickness(2.f);
    m_panel.setOutlineColor(sf::Color(125, 150, 255, 180));

    m_buttons.emplace_back(sf::Vector2f(860.f, 560.f), sf::Vector2f(180.f, 54.f), "Enter", font, []() {
        // Future callback for navigation.
    });
}

void HomeScene::handleEvent(const sf::Event& event) {
    if (!m_buttons.empty()) {
        m_buttons[0].handleEvent(event);
    }
}

void HomeScene::update(float deltaTime, const sf::Vector2f& pointerPosition) {
    (void)deltaTime;
    if (!m_buttons.empty()) {
        m_buttons[0].update(pointerPosition);
    }
}

void HomeScene::draw(sf::RenderTarget& target) {
    target.draw(m_background);
    target.draw(m_panel);
    target.draw(m_title);
    target.draw(m_subtitle);
    target.draw(m_hint);
    for (const auto& button : m_buttons) {
        button.draw(target);
    }
}
