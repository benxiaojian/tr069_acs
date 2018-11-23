#ifndef __HNB_CLIENT_FSM_H__
#define __HNB_CLIENT_FSM_H__

//////////////////////////////////////////////
// local include
//////////////////////////////////////////////
#include <nanohttp/nanohttp-server.h>
#include <nanohttp/nanohttp-client.h>
#include <nanohttp/nanohttp-logging.h>
#include <tr1/memory>

#include "../inc_bak/Event.h"
#include "../inc_bak/HttpPostRequest.h"
#include "../inc_bak/RpcMethod.h"
using namespace std;

class HnbClientFsm
{
public:
	HnbClientFsm(const char *name, httpd_conn_t *conn);
	~HnbClientFsm();

	void InjectInform(shared_ptr<Inform> inform);
	void InjectEmpty();
	void InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse);
	void InjectRequestHnbConnection(string &requestString);
	void Set_httpd_conn_t(httpd_conn_t *conn);
	hpair_t* hpairnode_set(const char *key, const char *value, hpair_t * next);
	
	bool GetDeviceOnline(){return mDeviceOnline;}
	string GetConnectionUrl() {return mUrl;}
	bool QueryStateIsIdle(){return mState->mName == "Idle";}

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
			mName(name){}
		~State(){}

		virtual void Entry()
		{
			log_verbose3("%s:%s(entry)", mFsm.mName.c_str(), mName.c_str());
			printf("%s:%s(entry)", mFsm.mName.c_str(), mName.c_str());
			mFsm.mState = this;
		}
		
		virtual void InjectInform(shared_ptr<Inform> inform){}
		virtual void InjectEmpty(){}
		virtual void InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse){}
		virtual void InjectRequestHnbConnection(string &requestString){}
		HnbClientFsm &mFsm;
		string mName;	
	};


	class Idle : public State
	{
	public:
		Idle(HnbClientFsm &fsm):
			State(fsm, "Idle"){}
		~Idle(){}

		void InjectInform(shared_ptr<Inform> inform);
		void InjectRequestHnbConnection(string &requestString);
	};

	class WaitInformInRequestHnbConneciton : public State
	{
	public:
		WaitInformInRequestHnbConneciton(HnbClientFsm &fsm):
			State(fsm, "WaitInformInRequestHnbConneciton"){}
		~WaitInformInRequestHnbConneciton(){}

		void InjectInform(shared_ptr<Inform> inform);
	};

	class WaitEmptyAfterInformInRequestHnbConnection : public State
	{
	public:
		WaitEmptyAfterInformInRequestHnbConnection(HnbClientFsm &fsm):
			State(fsm, "WaitEmptyAfterInformInRequestHnbConnection"){}
		~WaitEmptyAfterInformInRequestHnbConnection(){}

		void InjectEmpty();
	};

	class WaitRpcResponseInRequestHnbConnection : public State
	{
	public:
		WaitRpcResponseInRequestHnbConnection(HnbClientFsm &fsm):
			State(fsm, "WaitRpcResponseInRequestHnbConnection"){}
		~WaitRpcResponseInRequestHnbConnection(){}

		void InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse);
	};

	class WaitEmptyInOtherProcess : public State
	{
	public:
		WaitEmptyInOtherProcess(HnbClientFsm &fsm):
			State(fsm, "WaitEmptyInOtherProcess"){}
		~WaitEmptyInOtherProcess(){}

		void InjectEmpty();
	};

	class WaitEmptyInDiscovery : public State
	{
	public:
		WaitEmptyInDiscovery(HnbClientFsm &fsm):
			State(fsm, "WaitEmptyInDiscovry"){}
		~WaitEmptyInDiscovery(){}

		void InjectEmpty();
	};

	class WaitSetParameterValuesResponseInDiscovery : public State
	{
	public:
		WaitSetParameterValuesResponseInDiscovery(HnbClientFsm &fsm):
			State(fsm, "WaitSetParameterValuesResponseInDiscovery"){}
		~WaitSetParameterValuesResponseInDiscovery(){}

		void InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse);
	};

	class WaitGetParameterNamesResponseInDiscovery : public State
	{
	public:
		WaitGetParameterNamesResponseInDiscovery(HnbClientFsm &fsm):
			State(fsm, "WaitGetParameterNamesResponseInDiscovery"){}
		~WaitGetParameterNamesResponseInDiscovery(){}

		void InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse);
	};

	class WaitGetParameterValuesResponseInDiscovery : public State
	{
	public:
		WaitGetParameterValuesResponseInDiscovery(HnbClientFsm &fsm):
			State(fsm, "WaitGetParameterValuesResponseInDiscovery"){}
		~WaitGetParameterValuesResponseInDiscovery(){}

		void InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse);
	};

	//action
	void SendHttpPost(string &postString);
	void SendHttpPost();
	void SendHttpGet();

	//Guard Conditions
	bool QueryDeviceHasDiscovery(int SerialNumber);

	string mUrl;
	string mName;
	State *mState;
	string mRequestString;
	httpd_conn_t *mConn;
	bool mDeviceOnline;
	Idle mIdle;
	RequestRpcId mRequestRpcId;
	WaitInformInRequestHnbConneciton mWaitInformInRequestHnbConneciton;
	WaitEmptyAfterInformInRequestHnbConnection mWaitEmptyAfterInformInRequestHnbConnection;
	WaitRpcResponseInRequestHnbConnection mWaitRpcResponseInRequestHnbConnection;
	WaitEmptyInOtherProcess mWaitEmptyInOtherProcess;
	WaitEmptyInDiscovery mWaitEmptyInDiscovery;
	WaitSetParameterValuesResponseInDiscovery mWaitSetParameterValuesResponseInDiscovery;
	WaitGetParameterNamesResponseInDiscovery mWaitGetParameterNamesResponseInDiscovery;
	WaitGetParameterValuesResponseInDiscovery mWaitGetParameterValuesResponseInDiscovery;


};

#endif
