/*
 * NanoHttp.cpp
 * Author: Xiao Jian
 */
#include <HttpImpl.h>

using namespace std;

namespace acs
{

class NanoHttp : public HttpImpl
{
private:
    NanoHttp();

public:
    static NanoHttp &GetInstance();
    virtual ~NanoHttp() {}

    s32 HttpServerStart(const string &server_ip, const s32 &server_port);

private:
    static NanoHttp *instance;
};

}
