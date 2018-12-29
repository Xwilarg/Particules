#pragma once

# include <vector>
# include <memory>

# include <SFML/Graphics/RenderWindow.hpp>
# include <SFML/Graphics/Image.hpp>
# include <SFML/Graphics/Sprite.hpp>
# include <SFML/Graphics/Texture.hpp>

# include "Particle.hpp"
# include "Spawner.hpp"

namespace Particles
{
	class ParticlesManager final
	{
	public:
		ParticlesManager(sf::Vector2i &&winSize) noexcept;
		std::unique_ptr<Spawner> &AddSpawner(sf::Vector2i &&pos) noexcept;
		std::unique_ptr<Particle> &AddParticle(const sf::Vector2i &pos) noexcept;
		void Update() noexcept;
		void Draw(sf::RenderWindow &window) const noexcept;

	private:
		const sf::Vector2i _winSize;
		std::vector<std::unique_ptr<Particle>> _particles;
		std::vector<std::unique_ptr<Spawner>> _spawners;
		sf::Image _image;
		sf::Texture _texture;
		sf::Sprite _sprite;
	};
}