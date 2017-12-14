//////////////////////////////////////////////
// hms_server
// code by dzj on 2017.7
//////////////////////////////////////////////

//////////////////////////////////////////////
// local include
//////////////////////////////////////////////
#include "rpcMethod.h"
#include "parse.h"

//////////////////////////////////////////////
// system include
//////////////////////////////////////////////
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <nanohttp/nanohttp-server.h>
#include <nanohttp/nanohttp-client.h>
#include <nanohttp/nanohttp-logging.h>
#include <tr1/memory>
using namespace std;

bool device_online = false;
bool inform_empty = true;
bool session_end = false;
char url[512] = "http://192.168.57.66:7547";
pthread_t client_thread;

typedef enum{
	EMPTY,
	INFORM,
	GET_PARAMETER_NAMES,
	GET_PARAMETER_VALUES,
	SET_PARAMETER_VALUES,
	ADD_OBJECT,
	DELETE_OBJECT,
}RPC;

RPC input_rpc = GET_PARAMETER_NAMES;
string getParameterNamesPath;
string getParameterNamesNextLevel;

set<string> getParameterValuesSet;
map<string,string> setParameterValueMap;

void destroy();
void httpGet();
void httpPost(httpd_conn_t *conn, hrequest_t *req, RPC rpc);
void *http_client(void*);

hpair_t* hpairnode_set(const char *key, const char *value, hpair_t * next);

void signal_handle(int signal_num)
{
	cout << __FUNCTION__ << endl;

	destroy();
	exit(0);
}

extern "C"
{
    extern void httpd_term(int sig);
}

void destroy()
{
	cout << "destroy" << endl;

	httpd_term(2);

	httpd_destroy();
}

static void default_service(httpd_conn_t *conn, hrequest_t *req)
{

	HttpPostRequest *hprequest = new HttpPostRequest(req);
	if (!req->in->content_length)	
	{
		if (inform_empty == true && device_online == false)
		{
			httpPost(conn,req,GET_PARAMETER_NAMES);
			inform_empty = false;
			device_online = true;
		}
		else if (inform_empty == true && device_online == true)
		{
			httpPost(conn,req,input_rpc);
			inform_empty = false;
		}
		else 
			httpPost(conn,req,EMPTY);
	}
	else if (hprequest->parse->body->inform)
	{
		session_end = true;
		httpPost(conn,req,INFORM);
		inform_empty = true;
	}
	else
	{
		httpPost(conn,req,EMPTY);
	}

	//cout << "end default_service" << endl;
	hprequest->~HttpPostRequest();

	return;
}

void *ThreadProcedure(void*)
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


    printf("starting thread socketsListener[%s:%s]\n",localIpAddressAsStringU32, strPort);

    httpd_init( sizeof(argv)/sizeof(argv[0]) , (char **)argv );


    int result = httpd_register_default("", default_service);
    if (!result)
    {
    	cout << "can not register default service" << endl;
    }

    httpd_run();

    log_verbose1("end httpd_run");
}


void userChooseRpc()
{

}

int main()
{
	signal(SIGINT,signal_handle);

	hlog_set_file("nanohttp.log");
	hlog_set_level(HLOG_VERBOSE);

	pthread_create(&client_thread, NULL, http_client, NULL);

	ThreadProcedure(NULL);

	/*
	pthread_t server_thread;

	hlog_set_file("nanohttp.log");
	hlog_set_level(HLOG_VERBOSE);
	int err = pthread_create(&server_thread, NULL, ThreadProcedure, NULL);

	if (err != 0)
	{
		cout << "pthread create failed, exit 1" << endl;
		exit(1);
	}

	//ThreadProcedure(NULL);

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
					destroy();
					session_end = false;
					input_rpc = GET_PARAMETER_NAMES;
					httpGet();
					break;
			}
		}// end if
	}//end while
	*/
}//end main

// nanohttp has not hpair set, so w
hpair_t* hpairnode_set(const char *key, const char *value, hpair_t * next)
{
	hpair_t *pair = next;

  	while (pair != NULL)
  	{
  		if (pair->key != NULL)
  		{
  			if (!strcmp(pair->key, key))
  			{
  				free(pair->value);
  				pair->value = (char *) malloc(strlen(value) + 1);
  				strcpy(pair->value, value);

  				return next;
  			}
			pair = pair->next;
  		}
  	}

  	return  hpairnode_new(key,value,next);
}

hpair_t* hpairnode_delete(const char *key, hpair_t* next)
{
	hpair_t *pair = next;
	hpair_t *pre = NULL;

	while (pair != NULL)
	{
		if (pair->key != NULL)
		{
  			if (!strcmp(pair->key, key))
			{
				if(pre == NULL)
				{
					pre = pair->next;
				}
				else
				{
					pre->next = pair->next;
				}

				hpairnode_free(pair);
				return pre;
			}
		}

		pre = pair;
		pair = pair->next;
	}

	return next;
}

void httpGet()
{
	httpc_conn_t *conn;
	hresponse_t *res;
	
	cout << "input hnb connection url:";
	cin >> url;
	cout << "url:" << url << endl;
	int argc = 2;
	char *argv[] = {
		(char*)url,
	};
	if (httpc_init(argc, argv))
	{
		cout << "httpc init failed" << endl;
	}

	conn = httpc_new();

	herror_t err = httpc_get(conn, &res, url);
	log_verbose1("end http_get");
	hresponse_free(res);
	httpc_free(conn);
	
	log_verbose1("end http client");
	return;
}

void httpPost(httpd_conn_t *conn, hrequest_t *req, RPC rpc)
{
	string response;
	char content_len[15] = {0};

	switch(rpc)
	{
		case EMPTY:
		{
			//send empty
			response = "";
		}
		break;

		case INFORM:
		{
			response = inform_response();
			conn->out = http_output_stream_new(conn->sock, NULL);
		}
		break;

		case GET_PARAMETER_NAMES:
		{
			response = get_parameter_names(getParameterNamesPath,getParameterNamesNextLevel);
			//cout << response << endl;
		}
		break;

		case GET_PARAMETER_VALUES:
		{
			response = get_parameter_values(getParameterValuesSet);
		}
		break;

		case SET_PARAMETER_VALUES:
		{
			string commandKey = "123";
			response = set_parameter_values(setParameterValueMap, commandKey);
		}
		break;

		case ADD_OBJECT:
		case DELETE_OBJECT:
		break;
	}

	if (response == "")
	{
		cout << "hms send post empty" << endl;

		conn->out = http_output_stream_new(conn->sock, NULL);

		//hpairnode_free_deep(conn->header);
		//conn->header = hpairnode_delete(HEADER_CONTENT_TYPE, conn->header);
		conn->header = hpairnode_set(HEADER_CONTENT_LENGTH,"0",conn->header);

		httpd_send_header(conn, 200, "OK");

		hsocket_send(conn->sock, "");

		session_end = true;

		hsocket_close(conn->sock);
	}
	else
	{
		conn->out = http_output_stream_new(conn->sock, NULL);

		sprintf(content_len, "%d",(int)strlen(response.c_str()));
		//cout << "content len: " << content_len << endl;

		//hpairnode_free_deep(conn->header);
		conn->header = hpairnode_set(HEADER_CONTENT_TYPE,"text/xml",conn->header);
		conn->header = hpairnode_set(HEADER_CONTENT_LENGTH,content_len,conn->header);

		httpd_send_header(conn, 200, "OK");

		hsocket_send(conn->sock, response.c_str());
	}
}


void *http_client(void*)
{
	while (1)
	{
		if (session_end == true)
		{
			
			sleep(1);
			cout << "input rpc method number id :" << endl;
			cout << "0: close server" << endl;
			cout << "1: get parameter name" << endl;
			cout << "2: get parameter value" << endl;
			cout << "3: set parameter value" << endl;

			cout << "please input number id: ";
			int rpc_number = -1;
			cin >> rpc_number;

			switch(rpc_number)
			{
				case 0:
				{
					cout <<  "close server..." << endl;
					destroy();
					exit(0);
					break;
				}
				case 1:
				{
					cout << "send http get, get parameter names..." << endl; 
					session_end = false;
					input_rpc = GET_PARAMETER_NAMES;
					cout << "input get parameter name path:";
					cin >> getParameterNamesPath;
					cout << "input get parameter name nextLevel:";
					cin >> getParameterNamesNextLevel;
					httpGet();
					break;
				}
				case 2:
				{
					cout << "send http get, get parameter values..." << endl;
					session_end = false;
					input_rpc = GET_PARAMETER_VALUES;
					cout << "input get parameter names, input end to stop input" << endl;
					string path = "1";
					getParameterValuesSet.clear();
					while (1)
					{
						cin >> path;
						if (path == "end") break;
						getParameterValuesSet.insert(path);
					}
					httpGet();
					break;
				}
				case 3:
				{
					cout << "send http get, set parameter values..." << endl;
					session_end = false;
					input_rpc = SET_PARAMETER_VALUES;
					cout << "the commandKey is defautl value(123)" << endl;
					cout << "input get parameter names and value, use space to split, input end to stop input" << endl;
					cout << "eg. A B" << endl << "the name is A, the value is B" << endl;
					string a,b;
					setParameterValueMap.clear();
					while (1)
					{
						cin >> a;
						if (a == "end") break;
						cin >> b;

						setParameterValueMap[a] = b;
					}
					httpGet();
					break;
				}
				default:
					break;
			}
		}// end if
	}//end while
}
