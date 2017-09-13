#include "HttpConnect.h"
using namespace std;

HttpConnect* HttpConnect::instance = NULL;

HttpConnect& HttpConnect::GetInstance()
{
	if (instance == NULL)
	{
		pthread_mutex_lock(mutex);
		instance = new HttpConnect();
		pthread_mutex_unlock(mutex);

		return (*instance);
	}

	return (*instance);
}

HttpConnect::~HttpConnect()
{

}

static void default_service(httpd_conn_t *conn, hrequest_t *req)
{
	HttpPostRequest *hprequest = new HttpPostRequest(req);
	if (mDevice == NULL)
	{
		mDevice = new HnbClientFsm("hnb client", conn);
	}
	else
	{
		mDevice->Set_httpd_conn_t(conn);
	}

	if (!req->in->content_length)
	{
		mDevice->InjectEmpty();
	}
	else if (hprequest->parse->body->inform)
	{
		mDevice->InjectInform(hprequest->parse->body->inform);
	}
	else
	{
		mDevice->InjectResponse(hprequest);
	}
}

void HttpConnect::RunHttpServer()
{
	char localIpAddressAsStringU32[15] = {0};
	int socketNumber = socket(AF_INET,SOCK_STREAM,0);

	struct ifreq ifReq;
	memset(&ifReq, 0, sizeof(struct ifreq));

	strcpy(ifReq.ifr_name, "eth0");
	ifReq.ifr_addr.sa_family = AF_INET;

	if (ioctl(socketNumber, SIOCGIFADDR, &ifReq) == 0)
	{
		struct sockaddr_in sin;

		memcpy(&sin, &ifReq.ifr_addr, sizeof(sin));

		strcpy(localIpAddressAsStringU32, inet_ntoa(sin.sin_addr));

	}

	char strListener[] = "listener";
	char strArgPort [] = NHTTPD_ARG_PORT;
	char strPort[] = "8080";
	char strArgAddress[] = NHTTPD_ARG_ADDRESS;
    char strArgMaxConn[] = NHTTPD_ARG_MAXCONN;
    char strMaxConn[] = "3";
    char strArgTos[] = "TOS";
    char strTos[] = "20";
    char strArgTimeOut[] = NHTTPD_ARG_TIMEOUT;
    char strTimeOut[] = "30";


    const char *argv[] = {
            strListener,
            strArgPort, strPort,
            strArgAddress, localIpAddressAsStringU32,
            strArgMaxConn, strMaxConn,
            strArgTos, strTos // O&M QoS ToS - corresponds to QOS_CLASS_E - assert above ensures we validate this!
            };


    log_verbose3("starting thread socketsListener[%s:%s]\n",localIpAddressAsStringU32, strPort);

    httpd_init( sizeof(argv)/sizeof(argv[0]) , (char **)argv );


    int result = httpd_register_default("", default_service);
    if (!result)
    {
    	log_error1("can not register default service");
    }

    httpd_run();
}

void HttpConnect::RunHttpGetClient()
{
	if (mUrl == "")
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

void HttpConnect::SetHttpGetUrl(const char* url)
{
	mUrl = url;
}

char* HttpConnect::GetHttpGetUrl()
{
	return mUrl.c_str();
}

void *WaitUserInput(void*)
{
	while (1)
	{
		if (session_end == true)
		{
			
			sleep(1);
			cout << "input rpc method number id :" << endl;
			cout << "0: close server" << endl;
			cout << "1: get parameter name" << endl;

			cout << "please input number id: ";
			int rpc_number = -1;
			cin >> rpc_number;

			switch(rpc_number)
			{
				case 0:
					cout <<  "close server..." << endl;
					destroy();
					exit(0);
					break;
				case 1:
					cout << "send http get, get parameter names..." << endl; 
					session_end = false;
					input_rpc = GET_PARAMETER_NAMES;
					httpGet();
					break;
			}
		}// end if
	}//end while
}

int main()
{
	HttpConnect& httpConnect = HttpConnect::GetInstance();

	pthread_t clinetWatitUserInput;

	httpConnect.RunHttpServer();
	
}