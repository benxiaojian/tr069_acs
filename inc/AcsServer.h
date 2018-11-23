/*
 * AcsServer.h
 * Author: Xiao Jian
 */
#include <string>
#include <iostream>
#include <GobalType.h>
#include <Queueable.h>

namespace acs
{

class AcsServer : public Queueable
{
private:
    AcsServer();
public:
    ~AcsServer();

    s32 RunHttpServer(const std::string &server_ip, const s32 &server_port);
    static AcsServer &GetInstance();

private:
   static AcsServer *instance;
};

}
