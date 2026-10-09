# KAMI Hub

KAMI Hub is a desktop application foundation built in modern C++20 with SFML. The first milestone establishes the architecture for a scalable arcade platform, including a windowed application shell, centralized resource management, a scene system, and a reusable UI button abstraction.

## Project goals
- Provide a professional desktop application foundation for future arcade games.
- Keep the source tree modular and easy to extend.
- Use CMake from day one so the project remains cross-platform.

## Build instructions
Run all commands from the repository root (`KAMI Hub`).

### Windows verified path
This repository was verified with Visual Studio Build Tools 2019, CMake, and Ninja.

1. Configure the Visual C++ environment:
   ```cmd
   "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=x64
   ```
2. Configure the project:
   ```cmd
   cmake -S . -B build_cl_ninja -G Ninja -DCMAKE_MAKE_PROGRAM="%CD%\.tools\ninja\ninja.exe" -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
   ```
3. Build the executable:
   ```cmd
   cmake --build build_cl_ninja
   ```
4. Run the application:
   ```cmd
   build_cl_ninja\arcade_hub.exe
   ```
5. Run the automated startup smoke test:
   ```cmd
   build_cl_ninja\arcade_hub.exe --smoke-test
   ```

### Generic CMake path
1. Configure the project:
   ```bash
   cmake -S . -B build
   ```
2. Build the executable:
   ```bash
   cmake --build build --config Release
   ```
3. Run the application:
   ```bash
   ./build/arcade_hub
   ```
   On Windows with single-config generators such as MinGW Makefiles:
   ```powershell
   .\build\arcade_hub.exe
   ```

## Design preview
`arcade_hub --preview` fills the board, next piece and stats with a sample mid-game state so the screen can be compared against the concept art. A normal launch shows the empty hub.

## Third-party assets
- Orbitron and Exo 2 fonts: SIL Open Font License 1.1 (`assets/fonts/OFL-*.txt`).

## Architecture overview
- `include/core` contains the core application shell.
- `include/managers` holds centralized asset and resource access.
- `include/scenes` is responsible for scene abstractions.
- `include/ui` provides reusable UI primitives such as buttons.
- `src` mirrors the include tree with implementation files.
- `assets/fonts`, `assets/images`, and `assets/icons` contain runtime assets loaded through `ResourceManager`.

## Dependencies
- C++20 compiler
- CMake 3.20+
- SFML 2.6.1

CMake first looks for an installed SFML package. If SFML is not installed, it downloads SFML 2.6.1 with CMake `FetchContent` during configuration.

## Repository checks
From the project root, these commands should run without repository errors:
```bash
git status
git branch
git remote -v
```
