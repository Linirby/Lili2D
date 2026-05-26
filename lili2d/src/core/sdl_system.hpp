#pragma once

namespace lili {

/// \brief Initializes and cleans up SDL subsystem resources.
struct SDLSystem {
	/// \brief Initializes the SDL system.
	SDLSystem();

	/// \brief Shuts down the SDL system.
	~SDLSystem();
};

}  // namespace lili
