#include "main.h"

std::mutex fallMutex{};

void delay(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void fallThread (board::BlockBoard* board)
{
    while (1) 
    {
        fallMutex.lock();
        board->fall();
        fallMutex.unlock();
        delay(150);
    }
}

void fullexit (void* pauseMenu)
{
    LcdDevice* lcd = ((menu::Menu*)pauseMenu)->getLcd();

    lcd->clearScreen(ARGB_BLACK);
    lcd->refresh();

    exit(0);
}

void mutexUnlock (void* mutex)
{
    ((std::mutex*)mutex)->unlock();
}

void boardRestart (void* board)
{
    ((board::BlockBoard*)board)->resetBoard();
    fallMutex.unlock();
}

void seedInit ()
{
    // Get the current time in milliseconds since the epoch
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(millis));
}

int main ()
{
    seedInit();

    LcdDevice lcd{LCD0};
    board::BlockBoard board{&lcd};
    Tscreen ts(TS0);

    menu::Menu pauseMenu{&lcd,&ts};
    pauseMenu.setSrc("assets/pauseScreen.png");

    screenObj exitBtn(&pauseMenu,700,0);
    exitBtn.setSrc("assets/logout.png");
    pauseMenu.registerCallback(&exitBtn,fullexit,&pauseMenu);

    screenObj resumeBtn{&pauseMenu,300,190};
    resumeBtn.setSrc("assets/resume.png");
    pauseMenu.registerCallback(&resumeBtn,mutexUnlock,&fallMutex);

    screenObj restartBtn{&pauseMenu,400,190};
    restartBtn.setSrc("assets/restart.png");
    pauseMenu.registerCallback(&restartBtn,boardRestart,&board);


    std::thread t1{fallThread,&board};
    while (1) 
    {
        Tscreen::Action ac = ts.getAction();
        switch (ac){
            case Tscreen::Action::left:
                board.moveLeft();
                break;
            case Tscreen::Action::right:
                board.moveRight();
                break;
            case Tscreen::Action::up:
                board.rotateRight();
                break;
            case Tscreen::Action::down:
                board.fallThrough();
                break;
            case Tscreen::Action::tap:
                fallMutex.lock();
                pauseMenu.paint();
                lcd.refresh();
                pauseMenu.waitForClick();
                break;
            default:
                break;
        }
    }
}