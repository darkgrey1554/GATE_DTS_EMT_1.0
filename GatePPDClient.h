#ifndef GatePPDClient_H
#define GatePPDClient_H

#include "dtsclient.h"
#include <thread>
#include <pthread.h>
#include <vector>

#define TIMEDEV 20

static int FuncRcvPPD(void* argPtr, Value& value, int32_t chnlId);

enum class TypeSignalPPD
{
	Analog,
	Discrete,
	Binar,
	Group
};

struct InfobufPPD
{
	void* buf;
	pthread_mutex_t mutex;
};

struct InfoDataConfig
{
	int frequency;
	int size;
	int channel;

	void clear()
	{
		frequency=0;
		size=0;
		int channel=0;
	}
};

class GatePPDClient
{
	public:

	DTSClient* client;

	float* bufAnalogOut=0;
	float* bufAnalogIn=0;
	int* bufDiscreteOut=0;
	int* bufDiscreteIn=0;
	float* bufBinarOut = 0;
	float* bufBinarIn = 0;
	char* bufGroupOut=0;
	char* bufGroupgIn=0;

	pthread_mutex_t mutex_analog_out = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_analog_in = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_discrete_out = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_discrete_in = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_binar_out = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_binar_in = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_group_out = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_group_in = PTHREAD_MUTEX_INITIALIZER;

	std::thread threadWriteServerDTS;

	int SizeAnalogDataOut = 0;
	int SizeAnalogDataIn = 0;
	int SizeDiscreteDataOut = 0;
	int SizeDiscreteDataIn = 0;
	int SizeBinarDataOut = 0;
	int SizeBinarDataIn = 0;
	int SizeGroupDataOut = 0;
	int SizeGroupDataIn = 0;

	int channel = 0;

	std::vector<FrequencySndData> SndAnalogData;
	std::vector<FrequencySndData> SndDiscreteData;
	std::vector<FrequencySndData> SndGrpData;
	int FrequencySndAnalogData = 0;
	int FrequencySndDiscreteData = 0;
	int FrequencySndBinarData = 0;
	int FrequencyGroupData = 0;

	int statuswork = 0;
	int statusInitClient = 0;

	int read_config_file(const char* Namefile);
	friend int FuncRcvPPD(void* argPtr, Value& value, int32_t chnlId);
	int FuncWriteServerDTS();

    public:

		
		GatePPDClient();
		int InitClientPPD(const char* filename);
		InfobufPPD TakeInfoForReadPDD(TypeSignalPPD type_signal);
		InfobufPPD TakeInfoForWritePDD(TypeSignalPPD type_signal);
		int ReadDataFromPPD(TypeSignalPPD type_signal, void* buf, int offset, int size);
		int WriteDataInPPD(TypeSignalPPD type_signal, void* buf, int offset, int size);
		int startTransferPPD();
		~GatePPDClient();

		int sendvalana(int indx, float val);




};


#endif // !gate_PPD_client_H

