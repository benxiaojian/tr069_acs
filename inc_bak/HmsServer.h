/*
 * main.c
 * Author: Xiao Jian
 */
#pragma once
#include <nanohttp/nanohttp-server.h>
#include <nanohttp/nanohttp-client.h>
#include <nanohttp/nanohttp-logging.h>
#include "../inc_bak/HnbClientFsm.h"

using namespace std;

class HmsServer
{
private:
	HmsServer(){};
	static HmsServer *instance;


public:
	~HmsServer();
	static HmsServer& GetInstance();

	void RunHttpServer(string net_deivce, char *port);
	void RunHttpGetClient();

	void SetHttpGetUrl(const char* url);
	char* GetHttpGetUrl();
};

