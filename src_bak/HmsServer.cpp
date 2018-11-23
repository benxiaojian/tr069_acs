#include "../inc_bak/HmsServer.h"
using namespace std;


HmsServer* HmsServer::instance = NULL;
HnbClientFsm *mDevice = NULL;


HmsServer& HmsServer::GetInstance()
{
	if (instance == NULL) {
		instance = new HmsServer();
		return (*instance);
	}

	return (*instance);
}


HmsServer::~HmsServer()
{
    delete(instance);
}


void HmsServer::RunHttpGetClient()
{
    this->mUrl = mDevice->GetConnectionUrl();

    if (this->mUrl == "")
    {
        log_error1("Http Get URL is NULL, send http get failed");
        return;
    }

    httpc_conn_t *conn;
    hresponse_t *res;

    log_verbose2("Send Http Get to %s", mUrl.c_str());

    int argc = 2;
    char *argv[] = {
        (char*)mUrl.c_str(),
    };
    if (httpc_init(argc, argv))
    {
        log_error1("httpc init failed");
    }

    conn = httpc_new();

    herror_t err = httpc_get(conn, &res, mUrl.c_str());
    hresponse_free(res);
    httpc_free(conn);

    mUrl = "";
}


void HmsServer::SetHttpGetUrl(const char* url)
{
    this->mUrl = url;
}

char* HmsServer::GetHttpGetUrl()
{
    return (char*)mUrl.c_str();
}
