#include <iostream>
#include "PlaybackPlayer.h"
#include "UtilsTick.h"

PlaybackPlayer player;

int main(int argc, char* argv[]) {

    player.start();
    while(1)
    {
        LOGD("time {} ms", UtilsTick::getMsTick());
        usleep(1000000); // 1000ms
    }
    return 0;
}
