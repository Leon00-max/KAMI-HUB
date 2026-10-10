#include "games/tetris/Tetris.hpp"

#include <algorithm>
#include <cmath>

namespace tetris {

namespace {

// Spawn orientation of each piece inside its bounding box (y grows downward)
const Shape BaseShapes[PieceTypeCount] = {
    Shape{{{0, 1}, {1, 1}, {2, 1}, {3, 1}}}, // I
    Shape{{{1, 0}, {2, 0}, {1, 1}, {2, 1}}}, // O
    Shape{{{1, 0}, {0, 1}, {1, 1}, {2, 1}}}, // T
    Shape{{{1, 0}, {2, 0}, {0, 1}, {1, 1}}}, // S
    Shape{{{0, 0}, {1, 0}, {1, 1}, {2, 1}}}, // Z
    Shape{{{0, 0}, {0, 1}, {1, 1}, {2, 1}}}, // J
    Shape{{{2, 0}, {0, 1}, {1, 1}, {2, 1}}}, // L
};

int boxSize(PieceType type) {
    switch (type) {
        case PieceType::I:
        case PieceType::O:
            return 4;
        default:
            return 3;
    }
}

// SRS kick offsets, already converted to y-down. Indexed by transition:
// 0->1, 1->0, 1->2, 2->1, 2->3, 3->2, 3->0, 0->3
const Cell KicksJLSTZ[8][5] = {
    {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
    {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
    {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
    {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
    {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
    {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
    {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
    {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
};

const Cell KicksI[8][5] = {
    {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}},
    {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}},
    {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}},
    {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}},
    {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}},
    {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}},
    {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}},
    {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}},
};

int kickIndex(int from, int to) {
    if (from == 0 && to == 1) return 0;
    if (from == 1 && to == 0) return 1;
    if (from == 1 && to == 2) return 2;
    if (from == 2 && to == 1) return 3;
    if (from == 2 && to == 3) return 4;
    if (from == 3 && to == 2) return 5;
    if (from == 3 && to == 0) return 6;
    return 7; // 0 -> 3
}

constexpr int LinePoints[5] = {0, 100, 300, 500, 800};

} // namespace

Shape shapeOf(PieceType type, int rotation) {
    Shape shape = BaseShapes[static_cast<int>(type)];
    if (type == PieceType::O) {
        return shape;
    }
    const int n = boxSize(type);
    for (int r = 0; r < ((rotation % 4) + 4) % 4; ++r) {
        for (Cell& cell : shape) {
            cell = Cell{n - 1 - cell.y, cell.x};
        }
    }
    return shape;
}

BagRandomizer::BagRandomizer(std::uint32_t seed) : m_rng(seed) {
    for (int i = 0; i < PieceTypeCount; ++i) {
        m_bag[static_cast<std::size_t>(i)] = static_cast<PieceType>(i);
    }
}

PieceType BagRandomizer::next() {
    if (m_index >= PieceTypeCount) {
        std::shuffle(m_bag.begin(), m_bag.end(), m_rng);
        m_index = 0;
    }
    return m_bag[static_cast<std::size_t>(m_index++)];
}

Game::Game(std::uint32_t seed) : m_bag(seed) {
    m_next = m_bag.next();
}

void Game::start() {
    m_cells.fill(0);
    m_score = 0;
    m_lines = 0;
    m_level = 1;
    m_softDrop = false;
    m_clearingRows.clear();
    m_state = State::Playing;
    spawn(m_next);
}

void Game::togglePause() {
    if (m_state == State::Paused) {
        m_state = m_stateBeforePause;
    } else if (m_state == State::Playing || m_state == State::Clearing) {
        m_stateBeforePause = m_state;
        m_state = State::Paused;
    }
}

float Game::gravityInterval(int level) {
    // Tetris guideline curve, floored so very high levels stay playable
    const float base = 0.8f - (level - 1) * 0.007f;
    return std::max(0.01f, std::pow(base, static_cast<float>(level - 1)));
}

void Game::update(float deltaTime) {
    if (m_state == State::Clearing) {
        m_clearTimer -= deltaTime;
        if (m_clearTimer <= 0.f) {
            collapseClearedRows();
            m_state = State::Playing;
            spawn(m_next);
        }
        return;
    }
    if (m_state != State::Playing) {
        return;
    }

    const float natural = gravityInterval(m_level);
    const float interval = m_softDrop ? std::max(0.02f, natural / 20.f) : natural;

    bool fell = false;
    m_gravityTimer += deltaTime;
    while (m_gravityTimer >= interval) {
        m_gravityTimer -= interval;
        if (!tryMove(0, 1)) {
            m_gravityTimer = 0.f;
            break;
        }
        fell = true;
        m_lockTimer = 0.f;
        if (m_softDrop) {
            m_score += 1;
        }
    }

    if (!fits(m_piece.type, m_piece.rotation, m_piece.x, m_piece.y + 1)) {
        // Only time spent resting counts; a long frame must not skip the delay
        m_lockTimer += fell ? m_gravityTimer : deltaTime;
        if (m_lockTimer >= LockDelay) {
            lockPiece();
        }
    }
}

bool Game::moveLeft() {
    if (m_state != State::Playing || !tryMove(-1, 0)) {
        return false;
    }
    onPlayerMoved();
    return true;
}

bool Game::moveRight() {
    if (m_state != State::Playing || !tryMove(1, 0)) {
        return false;
    }
    onPlayerMoved();
    return true;
}

bool Game::rotateClockwise() {
    if (m_state != State::Playing || !tryRotate(1)) {
        return false;
    }
    onPlayerMoved();
    return true;
}

bool Game::rotateCounterClockwise() {
    if (m_state != State::Playing || !tryRotate(-1)) {
        return false;
    }
    onPlayerMoved();
    return true;
}

void Game::hardDrop() {
    if (m_state != State::Playing) {
        return;
    }
    const int distance = ghostY() - m_piece.y;
    m_piece.y += distance;
    m_score += distance * 2;
    lockPiece();
}

std::uint8_t Game::cellAt(int x, int y) const {
    if (x < 0 || x >= Width || y < 0 || y >= Height) {
        return 0;
    }
    return m_cells[static_cast<std::size_t>(y * Width + x)];
}

int Game::ghostY() const {
    int y = m_piece.y;
    while (fits(m_piece.type, m_piece.rotation, m_piece.x, y + 1)) {
        ++y;
    }
    return y;
}

float Game::clearProgress() const {
    if (m_state != State::Clearing && !(m_state == State::Paused && m_stateBeforePause == State::Clearing)) {
        return 0.f;
    }
    return std::clamp(1.f - m_clearTimer / ClearDuration, 0.f, 1.f);
}

void Game::setCell(int x, int y, std::uint8_t value) {
    if (x >= 0 && x < Width && y >= 0 && y < Height) {
        m_cells[static_cast<std::size_t>(y * Width + x)] = value;
    }
}

void Game::setActivePiece(PieceType type) {
    m_piece = ActivePiece{type, 0, 3, 0};
}

bool Game::fits(PieceType type, int rotation, int x, int y) const {
    for (const Cell& cell : shapeOf(type, rotation)) {
        const int bx = x + cell.x;
        const int by = y + cell.y;
        if (bx < 0 || bx >= Width || by >= Height) {
            return false;
        }
        if (by >= 0 && cellAt(bx, by) != 0) {
            return false;
        }
    }
    return true;
}

bool Game::tryMove(int dx, int dy) {
    if (!fits(m_piece.type, m_piece.rotation, m_piece.x + dx, m_piece.y + dy)) {
        return false;
    }
    m_piece.x += dx;
    m_piece.y += dy;
    return true;
}

bool Game::tryRotate(int direction) {
    if (m_piece.type == PieceType::O) {
        return false;
    }
    const int from = m_piece.rotation;
    const int to = (from + direction + 4) % 4;
    const auto& kicks = m_piece.type == PieceType::I ? KicksI : KicksJLSTZ;
    for (const Cell& kick : kicks[kickIndex(from, to)]) {
        if (fits(m_piece.type, to, m_piece.x + kick.x, m_piece.y + kick.y)) {
            m_piece.rotation = to;
            m_piece.x += kick.x;
            m_piece.y += kick.y;
            return true;
        }
    }
    return false;
}

void Game::onPlayerMoved() {
    // Moving on the ground buys more time, up to a limit
    if (m_lockTimer > 0.f && m_lockResets < MaxLockResets) {
        m_lockTimer = 0.f;
        ++m_lockResets;
    }
}

void Game::lockPiece() {
    for (const Cell& cell : shapeOf(m_piece.type, m_piece.rotation)) {
        setCell(m_piece.x + cell.x, m_piece.y + cell.y, static_cast<std::uint8_t>(static_cast<int>(m_piece.type) + 1));
    }

    m_clearingRows.clear();
    for (int y = 0; y < Height; ++y) {
        bool full = true;
        for (int x = 0; x < Width && full; ++x) {
            full = cellAt(x, y) != 0;
        }
        if (full) {
            m_clearingRows.push_back(y);
        }
    }

    if (m_clearingRows.empty()) {
        spawn(m_next);
        return;
    }

    const int cleared = static_cast<int>(m_clearingRows.size());
    m_score += LinePoints[std::min(cleared, 4)] * m_level;
    m_lines += cleared;
    m_level = 1 + m_lines / 10;
    m_clearTimer = ClearDuration;
    m_state = State::Clearing;
}

void Game::collapseClearedRows() {
    // Rows are ascending; removing from the top keeps later indices valid
    for (int row : m_clearingRows) {
        for (int y = row; y > 0; --y) {
            for (int x = 0; x < Width; ++x) {
                setCell(x, y, cellAt(x, y - 1));
            }
        }
        for (int x = 0; x < Width; ++x) {
            setCell(x, 0, 0);
        }
    }
    m_clearingRows.clear();
}

void Game::spawn(PieceType type) {
    m_piece = ActivePiece{type, 0, 3, 0};
    m_next = m_bag.next();
    m_gravityTimer = 0.f;
    m_lockTimer = 0.f;
    m_lockResets = 0;
    ++m_pieceSerial;
    if (!fits(m_piece.type, m_piece.rotation, m_piece.x, m_piece.y)) {
        m_state = State::GameOver;
    }
}

} // namespace tetris
