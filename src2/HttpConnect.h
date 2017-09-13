#ifndef __HTTP_CONNECT_H__
#define __HTTP_CONNECT_H__

//////////////////////////////////////////////
// local include
//////////////////////////////////////////////
#include "Event.h"
using namespace std;

class HttpConnect
{
private:
	HttpConnect(){};
	static HttpConnect *instance;
	static pthread_mutex_t *mutex;

	string mUrl;
	static void default_service(httpd_conn_t *conn, hrequest_t *req);

public:
	~HttpConnect();
	static HttpConnect& GetInstance();

	void RunHttpServer();
	void RunHttpGetClient();

	void SetHttpGetUrl(const char* url);
	char* GetHttpGetUrl();
}

#endif