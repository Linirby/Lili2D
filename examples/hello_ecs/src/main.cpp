#include <iostream>

#include "game.hpp"

int main() {
	Game game;

	try {
		game.run();
	} catch (std::exception &e) {
		std::cerr << e.what() << '\n';
	}

	return 0;
}
