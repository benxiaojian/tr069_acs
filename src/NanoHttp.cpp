/*
 * NanoHttp.cpp
 * Author: Xiao Jian
 */
#include <cstdlib>
#include <string>
#include <sstream>
#include <NanoHttp.h>
#include <HttpPostRequest.h>
#include <AcsServer.h>
#include <nanohttp/nanohttp-server.h>
#include <nanohttp/nanohttp-client.h>
#include <nanohttp/nanohttp-request.h>
#include <nanohttp/nanohttp-logging.h>

namespace acs
{

#define NHTTPD_ARG_ADDRESS  "-NHTTPaddress"

NanoHttp *NanoHttp::instance = NULL;

static void fap_service(httpd_conn_t *conn, hrequest_t *req)
{
    HttpPostRequest http_post(req);

    if (http_post.parse->body->inform) {
        shared_pte<Inform> inform(http_post.parse->body->inform);
        AcsServer::GetInstance().Enqueue(inform);
    }

}


NanoHttp::NanoHttp()
{
    // empty
}


NanoHttp &NanoHttp::GetInstance()
{
    if (instance == NULL) {
        instance  = new NanoHttp();
    }

    return *instance;
}


s32 NanoHttp::HttpServerStart(const string &server_ip, const s32 &server_port)
{
    s32 ret;

    char strListener[] = "listener";
    char strArgPort [] = NHTTPD_ARG_PORT;
    ostringstream os;
    os << server_port;
    string strPort = os.str();
    char strArgAddress[] = NHTTPD_ARG_ADDRESS;
    char strArgMaxConn[] = NHTTPD_ARG_MAXCONN;
    char strMaxConn[] = "3";
    char strArgTos[] = "TOS";
    char strTos[] = "20";
    char strArgTimeOut[] = NHTTPD_ARG_TIMEOUT;
    char strTimeOut[] = "30";

    const char *argv[] = {
            strListener,
            strArgPort, strPort.c_str(),
            strArgAddress, server_ip.c_str(),
            strArgMaxConn, strMaxConn,
            strArgTos, strTos // O&M QoS ToS - corresponds to QOS_CLASS_E - assert above ensures we validate this!
            };

    log_verbose3("starting thread socketsListener[%s:%s]\n",server_ip, strPort);

    httpd_init(sizeof(argv)/sizeof(argv[0]), (char **)argv);

    ret = httpd_register_default("", fap_service);
    if (!ret) {
        log_error1("can not register default service");
        return ret;
    }

    httpd_run();

    return 0;
}

}
