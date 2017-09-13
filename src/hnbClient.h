#ifndef __HNB_CLIENT_H__
#define __HNB_CLIENT_H__

//////////////////////////////////////////////
// local include
//////////////////////////////////////////////
#include "rpcMethod.h"
#include "parse.h"
#include "event.h"
//////////////////////////////////////////////
// system include
//////////////////////////////////////////////
#include <nanohttp/nanohttp-server.h>
#include <nanohttp/nanohttp-client.h>
#include <nanohttp/nanohttp-logging.h>
#include <tr1/memory>
using namespace std;

class HnbClientFsm
{
public:
	HnbClientFsm();
	~HnbClientFsm();

private:
	typedef enum EventId_tag
	{
		INFORM,
		REQUESET_HNB_CONNECTION,
		EMPTY,
		SET_PARAMETER_VALUES_RESPONSE,
		GET_PARAMETER_NAMES_RESPONSE,
		GET_PARAMETER_VALUES_RESPONSE,
		ADD_OBJECT_RESPONSE,
		DELETE_OBJECT_RESPONSE
	}EventId;



	class State
	{
	public:
		State(HnbClientFsm &fsm, const char* name):
			mFsm(fsm),
			mname(name)();
		~State();

		virtual void Inject(EventId id, shared_ptr<>)

		HnbClientFsm &mFsm;
		string mname;	
	}
}

#endif