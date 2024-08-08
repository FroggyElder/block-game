#include "touch_screen.h"
using namespace Tsc;

Tscreen::Tscreen(const char* path) {
    //initiate status
    this->status.pressed = false;
    this->status.x = -1;
    this->status.y = -1;
    this->fd = open(path,O_RDONLY);
    if (this->fd<0) throw std::runtime_error("failed to open touch screen file");
}

Tscreen::~Tscreen()
{
    close(this->fd);
}

int Tscreen::updateStatus () {
    std::lock_guard<std::mutex> lock(m_lock);

    struct input_event input_struct;

    //loop read the struct tscreen till info appears.
    while (1) {
        if(read(this->fd,&input_struct,sizeof(struct input_event))<0) {
            perror("failed to access touch screen.");
            goto general_error;
        }

        if(input_struct.type==EV_KEY&&input_struct.code==BTN_TOUCH){
            if (input_struct.value==0) this->status.pressed = false;
            if (input_struct.value>0) this->status.pressed = true;
            break;
        }

        if(input_struct.type==EV_ABS) {
            if (input_struct.code==ABS_X) this->status.x = input_struct.value*X_TOUCH_TO_LCD;
            if (input_struct.code==ABS_Y) this->status.y = input_struct.value*Y_TOUCH_TO_LCD;
            break;
        }
    }

    //if all is good
    return 0;

general_error:
init_error:
    return -1;
}

Tscreen::Action Tscreen::getAction ()
{
    while (!this->status.pressed) 
    {
        this->updateStatus();
    }
    int x0 = this->status.x;
    int y0 = this->status.y;

    while (this->status.pressed) 
    {
        this->updateStatus();
    }
    int x1 = this->status.x;
    int y1 = this->status.y;

    if (x0==x1&&y0==y1) return Action::tap;

    int x_drift = x1>x0 ? x1-x0 : x0 - x1;
    int y_drift = y1>y0 ? y1-y0 : y0 - y1;

    if (x_drift>y_drift) return x1>x0 ? Action::right : Action::left;
    else return y1>y0 ? Action::down : Action::up;
}