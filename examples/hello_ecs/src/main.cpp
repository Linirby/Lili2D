#include <iostream>

#include "game.hpp"

int main() {
	App app;

	try {
		app.run();
	} catch (std::exception &e) {
		std::cerr << e.what() << '\n';
	}

	return 0;
}
