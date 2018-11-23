#include "../inc_bak/HnbClientFsm.h"

HnbClientFsm::HnbClientFsm(const char* name, httpd_conn_t *conn):
	mName(name),
	mState(NULL),
	mUrl(""),
	mRequestString(""),
	mConn(conn),
	mIdle(*this),
	mDeviceOnline(false),
	mRequestRpcId(NO_ACTION),
	mWaitInformInRequestHnbConneciton(*this),
	mWaitEmptyAfterInformInRequestHnbConnection(*this),
	mWaitRpcResponseInRequestHnbConnection(*this),
	mWaitEmptyInOtherProcess(*this),
	mWaitEmptyInDiscovery(*this),
	mWaitSetParameterValuesResponseInDiscovery(*this),
	mWaitGetParameterNamesResponseInDiscovery(*this),
	mWaitGetParameterValuesResponseInDiscovery(*this)
{
	mIdle.Entry();
}

HnbClientFsm::~HnbClientFsm()
{

}

void HnbClientFsm::InjectInform(shared_ptr<Inform> inform)
{
	mUrl = inform->parameterList->mConnectionUrl;
	mState->InjectInform(inform);
}

void HnbClientFsm::InjectEmpty()
{
	mState->InjectEmpty();
}

void HnbClientFsm::InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse)
{
	mState->InjectResponse(httpPostResponse);
}

void HnbClientFsm::InjectRequestHnbConnection(string &requestString)
{
	mState->InjectRequestHnbConnection(requestString);
}

void HnbClientFsm::Set_httpd_conn_t(httpd_conn_t *conn)
{
	mConn = conn;
}

// nanohttp has not hpair set, so w
hpair_t* HnbClientFsm::hpairnode_set(const char *key, const char *value, hpair_t * next)
{
	hpair_t *pair = next;

  	while (pair != NULL)
  	{
  		if (pair->key != NULL)
  		{
  			if (!strcmp(pair->key, key))
  			{
  				free(pair->value);
  				pair->value = (char *) malloc(strlen(value) + 1);
  				strcpy(pair->value, value);

  				return next;
  			}
			pair = pair->next;
  		}
  	}

  	return  hpairnode_new(key,value,next);
}

void HnbClientFsm::Idle::InjectInform(shared_ptr<Inform> inform)
{
	if (mFsm.QueryDeviceHasDiscovery(123))
	{
		log_verbose1("Device has Discovered.");
		string postString = inform_response();
		mFsm.SendHttpPost(postString);
		mFsm.mWaitEmptyInOtherProcess.Entry();
	}
	else
	{
		log_verbose1("Device has not Discovered, now into Discovered process.");
		string postString = inform_response();
		mFsm.SendHttpPost(postString);

		mFsm.mWaitEmptyInDiscovery.Entry();
	}
}

void HnbClientFsm::Idle::InjectRequestHnbConnection(string &requestString)
{
	mFsm.mRequestString = requestString;
	mFsm.SendHttpGet();
	mFsm.mWaitInformInRequestHnbConneciton.Entry();
}

void HnbClientFsm::WaitInformInRequestHnbConneciton::InjectInform(shared_ptr<Inform> inform)
{
	string postString = inform_response();
	mFsm.SendHttpPost(postString);

	mFsm.mWaitEmptyAfterInformInRequestHnbConnection.Entry();
}

void HnbClientFsm::WaitEmptyAfterInformInRequestHnbConnection::InjectEmpty()
{
	mFsm.SendHttpPost(mFsm.mRequestString);
	mFsm.mWaitRpcResponseInRequestHnbConnection.Entry();
}

void HnbClientFsm::WaitRpcResponseInRequestHnbConnection::InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse)
{
	mFsm.SendHttpPost();
	mFsm.mIdle.Entry();
}

void HnbClientFsm::WaitEmptyInDiscovery::InjectEmpty()
{
	map<string, string> paramList;
	paramList["InternetGatewayDevice.ManagementServer.ConnectionRequestUsername"] = "casa";
	paramList["InternetGatewayDevice.ManagementServer.ConnectionRequestPassword"] = "casa";
	string commandKey("123");
	string request = set_parameter_values(paramList,commandKey);
	cout << "http post stream:" << endl << request << endl;
	mFsm.SendHttpPost(request);
	mFsm.mWaitSetParameterValuesResponseInDiscovery.Entry();

}

void HnbClientFsm::WaitSetParameterValuesResponseInDiscovery::InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse)
{
	string paramName = "InternetGatewayDevice.";
	string nextLevel = "true";

	string request = get_parameter_names(paramName, nextLevel);
	mFsm.SendHttpPost(request);
	mFsm.mWaitGetParameterNamesResponseInDiscovery.Entry();
}

void HnbClientFsm::WaitGetParameterNamesResponseInDiscovery::InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse)
{
	set<string> paramNameSet;
	paramNameSet.insert("InternetGatewayDevice.ManagementServer.ConnectionRequestUsername");
	paramNameSet.insert("InternetGatewayDevice.ManagementServer.ConnectionRequestPassword");

	string request = get_parameter_values(paramNameSet);
	mFsm.SendHttpPost(request);
	mFsm.mWaitGetParameterValuesResponseInDiscovery.Entry();
}

void HnbClientFsm::WaitGetParameterValuesResponseInDiscovery::InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse)
{
	mFsm.SendHttpPost();
	mFsm.mDeviceOnline = true;
	mFsm.mIdle.Entry();
}

void HnbClientFsm::WaitEmptyInOtherProcess::InjectEmpty()
{
	mFsm.SendHttpPost();
	mFsm.mIdle.Entry();
}


void HnbClientFsm::SendHttpPost(string &postString)
{
	log_verbose1("send Http Post");

	mConn->out = http_output_stream_new(mConn->sock, NULL);
	
	char content_len[1024] = {};
	sprintf(content_len, "%d",(int)strlen(postString.c_str()));
	//cout << "content len: " << content_len << endl;

	//hpairnode_free_deep(conn->header);
	mConn->header = hpairnode_set(HEADER_CONTENT_TYPE,"text/xml",mConn->header);
	mConn->header = hpairnode_set(HEADER_CONTENT_LENGTH,content_len,mConn->header);

	httpd_send_header(mConn, 200, "OK");

	hsocket_send(mConn->sock, postString.c_str());
}

void HnbClientFsm::SendHttpPost()
{
	log_verbose1("hms send post empty");

	mConn->out = http_output_stream_new(mConn->sock, NULL);

	//hpairnode_free_deep(conn->header);
	//conn->header = hpairnode_delete(HEADER_CONTENT_TYPE, conn->header);
	mConn->header = hpairnode_set(HEADER_CONTENT_LENGTH,"0",mConn->header);

	httpd_send_header(mConn, 200, "OK");

	hsocket_send(mConn->sock, "");

	hsocket_close(mConn->sock);
}

void HnbClientFsm::SendHttpGet()
{
	//cout << "input http get url:";
	//string url;
	//cin >> url;

	//HttpConnect &httpConnect = HttpConnect::GetInstance();
	//httpConnect.SetHttpGetUrl(url.c_str());
	//httpConnect.RunHttpGetClient();

}

bool HnbClientFsm::QueryDeviceHasDiscovery(int SerialNumber)
{
	return false;
}
