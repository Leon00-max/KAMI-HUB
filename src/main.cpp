#include "core/Application.hpp"

#include <filesystem>
#include <string>
#include <system_error>

namespace {

// Prefer ./assets (running from the repo), else assets next to the executable
// (the build copies them there, e.g. when launched from Explorer)
std::filesystem::path findAssetRoot(const char* argv0) {
    std::error_code error;
    if (std::filesystem::is_directory("assets", error)) {
        return "assets";
    }
    const std::filesystem::path besideExecutable = std::filesystem::path(argv0).parent_path() / "assets";
    if (std::filesystem::is_directory(besideExecutable, error)) {
        return besideExecutable;
    }
    return "assets";
}

} // namespace

int main(int argc, char* argv[]) {
    bool smokeTest = false;
    Application::Options options;
    options.assetRoot = findAssetRoot(argc > 0 ? argv[0] : "");
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
