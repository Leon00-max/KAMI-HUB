#include "core/Application.hpp"

#include <string>

int main(int argc, char* argv[]) {
    const bool smokeTest = argc > 1 && std::string(argv[1]) == "--smoke-test";

    Application app;
    app.run(smokeTest ? 5U : 0U);
    return 0;
}
