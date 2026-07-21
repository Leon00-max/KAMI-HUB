#pragma once

#include <algorithm>
#include <SFML/Graphics/Color.hpp>

/**
 * @class Theme
 * @brief Centralized theme system defining all colors, spacing, typography, and visual properties.
 *
 * This system ensures visual consistency across the entire application and makes it trivial
 * to modify the appearance by changing values in one place.
 */
class Theme {
public:
    // ==================== COLOR PALETTE ====================
    // Primary Colors - from reference image
    struct Colors {
        // Neon Green - Primary accent
        static sf::Color PrimaryGreen() { return sf::Color(0x6D, 0xF9, 0x50, 0xFF); }

        // Deep Blues - Background and structure
        static sf::Color DarkBg() { return sf::Color(0x0A, 0x0C, 0x18, 0xFF); }       // Main background
        static sf::Color MediumBg() { return sf::Color(0x14, 0x18, 0x30, 0xFF); }     // Panel background
        static sf::Color LightBg() { return sf::Color(0x1E, 0x24, 0x3E, 0xFF); }      // Lighter panels

        // Glass effect - Semi-transparent
        static sf::Color GlassBase() { return sf::Color(0x14, 0x18, 0x30, 0xAA); }    // 2/3 opacity
        static sf::Color GlassBorder() { return sf::Color(0x7D, 0x96, 0xFF, 0xB4); }  // Light blue glow

        // Accent Colors
        static sf::Color AccentYellow() { return sf::Color(0xFF, 0xC1, 0x07, 0xFF); } // #FFC107
        static sf::Color AccentRed() { return sf::Color(0xFF, 0x6B, 0x6B, 0xFF); }    // #FF6B6B
        static sf::Color AccentOrange() { return sf::Color(0xFF, 0xA5, 0x3D, 0xFF); } // #FFA53D
        static sf::Color AccentPurple() { return sf::Color(0xB8, 0x5B, 0xDB, 0xFF); } // #B85BDB

        // Neutral Grays
        static sf::Color TextWhite() { return sf::Color(0xFF, 0xFF, 0xFF, 0xFF); }    // Pure white
        static sf::Color TextGray() { return sf::Color(0xCC, 0xD8, 0xF8, 0xFF); }     // Light gray-blue
        static sf::Color TextDarkGray() { return sf::Color(0x8C, 0xA2, 0xBC, 0xFF); } // Medium gray

        // Button states
        static sf::Color ButtonDefault() { return sf::Color(0x35, 0x61, 0xA2, 0xE6); }   // Muted blue
        static sf::Color ButtonHover() { return sf::Color(0x53, 0x89, 0xE2, 0xF0); }     // Bright blue
        static sf::Color ButtonPressed() { return sf::Color(0x12, 0x3A, 0x70, 0xFF); }   // Dark blue
        static sf::Color ButtonDisabled() { return sf::Color(0x28, 0x32, 0x52, 0x99); }  // Gray-blue, 60% opacity
    };

    // ==================== SPACING & LAYOUT ====================
    struct Spacing {
        static constexpr float XSmall = 4.f;
        static constexpr float Small = 8.f;
        static constexpr float Medium = 16.f;
        static constexpr float Large = 24.f;
        static constexpr float XLarge = 32.f;
        static constexpr float XXLarge = 48.f;

        // Padding for panels and containers
        static constexpr float PanelPadding = 20.f;
        static constexpr float ButtonPadding = 12.f;
    };

    // ==================== TYPOGRAPHY ====================
    struct Typography {
        // Font names (files in assets/fonts/)
        static constexpr const char* FontMain = "main";           // Regular font
        static constexpr const char* FontBold = "main_bold";      // Bold variant

        // Font sizes
        struct FontSizes {
            static constexpr unsigned int Heading1 = 48;          // Main title
            static constexpr unsigned int Heading2 = 32;          // Section titles
            static constexpr unsigned int Heading3 = 24;          // Subsection titles
            static constexpr unsigned int BodyLarge = 20;         // Large body text
            static constexpr unsigned int Body = 16;              // Standard body
            static constexpr unsigned int BodySmall = 14;         // Small text
            static constexpr unsigned int Label = 12;             // Labels and captions
            static constexpr unsigned int Button = 18;            // Button text
            static constexpr unsigned int NavItem = 16;           // Navigation items
        };
    };

    // ==================== COMPONENT SIZES ====================
    struct Sizes {
        // Navigation bar
        static constexpr float NavBarHeight = 70.f;

        // Game board / Glass panel
        static constexpr float GameBoardWidth = 330.f;
        static constexpr float GameBoardHeight = 460.f;

        // Side panels
        static constexpr float SidePanelWidth = 200.f;
        static constexpr float SidePanelHeight = 500.f;

        // Game cards
        static constexpr float GameCardWidth = 160.f;
        static constexpr float GameCardHeight = 140.f;

        // Standard button
        static constexpr float ButtonWidth = 140.f;
        static constexpr float ButtonHeight = 48.f;

        // Small button
        static constexpr float SmallButtonWidth = 100.f;
        static constexpr float SmallButtonHeight = 36.f;
    };

    // ==================== EFFECTS ====================
    struct Effects {
        // Border radius / corner rounding (approximated with lines for SFML)
        static constexpr float CornerRadius = 8.f;

        // Glow effect thickness
        static constexpr float GlowThickness = 2.f;

        // Shadow offset
        static constexpr float ShadowOffset = 4.f;

        // Animation timings (in seconds)
        static constexpr float HoverAnimationDuration = 0.15f;
        static constexpr float ButtonPressAnimationDuration = 0.1f;
        static constexpr float PanelFadeInDuration = 0.3f;
    };

    // ==================== HELPER METHODS ====================
    /**
     * Get a color with modified opacity
     */
    static sf::Color withAlpha(const sf::Color& color, sf::Uint8 alpha) {
        return sf::Color(color.r, color.g, color.b, alpha);
    }

    /**
     * Get a darker shade of a color
     */
    static sf::Color darken(const sf::Color& color, float factor = 0.7f) {
        return sf::Color(
            static_cast<sf::Uint8>(color.r * factor),
            static_cast<sf::Uint8>(color.g * factor),
            static_cast<sf::Uint8>(color.b * factor),
            color.a
        );
    }

    /**
     * Get a lighter shade of a color
     */
    static sf::Color lighten(const sf::Color& color, float factor = 1.3f) {
        return sf::Color(
            static_cast<sf::Uint8>(std::min(255.f, color.r * factor)),
            static_cast<sf::Uint8>(std::min(255.f, color.g * factor)),
            static_cast<sf::Uint8>(std::min(255.f, color.b * factor)),
            color.a
        );
    }
};
