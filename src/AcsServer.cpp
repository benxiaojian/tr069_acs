/*
 * AcsServer.cpp
 * Author: Xiao Jian
 */

#include <AcsServer.h>
#include <NanoHttp.h>
using namespace std;
using namespace acs;

AcsServer *AcsServer::instance = NULL;

AcsServer::AcsServer()
{
    // empty
}


AcsServer::~AcsServer()
{
    // emtpy
}


AcsServer &AcsServer::GetInstance()
{
    if (instance == NULL) {
        instance = new AcsServer();
    }

    return *instance;
}


s32 AcsServer::RunHttpServer(const string &server_ip, const s32 &server_port)
{
    NanoHttp::GetInstance().HttpServerStart(server_ip, server_port);

    return 0;
}


int main()
{

}
