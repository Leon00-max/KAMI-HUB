#include "core/Application.hpp"

#include <string>

int main(int argc, char* argv[]) {
    bool smokeTest = false;
    Application::Options options;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--smoke-test") {
            smokeTest = true;
        } else if (arg == "--preview") {
            options.preview = true;
        }
    }

    Application app(options);
    app.run(smokeTest ? 5U : 0U);
    return 0;
}
