#pragma once

#include <array>
#include <cstdint>
#include <random>
#include <vector>

/**
 * Tetris rules with no rendering or input dependencies.
 *
 * Follows the modern guideline basics: 10x20 well with two hidden spawn
 * rows, SRS rotation with wall kicks, 7-bag randomizer, lock delay with
 * move reset, and level-based gravity. Drive it with update(dt) and the
 * action methods; read the board and piece state back for drawing.
 */
namespace tetris {

enum class PieceType : std::uint8_t { I, O, T, S, Z, J, L };

constexpr int PieceTypeCount = 7;

struct Cell {
    int x;
    int y;
};

using Shape = std::array<Cell, 4>;

// Cells of a piece in its bounding box for a rotation state (0..3, clockwise)
Shape shapeOf(PieceType type, int rotation);

// Shuffles all seven pieces, deals them, repeats
class BagRandomizer {
public:
    explicit BagRandomizer(std::uint32_t seed);
    PieceType next();

private:
    std::mt19937 m_rng;
    std::array<PieceType, PieceTypeCount> m_bag{};
    int m_index = PieceTypeCount;
};

class Game {
public:
    static constexpr int Width = 10;
    static constexpr int VisibleHeight = 20;
    static constexpr int HiddenRows = 2;
    static constexpr int Height = VisibleHeight + HiddenRows;

    static constexpr float LockDelay = 0.5f;
    static constexpr int MaxLockResets = 15;
    static constexpr float ClearDuration = 0.4f;

    enum class State { Ready, Playing, Paused, Clearing, GameOver };

    struct ActivePiece {
        PieceType type = PieceType::T;
        int rotation = 0;
        int x = 0; // Board column of the bounding box's left edge
        int y = 0; // Board row of the bounding box's top edge (0 = top hidden row)
    };

    explicit Game(std::uint32_t seed = std::random_device{}());

    void start();
    void togglePause();
    void update(float deltaTime);

    // Player actions; each returns whether it changed anything
    bool moveLeft();
    bool moveRight();
    bool rotateClockwise();
    bool rotateCounterClockwise();
    void hardDrop();
    void setSoftDrop(bool active) { m_softDrop = active; }

    State state() const { return m_state; }
    int score() const { return m_score; }
    int lines() const { return m_lines; }
    int level() const { return m_level; }

    // 0 = empty, otherwise PieceType + 1
    std::uint8_t cellAt(int x, int y) const;
    const ActivePiece& activePiece() const { return m_piece; }
    PieceType nextPiece() const { return m_next; }
    // Row the active piece would land on with a hard drop
    int ghostY() const;
    // Increments whenever a new piece spawns (lets views reset animation)
    unsigned int pieceSerial() const { return m_pieceSerial; }

    // Rows being cleared and 0..1 progress, valid in State::Clearing
    const std::vector<int>& clearingRows() const { return m_clearingRows; }
    float clearProgress() const;

    // Seconds per row of natural gravity at a level
    static float gravityInterval(int level);

    // Test and tooling hooks
    void setCell(int x, int y, std::uint8_t value);
    void setActivePiece(PieceType type);

private:
    bool fits(PieceType type, int rotation, int x, int y) const;
    bool tryMove(int dx, int dy);
    bool tryRotate(int direction);
    void onPlayerMoved();
    void lockPiece();
    void collapseClearedRows();
    void spawn(PieceType type);

    std::array<std::uint8_t, Width * Height> m_cells{};
    BagRandomizer m_bag;
    ActivePiece m_piece;
    PieceType m_next = PieceType::T;

    State m_state = State::Ready;
    State m_stateBeforePause = State::Playing;
    int m_score = 0;
    int m_lines = 0;
    int m_level = 1;

    bool m_softDrop = false;
    float m_gravityTimer = 0.f;
    float m_lockTimer = 0.f;
    int m_lockResets = 0;
    float m_clearTimer = 0.f;
    std::vector<int> m_clearingRows;
    unsigned int m_pieceSerial = 0;
};

} // namespace tetris
