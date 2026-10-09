#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "UIComponent.hpp"
#include "Theme.hpp"

/**
 * @class GlassPanel
 * @brief Central game board container with glass morphism effect
 * Used to display game content with elegant transparency and glow
 */
class GlassPanel : public UIComponent {
public:
    GlassPanel();

    GlassPanel(
        const sf::Vector2f& position,
        const sf::Vector2f& size
    );

    // Set panel appearance
    void setGlassColor(const sf::Color& color);
    void setGlowColor(const sf::Color& color);

    // Drawing
    void draw(sf::RenderTarget& target) override;

private:
    sf::Color m_glassColor = Theme::Colors::GlassBase();
    sf::Color m_glowColor = Theme::Colors::GlassBorder();
};
