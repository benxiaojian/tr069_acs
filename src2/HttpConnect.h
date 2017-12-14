#ifndef __HTTP_CONNECT_H__
#define __HTTP_CONNECT_H__

//////////////////////////////////////////////
// local include
//////////////////////////////////////////////
#include "HnbClientFsm.h"

#include <nanohttp/nanohttp-server.h>
#include <nanohttp/nanohttp-client.h>
#include <nanohttp/nanohttp-logging.h>

extern HnbClientFsm *mDevice;
using namespace std;

class HttpConnect
{
private:
	HttpConnect(){};

	static HttpConnect *instance;

	string mUrl;
	//static void default_service(httpd_conn_t *conn, hrequest_t *req);

public:
	~HttpConnect();
	static HttpConnect& GetInstance();

	void RunHttpServer();
	void RunHttpGetClient();

	void SetHttpGetUrl(const char* url);
	char* GetHttpGetUrl();
};

#endif
