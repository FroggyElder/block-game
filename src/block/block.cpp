#include "block.h"

namespace board
{

objType getRandomType()
{
    int randIndex = std::rand() % 7; // There are now 7 types of objects
    return static_cast<objType>(randIndex);
}

fallingObj::fallingObj(BlockBoard* board)
    :m_board{board},screenObj{board}
{
    int startX = boardWidth / 2;
    int startY = -1; // just above the board

    switch (getRandomType())
    {
    case objType::loong:
        m_blocks = {{startX - 1, startY}, {startX, startY}, {startX + 1, startY}, {startX + 2, startY}};
        m_center = {startX, startY};
        break;
    case objType::square:
        m_blocks = {{startX, startY}, {startX + 1, startY}, {startX, startY - 1}, {startX + 1, startY - 1}};
        m_center = {startX, startY};
        break;
    case objType::chair:
        m_blocks = {{startX, startY}, {startX -1, startY}, {startX - 1, startY - 1}, {startX - 2, startY - 1}};
        m_center = {startX -1, startY};
        break;
    case objType::hook:
        m_blocks = {{startX, startY}, {startX, startY - 1}, {startX, startY - 2}, {startX + 1, startY - 2}};
        m_center = {startX, startY - 1};
        break;
    case objType::Rchair:
        m_blocks = {{startX, startY}, {startX + 1, startY}, {startX + 1, startY - 1}, {startX + 2, startY - 1}};
        m_center = {startX + 1, startY - 1};
        break;
    case objType::Rhook:
        m_blocks = {{startX, startY}, {startX, startY - 1}, {startX - 1, startY - 1}, {startX - 1, startY - 2}};
        m_center = {startX, startY - 1};
        break;
    case objType::triangle:
        m_blocks = {{startX, startY}, {startX + 1, startY}, {startX + 2, startY}, {startX + 1, startY - 1}};
        m_center = {startX + 1, startY};
        break;
    default:
        throw std::invalid_argument("Unknown object type");
    }
    setSrc();
}

fallingObj::~fallingObj()
{
    if (m_board) {
        for (const auto& block : m_blocks)
        {
            if (block.second >= 0) // Only set blocks within the board boundaries
            {
                m_board->setBlock(block.first, block.second, blockType::blocked);
            }
        }
    }
    m_blocks.clear();
}

void fallingObj::paint()
{

    size_t blockWidth = m_w;
    size_t blockHeight = m_h;

    for (const auto& block : m_blocks)
    {
        if (block.second >= 0) // Only paint blocks within the board boundaries
        {
            m_lcd->paintMap(m_bitmap, blockWidth, blockHeight, block.first * blockWidth, block.second * blockHeight);
        }
    }

}

bool fallingObj::isOutOfUpperBound() const
{
    // Check if any block of the falling object is above the board
    for (const auto& block : m_blocks)
    {
        if (block.second < 0)
        {
            return true; // Out of upper bound
        }
    }
    return false; // All blocks are within bounds
}


bool fallingObj::fall()
{
    for (auto &block : m_blocks)
    {
        if (block.second+1>=boardHeight||m_board->isBlocked(block.first,block.second+1)) 
        {
            // m_board->switchNext();
            return false;
        }
    }
    for (auto &block : m_blocks)
    {
        ++block.second;
    }
    ++m_center.second;

    m_board->checkAndDeleteFullLines();

    return true;
}

void fallingObj::fallThrough()
{
    bool canFall = true;
    
    while (canFall)
    {
        for (const auto& block : m_blocks)
        {
            if (block.second + 1 >= boardHeight || m_board->isBlocked(block.first, block.second + 1))
            {
                canFall = false;
                break;
            }
        }

        if (canFall)
        {
            // Move the object down
            for (auto& block : m_blocks)
            {
                ++block.second;
            }
            ++m_center.second;
        }
    }

    // After falling, update the board and refresh the display
    m_board->checkAndDeleteFullLines();
    m_board->paint(); // Paint the updated board state
}


void fallingObj::rotateRight()
{
    std::list<std::pair<int, int>> newPositions;

    for (auto& block : m_blocks)
    {
        int x = block.first - m_center.first;
        int y = block.second - m_center.second;
        int newX = m_center.first + y;
        int newY = m_center.second - x;

        if (newX < 0 || newX >= boardWidth || newY >= boardHeight || m_board->isBlocked(newX, newY))
        {
            return; // Do not rotate if any new position is out of bounds or blocked
        }

        newPositions.push_back({newX, newY});
    }

    m_blocks = newPositions;
}

void fallingObj::rotateLeft()
{
    std::list<std::pair<int, int>> newPositions;

    for (auto& block : m_blocks)
    {
        int x = block.first - m_center.first;
        int y = block.second - m_center.second;
        int newX = m_center.first - y;
        int newY = m_center.second + x;

        if (newX < 0 || newX >= boardWidth || newY >= boardHeight || m_board->isBlocked(newX, newY))
        {
            return; // Do not rotate if any new position is out of bounds or blocked
        }

        newPositions.push_back({newX, newY});
    }

    m_blocks = newPositions;
}

void fallingObj::moveLeft()
{
    std::list<std::pair<int, int>> newPositions;

    for (auto& block : m_blocks)
    {
        int newX = block.first-1;
        int newY = block.second;

        if (newX < 0 || newX >= boardWidth || newY >= boardHeight || m_board->isBlocked(newX, newY))
        {
            return; // Do not move if any new position is out of bounds or blocked
        }

        newPositions.push_back({newX, newY});
    }

    m_center.first--;
    m_blocks = newPositions;
}

void fallingObj::moveRight()
{
    std::list<std::pair<int, int>> newPositions;

    for (auto& block : m_blocks)
    {
        int newX = block.first+1;
        int newY = block.second;

        if (newX < 0 || newX >= boardWidth || newY >= boardHeight || m_board->isBlocked(newX, newY))
        {
            return; // Do not move if any new position is out of bounds or blocked
        }

        newPositions.push_back({newX, newY});
    }

    m_center.first++;
    m_blocks = newPositions;
}

BlockBoard::BlockBoard(LcdDevice* lcd) 
    :screenObj{lcd}
,m_obj{new fallingObj{this}},m_nextObj{new fallingObj{this}}
{
    setSrc();
}

BlockBoard::~BlockBoard()
{
    delete m_obj;
    delete m_nextObj;
}

bool BlockBoard::fall()
{
    bool res{true};
    {
    std::lock_guard<std::mutex> lock(m_lcdLock);
    res = m_obj->fall();
    }
    if (!res) {
        if (m_obj->isOutOfUpperBound()) {
            resetBoard();
            return res;
        }
        // std::lock_guard<std::mutex> lock(m_lcdLock);
        switchNext();
    }

    paint();
    return res;
}

void BlockBoard::fallThrough()
{
    {
    std::lock_guard<std::mutex> lock(m_lcdLock);
    m_obj->fallThrough();
    }
    paint();
}

void BlockBoard::rotateLeft()
{
    {
    std::lock_guard<std::mutex> lock(m_lcdLock);
    m_obj->rotateLeft ();
    }
    paint();

}

void BlockBoard::rotateRight()
{
    {
    std::lock_guard<std::mutex> lock(m_lcdLock);
    m_obj->rotateRight ();
    }
    paint();
}

void BlockBoard::moveLeft()
{
    {
    std::lock_guard<std::mutex> lock(m_lcdLock);
    m_obj->moveLeft ();
    }
    paint();
}

void BlockBoard::moveRight()
{
    {
    std::lock_guard<std::mutex> lock(m_lcdLock);
    m_obj->moveRight ();
    }
    paint();
}

bool BlockBoard::isLineFull(int y) const
{
    for (int x = 0; x < boardWidth; ++x)
    {
        if (m_blocks[x][y] != static_cast<unsigned char>(blockType::blocked))
        {
            return false;
        }
    }
    return true;
}

void BlockBoard::deleteLine(int y)
{
    // Shift all lines above the deleted line down by one
    for (int row = y; row > 0; --row)
    {
        for (int col = 0; col < boardWidth; ++col)
        {
            m_blocks[col][row] = m_blocks[col][row - 1];
        }
    }

    // Clear the top line
    for (int col = 0; col < boardWidth; ++col)
    {
        m_blocks[col][0] = static_cast<unsigned char>(blockType::empty);
    }
}

void BlockBoard::checkAndDeleteFullLines()
{
    for (int y = 0; y < boardHeight; ++y)
    {
        if (isLineFull(y))
        {
            m_score++;
            deleteLine(y);
        }
    }
}

void BlockBoard::switchNext()
{
    std::lock_guard<std::mutex> lock(m_lcdLock);
    delete m_obj;
    m_obj = m_nextObj;
    m_nextObj = new fallingObj{this};
}

void BlockBoard::paint()
{
    m_lcd->clearScreen(0xff000000);

    size_t blockWidth = m_w;
    size_t blockHeight = m_h;

    for (int y = 0; y < boardHeight; ++y)
    {
        for (int x = 0; x < boardWidth; ++x)
        {
            if (isBlocked(x, y))
            {
                m_lcd->paintMap(m_bitmap, blockWidth, blockHeight, x * blockWidth, y * blockHeight);
            }
        }
    }

    m_obj->paint();
    m_lcd->refresh();
}

void BlockBoard::resetBoard()
{
    fallingObj* oldObj;
    fallingObj* oldNextObj;
    {
        std::lock_guard<std::mutex> lock(m_lcdLock);

        // Clear all blocks on the board
        for (int y = 0; y < boardHeight; ++y)
        {
            for (int x = 0; x < boardWidth; ++x)
            {
                m_blocks[x][y] = static_cast<unsigned char>(blockType::empty);
            }
        }

        // Delete current and next falling objects
        oldObj = m_obj;
        oldNextObj = m_nextObj;

        // Create new falling objects
        m_obj = new fallingObj(this);
        m_nextObj = new fallingObj(this);
    }

    // Deleting old objects outside the lock to avoid deadlock
    if (oldObj) {
        oldObj->detachBoard();
        delete oldObj;
        oldObj = nullptr;
    }
    if (oldNextObj) {
        oldNextObj->detachBoard();
        delete oldNextObj;
        oldNextObj = nullptr;
    }
    // Refresh the display
    paint();
}


bool BlockBoard::isBlocked(int x, int y) const
{
    if (x < 0 || x >= boardWidth || y >= boardHeight)
    {
        return true;
    }

    if (y<0) return false;

    return m_blocks[x][y] == static_cast<unsigned char>(blockType::blocked);
}

}