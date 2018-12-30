#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <SFML/Graphics.hpp>
#include "ParticlesManager.hpp"

enum mousePressed
{
	Left,
	Right,
	None
};

std::vector<std::string> ParseString(std::string input)
{
	std::vector<std::string> allWords;

	int pos;
	do
	{
		pos = input.find(" ");
		allWords.push_back(input.substr(0, pos));
		input = input.erase(0, pos + 1);
	} while (pos != -1);
	return (allWords);
}

[[noreturn]] void ExitError(std::string msg)
{
	std::cerr << msg << "\nPress enter to continue..." << std::endl;
	std::getchar();
	exit(1);
}

int main()
{
	srand(time(0));
	int xWin, yWin;

	std::ifstream file("config.txt");
	std::string line;
	std::unique_ptr<Particles::ParticlesManager> manager;
	bool isWindowInit = false;
	while (std::getline(file, line))
	{
		if (line.length() == 0 || line[0] == '#')
			continue;
		std::vector<std::string> allWords = ParseString(line);
		if (!isWindowInit)
		{
			if (allWords.size() != 2)
				ExitError("Error while parsing configuration file: invalid number of arguments");
			xWin = std::stoi(allWords[0]);
			yWin = std::stoi(allWords[1]);
			manager = std::make_unique<Particles::ParticlesManager>(sf::Vector2i(xWin, yWin));
			isWindowInit = true;
		}
		else
		{
			if (allWords.size() != 6)
				ExitError("Error while parsing configuration file: invalid number of arguments");
			auto &spawner = manager->AddSpawner(sf::Vector2i(std::stoi(allWords[0]), std::stoi(allWords[1])));
			spawner->SetColor(sf::Color(std::stoi(allWords[2]), std::stoi(allWords[3]), std::stoi(allWords[4])));
			spawner->SetSpawnRate(std::stod(allWords[5]));
		}
	}
	if (!isWindowInit)
		ExitError("Error while parsing configuration file: the file is empty");

	sf::RenderWindow window(sf::VideoMode(xWin, yWin), "Particles");

	mousePressed mouse = mousePressed::None;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
					mouse = mousePressed::Left;
				else
					mouse = mousePressed::Right;
			}
			else if (event.type == sf::Event::MouseButtonReleased)
				mouse = mousePressed::None;
		}

		if (mouse == mousePressed::Left)
			manager->DrawWall(sf::Mouse::getPosition(window), true);
		else if (mouse == mousePressed::Right)
			manager->DrawWall(sf::Mouse::getPosition(window), false);

		window.clear();
		manager->Update();
		manager->Draw(window);
		window.display();
	}

	return 0;
}