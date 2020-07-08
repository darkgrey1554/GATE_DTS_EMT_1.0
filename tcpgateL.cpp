#include "tcpgateL.h"



int conver_IPstr_int(const char* str)
{
    int res = 0;
    int var = 0;
    int count = 0;
    int count2 = 0;
    const char* simvol = str;


    while (*simvol != '\000')//while (*simvol != '\0')
    {
        while (*simvol != '\000' && *simvol != '.') //while (*simvol != '\0' && *simvol != '.')
        {
            if (*simvol < 0x30 || *simvol > 0x39) return -1;
            var *= 10;
            var += *simvol - 0x30;
            count++;
            simvol++;
        }
        if (*simvol == '.') simvol++;
        count2++;
        if (count > 3) return -1;
        if (count2 > 4) return -1;
        count = 0;
        res = res << 8;
        res += var;
        var = 0;
    }
    return res;
}

std::ostream& operator<<(std::ostream& out, TypeSignal& m)
{
    if (m == TypeSignal::INPUT) out << "InPut";
    if (m == TypeSignal::OUTPUT) out << "OutPut";
    if (m == TypeSignal::EMPTY) out << "Empty";
    return out;
}

std::ostream& operator<<(std::ostream& out, TypeData& m)
{
    if (m == TypeData::ANALOG) out << "Analog";
    if (m == TypeData::DISCRETE) out << "Discrete";
    if (m == TypeData::BINAR) out << "Binar";
    if (m == TypeData::GROUP) out << "Group";
    if (m == TypeData::EMPTY) out << "Empty";
    return out;
}

void ConfigInfoTCPUnit::clear()
{
    type_data = TypeData::EMPTY;
    type_signal = TypeSignal::EMPTY;
    size_data = 0;
    ip_address.clear();
    namesharedmemory.clear();
    port = 0;
    frequency_time = 0;
    id_unit = -1;
    channel = -1;
}

int ConfigReader::ReadConfigFile(const char* filename)
{

    FILE* config_file = NULL;
    char simvol = 0;
    std::string str_info;
    std::string helpstr;
    int res_read = 0;
    int pos[2] = { 0,0 };
    ConfigInfoTCPUnit unit;
    int count = 0;

    config_file = fopen(filename, "r");
    if (config_file == NULL)
    {
        std::cout << "MAIN\tERROR_OPEN_CONFIG_FILE" << std::endl;
        return -1;
    }

    for (;;)
    {
        simvol = 0;
        str_info.clear();
        while (simvol != '\n' && res_read != EOF)
        {
            res_read = fscanf(config_file, "%c", &simvol);
            if ((simvol > 0x1F || simvol == '\t') && res_read != EOF) str_info += simvol;
        }

        if (str_info == "[List]" || res_read == EOF)
        {
            if (res_read == EOF)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE1" << std::endl;
                return -1;
            }
            break;
        }

        if (str_info.substr(0, 4) == "@EMT")
        {
            pos[0] = 0;
            pos[1] = 0;
            unit.clear();

            if (str_info.find("Input") != -1)
            {
                unit.type_signal = TypeSignal::INPUT;
            }
            else if (str_info.find("Output") != -1)
            {
                unit.type_signal = TypeSignal::OUTPUT;
            }
            else
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE2" << std::endl;
                return -1;
            }

            if (str_info.find("Discrete") != -1)
            {
                unit.type_data = TypeData::DISCRETE;
            }
            else if (str_info.find("Analog") != -1)
            {
                unit.type_data = TypeData::ANALOG;
            }
            else if (str_info.find("Binar") != -1)
            {
                unit.type_data = TypeData::BINAR;
            }
            else if (str_info.find("Group") != -1)
            {
                unit.type_data = TypeData::GROUP;
            }
            else
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE3" << std::endl;
                return -1;
            }

            pos[0] = str_info.find('\t', 0);
            if (pos[0] == -1)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE4" << std::endl;
                return -1;
            }

            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE5" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            unit.size_data = atoi(helpstr.c_str());

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE6" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            unit.namesharedmemory = helpstr;

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE7" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            unit.ip_address = helpstr;

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE8" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            unit.port = atoi(helpstr.c_str());

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE9" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            unit.frequency_time = atoi(helpstr.c_str());

            pos[0] = pos[1];
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1);
            unit.channel = atoi(helpstr.c_str());

            unit.id_unit = count;
            count++;
            listinfo.push_back(unit);

        }
        else if (str_info.substr(0, 5) != "[EMT]")
        {
            std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE10" << std::endl;
            return -1;
        }
    }
    fclose(config_file);
    return 0;
}


ConfigReader::ConfigReader()
{}

ConfigReader::~ConfigReader()
{}

int ConfigReader::NumberItem()
{
    return listinfo.size();
}

ConfigInfoTCPUnit ConfigReader::ReadConfigItem(int number)
{
    if (number > listinfo.size())
    {
        ConfigInfoTCPUnit conf;
        conf.clear();
        return conf;
    }
    auto iter = listinfo.begin();
    for (int i = 0; i < number; i++)
    {
        iter++;
    }
    return (*iter);
}


TCPUnit* TCPUnit::create_tcp_unit(ConfigInfoTCPUnit gate)
{
    if (gate.type_signal == TypeSignal::INPUT) return new TCPServer(gate);
    if (gate.type_signal == TypeSignal::OUTPUT) return new TCPClient(gate);
}


TCPServer::TCPServer(ConfigInfoTCPUnit gate)
{
    set = gate;
    if (set.id_unit == -1 || set.ip_address.size() == 0 || set.namesharedmemory.size() == 0 || set.port == 0 ||
        set.type_data == TypeData::EMPTY || set.size_data == 0 || set.type_signal == TypeSignal::EMPTY)
    {
        std::cout << "SERVER ID:" << set.id_unit << "\tERROR_CONFIG_INFO" << std::endl;
    }
    else thread_unit = std::thread(&TCPServer::thread_tcp_server, this);
}

int TCPServer::thread_tcp_server()
{

    std::cout << "START SERVER ID:" << set.id_unit << "\tPORT:" << set.port << "\tIP_ADDRESS: " << set.ip_address
        << "\tTYPEDATA:" << set.type_data << std::endl;

    int k_data = 0;
    if (set.type_data == TypeData::ANALOG || set.type_data == TypeData::DISCRETE) k_data = 4;
    if (set.type_data == TypeData::GROUP || set.type_data == TypeData::BINAR)     k_data = 1;

    int listensocket = 0;
    int client;
    sockaddr_in addr_server;
    sockaddr_in addr_client;
    socklen_t lenght_addr_client = sizeof(addr_client);
    int ip=0;
    int result = 0;
    std::string ipclientstr;
    unsigned long ipclinet=0;
    
    int count_recv = 0;
    int res_recv = 0;
    char num_recv = 1;

    int count_send = 0;
    int res_send = 0;
    char num_send = 0;


    char* buf_recv = new char[1];
    char* buf_send; //= new char[set.size_data*k_data+5];
    int size_data_byte;
    int size_data_send;
    if (set.type_data != TypeData::GROUP)
    {
        size_data_byte = set.size_data * k_data;
        size_data_send = size_data_byte + 5;
        buf_send = new char[size_data_send];
    }
    else
    {
        size_data_byte = 4108;
        size_data_send = 4108;
        buf_send = new char[4108];
    }


    for (int i = 0; i < size_data_send; i++) buf_send[i] = 0;
    
    int command = 0;
    char* ibuf;
    char* jbuf;

    aiocb aiobufsend;
    aiocb aiobufrecv;
    sigevent sigrecv;
    sigevent sigsend;
    aiocb* aio_point;

    listensocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listensocket == -1)
    {
        std::cout << "SERVER ID: " << set.id_unit << "/tERROR INITIALIZATION CODE ERROR: "<< errno << std::endl;
        sleep(2);
        return -1;
    }

    ip = conver_IPstr_int(set.ip_address.c_str());
    if (ip == -1)
    {
        std::cout << "SERVER ID: " << set.id_unit << "/tERROR INITIALIZATION ERROR: BAD IP" << std::endl;
        sleep(2);
        return -1;
    }

    addr_server.sin_family = AF_INET;
    addr_server.sin_addr.s_addr = htonl(ip);
    addr_server.sin_port = htons(set.port);

    result = bind(listensocket, (sockaddr*)&addr_server, sizeof(addr_server));
    if (result == -1)
    {
        std::cout << "SERVER ID: " << set.id_unit << "\tERROR INITIALIZATION CODE ERROR: " << errno << std::endl;
        sleep(2);
        return -1;

    }

    if (listen(listensocket, 2) == -1)
    {
        std::cout << "SERVER ID: " << set.id_unit << "\tERROR INITIALIZATION CODE ERROR: " << errno << std::endl;
        close(listensocket);
        sleep(2);
        return -1;
    }

    for (;;)
    {
        client = accept(listensocket, (sockaddr*)&addr_client, &lenght_addr_client);
        if (client == -1)
        {
            std::cout << "SERVER ID: " << set.id_unit << "\tERROR CONNECTION CLIENT CODE ERROR: "<< errno << std::endl;
            close(client);
            sleep(2);
            continue;
        }
        
        ipclientstr.clear();
        for (int i = 0; i < 4; i++)
        {
            ipclinet = (addr_client.sin_addr.s_addr >> (8 * i))&0xFF;
            ipclientstr += std::to_string(ipclinet);
            if (i != 3) ipclientstr += ".";
        }
        

        std::cout << "SERVER ID: " << set.id_unit << "\tCONNECTION WITH CLIENT IP: "
            << ipclientstr.c_str()
            << "\tPORT: " << addr_client.sin_port << std::endl;

        sigrecv.sigev_notify=SIGEV_NONE;
        aiobufrecv.aio_fildes = client;
        aiobufrecv.aio_offset = 0;
        aiobufrecv.aio_buf = buf_recv;
        aiobufrecv.aio_nbytes = 1;
        aiobufrecv.aio_reqprio = 0;
        aiobufrecv.aio_sigevent = sigrecv;
        aiobufrecv.aio_lio_opcode = LIO_READ;

        sigsend.sigev_notify = SIGEV_NONE;
        aiobufsend.aio_fildes = client;
        aiobufsend.aio_offset = 0;
        aiobufsend.aio_buf = buf_send;
        aiobufsend.aio_nbytes = size_data_send;
        aiobufsend.aio_reqprio = 0;
        aiobufsend.aio_sigevent = sigsend;
        aiobufsend.aio_lio_opcode = LIO_READ;


        for (;;)
        {
            count_recv = 0;
            res_recv = 0;
            aiobufrecv.aio_offset = 0;
            aiobufrecv.aio_buf = buf_recv;

            for (;;)
            {
                aiobufrecv.aio_offset = 0;
                aiobufrecv.aio_buf = buf_recv + count_recv;
                result=aio_read(&aiobufrecv);

                if (result == -1)
                {
                    break;
                }

                aio_point = &aiobufrecv;
                result = aio_suspend(&aio_point, 1, NULL);

                if (result == -1)
                {
                    break;
                }

                res_recv = aio_return(&aiobufrecv);
                count_recv += res_recv;
                if (res_recv <= 0 || count_recv > num_recv)
                {
                    result = -10;
                    break;
                }

                if (count_recv < num_recv) continue;

                break;

                /*for (;;)
                {
                    result = aio_error(&aiobufrecv);
                    if (result != 0)
                    {
                        if (result == EINPROGRESS)
                        {
                            usleep(100);
                            continue;
                        }
                        else break;
                    }
                    else break;
                }

                if (result != 0) break;

                res_recv = aio_return(&aiobufrecv);
                count_recv += res_recv;
                if (res_recv <= 0 || count_recv>num_recv)
                {
                    result = -10;
                    break;
                }

                
                if (count_recv < num_recv) continue;

                break;*/
            }

            if (result != 0)
            {
                if (result == -1)
                {
                    std::cout << "SERVER ID: " << set.id_unit << "\tERROR ASYNC_READ CODE ERROR: " << errno << std::endl;
                }
                else if (result == -10)
                {
                    std::cout << "SERVER ID: " << set.id_unit << "\tERROR ASYNC_READ: " << "messeng size  is zero or too large" << std::endl;
                }
                else
                {
                    std::cout << "SERVER ID: " << set.id_unit << "\tERROR ASYNC_READ CODE ERROR: " << result << std::endl;  /// так тут норм описать ошибку надо
                }

                close(client);
                sleep(2);
                break;
            }

            command = *buf_recv;
            if (command != 3) continue;

            ibuf = buf;
            jbuf = buf_send;
            if (set.type_data != TypeData::GROUP)
            {
                *jbuf = 3;
                jbuf++;
                for (int i = 0; i < 4; i++)
                {

                    *jbuf = *(((char*)&set.size_data) + i);
                    jbuf++;
                }
            }
            
            if (pthread_mutex_lock(&mutex) == 0)
            {
                if (ibuf != 0)
                {
                    for (int i = 0; i < size_data_byte; i++)
                    {
                        *jbuf = *ibuf;
                        jbuf++;
                        ibuf++;
                    }
                }

                pthread_mutex_unlock(&mutex);
            }

            count_send = 0;

            for (;;)
            {
                
                aiobufsend.aio_offset = 0;
                aiobufsend.aio_buf = buf_send+count_send;
                aiobufsend.aio_nbytes =size_data_send-count_send;

                aio_write(&aiobufsend);

                for (;;)
                {
                    result = aio_error(&aiobufsend);
                    if (result != 0)
                    {
                        if (result == EINPROGRESS)
                        {
                            usleep(100);
                            continue;
                        }
                        else break;
                    }
                    else break;
                }

                if (result != 0) break;

                res_send = aio_return(&aiobufsend);
                count_send += res_send;
                if (res_send <= 0 )
                {
                    result = -10;
                    break;
                }


                if (count_send < size_data_send) continue;

                break;

            }

            if (result != 0)
            {
                if (result == -1)
                {
                    std::cout << "SERVER ID: " << set.id_unit << "\tERROR ASYNC_SEND CODE ERROR: " << errno << std::endl;
                }
                else if (result == -10)
                {
                    std::cout << "SERVER ID: " << set.id_unit << "\tERROR ASYNC_READ: " << "TRANSFER MESSENG IS ZERO" << std::endl;
                }
                else
                {
                    std::cout << "SERVER ID: " << set.id_unit << "\tERROR ASYNC_READ CODE ERROR: " << result << std::endl;  /// так тут норм описать ошибку надо
                }

                close(client);
                sleep(2);
                break;
            }

        }
    }
    return 0;
}

void TCPServer::close_tcp_unit()
{}

void TCPServer::restart_thread()
{}

void TCPServer::initmutexbuffer(pthread_mutex_t inmutex)
{
    mutex = inmutex;
}

void TCPServer::initpointbuffer(char* inbuf)
{
    buf = inbuf;
}

TCPClient::TCPClient(ConfigInfoTCPUnit gate)
{
    set = gate;
    if (set.id_unit == -1 || set.ip_address.size() == 0 || set.namesharedmemory.size() == 0 || set.port == 0 ||
        set.type_data == TypeData::EMPTY || set.size_data == 0 || set.type_signal == TypeSignal::EMPTY)
    {
        std::cout << "CLIENT ID:" << set.id_unit << "\tERROR_CONFIG_INFO" << std::endl;
    }
    else thread_unit = std::thread(&TCPClient::thread_tcp_client, this);
}

int TCPClient::thread_tcp_client()
{
    std::cout << "START CLIENT ID:" << set.id_unit << "\tPORT:" << set.port << "\tIP_ADDRESS: " << set.ip_address
        << "\tTYPEDATA:" << set.type_data << std::endl;
  
    int k_data = 0;
    if (set.type_data == TypeData::ANALOG || set.type_data == TypeData::DISCRETE) k_data = 4;
    if (set.type_data == TypeData::GROUP || set.type_data == TypeData::BINAR)     k_data = 1;
    
    int ip=-1;
    ip = conver_IPstr_int(set.ip_address.c_str());
    sockaddr_in addr_server;
    int connect_socket;
    size_t count_recv;
    size_t res_recv;
    int num_recv;
    char* buf_recv;// = new char[set.size_data * k_data + 4];
    int size_data_recv;
    int size_data_byte;

    if (set.type_data != TypeData::GROUP)
    {
        size_data_byte = set.size_data * k_data;
        size_data_recv = size_data_byte + 4;
        buf_recv = new char[size_data_recv];        
    }
    else
    {
        buf_recv = new char[4108];
        size_data_recv = 4108;
        size_data_byte = 4108;
    }

    char* ibuf;
    char* jbuf;
    int result;
    aiocb aiobuf;
    sigevent sig;


    if (ip == -1)
    {
        std::cout << "CLIENT ID: " << set.id_unit << "/tERROR INITIALIZATION ERROR: BAD IP" << std::endl;
        return -1;
    }

    addr_server.sin_family = AF_INET;
    addr_server.sin_addr.s_addr = htonl(ip);
    addr_server.sin_port = htons(set.port);

    for (;;)
    {
                                 
        connect_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (connect_socket == -1)
        {
            std::cout << "CLIENT ID: " << set.id_unit << "/tERROR INITIALIZATION CODE ERROR: " << errno << std::endl;
            return -1;
        }

        result = connect(connect_socket, (sockaddr*)&addr_server, sizeof(addr_server));
        if (result == -1)
        {
            std::cout << "CLIENT ID: " << set.id_unit << "\tERROR CONNECTION WITH SERVER CODE ERROR: " << errno << std::endl;
            close(connect_socket);
            sleep(2);
            continue;
        }
        else
        {
            std::cout << "CLIENT ID: " << set.id_unit << " CONNECTED WITH SERVER DONE " << std::endl;
        }

        sig.sigev_notify=SIGEV_NONE;

        aiobuf.aio_fildes = connect_socket;
        aiobuf.aio_offset = 0;
        aiobuf.aio_buf = buf_recv;
        aiobuf.aio_nbytes = size_data_recv;
        aiobuf.aio_reqprio = 0;
        aiobuf.aio_sigevent = sig;
        aiobuf.aio_lio_opcode = LIO_READ;

        
        for (;;)
        {

            count_recv = 0;
            res_recv = 0;
            aiobuf.aio_offset = 0;
            aiobuf.aio_buf = buf_recv;
           
            for (;;)
            {
                aiobuf.aio_offset = 0;
                aiobuf.aio_buf = buf_recv + count_recv;
                aio_read(&aiobuf);
                
                for (;;)
                {
                    result = aio_error(&aiobuf);
                    if (result != 0)
                    {
                        if (result == EINPROGRESS)
                        {
                            usleep(100);
                            continue;
                        }
                        else break;
                    }
                    else break;
                }
               
                if (result != 0) break;
                
                res_recv = aio_return(&aiobuf);
                if (res_recv <= 0)
                {
                    result = -1;
                    break;
                }

                count_recv += res_recv;
                if (count_recv < 4) continue;

                if (set.type_data != TypeData::GROUP)
                {
                    num_recv = *((int*)buf_recv);
                    if (num_recv != set.size_data)
                    {
                        result = -10;
                        break;
                    }
                }
                
                if (count_recv >= size_data_recv) break;   
            }

            if (result != 0)
            {
                if (result == -1)
                {
                    std::cout << "CLIENT ID: " << set.id_unit << "\tERROR ASYNC_READ CODE ERROR: " << errno << std::endl;
                }
                else if (result == -10)
                {
                    std::cout << "CLIENT ID: " << set.id_unit << "\tERROR ASYNC_READ: " << "ERROR SIZE_DATA (watch config file)" << std::endl;  /// так тут норм описать ошибку надо
                }
                else
                {
                    std::cout << "CLIENT ID: " << set.id_unit << "\tERROR ASYNC_READ CODE ERROR: " << result << std::endl;  /// так тут норм описать ошибку надо
                }

                close(connect_socket);
                sleep(2);
                break;
            }


            ibuf = buf_recv;
            if (set.type_data != TypeData::GROUP) ibuf += 4;
            jbuf = buf;

            if (pthread_mutex_lock(&mutex) == 0) /// вот тут надо try
            {
                if (buf != 0)
                {
                    for (int i = 0; i < size_data_byte; i++)
                    {
                        *jbuf = *ibuf;
                        jbuf++;
                        ibuf++;
                    }
                }   
                if (*(buf + 99) == -1) std::cout << "sdfsdf" <<std::endl;
                pthread_mutex_unlock(&mutex);
            }
            
            
        }
    }

    return 0;
}

void TCPClient::close_tcp_unit()
{}

void TCPClient::restart_thread()
{}

void TCPClient::initmutexbuffer(pthread_mutex_t inmutex)
{
    mutex = inmutex;
}

void TCPClient::initpointbuffer(char* inbuf)
{
    buf = inbuf;
}

TypeData TCPClient::infotypedata() { return set.type_data; }
TypeSignal TCPClient::infotypesignal() { return set.type_signal; }
TypeData TCPServer::infotypedata() { return set.type_data; }
TypeSignal TCPServer::infotypesignal() { return set.type_signal; }