#ifndef __EVENT_H__
#define __EVENT_H__

//////////////////////////////////////////////
// local include 
//////////////////////////////////////////////

//////////////////////////////////////////////
// system include
//////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <tr1/memory>
#include <set>
#include <map>
using namespace std;

#define TRACE(pRINTFaRGS...) \
		TracePrintf(pRINTFaRGS);

typedef enum RequestRpc_tag{
	NO_ACTION,
	SEND_EMPTY,
	GET_PARAMETER_NAMES,
	GET_PARAMETER_VALUES,
	SET_PARAMETER_VALUES,
	ADD_OBJECT,
	DELETE_OBJECT,
}RequestRpcId;

#endif
