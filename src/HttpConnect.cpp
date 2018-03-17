#include <HttpConnect.h>

using namespace std;

#define NHTTPD_ARG_ADDRESS	"-NHTTPaddress"

HttpConnect* HttpConnect::instance = NULL;
HnbClientFsm *mDevice = NULL;


static void default_service(httpd_conn_t *conn, hrequest_t *req);

HttpConnect& HttpConnect::GetInstance()
{
	if (instance == NULL)
	{
		instance = new HttpConnect();
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
		shared_ptr<Inform> inform(hprequest->parse->body->inform);
		mDevice->InjectInform(inform);
	}
	else
	{
		shared_ptr<HttpPostRequest> hp(hprequest);
		mDevice->InjectResponse(hp);
	}

	//delete hprequest;
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
	char strPort[] = "8083";
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
	mUrl = mDevice->GetConnectionUrl();

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
	return (char*)mUrl.c_str();
}

void *WaitUserInput(void*)
{
	cout << "WaitUserInput" << endl;
	while (1)
	{
		if (mDevice && mDevice->QueryStateIsIdle() && mDevice->GetDeviceOnline())
		{
			
			sleep(1);
			cout << "input rpc method number id :" << endl;
			cout << "0: close server" << endl;
			cout << "1: get parameter name" << endl;
			cout << "2: get parameter value" << endl;
			cout << "3: set parameter value" << endl;
			cout << "4: add object" << endl;
			cout << "5: delete object" << endl;

			cout << "please input number id: ";
			int rpc_number = -1;
			cin >> rpc_number;

			switch(rpc_number)
			{
				case 0:{
						cout <<  "close server..." << endl;
						//destroy();
						exit(0);
					}
					break;

				case 1:{
					cout << "send http get, get parameter names..." << endl; 
					//input_rpc = GET_PARAMETER_NAMES;                   
					string getParameterNamesPath;
					string getParameterNamesNextLevel;
					cout << "input get parameter name path:";
                    cin >> getParameterNamesPath;
                    cout << "input get parameter name nextLevel:";
					cin >> getParameterNamesNextLevel;
					
					string requestString = get_parameter_names(getParameterNamesPath,getParameterNamesNextLevel);
					mDevice->InjectRequestHnbConnection(requestString);
					HttpConnect::GetInstance().RunHttpGetClient();			
					}
					break;

				case 2:{
					cout << "send http get, get parameter value..." << endl;
					set<string> paramName;
					cout << "input parameter Names, finally input # to end input" << endl;
					string tmp;
					while (1)
					{
						cin >> tmp;
						if (tmp == "#")
							break;

						paramName.insert(tmp);
					}
					string requestString = get_parameter_values(paramName);
					mDevice->InjectRequestHnbConnection(requestString);
					HttpConnect::GetInstance().RunHttpGetClient();
					}
					break;

				case 3:{
					cout << "send http get, set parameter values..." << endl;
					//set_parameter_values(map<string,string> &paramList, string &paramKey)
					map<string,string> paramList;
					string paramKry("123");
					cout << "the commandKey is defautl value(123)" << endl;
					cout << "input get parameter names and value, use space to split, input # to stop input" << endl;
					cout << "eg. A B" << endl << "the name is A, the value is B" << endl;
					while (1)
					{
						string a,b;
						cin >> a;
						if (a == "#") break;

						cin >> b;
						paramList[a] = b;
					}
					string requestString = set_parameter_values(paramList,paramKry);
					mDevice->InjectRequestHnbConnection(requestString);
					HttpConnect::GetInstance().RunHttpGetClient();
					}
					break;

				case 4:{
					cout << "send http get, add object..." << endl;
					string objectName;
					string parameterKey;
					cout << "input object name:";
					cin >> objectName;
					cout << "input parameter key:";
					cin >> parameterKey;

					string requestString = add_object(objectName,parameterKey);
					mDevice->InjectRequestHnbConnection(requestString);
					HttpConnect::GetInstance().RunHttpGetClient();
					}
					break;

				case 5:{
					cout << "send http get, delete object..." << endl;
					string objectName;
					string parameterKey;
					cout << "input object name:";
					cin >> objectName;
					cout << "input parameter key:";
					cin >> parameterKey;

					string requestString = delete_object(objectName,parameterKey);
					mDevice->InjectRequestHnbConnection(requestString);
					HttpConnect::GetInstance().RunHttpGetClient();
					}
					break;
			}
		}// end if
	}//end while
}

int main()
{
	HttpConnect& httpConnect = HttpConnect::GetInstance();

	pthread_t clientWatitUserInput;
	pthread_create(&clientWatitUserInput, NULL, WaitUserInput, NULL);

	httpConnect.RunHttpServer();
	
}
