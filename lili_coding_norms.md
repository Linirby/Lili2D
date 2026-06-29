# Lili Coding Norms

## General C++ Rules
- 80 chars max per line.
- Indentation must use tabs (size 4), absolutely no spaces for indentation.
- No trailing spaces allowed on any line.
- Blank lines must be completely empty (no spaces or tabs).
- Header files must have only declarations (no logics). Namespaces
(e.g., `namespace lili { ... }`) are fully allowed in headers.
- Namespaces do not add an indent for the code inside them.
- A blank line must separate the start and end of the namespace from the code inside.
- Namespace closing brackets must be commented with exactly 2 spaces of
separation: `}  // namespace name_of_the_namespace`.
- Source files must have implementation (logics).
- Classes with manual memory management (destructor needed) must respect the
Rule of 5.
- Classes with dynamic memory management (no destructor needed) must respect the Rule of 0.

## Naming Conventions
- **Variables**: `snake_case`
- **Classes, Structs, Types**: `PascalCase`
- **Functions**: `camelCase`
- **Constants**: `UPPER_CASE`

## Class Structure in Headers
In header files, classes must declare the `public` section first and then the `private` section. 
Within each section, members must be ordered as follows:
1. Constructor/Destructor
2. Attributes
3. Methods

## Temporary Rules: Managing 80 Character Limits
*(Note: More examples will be added here later.)*

Based on the `Lili2D` project, here are some examples of how to manage the 80 character limit in different situations:

**Function Arguments / String Concatenation:**
When arguments or concatenations exceed 80 characters, break them into a new
line and align or indent appropriately.
```cpp
throw std::runtime_error(
	"Failed to change window to resizable: " +
	std::string(SDL_GetError())
);
```

**Constructor Initialization Lists:**
When an initialization list is too long, break it so that each initializer is on its own line and aligned with the `:`:
```cpp
Window::Window(Window &&other) noexcept
	: resizable(other.resizable),
	borderless(other.borderless),
	fullscreen(other.fullscreen),
	window(other.window) {
	other.window = nullptr;
}
```

**Variable Assignment:**
When a variable assignment is too long, add parentheses to move the assigned data to the line below:
```cpp
HereIsMyType here_is_my_name_variable = (
	here_is_my_really_long_name_for_data
);
```
