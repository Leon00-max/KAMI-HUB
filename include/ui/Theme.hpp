#pragma once

#include <algorithm>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

/**
 * @class Theme
 * @brief Centralized theme: colors, typography, layout and effect constants.
 *
 * Values follow the KAMI Hub concept design (nature background, glassmorphism
 * panels, glossy neon-accented buttons). Layout is in logical pixels of the
 * fixed 1280x720 view; the window letterboxes it on resize.
 */
class Theme {
public:
    // ==================== COLOR PALETTE ====================
    struct Colors {
        // Core palette from the design system
        static sf::Color Green() { return sf::Color(0x4C, 0xAF, 0x50); }        // #4CAF50
        static sf::Color GreenBright() { return sf::Color(0x7E, 0xD9, 0x3A); }  // Neon accent
        static sf::Color Blue() { return sf::Color(0x21, 0x96, 0xF3); }         // #2196F3
        static sf::Color BlueBright() { return sf::Color(0x29, 0xB6, 0xF6); }   // #29B6F6
        static sf::Color Yellow() { return sf::Color(0xFF, 0xC1, 0x07); }       // #FFC107
        static sf::Color Amber() { return sf::Color(0xFF, 0xB3, 0x00); }        // #FFB300
        static sf::Color Orange() { return sf::Color(0xFF, 0x8F, 0x00); }
        static sf::Color Red() { return sf::Color(0xF4, 0x43, 0x36); }          // #F44336
        static sf::Color Purple() { return sf::Color(0x9C, 0x27, 0xB0); }       // #9C27B0
        static sf::Color PurpleBright() { return sf::Color(0xB3, 0x6B, 0xF0); }

        // Neutrals
        static sf::Color TextWhite() { return sf::Color(0xFF, 0xFF, 0xFF); }    // #FFFFFF
        static sf::Color TextGray() { return sf::Color(0xE0, 0xE0, 0xE0); }     // #E0E0E0
        static sf::Color TextDarkGray() { return sf::Color(0xBD, 0xBD, 0xBD); } // #BDBDBD
        static sf::Color Charcoal() { return sf::Color(0x21, 0x21, 0x21); }     // #212121

        // Surfaces
        static sf::Color NavBg() { return sf::Color(0x06, 0x0D, 0x1F, 0xEB); }
        static sf::Color PanelBg() { return sf::Color(0x0A, 0x14, 0x28, 0xD9); }      // Dark glass
        static sf::Color PanelBorder() { return sf::Color(0x8C, 0xB4, 0xE6, 0x55); }
        static sf::Color FrostTop() { return sf::Color(0x5A, 0x78, 0x9A, 0x70); }     // Light glass
        static sf::Color FrostBottom() { return sf::Color(0x1E, 0x32, 0x50, 0x8C); }
        static sf::Color FrostBorder() { return sf::Color(0xFF, 0xFF, 0xFF, 0x60); }
        static sf::Color BoardBg() { return sf::Color(0x07, 0x0E, 0x1C, 0xF2); }
        static sf::Color BoardGrid() { return sf::Color(0x50, 0x6E, 0x96, 0x40); }
        static sf::Color Divider() { return sf::Color(0xFF, 0xFF, 0xFF, 0x22); }

        // Legacy names used by older components
        static sf::Color PrimaryGreen() { return GreenBright(); }
        static sf::Color DarkBg() { return sf::Color(0x0A, 0x0C, 0x18); }
        static sf::Color MediumBg() { return sf::Color(0x14, 0x18, 0x30); }
        static sf::Color LightBg() { return sf::Color(0x1E, 0x24, 0x3E); }
        static sf::Color GlassBase() { return PanelBg(); }
        static sf::Color GlassBorder() { return PanelBorder(); }
        static sf::Color AccentYellow() { return Yellow(); }
        static sf::Color AccentRed() { return Red(); }
        static sf::Color AccentOrange() { return Orange(); }
        static sf::Color AccentPurple() { return Purple(); }
    };

    // ==================== TYPOGRAPHY ====================
    struct Fonts {
        static constexpr const char* Display = "display";     // Orbitron Bold (headers)
        static constexpr const char* Regular = "regular";     // Exo 2 Regular (body)
        static constexpr const char* SemiBold = "semibold";   // Exo 2 SemiBold (subheaders)
        static constexpr const char* Bold = "bold";           // Exo 2 Bold (UI / buttons)
    };

    struct FontSizes {
        static constexpr unsigned int Logo = 22;
        static constexpr unsigned int LogoSub = 11;
        static constexpr unsigned int NavItem = 14;
        static constexpr unsigned int PanelTitle = 13;
        static constexpr unsigned int StatLabel = 13;
        static constexpr unsigned int StatValue = 21;
        static constexpr unsigned int Button = 15;
        static constexpr unsigned int CardLabel = 13;
    };

    // ==================== LAYOUT (logical 1280x720) ====================
    struct Layout {
        static constexpr float WindowWidth = 1280.f;
        static constexpr float WindowHeight = 720.f;

        static constexpr float NavBarHeight = 78.f;

        // Game mode selector
        static constexpr float ModePanelX = 262.f;
        static constexpr float ModePanelY = 202.f;
        static constexpr float ModePanelWidth = 150.f;
        static constexpr float ModePanelHeight = 200.f;

        // Frosted container holding board, preview, stats and pause
        static constexpr float StageX = 431.f;
        static constexpr float StageY = 92.f;
        static constexpr float StageWidth = 467.f;
        static constexpr float StageHeight = 473.f;

        // Tetris board (10x20 cells)
        static constexpr int BoardColumns = 10;
        static constexpr int BoardRows = 20;
        static constexpr float CellSize = 21.f;
        static constexpr float BoardPadding = 12.f;
        static constexpr float BoardX = 505.f;
        static constexpr float BoardY = 105.f;

        // Right column inside the stage
        static constexpr float SideX = 758.f;
        static constexpr float SideWidth = 126.f;
        static constexpr float NextY = 116.f;
        static constexpr float NextHeight = 100.f;
        static constexpr float StatsY = 226.f;
        static constexpr float StatsHeight = 246.f;
        static constexpr float SidePauseY = 487.f;
        static constexpr float SidePauseHeight = 50.f;

        // Bottom action buttons
        static constexpr float ActionY = 580.f;
        static constexpr float ActionHeight = 48.f;
        static constexpr float ActionGap = 13.f;
    };

    // ==================== EFFECTS ====================
    struct Effects {
        static constexpr float CornerRadius = 10.f;
        static constexpr float PanelRadius = 12.f;
        static constexpr float StageRadius = 16.f;
        static constexpr float HoverScale = 1.05f;
        static constexpr float HoverAnimationDuration = 0.15f;
        static constexpr float ButtonPressAnimationDuration = 0.1f;
    };

    // ==================== HELPERS ====================
    static sf::Color withAlpha(const sf::Color& color, sf::Uint8 alpha) {
        return sf::Color(color.r, color.g, color.b, alpha);
    }

    static sf::Color darken(const sf::Color& color, float factor = 0.7f) {
        return sf::Color(
            static_cast<sf::Uint8>(color.r * factor),
            static_cast<sf::Uint8>(color.g * factor),
            static_cast<sf::Uint8>(color.b * factor),
            color.a
        );
    }

    static sf::Color lighten(const sf::Color& color, float factor = 1.3f) {
        return sf::Color(
            static_cast<sf::Uint8>(std::min(255.f, color.r * factor)),
            static_cast<sf::Uint8>(std::min(255.f, color.g * factor)),
            static_cast<sf::Uint8>(std::min(255.f, color.b * factor)),
            color.a
        );
    }

    static sf::Color mix(const sf::Color& a, const sf::Color& b, float t) {
        auto lerp = [t](sf::Uint8 x, sf::Uint8 y) {
            return static_cast<sf::Uint8>(x + (static_cast<float>(y) - x) * t);
        };
        return sf::Color(lerp(a.r, b.r), lerp(a.g, b.g), lerp(a.b, b.b), lerp(a.a, b.a));
    }

    // Desaturate toward gray and fade, for disabled controls
    static sf::Color muted(const sf::Color& color) {
        const auto gray = static_cast<sf::Uint8>((color.r + color.g + color.b) / 3);
        return withAlpha(mix(color, sf::Color(gray, gray, gray), 0.75f), static_cast<sf::Uint8>(color.a * 0.55f));
    }
};
