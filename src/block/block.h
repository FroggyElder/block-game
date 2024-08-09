#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <list>
#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <thread>

#include "screenObj.h"
#include "lcd.h"
#include "observer.h"

namespace board
{

constexpr int boardWidth{40};
constexpr int boardHeight{24};

enum class blockType
{
    empty,blocked,falling
};

enum class objType
{
    loong,square,chair,hook,Rchair,Rhook,triangle
};
objType getRandomType();
class fallingObj;

class BlockBoard: public screenObj,public obs::Subject
{
private:
    //lock
    mutable std::mutex m_lcdLock;
    //board of blocks
    int m_blocks[boardWidth][boardHeight] {};
    //current object
    fallingObj* m_obj{};
    //next object
    fallingObj* m_nextObj{};
    //current score
    int m_score{};

public:
    BlockBoard(LcdDevice* lcd);
    ~BlockBoard();

    int getScore () {return m_score;}

    std::mutex& getLock() {return m_lcdLock;}

    bool fall();
    void fallThrough();
    void rotateRight();
    void rotateLeft();
    void moveLeft();
    void moveRight();

    bool isLineFull(int y) const;
    void deleteLine(int y);
    void checkAndDeleteFullLines();

    void switchNext ();

    void setSrc() {screenObj::setSrc("assets/block.png");}

    void resetBoard();
    void paint();

    void setBlock (int x,int y,blockType type) {m_blocks[x][y]=(unsigned char)type;}
    bool isBlocked(int x, int y) const;
};

class fallingObj: public screenObj
{
private:
    std::list<std::pair<int,int>> m_blocks{};
    std::pair<int,int> m_center{};
    BlockBoard* m_board{};

public:
    fallingObj(BlockBoard* board);
    ~fallingObj();

    void detachBoard() {
        //The lock guard seems dangerous, keep it in check
        if (m_board) {
            std::lock_guard<std::mutex> lock(m_board->getLock());
            m_board = nullptr;
        }
    }
    void setSrc() {screenObj::setSrc("assets/block.png");}
    void paint();

    bool isOutOfUpperBound() const;

    bool fall();
    void fallThrough();
    void rotateRight();
    void rotateLeft();
    void moveLeft();
    void moveRight();
};

class ScoreObserver : public obs::Observer {
private:
    screenObj* scoreDisplay;
    board::BlockBoard* board;

public:
    ScoreObserver(screenObj* display, board::BlockBoard* b) 
        : scoreDisplay(display), board(b) 
        {
            board->addObserver(this);
        }

    void update() override {
        std::string scoreStr = std::to_string(board->getScore());
        scoreDisplay->setSrc(scoreStr.c_str(), 100, scoreStr.length());
    }
};

}
#endif