#include <iostream>

#include "app.hpp"

int main() {
	HelloSprite app;

	try {
		app.run();
	} catch (std::exception &e) {
		std::cerr << e.what() << '\n';
	}

	return 0;
}
