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
	bool isThreadExit(){ return _threadExit;};
	void setTreadExit(bool isExit);
	void recordLog(int errorCode);
private:
	/*ITfThreadMgr *_pThreadMgr;
	TfClientId _tfClientId;*/
	CSampleIME* _pTextService;
	bool _threadExit;
	std::thread _processThread;
	SOCKET ClientSocket;
};


#endif