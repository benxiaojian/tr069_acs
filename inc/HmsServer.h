/*
 * main.c
 * Author: Xiao Jian
 */
#pragma once
#include <HnbClientFsm.h>

#include <nanohttp/nanohttp-server.h>
#include <nanohttp/nanohttp-client.h>
#include <nanohttp/nanohttp-logging.h>

extern HnbClientFsm *mDevice;
using namespace std;

class HmsServer
{
private:
	HmsServer(){};

	static HmsServer *instance;

	string mUrl;
	//static void default_service(httpd_conn_t *conn, hrequest_t *req);

public:
	~HmsServer();
	static HmsServer& GetInstance();

	void RunHttpServer(string net_deivce, char *port);
	void RunHttpGetClient();

	void SetHttpGetUrl(const char* url);
	char* GetHttpGetUrl();
};

