/*
 * HttpImpl.h
 * Author: Xiao Jian
 */
#include <string>

#include <GobalType.h>

namespace acs
{

class HttpImpl
{
public:
    virtual ~HttpImpl() {}

    virtual s32 HttpServerStart(const std::string &, const s32 &) = 0;

private:

};

}
