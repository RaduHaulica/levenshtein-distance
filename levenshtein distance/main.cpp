#include <SFML/Graphics.hpp>

#include <iostream>
#include <format>
#include <array>

const int n{ 10 };
std::array<std::array<std::string, n>, n> LCSCache;
std::array<std::array<std::string, n>, n> LCSRecursiveCache;
int LCSRecursiveCalls = 0;

// =========================
//	UTILITY
// =========================

std::string popEnd(std::string& s)
{
	//s.erase(s.begin());
	s.erase(s.end() - 1);
	return s;
}

template <int n>
void printArray(std::array<std::array<int, n>, n> numbers)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            std::cout << std::format("{0:3} ",numbers[i][j]);
        std::cout << '\n';
    }
}

// =========================
//	LEVENSHTEIN DISTANCE
// =========================

int callsRecursiveLevenshtein = 0;
int callsDynamicLevenshtein = 0;
std::array<std::array<int, 10>, 10> levCache;

int computeLevenshteinDistanceRecursive(std::string s1, std::string s2)
{
	callsRecursiveLevenshtein++;
	if (s1.size() == 0)
		return s2.size();
	if (s2.size() == 0)
		return s1.size();
	if (s1[s1.size() - 1] == s2[s2.size() - 1])
	{
		popEnd(s1);
		popEnd(s2);
		return computeLevenshteinDistanceRecursive(s1, s2);
	}
	std::string front1 = s1;
	popEnd(front1);
	std::string front2 = s2;
	popEnd(front2);
	return 1 + std::min(computeLevenshteinDistanceRecursive(front1, front2), std::min(computeLevenshteinDistanceRecursive(s1, front2), computeLevenshteinDistanceRecursive(front1, s2)));
}

int computeLevenshteinDistanceDynamic(std::string s1, std::string s2)
{
	callsDynamicLevenshtein++;
	if (s1.size() == 0)
	{
		if (levCache[s1.size()][s2.size()] == 0)
			levCache[s1.size()][s2.size()] = s2.size();
		return levCache[s1.size()][s2.size()];
	}
	if (s2.size() == 0)
	{
		if (levCache[s1.size()][s2.size()] == 0)
			levCache[s1.size()][s2.size()] = s1.size();
		return levCache[s1.size()][s2.size()];
	}
	if (s1[s1.size() - 1] == s2[s2.size() - 1])
	{
		if (levCache[s1.size()][s2.size()] == 0)
		{
			std::string front1 = s1;
			popEnd(front1);
			std::string front2 = s1;
			popEnd(front2);
			levCache[s1.size()][s2.size()] = computeLevenshteinDistanceDynamic(front1, front2);
		}
		return levCache[s1.size()][s2.size()];
	}
	if (levCache[s1.size()][s2.size()] == 0)
	{
		std::string front1 = s1;
		popEnd(front1);
		std::string front2 = s2;
		popEnd(front2);
		levCache[s1.size()][s2.size()] = 1 + std::min(computeLevenshteinDistanceDynamic(front1, front2), std::min(computeLevenshteinDistanceDynamic(s1, front2), computeLevenshteinDistanceDynamic(front1, s2)));
	}
	return levCache[s1.size()][s2.size()];
}

// =========================
//		MAIN
// =========================

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	std::string l1 = "kitten";
	std::string l2 = "sitting";
	std::cout << std::format("Recursive Levenshtein distance({}, {}) = {} with {} calls\n", l1, l2, computeLevenshteinDistanceRecursive(l1, l2), callsRecursiveLevenshtein);
	std::cout << std::format("Dynamic Levenshtein distance({}, {}) = {} with {} calls\n", l1, l2, computeLevenshteinDistanceDynamic(l1, l2), callsDynamicLevenshtein);
	printArray<n>(levCache);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}