#include <cstdio>
#include "tcpgateL.h"

#define SIZEMASS 100000
int main()
{
    printf("hello from GATE_DTS_EMT_1_0!\n");

    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    char* buf = new char[SIZEMASS*4];
    float f;

    ConfigReader* config = new ConfigReader();
    config->ReadConfigFile("conf.txt");
    std::cout << config->NumberItem() << std::endl;
    std::list<TCPUnit*> tcpunit;
    for (int i = 0; i < config->NumberItem(); i++)
    {
        tcpunit.push_back(TCPUnit::create_tcp_unit(config->ReadConfigItem(i)));
    }

    for (auto item = tcpunit.begin(); item != tcpunit.end(); item++)
    {
        (*item)->initmutexbuffer(mut);
        (*item)->initpointbuffer(buf);
    }

    next:

    pthread_mutex_lock(&mut);
    //f = *(float*)buf;
    //std::cout << f << "  " << *(float*)(buf+SIZEMASS*4-4) <<std::endl;
    pthread_mutex_unlock(&mut);
    usleep(100000);

    goto next;

    char s;
    std::cin >> s;
    return 0;
}