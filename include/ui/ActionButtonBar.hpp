#pragma once

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "managers/ResourceManager.hpp"
#include "AnimatedButton.hpp"
#include "Theme.hpp"
#include "UIComponent.hpp"

/**
 * @class ActionButtonBar
 * @brief Centered row of colored action buttons: Play, Pause, Restart, Menu, Settings
 */
class ActionButtonBar : public UIComponent {
public:
    enum class ButtonType {
        Play,
        Pause,
        Restart,
        Menu,
        Settings
    };

    explicit ActionButtonBar(const ResourceManager& resources);

    void setButtonEnabled(ButtonType type, bool enabled);
    void setButtonLabel(ButtonType type, const std::string& label);

    bool handleEvent(const sf::Event& event);
    void update(float deltaTime, const sf::Vector2f& mousePosition);
    void draw(sf::RenderTarget& target) override;

    // Index (ButtonType order) of the button clicked by the last handleEvent, or -1
    int getClickedButtonIndex() const { return m_clickedButtonIndex; }

private:
    std::vector<AnimatedButton> m_buttons;
    int m_clickedButtonIndex = -1;
};
