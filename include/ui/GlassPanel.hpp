#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Theme.hpp"
#include "UIComponent.hpp"

/**
 * @class GlassPanel
 * @brief Light frosted container that groups the board and its side column
 */
class GlassPanel : public UIComponent {
public:
    GlassPanel(const sf::Vector2f& position, const sf::Vector2f& size);

    void draw(sf::RenderTarget& target) override;
};
