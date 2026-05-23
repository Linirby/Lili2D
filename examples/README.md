# Examples

## Build and run examples

Each example folder will have a `src/` folder with code example, a
CMakeLists.txt as an example of how to add the lib to your project.
And to easily use the `CMakeLists.txt` (on linux) there's 3 bash scripts:

- `run.sh` to run the build example
- `build.sh` to build the example
- `build_and_run.sh` to do both

Bash scripts need to be executed at the root of the example folder/ (where
scripts are placed).

## Smart Pointer mention

I'll use the smart pointer `unique_ptr` during those examples. If you're not
used to them, don't panic.

Here are some basics rules to easily replace my smart pointers (`unique_ptr`,
`shared_ptr`, `weak_ptr`) to raw pointer `*` (in your mind or directly in the
code):

1. Replace every `var = std::make_unique<type>(args);` by `var = new type(args);`
and check where this variable goes out of scope and put `delete var` right
before it goes out of the scope.
2. Replace every `std::unique_ptr<type> var;` by `type *var`.
3. Most of the time if a `unique_ptr` is in the `App` class, for a `*` the
`delete` of this pointer could be place after the `while (running)` loop in
`App::run()`.
4. When I use `.get()` on a `unique_ptr` is to get the raw pointer version, so
if you are using `*` you can remove the `.get()`.
