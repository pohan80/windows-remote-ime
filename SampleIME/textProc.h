#ifndef TEXTPROC_H
#define TEXTPROC_H

#define WIN32_LEAN_AND_MEAN
#include "Msctf.h"
#include "SampleIME.h"
#include <winsock2.h>
#include <thread>
#include <windows.h>

#define DEFAULT_BUFLEN 10
#define DEFAULT_PORT "40"

class textProc
{
public:
	textProc(CSampleIME *pTextService);
	~textProc();
	void startConnectionProc();
	void setTreadExit(bool isExit);
	void recordLog(int errorCode);
	void stopSocket();
private:
	/*ITfThreadMgr *_pThreadMgr;
	TfClientId _tfClientId;*/
	CSampleIME* _pTextService;
	std::thread _processThread;
	SOCKET ClientSocket;
	SOCKET ListenSocket;
};


#endif