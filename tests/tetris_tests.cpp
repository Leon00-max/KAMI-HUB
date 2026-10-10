// Unit tests for the Tetris rules. Plain asserts keep the build free of
// third-party test frameworks; run with `ctest` or the executable directly.

#include <algorithm>
#include <cstdio>
#include <functional>
#include <set>
#include <string>
#include <vector>

#include "games/tetris/Tetris.hpp"

using tetris::Game;
using tetris::PieceType;

namespace {

int g_failures = 0;

void check(bool condition, const std::string& message, int line) {
    if (!condition) {
        std::printf("  FAILED (line %d): %s\n", line, message.c_str());
        ++g_failures;
    }
}

#define CHECK(cond) check((cond), #cond, __LINE__)

std::uint8_t id(PieceType type) {
    return static_cast<std::uint8_t>(static_cast<int>(type) + 1);
}

// Fill a row except for the given columns
void fillRow(Game& game, int y, std::initializer_list<int> gaps = {}) {
    for (int x = 0; x < Game::Width; ++x) {
        if (std::find(gaps.begin(), gaps.end(), x) == gaps.end()) {
            game.setCell(x, y, id(PieceType::O));
        }
    }
}

void startedGame(Game& game) {
    game.start();
    CHECK(game.state() == Game::State::Playing);
}

void testShapesHaveFourDistinctCells() {
    for (int t = 0; t < tetris::PieceTypeCount; ++t) {
        for (int r = 0; r < 4; ++r) {
            const auto shape = tetris::shapeOf(static_cast<PieceType>(t), r);
            std::set<std::pair<int, int>> unique;
            for (const auto& cell : shape) {
                unique.insert({cell.x, cell.y});
            }
            CHECK(unique.size() == 4);
        }
    }
}

void testRotatingFourTimesIsIdentity() {
    for (int t = 0; t < tetris::PieceTypeCount; ++t) {
        const auto type = static_cast<PieceType>(t);
        const auto a = tetris::shapeOf(type, 0);
        const auto b = tetris::shapeOf(type, 4);
        for (std::size_t i = 0; i < 4; ++i) {
            CHECK(a[i].x == b[i].x && a[i].y == b[i].y);
        }
    }
}

void testBagDealsEachPieceOncePerSeven() {
    tetris::BagRandomizer bag(1234);
    for (int round = 0; round < 5; ++round) {
        std::set<PieceType> seen;
        for (int i = 0; i < 7; ++i) {
            seen.insert(bag.next());
        }
        CHECK(seen.size() == 7);
    }
}

void testStartSpawnsPieceInHiddenRows() {
    Game game(1);
    startedGame(game);
    CHECK(game.activePiece().y == 0);
    CHECK(game.activePiece().x == 3);
    CHECK(game.score() == 0);
    CHECK(game.level() == 1);
}

void testMovesStopAtWalls() {
    Game game(2);
    startedGame(game);
    game.setActivePiece(PieceType::O);
    int moves = 0;
    while (game.moveLeft()) {
        ++moves;
    }
    CHECK(moves == 4); // O occupies box columns 1-2, starting at board column 4
    CHECK(!game.moveLeft());
    while (game.moveRight()) {
    }
    CHECK(game.activePiece().x == Game::Width - 3);
}

void testGravityMovesPieceDown() {
    Game game(3);
    startedGame(game);
    const int startY = game.activePiece().y;
    game.update(Game::gravityInterval(1) + 0.001f);
    CHECK(game.activePiece().y == startY + 1);
}

void testHardDropLocksAndScores() {
    Game game(4);
    startedGame(game);
    game.setActivePiece(PieceType::O);
    const unsigned int serial = game.pieceSerial();
    const int distance = game.ghostY() - game.activePiece().y;
    game.hardDrop();
    CHECK(game.score() == distance * 2);
    CHECK(game.pieceSerial() == serial + 1);
    CHECK(game.cellAt(4, Game::Height - 1) == id(PieceType::O));
    CHECK(game.cellAt(5, Game::Height - 2) == id(PieceType::O));
}

void testSingleLineClear() {
    Game game(5);
    startedGame(game);
    fillRow(game, Game::Height - 1, {4, 5});
    game.setActivePiece(PieceType::O); // Fills columns 4-5
    game.hardDrop();
    CHECK(game.state() == Game::State::Clearing);
    CHECK(game.clearingRows().size() == 1);
    CHECK(game.lines() == 1);

    const int dropScore = game.score() - 100;
    CHECK(dropScore > 0);

    game.update(Game::ClearDuration + 0.01f);
    CHECK(game.state() == Game::State::Playing);
    // The O's upper half falls into the cleared row
    CHECK(game.cellAt(4, Game::Height - 1) == id(PieceType::O));
    CHECK(game.cellAt(0, Game::Height - 1) == 0);
}

void testTetrisScoresEightHundredTimesLevel() {
    Game game(6);
    startedGame(game);
    for (int i = 1; i <= 4; ++i) {
        fillRow(game, Game::Height - i, {0});
    }
    game.setActivePiece(PieceType::I);
    CHECK(game.rotateClockwise()); // Vertical I
    while (game.moveLeft()) {
    }
    const int before = game.score();
    game.hardDrop();
    CHECK(game.lines() == 4);
    CHECK(game.score() - before >= 800);
    game.update(Game::ClearDuration + 0.01f);
    for (int x = 0; x < Game::Width; ++x) {
        CHECK(game.cellAt(x, Game::Height - 1) == 0);
    }
}

void testLevelRisesEveryTenLines() {
    Game game(7);
    startedGame(game);
    for (int round = 0; round < 10; ++round) {
        fillRow(game, Game::Height - 1, {0});
        game.setActivePiece(PieceType::I);
        game.rotateClockwise();
        while (game.moveLeft()) {
        }
        game.hardDrop();
        game.update(Game::ClearDuration + 0.01f);
        // Clear the leftover I cells so each round clears exactly one row
        for (int y = 0; y < Game::Height; ++y) {
            game.setCell(0, y, 0);
        }
    }
    CHECK(game.lines() == 10);
    CHECK(game.level() == 2);
}

void testWallKickRotatesAgainstWall() {
    Game game(8);
    startedGame(game);
    game.setActivePiece(PieceType::T);
    game.rotateClockwise(); // Points right; box column 0 is empty
    while (game.moveLeft()) {
    }
    // Rotating back would poke out of the left wall without a kick
    CHECK(game.rotateCounterClockwise());
    for (const auto& cell : tetris::shapeOf(PieceType::T, game.activePiece().rotation)) {
        CHECK(game.activePiece().x + cell.x >= 0);
    }
}

void testLockDelayWaitsBeforeLocking() {
    Game game(9);
    startedGame(game);
    game.setActivePiece(PieceType::O);
    while (game.activePiece().y < game.ghostY()) {
        game.update(Game::gravityInterval(1) + 0.001f);
    }
    const unsigned int serial = game.pieceSerial();
    game.update(Game::LockDelay * 0.5f);
    CHECK(game.pieceSerial() == serial); // Still sliding
    game.update(Game::LockDelay);
    CHECK(game.pieceSerial() == serial + 1);
}

void testPauseFreezesGravity() {
    Game game(10);
    startedGame(game);
    const int y = game.activePiece().y;
    game.togglePause();
    CHECK(game.state() == Game::State::Paused);
    game.update(5.f);
    CHECK(game.activePiece().y == y);
    CHECK(!game.moveLeft());
    game.togglePause();
    CHECK(game.state() == Game::State::Playing);
}

void testBlockedSpawnEndsGame() {
    Game game(11);
    startedGame(game);
    for (int y = 0; y < Game::Height; ++y) {
        fillRow(game, y, {(y % 2) * 9}); // Never a full row
    }
    game.setActivePiece(PieceType::O);
    game.hardDrop();
    CHECK(game.state() == Game::State::GameOver);
}

void testSoftDropScoresPerRow() {
    Game game(12);
    startedGame(game);
    game.setSoftDrop(true);
    const int y = game.activePiece().y;
    game.update(0.1f);
    const int rows = game.activePiece().y - y;
    CHECK(rows > 0);
    CHECK(game.score() == rows);
}

} // namespace

int main() {
    const std::vector<std::pair<const char*, std::function<void()>>> tests = {
        {"shapes have four distinct cells", testShapesHaveFourDistinctCells},
        {"rotating four times is identity", testRotatingFourTimesIsIdentity},
        {"bag deals each piece once per seven", testBagDealsEachPieceOncePerSeven},
        {"start spawns piece in hidden rows", testStartSpawnsPieceInHiddenRows},
        {"moves stop at walls", testMovesStopAtWalls},
        {"gravity moves piece down", testGravityMovesPieceDown},
        {"hard drop locks and scores", testHardDropLocksAndScores},
        {"single line clear", testSingleLineClear},
        {"tetris scores 800 x level", testTetrisScoresEightHundredTimesLevel},
        {"level rises every ten lines", testLevelRisesEveryTenLines},
        {"wall kick rotates against wall", testWallKickRotatesAgainstWall},
        {"lock delay waits before locking", testLockDelayWaitsBeforeLocking},
        {"pause freezes gravity", testPauseFreezesGravity},
        {"blocked spawn ends game", testBlockedSpawnEndsGame},
        {"soft drop scores per row", testSoftDropScoresPerRow},
    };

    for (const auto& [name, test] : tests) {
        const int before = g_failures;
        test();
        std::printf("%s %s\n", g_failures == before ? "[pass]" : "[FAIL]", name);
    }

    std::printf("\n%d failure(s)\n", g_failures);
    return g_failures == 0 ? 0 : 1;
}
