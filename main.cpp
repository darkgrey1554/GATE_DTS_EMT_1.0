#include <cstdio>
#include "tcpgateL.h"
#include "PPDClient.h"


#define SIZEMASS 100000


int main()
{
    
    int res=0;
    PPDClient* ppdcl = new PPDClient();
    res=ppdcl->InitClientPPD("conf.txt");
    std::cout << res << std::endl;
    sleep(5);
    /*InfobufPPD analogR;
    InfobufPPD analogW;
    analogR = ppdcl->TakeInfoForReadPDD(TypeSignalPPD::Group);
    analogW = ppdcl->TakeInfoForWritePDD(TypeSignalPPD::Group);
    char* buff = (char*)analogW.buf;
    char* ibuff = (char*)analogR.buf;
    int tick = 0;
    int index = 1;
    int grtype = 1;
    int size = 10;
    next2:
    tick++;
    buff = (char*)analogW.buf;
    pthread_mutex_lock(&analogW.mutex);
    std::cout << "22" << std::endl;
    for (int i = 0; i < 4; i++)
    {
        *buff = *(((char*)(&index)) + i);
        buff++;
    }

    for (int i = 0; i < 4; i++)
    {
        *buff = *(((char*)(&grtype)) + i);
        buff++;
    }

    for (int i = 0; i < 4; i++)
    {
        *buff = *(((char*)(&size)) + i);
        buff++;
    }

    for (int i = 0; i < size; i++)
    {
        *buff = i;
        buff++;
    }
    
    /*for (int i = 0; i < 100; i++)
    {
        *(buff+i) = i+tick;
    }*/
    /*pthread_mutex_unlock(&analogW.mutex);
    
    sleep(2);

    pthread_mutex_lock(&analogR.mutex);
    for (int i = 0; i < 100; i++)
    {
        std::cout << i <<"-" << (int)*(ibuff+i) << std::endl;
    }
    pthread_mutex_unlock(&analogR.mutex);

    sleep(1);
    goto next2;*/




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

    InfobufPPD analogR = ppdcl->TakeInfoForReadPDD(TypeSignalPPD::Binar);
    InfobufPPD analogW = ppdcl->TakeInfoForWritePDD(TypeSignalPPD::Binar);

    for (auto item = tcpunit.begin(); item != tcpunit.end(); item++)
    {
        if ((*item)->infotypedata() == TypeData::BINAR && (*item)->infotypesignal() == TypeSignal::OUTPUT)
        {
            (*item)->initmutexbuffer(analogW.mutex);
            (*item)->initpointbuffer((char*)analogW.buf);
        }
        if ((*item)->infotypedata() == TypeData::BINAR && (*item)->infotypesignal() == TypeSignal::INPUT)
        {
            (*item)->initmutexbuffer(analogR.mutex);
            (*item)->initpointbuffer((char*)analogR.buf);
        }
    }

    next:

    /*pthread_mutex_lock(&mut);
    f = *(float*)buf;
    //std::cout << f << "  " << *(float*)(buf+SIZEMASS*4-4) <<std::endl;
    pthread_mutex_unlock(&mut);*/
    usleep(100000);

    goto next;

    char s;
    std::cin >> s;
    return 0;
}