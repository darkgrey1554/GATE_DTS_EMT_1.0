#include <cstdio>
#include "tcpgateL.h"
#include "PPDClient.h"
#include <termios.h>
#include <cstdlib>

int getch()
{
    int ch;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

#define SIZEMASS 100000


int main()
{

    int res=0;
    PPDClient* ppdcl = new PPDClient();
    res=ppdcl->InitClientPPD("conf.txt");
    if (res == 0)
    {
        std::cout << "CONNECT WITH PPD: DONE" << std::endl;
    }
    else
    {
        std::cout << "CONNECT WITH PPD: FALL" << std::endl;
        sleep(5);
        return -1;
    }
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

    //pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    //char* buf = new char[SIZEMASS*4];
    //float f;

    ConfigReader* config = new ConfigReader();
    res=config->ReadConfigFile("conf.txt");
    if (res !=0)
    {
        std::cout << "ERROR READ CONFIG FILE" << std::endl;
        sleep(5);
        return -1;
    }
    std::list<TCPUnit*> tcpunit;
    for (int i = 0; i < config->NumberItem(); i++)
    {
        tcpunit.push_back(TCPUnit::create_tcp_unit(config->ReadConfigItem(i)));
    }

    //InfobufPPD analogR = ppdcl->TakeInfoForReadPDD(TypeSignalPPD::Binar);
    //InfobufPPD analogW = ppdcl->TakeInfoForWritePDD(TypeSignalPPD::Binar);
    TypeSignalPPD sig;
    for (auto item = tcpunit.begin(); item != tcpunit.end(); item++)
    {
        if ((*item)->infotypesignal() == TypeSignal::OUTPUT)
        {
            if ((*item)->infotypedata() == TypeData::BINAR) sig = TypeSignalPPD::Binar;
            if ((*item)->infotypedata() == TypeData::ANALOG) sig = TypeSignalPPD::Analog;
            if ((*item)->infotypedata() == TypeData::DISCRETE) sig = TypeSignalPPD::Discrete;
            if ((*item)->infotypedata() == TypeData::GROUP) sig = TypeSignalPPD::Group;
            (*item)->initmutexbuffer(ppdcl->TakeInfoForWritePDD(sig).mutex);
            (*item)->initpointbuffer((char*)ppdcl->TakeInfoForWritePDD(sig).buf);
        }
        if ((*item)->infotypesignal() == TypeSignal::INPUT)
        {
            if ((*item)->infotypedata() == TypeData::BINAR) sig = TypeSignalPPD::Binar;
            if ((*item)->infotypedata() == TypeData::ANALOG) sig = TypeSignalPPD::Analog;
            if ((*item)->infotypedata() == TypeData::DISCRETE) sig = TypeSignalPPD::Discrete;
            if ((*item)->infotypedata() == TypeData::GROUP) sig = TypeSignalPPD::Group;
            (*item)->initmutexbuffer(ppdcl->TakeInfoForReadPDD(sig).mutex);
            (*item)->initpointbuffer((char*)ppdcl->TakeInfoForReadPDD(sig).buf);
        }
    }

    /*next:

    /*pthread_mutex_lock(&mut);
    f = *(float*)buf;
    //std::cout << f << "  " << *(float*)(buf+SIZEMASS*4-4) <<std::endl;
    pthread_mutex_unlock(&mut);*/
    //usleep(100000);

    //goto next;

    //char s;
    //std::cin >> s;
    //return 0;
    char s;
    s = 0;
    while (s != 0x1B)
    {
        s = (char)getch();
        sleep(1);
    }

    return 0;
}