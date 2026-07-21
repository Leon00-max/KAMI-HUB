#include "managers/ResourceManager.hpp"

#include <SFML/Graphics/Image.hpp>

namespace {
sf::Uint8 toColorChannel(const unsigned int value) {
    return static_cast<sf::Uint8>(value);
}
}

bool ResourceManager::loadFont(const std::string& key, const std::string& path) {
    auto font = std::make_shared<sf::Font>();
    if (!font->loadFromFile(path)) {
        return false;
    }

    m_fonts[key] = std::move(font);
    return true;
}

bool ResourceManager::loadTexture(const std::string& key, const std::string& path) {
    auto texture = std::make_shared<sf::Texture>();
    bool loaded = true;
    if (!texture->loadFromFile(path)) {
        *texture = createPlaceholderTexture();
        loaded = false;
    }

    m_textures[key] = std::move(texture);
    return loaded;
}

sf::Texture ResourceManager::createPlaceholderTexture() const {
    sf::Image image;
    image.create(512, 512, sf::Color(18, 25, 38));
    for (unsigned int y = 0; y < image.getSize().y; ++y) {
        for (unsigned int x = 0; x < image.getSize().x; ++x) {
            const float px = static_cast<float>(x) / static_cast<float>(image.getSize().x);
            const float py = static_cast<float>(y) / static_cast<float>(image.getSize().y);
            sf::Color color(
                toColorChannel(18 + static_cast<unsigned int>(px * 16.f)),
                toColorChannel(30 + static_cast<unsigned int>(py * 30.f)),
                toColorChannel(60 + static_cast<unsigned int>(px * 30.f)));
            image.setPixel(x, y, color);
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    return texture;
}

const sf::Font& ResourceManager::getFont(const std::string& key) const {
    static sf::Font fallbackFont;
    const auto it = m_fonts.find(key);
    if (it != m_fonts.end() && it->second) {
        return *it->second;
    }
    return fallbackFont;
}

const sf::Texture& ResourceManager::getTexture(const std::string& key) const {
    static sf::Texture fallbackTexture;
    const auto it = m_textures.find(key);
    if (it != m_textures.end() && it->second) {
        return *it->second;
    }
    return fallbackTexture;
}
