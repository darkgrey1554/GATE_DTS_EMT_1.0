#include "PPDClient.h"

int PPDClient::read_config_file(const char* Namefile)
{
    FILE* config_file = NULL;
    char simvol = 0;
    std::string str_info;
    std::string helpstr;
    int res_read = 0;
    int pos[2] = { 0,0 };
    InfoConfig info;

    config_file = fopen(Namefile, "r");
    if (config_file == NULL)
    {
        fclose(config_file);
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
                std::cout << "MAIT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                fclose(config_file);
                return -1;
            }
            break;
        }

        if (str_info.substr(0, 4) == "@EMT")
        {
            pos[0] = 0;
            pos[1] = 0;
            info.clear();


            pos[0] = str_info.find('\t', 0);
            if (pos[0] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE1" << std::endl;
                fclose(config_file);
                return -1;
            }
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE2" << std::endl;
                fclose(config_file);
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            info.size = atoi(helpstr.c_str());

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE3" << std::endl;
                fclose(config_file);
                return -1;
            }

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE4" << std::endl;
                fclose(config_file);
                return -1;
            }

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE5" << std::endl;
                fclose(config_file);
                return -1;
            }

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE6" << std::endl;
                fclose(config_file);
                return -1;
            }

            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            info.frequency = atoi(helpstr.c_str());

            pos[0] = pos[1];
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1);
            info.channel = atoi(helpstr.c_str());

            if (str_info.find("Analog") != -1)
            {
                if (str_info.find("Input") != -1)
                {
                    SizeAnalogDataOut = info.size;
                    channelAnalogDataOut = info.channel;
                }
                else if (str_info.find("Output") != -1)
                {
                    SizeAnalogDataIn = info.size;
                    channelAnalogDataIn = info.channel;
                    FrequencySndAnalogData = info.frequency;
                }
            }
            else if (str_info.find("Discrete") != -1)
            {
                if (str_info.find("Input") != -1)
                {
                    SizeDiscreteDataOut = info.size;
                    channelDiscreteDataOut = info.channel;

                }
                else if (str_info.find("Output") != -1)
                {
                    SizeDiscreteDataIn = info.size;
                    channelDiscreteDataIn = info.channel;
                    FrequencySndDiscreteData = info.frequency;
                }
            }
            else if (str_info.find("Group") != -1)
            {
                if (str_info.find("Input") != -1)
                {
                    SizeGroupDataOut = info.size;
                    channelGroupDataOut = info.channel;

                }
                else if (str_info.find("Output") != -1)
                {
                    SizeGroupDataIn = info.size;
                    channelGroupDataIn = info.channel;
                    FrequencyGroupData = info.frequency;
                }
            }
            else if (str_info.find("Binar") != -1)
            {
                if (str_info.find("Input") != -1)
                {
                    SizeBinarDataOut = info.size;
                    channelBinarDataOut = info.channel;

                }
                else if (str_info.find("Output") != -1)
                {
                    SizeBinarDataIn = info.size;
                    channelBinarDataIn = info.channel;
                    FrequencySndBinarData = info.frequency;
                }
            }

        }
        else if (str_info.substr(0, 5) != "[EMT]")
        {
            std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE8" << std::endl;
            fclose(config_file);
            return -1;
        }
    }
    fclose(config_file);
    return 0;
}

int PPDClient::InitClientPPD(const char* filename)
{
    client = new DTSClient(FuncRcvPPD, this);
    if (client->getState() < 0)
    {
        return client->getError();
    }    

    int result = 0;
    if (read_config_file(filename) == -1)
    {
        std::cout << "ERROR INIT PPClient: BAD CONFIG FILE" << std::endl;
        client->~DTSClient();
        return 1;
    }

    result = client->getRcvAnaNum();
    if (result == 0 && client->getError() != 0)
    {
        std::cout << "ERROR INIT PPDClient: ERROR DTSClient fub getRcvAnaNum CODE ERROR " << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    if (result < SizeAnalogDataOut)
    {
        std::cout << "ERROR INIT PPDClient: BAD SISE MASS ANALOG-OUT" << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    bufAnalogOut = new float[result];
    memset(bufAnalogOut, 0, sizeof(float)*result);

    result = client->getRcvIntNum();
    if (result == 0 && client->getError() != 0)
    {
        std::cout << "ERROR INIT PPDClient: ERROR DTSClient fub getRcvIntNum CODE ERROR " << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    if (result < SizeDiscreteDataOut)
    {
        std::cout << "ERROR INIT PPDClient: BAD SISE MASS DISCRETE-OUT" << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    bufDiscreteOut = new int[result];
    memset(bufDiscreteOut, 0, sizeof(int) * result);

    result = client->getRcvBinNum();
    if (result == 0 && client->getError() != 0)
    {
        std::cout << "ERROR INIT PPDClient: ERROR DTSClient fub getRcvIntNum CODE ERROR " << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    if (result < SizeBinarDataOut)
    {
        std::cout << "ERROR INIT PPDClient: BAD SISE MASS BINAR-OUT" << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    bufBinarOut = new char[result];
    memset(bufBinarOut, 0, sizeof(char) * result);

    result = client->getSndAnaNum();
    if (result == 0 && client->getError() != 0)
    {
        std::cout << "ERROR INIT PPDClient: ERROR DTSClient fub getRcvAnaNum CODE ERROR " << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    if (result < SizeAnalogDataIn)
    {
        std::cout << "ERROR INIT PPDClient: BAD SISE MASS ANALOG-IN" << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    bufAnalogIn = new float[result];
    memset(bufAnalogIn, 0, sizeof(float) * result);

    result = client->getSndIntNum();
    if (result == 0 && client->getError() != 0)
    {
        std::cout << "ERROR INIT PPDClient: ERROR DTSClient fub getRcvIntNum CODE ERROR " << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    if (result < SizeDiscreteDataIn)
    {
        std::cout << "ERROR INIT PPDClient: BAD SISE MASS DISCRETE-IN" << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    bufDiscreteIn = new int[result];
    memset(bufDiscreteIn, 0, sizeof(int) * result);

    result = client->getSndBinNum();
    if (result == 0 && client->getError() != 0)
    {
        std::cout << "ERROR INIT PPDClient: ERROR DTSClient fub getRcvIntNum CODE ERROR " << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    if (result < SizeBinarDataIn)
    {
        std::cout << "ERROR INIT PPDClient: BAD SISE MASS BINAR-IN" << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    bufBinarIn = new char[result];
    memset(bufBinarIn, 0, sizeof(char) * result);

    if (4096 < SizeGroupDataOut)
    {
        std::cout << "ERROR INIT PPDClient: BAD SISE MASS GROUP-OUT" << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    SizeGroupDataOut = 4096;

    if (4096 < SizeGroupDataIn)
    {
        std::cout << "ERROR INIT PPDClient: BAD SISE MASS GROUP-IN" << client->getError() << std::endl;
        client->~DTSClient();
        return 1;
    }
    SizeGroupDataIn = 4096;

    bufGroupIn = new char[4112];
    bufGroupOut = new char[4112];

    statusInitClient = 1;
    threadWriteServerDTS = std::thread(&PPDClient::FuncWriteServerDTS, this);

    return 0;
}


static int FuncRcvPPD(void* argPtr, Value& value, int32_t chnlId)
{
    PPDClient* gate = (PPDClient*)argPtr;

    float* ibuff;
    int* ibufi;
    char* ibufc;
    int grtype;
    int size;
    int index;

    switch (value.type)
    {
    case Ana_VT:
    {        
        AData* dataPtr = NULL;
        value.getData(dataPtr);
        if (gate->channelAnalogDataOut != chnlId) return 0;
        if (gate->statusInitClient != 1) break;
        if (value.idx >= gate->SizeAnalogDataOut)
        {
            //std::cout << "ERROR BAD ANALOG VALUE" << std::endl;
            return 0;
        }
        ibuff = gate->bufAnalogOut;
        ibuff += value.idx;
        pthread_mutex_lock(&gate->mutex_analog_out);
        *ibuff = (dataPtr->value);
        pthread_mutex_unlock(&gate->mutex_analog_out);
    }
    break;

    case Bin_VT:
    {       
        BData* dataPtr = NULL;
        value.getData(dataPtr);
        if (gate->channelBinarDataOut != chnlId) return 0;
        if (gate->statusInitClient != 1) break;
        if (value.idx > gate->SizeBinarDataOut)
        {
            //std::cout << "ERROR BAD BINAR VALUE" << std::endl;
            return 0;
        }
        ibufc = gate->bufBinarOut;
        ibufc += value.idx;
        pthread_mutex_lock(&gate->mutex_binar_out);
        *ibufc = (dataPtr->value);
        pthread_mutex_unlock(&gate->mutex_binar_out);
    }
    break;

    case Int_VT:
    {
        IData* dataPtr = NULL;
        value.getData(dataPtr);
        if (gate->channelDiscreteDataOut != chnlId) return 0;
        if (gate->statusInitClient != 1) break;
        if (value.idx > gate->SizeDiscreteDataOut)
        {
            //std::cout << "ERROR BAD DISCRETE VALUE" << std::endl;
            return 0;
        }
        ibufi = gate->bufDiscreteOut;
        ibufi += value.idx;
        pthread_mutex_lock(&gate->mutex_discrete_out);
        *ibufi = (dataPtr->value);
        pthread_mutex_unlock(&gate->mutex_discrete_out);

    }
    break;

    case Grp_VT:
    {
        GData* dataPtr = NULL;
        value.getData(dataPtr);
        if (gate->channelGroupDataOut != chnlId) return 0;
        if (gate->statusInitClient != 1) break;
        if (dataPtr->size > gate->SizeGroupDataOut)
        {
            //std::cout << "ERROR BAD GROUP VALUE" << std::endl;
            return 0;
        }
        ibufc = gate->bufGroupOut;
        grtype = dataPtr->groupType;
        size = dataPtr->size;
        index = value.idx;
        pthread_mutex_lock(&gate->mutex_group_out);
        for (int i = 0; i < 4; i++)
        {
            *ibufc = *(((char*)(&index)) + i);
            ibufc++;
        }

        for (int i = 0; i < 4; i++)
        {
            *ibufc = *(((char*)(&grtype)) + i);
            ibufc++;
        }

        for (int i = 0; i < 4; i++)
        {
            *ibufc = *(((char*)(&size)) + i);
            ibufc++;
        }

        for (int i = 0; i < dataPtr->size; i++)
        {
            *ibufc = dataPtr->value[i];
            ibufc++;
        }
        pthread_mutex_unlock(&gate->mutex_group_out);
    }
    break;

    default:
        break;
    }

    return 0;
}

int PPDClient::FuncWriteServerDTS()
{
    timeval timenow;
    timeval timeanaloglast;
    timeval timediscretelast;
    timeval timebinarlast;
    timeval timegrouplast;
    float time;
    float* ibuff;
    int* ibufi;
    char* ibufc;
    Value value;
    int result;
    float ff;
    int fi;
    char fc;

    int index;
    int size;
    int grtype;

    gettimeofday(&timeanaloglast, NULL);
    gettimeofday(&timediscretelast, NULL);
    gettimeofday(&timebinarlast, NULL);
    gettimeofday(&timegrouplast, NULL);

    for (;;)
    {
        gettimeofday(&timenow, NULL);
        

        if (FrequencySndAnalogData > 0)
        {
            time = (timenow.tv_sec * 1000. + timenow.tv_usec / 1000.) - (timeanaloglast.tv_sec * 1000. + timeanaloglast.tv_usec / 1000.);
            
            if (FrequencySndAnalogData < time)
                std::cout << "DTSCLEINT ERROR SEND ANALOG DATA TIME: " << time << std::endl;

            if (FrequencySndAnalogData - TIMEDEV < time)
            {

                ibuff = bufAnalogIn;
                pthread_mutex_lock(&mutex_analog_in);
                for (int j = 0; j < SizeAnalogDataIn; j++)
                {
                    ff = *ibuff;
                    makeAValue(&value, j, 1, ff);
                    result = client->put(&value, channelAnalogDataIn);
                    if (result < 0) std::cout << "DTSCLEINT ERROR SEND ANALOG DATA ERROR" << client->getError() << std::endl;
                    ibuff++;   
                }
                pthread_mutex_unlock(&mutex_analog_in);
                gettimeofday(&timeanaloglast, NULL);
            }
        }

        if (FrequencySndDiscreteData > 0)
        {
            time = (timenow.tv_sec * 1000. + timenow.tv_usec / 1000.) - (timediscretelast.tv_sec * 1000. + timediscretelast.tv_usec / 1000.);
           
            if (FrequencySndDiscreteData < time)
                std::cout << "DTSCLEINT ERROR SEND DISCRETE DATA TIME: " << time << std::endl;

            if (FrequencySndDiscreteData - TIMEDEV < time)
            {
                ibufi = bufDiscreteIn;
                pthread_mutex_lock(&mutex_discrete_in);
                for (int j = 0; j < SizeDiscreteDataIn; j++)
                {
                    fi = *ibufi;
                    makeIValue(&value, j, 1, fi);
                    result = client->put(&value, channelDiscreteDataIn);
                    if (result < 0) std::cout << "DTSCLEINT ERROR SEND DISCRETE DATA ERROR" << client->getError() << std::endl;
                    ibufi++;
                }
                pthread_mutex_unlock(&mutex_discrete_in);
                gettimeofday(&timediscretelast, NULL);
            }
        }

        if (FrequencySndBinarData > 0)
        {
            time = (timenow.tv_sec * 1000. + timenow.tv_usec / 1000.) - (timebinarlast.tv_sec * 1000. + timebinarlast.tv_usec / 1000.);

            if (FrequencySndBinarData < time)
                std::cout << "DTSCLEINT ERROR SEND BINAR DATA TIME: " << time << std::endl;

            if (FrequencySndBinarData - TIMEDEV < time)
            {

                ibufc = bufBinarIn;
                pthread_mutex_lock(&mutex_binar_in);
                for (int j = 0; j < SizeBinarDataIn; j++)
                {
                    fi = *ibufc;
                    makeBValue(&value, j, 1, fi);
                    result = client->put(&value, channelBinarDataIn);
                    if (result < 0) std::cout << "DTSCLEINT ERROR SEND BINAR DATA ERROR" << client->getError() << std::endl;
                    ibufc++;
                }
                pthread_mutex_unlock(&mutex_binar_in);
                gettimeofday(&timebinarlast, NULL);
            }
        }

        if (FrequencyGroupData > 0)
        {
            time = (timenow.tv_sec * 1000. + timenow.tv_usec / 1000.) - (timegrouplast.tv_sec * 1000. + timegrouplast.tv_usec / 1000.);

            if (FrequencyGroupData < time)
                std::cout << "DTSCLEINT ERROR SEND GROUP DATA TIME: " << time << std::endl;

            if (FrequencyGroupData - TIMEDEV < time)
            {

                ibufc = bufGroupIn;
                pthread_mutex_lock(&mutex_group_in);
                index = *(int*)ibufc;
                grtype = *(int*)(ibufc + 4);
                size = *(int*)(ibufc + 8);
                if (index > 0 && grtype > 0 && size > 0)
                {
                    makeGValue(&value, *(int*)ibufc, *(int*)(ibufc + 4), ibufc + 12, *(int*)(ibufc + 8));
                    result = client->put(&value, channelGroupDataIn);
                    if (result < 0) std::cout << "DTSCLEINT ERROR SEND GROUP DATA ERROR" << client->getError() << std::endl;
                }
                
                pthread_mutex_unlock(&mutex_group_in);
                gettimeofday(&timegrouplast, NULL);
            }
        }
        
    
    }
 }

PPDClient::PPDClient()
{
}

InfobufPPD PPDClient::TakeInfoForReadPDD(TypeSignalPPD type_signal)
{
    InfobufPPD infobuf;

    if (type_signal == TypeSignalPPD::Analog)
    {
        infobuf.buf = bufAnalogOut;
        infobuf.mutex = mutex_analog_out;
    }
    else if (type_signal == TypeSignalPPD::Discrete)
    {
        infobuf.buf = bufDiscreteOut;
        infobuf.mutex = mutex_discrete_out;
    }
    else if (type_signal == TypeSignalPPD::Binar)
    {
        infobuf.buf = bufBinarOut;
        infobuf.mutex = mutex_binar_out;
    }
    else if (type_signal == TypeSignalPPD::Group)
    {
        infobuf.buf = bufGroupOut;
        infobuf.mutex = mutex_group_out;
    }

    return infobuf;
}

InfobufPPD PPDClient::TakeInfoForWritePDD(TypeSignalPPD type_signal)
{
    InfobufPPD infobuf;
    infobuf.buf = 0;

    if (type_signal == TypeSignalPPD::Analog)
    {
        infobuf.buf = bufAnalogIn;
        infobuf.mutex = mutex_analog_in;
    }
    else if (type_signal == TypeSignalPPD::Discrete)
    {
        infobuf.buf = bufDiscreteIn;
        infobuf.mutex = mutex_discrete_in;
    }
    else if (type_signal == TypeSignalPPD::Binar)
    {
        infobuf.buf = bufBinarIn;
        infobuf.mutex = mutex_binar_in;
    }
    else if (type_signal == TypeSignalPPD::Group)
    {
        infobuf.buf = bufGroupIn;
        infobuf.mutex = mutex_group_in;
    }
    
    return infobuf;
}
    
PPDClient::~PPDClient()
{
    delete(client);
    delete(bufAnalogOut);
    delete(bufAnalogIn);
    delete(bufDiscreteOut);
    delete(bufDiscreteIn);
    delete(bufGroupOut);
    delete(bufGroupIn);
}
