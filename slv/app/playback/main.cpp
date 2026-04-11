#include <iostream>
#include "PlaybackPlayer.h"
#include "UtilsTick.h"

PlaybackPlayer player;
void handleCmd(const std::string& cmd, int& sec);

int main(int argc, char* argv[]) {
    int sec = 0;
    player.start();
    while(1)
    {
        // LOGD("time {} ms", UtilsTick::getMsTick());
        // usleep(1000000); // 1000ms
        std::string s;
        getline(std::cin, s);
        LOGI("Ban vua nhap: {}", s);
        handleCmd(s, sec);
    }
    return 0;
}

void handleCmd(const std::string& cmd, int& sec)
{
    if(cmd == "exit")
    {
        LOGD("Exiting ....");
        exit(0);
    }
    else if(cmd == "skp")
    {
        sec=55;
        LOGD("Seek to {}s....", sec);
        player.Seek(sec);
    }
    else if(cmd == "skm")
    {
        sec=15;
        LOGD("Seek to {}s....", sec);
        player.Seek(sec);
    }
    else
    {
        LOGD("Exit unexpected ....");
        exit(1);
    }
}
