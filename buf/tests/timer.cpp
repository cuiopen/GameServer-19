
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/Timer.h"

using namespace buf;

int main(int argc, char* argv[])
{
    TimerThread tt;
    Timer* t = 0;
#if 1
    tt.addTimer(new Timer(3000, 10));
    tt.addTimer(t = new Timer(4000));
    tt.addTimer(t = new Timer(5000));
    tt.addTimer(t = new Timer(6000));
    tt.addTimer(t = new Timer(7000));
    tt.addTimer(t = new Timer(8000));
    tt.addTimer(t = new Timer(9000));
#endif
#if 0
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
    tt.addTimer(t = new Timer(2000));
#endif
    //tt.delTimer(t);
    tt.start();
    tt.join();
	return 0;
}
