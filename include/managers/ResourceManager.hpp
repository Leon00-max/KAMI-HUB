#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

class ResourceManager {
public:
    ResourceManager() = default;

    bool loadFont(const std::string& key, const std::string& path);
    bool loadTexture(const std::string& key, const std::string& path);
    sf::Texture createPlaceholderTexture() const;

    const sf::Font& getFont(const std::string& key) const;
    const sf::Texture& getTexture(const std::string& key) const;

private:
    std::unordered_map<std::string, std::shared_ptr<sf::Font>> m_fonts;
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_textures;
};
