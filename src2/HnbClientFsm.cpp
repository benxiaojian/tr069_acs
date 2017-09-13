#include "HnbClentFsm.h"

HnbClientFsm::HnbClientFsm(const char* name, httpd_conn_t *conn):
	mName(name),
	mState(NULL),
	mRequestString(""),
	mConn(conn),
	mIdle(*this),
	mRequestRpcId(*this),
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

void HnbClientFsm::InjectRequestHnbConnection()
{
	mState->InjectRequestHnbConnection();
}

void HnbClientFsm::Set_httpd_conn_t(httpd_conn_t *conn)
{
	mConn = conn;
}

void HnbClientFsm::Idle::InjectInform(shared_ptr<Inform> inform)
{
	if (QueryDeviceHasDiscovery(123))
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
	mRequestString = requestString;
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
	mFsm.SendHttpPost(mRequestString);
	mFsm.mWaitRpcResponseInRequestHnbConnection.Entry();
}

void HnbClientFsm::WaitRpcResponseInRequestHnbConnection::InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse)
{
	mFsm.SendHttpPost();
	mFsm.Idle.Entry();
}

void HnbClientFsm::WaitEmptyInDiscovery::InjectEmpty()
{
	map<string, string> paramList;
	paramList["InternetGatewayDevice.ManagementServer.ConnectionRequestUsername"] = "casa";
	paramList["InternetGatewayDevice.ManagementServer.ConnectionRequestPassword"] = "casa";

	string request = set_parameter_values(paramList,"123");
	mFsm.SendHttpPost(request);
	mFsm.mWaitSetParameterValuesResponseInDiscovery.Entry();

}

void HnbClientFsm::WaitGetParameterNamesResponseInDiscovery::InjectResponse(shared_ptr<HttpPostRequest> httpPostResponse)
{
	string paramName = "InternetGatewayDevice.";
	string nextLevel = "false";

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
	mFsm.Idle.Entry();
}

void HnbClientFsm::WaitEmptyInOtherProcess::InjectEmpty()
{
	mFsm.SendHttpPost();
	mFsm.Idle.Entry();
}

void HnbClientFsm::Set_httpd_conn_t(httpd_conn_t *conn)
{
	mConn = conn;
}

void HnbClientFsm::SendHttpPost(string &postString)
{
	log_verbose1("send Http Post");

	conn->out = http_output_stream_new(conn->sock, NULL);

	sprintf(content_len, "%d",(int)strlen(response.c_str()));
	//cout << "content len: " << content_len << endl;

	//hpairnode_free_deep(conn->header);
	conn->header = hpairnode_set(HEADER_CONTENT_TYPE,"text/xml",conn->header);
	conn->header = hpairnode_set(HEADER_CONTENT_LENGTH,content_len,conn->header);

	httpd_send_header(conn, 200, "OK");

	hsocket_send(conn->sock, response.c_str());
}

void HnbClientFsm::SendHttpPost()
{
	log_verbose1("hms send post empty");

	conn->out = http_output_stream_new(conn->sock, NULL);

	//hpairnode_free_deep(conn->header);
	//conn->header = hpairnode_delete(HEADER_CONTENT_TYPE, conn->header);
	conn->header = hpairnode_set(HEADER_CONTENT_LENGTH,"0",conn->header);

	httpd_send_header(conn, 200, "OK");

	hsocket_send(conn->sock, "");

	session_end = true;

	hsocket_close(conn->sock);
}

void HnbClientFsm::SendHttpGet()
{
	cout << "input http get url:";
	string url;
	cin >> url;

	HttpConnect &httpConnect = HttpConnect::GetInstance();
	httpConnect.SetHttpGetUrl(url.c_str());
	httpConnect.RunHttpGetClient();

}

bool HnbClientFsm::QueryDeviceHasDiscovery(int SerialNumber)
{
	return false;
}