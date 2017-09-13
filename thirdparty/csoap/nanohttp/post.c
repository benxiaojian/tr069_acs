#include <stdio.h>
#include <string.h>
#include <nanohttp/nanohttp-client.h>
#include <libcsoap/soap-server.h>
#include <netdb.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "post.h"


int user = 0;
char *path = "http://172.0.10.15:8080/hems-web-ui/ws/cwmp/";
hurl_t url;
char buffer[4096];
herror_t status;
int ssl;
httpc_conn_t *conn;
char *ID;
int  connectRequest = 0;
int rpc = 0;




int hsocket_open_ipsec(hsocket_t * dsock, const char *hostname, int port);
int Connect();
void httpc_header_free();
void httpc_header_add_date(void);
void httpc_header_new(hreq_method_t method,int txContentLength);
int post(char *content,hresponse_t **response);
int choiceRPC();




int hsocket_open_ipsec(hsocket_t * dsock, const char *hostname, int port)
{
	struct sockaddr_in address;
	struct sockaddr_in localInnerIpAddr;
	struct hostent *host = NULL;
	char *ip = NULL;
	
	memset( &address, 0, sizeof(address) );
	memset( &localInnerIpAddr, 0, sizeof(localInnerIpAddr));

	if ((dsock->sock = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
	{
		return 0;
	}

	// getsockopt call is need only to prevent setsockopt to fail
	{
		int getVal = 0;
		socklen_t len = sizeof(getVal);
		getsockopt(dsock->sock, SOL_SOCKET, IP_TOS, &getVal, &len );
	}
	
	int val = (int)0x14;
	int err = setsockopt(dsock->sock, SOL_IP, IP_TOS, &val, ((socklen_t)sizeof(val)) );
	if (err < 0)
	{
		return 0;
	}
	
	if (!(host = gethostbyname(hostname)))
	{
		return 0;
	}
	
	ip = inet_ntoa(*(struct in_addr *) *host->h_addr_list);
	address.sin_addr.s_addr = inet_addr(ip);

	/* set server address */
	address.sin_family = host->h_addrtype;
	address.sin_port = htons((unsigned short) port);

	localInnerIpAddr.sin_family = AF_INET;
	localInnerIpAddr.sin_port = 0;
	localInnerIpAddr.sin_addr.s_addr = inet_addr("172.0.10.148");
	
	if (bind(dsock->sock, (struct sockaddr *) &localInnerIpAddr, sizeof(struct sockaddr)) ==
      -1)
	{
		return 0;
	}
	else
	{
		
		printf("connection to %s:%u", ip,port);
		printf("successful bind to %s:%d", inet_ntoa(localInnerIpAddr.sin_addr), ntohs(localInnerIpAddr.sin_port) );
	}
	
	if (connect(dsock->sock, (struct sockaddr *) &address, sizeof(address)) != 0)
	{
		//printf("TR069:hsocket_open_ipsec[connect] failure (%i, %s).", err, strerror( errno ) );

		return 0;
	}
	else
	{ 
		printf("Connected to %s:%d", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
	}

}


int Connect()
{
	if (H_OK == hurl_parse(&url,path))
	{
		conn = httpc_new();
		
		if (hsocket_open_ipsec(&conn->sock,url.host,url.port))
		{
			return 1;
		}
		else
		{
			printf("httpc_new failure\n");
			return -1;
		}

	}// end hurl_parse
	else
	{
		printf("hurl_parse failure\n");
		return -1;
	}
}

void httpc_header_free()
{
	while (conn->header != NULL)
	{
		hpair_t *tmp = conn->header;
		conn->header = conn->header->next;
		hpairnode_free(tmp);
	}
}


void httpc_header_add_date(void)
{
	char buffer[32];
	time_t ts;
	struct tm stm;

	ts = time(NULL);
	localtime_r(&ts,&stm);
	strftime(buffer, 32, "%a, %d %b %Y %H:%M:%S GMT", &stm);

	httpc_add_header(conn,HEADER_DATE,buffer);
}

void httpc_header_new(hreq_method_t method,int txContentLength)
{

	char header[1024];
	memset(header,0,sizeof(header));

	if (conn->header)
	{
		httpc_header_free();
	}
	
	if (txContentLength)
	{
		httpc_add_header(conn,HEADER_CONTENT_TYPE,"text/xml");
	}

	if (user == 1)
	{
		printf("add username\n");
		httpc_add_header(conn, "Cookie", "Cookie");//key = Cookie, is valid but Value = Cookie, is just a dummy value
		httpc_set_basic_authorization(conn, "casa", "casa");
		httpc_add_header(conn, "Password", "casa");
		httpc_add_header(conn, "Username", "casa");
	}
		

	
	char tmp[15];
	sprintf(tmp,"%d",(int)txContentLength);
	httpc_add_header(conn,HEADER_CONTENT_LENGTH,tmp);

	httpc_header_add_date();

	httpc_add_header(conn,HEADER_HOST,url.host);
	/*
	switch(method)
	{
		
		case HTTP_REQUEST_GET:
			sprintf(buffer,"GET %s HTTP/%s\r\n",
			(url.context[0] != '\0') ? url.context: ("/"),
			(conn->version == HTTP_1_0) ? "1.0" : "1.1");
			break;
		
		case HTTP_REQUEST_POST:
			sprintf(buffer,"POST %s HTTP/%s\r\n",
			(url.context[0] != '\0') ? url.context: ("/"),
			(conn->version == HTTP_1_0) ? "1.0" : "1.1");
			break;
		
		
	}

	hsocket_send(&(conn->sock),buffer);

	httpc_send_header(conn);
	*/
	switch(method)
	{
		
		case HTTP_REQUEST_GET:
			sprintf(buffer,"GET %s HTTP/%s\r\n",
			(url.context[0] != '\0') ? url.context: ("/"),
			(conn->version == HTTP_1_0) ? "1.0" : "1.1");
			break;
		
		case HTTP_REQUEST_POST:
			sprintf(buffer,"POST %s HTTP/%s\r\n",
			(url.context[0] != '\0') ? url.context: ("/"),
			(conn->version == HTTP_1_0) ? "1.0" : "1.1");
			break;
	}
	
	hpair_t *walker;

    for (walker = conn->header; walker; walker = walker->next)
    {
        if (walker->key && walker->value)
        {
			strcat(buffer,walker->key);
			strcat(buffer,":");
            strcat(buffer,walker->value);
            strcat(buffer,"\r\n");
        }
    }
	
	strcat(buffer,"\r\n");
	
}

void disconnect()
{
	if (conn)
	{
		printf("close connection\n");
		httpc_close_free(conn);
		//httpc_free(conn);
	}
	conn = NULL;
	user = 0;
}


char * ExtractFromNode( xmlNodePtr node )
{
    char *xmlString = NULL;
    xmlChar * xmlNodeListString = xmlNodeListGetString( node->doc, node->xmlChildrenNode, 1);

    if(xmlNodeListString)
    {
        xmlString = (char *)xmlNodeListString;
        //xmlFree(xmlNodeListString);
    }

    return xmlString;
}


/***********InformResponse RPC end*****************/

void Tr069ParseBody(xmlNodePtr cur)
{
    printf("TR069-Parse:" "Body");
	printf("\n");
    /* We don't care what the top level element name is */
    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
        if(!strcmp((const char*)cur->name, "AddObject"))
        {
            //addObject = shared_ptr<AddObject>(new AddObject(cur));
			rpc = 9;
        }
        else if(!strcmp((const char*)cur->name, "SetParameterValues"))
        {
 	        // SetParameterValues(cur);
			rpc = 7;
        }
        else if(!strcmp((const char*)cur->name, "SetParameterAttributes"))
        {
            //setParameterAttributes = shared_ptr<Tr069ParseSetParameterAttributes>(new Tr069ParseSetParameterAttributes(cur));
        }
        else if(!strcmp((const char*)cur->name, "InformResponse"))
        {
            //Tr069ParseInformResponse(cur);
			rpc = 4;
        }
        else if(!strcmp((const char*)cur->name, "GetParameterValues"))
        {
            //getParameterValues = shared_ptr<Tr069ParseGetParameterValues>(new Tr069ParseGetParameterValues(cur));
			rpc = 6;
        }
        else if(!strcmp((const char*)cur->name, "Download"))
        {
            //download = shared_ptr<Tr069ParseDownload>(new Tr069ParseDownload(cur));
        }
        else if(!strcmp((const char*)cur->name, "FactoryReset"))
        {
            //factoryReset = shared_ptr<Tr069ParseFactoryReset>(new Tr069ParseFactoryReset(cur));
        }
        else if(!strcmp((const char*)cur->name, "Fault"))
        {
            //fault = shared_ptr<Tr069ParseFault>(new Tr069ParseFault(cur));
        }
        else if(!strcmp((const char*)cur->name, "TransferCompleteResponse"))
        {
            //transferCompleteResponse = shared_ptr<Tr069ParseTransferCompleteResponse>(new Tr069ParseTransferCompleteResponse(cur));
        }
        else if(!strcmp((const char*)cur->name, "Reboot"))
        {
            //reboot = shared_ptr<Tr069ParseReboot>(new Tr069ParseReboot(cur));
			rpc = 8;
        }
        else if(!strcmp((const char*)cur->name, "Upload"))
        {
            //upload = shared_ptr<Tr069ParseUpload>(new Tr069ParseUpload(cur));
        }
		else if (!strcmp((const char*)cur->name, "GetParameterNames"))
		{
			rpc = 5;
		}
		else if (!strcmp((const char*)cur->name, "DeleteObject"))
		{
			rpc = 10;
		}
        else
        {
            printf("TR069-Parse:" "Unhandled %s", (const char*)cur->name);
        }

        cur = cur->next;
    }
}


void Tr069ParseHeader(xmlNodePtr cur)
{
    printf("TR069-Parse:" "Header");

    char *holdRequests = NULL; //just for trace

    /* We don't care what the top level element name is */
    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
		//puts(cur->name);
        if(!strcmp((const char*)cur->name, "ID"))
        {
            holdRequests = ExtractFromNode(cur);
			
			if (ID)
				free(ID);
			if (holdRequests!=NULL)
			{
				puts(holdRequests);
            	printf("TR069-Parse:" "HoldRequests=%s\n", holdRequests );
				ID = (char*)malloc(strlen(holdRequests)*sizeof(char));
				strcpy(ID,holdRequests);
	      		xmlFree((xmlChar *)holdRequests); //free buffer
			}
        }
        else
        {
            printf("TR069-Parse:" "Unhandled %s", (const char*)cur->name);
        }

        cur = cur->next;
    }
}

void Tr069Parse(xmlNodePtr cur)
{
    printf("TR069-Parse:" ">>");
    printf("TR069-Parse:" "Envelope");
	printf("\n");
    /* We don't care what the top level element name is */
    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
        if(!strcmp((const char*)cur->name, "Body"))
        {
       		Tr069ParseBody(cur);
			printf("rpc is : %d\n",rpc);
        }
        else if(!strcmp((const char*)cur->name, "Header"))
        {
            Tr069ParseHeader(cur);
        }
        else
        {
            printf("TR069-Parse:" "Unhandled %s", (const char*)cur->name);
        }

        cur = cur->next;
    }
}


/** init a soap context **/
SoapCtx * InitSoapCtx(SoapEnv * soapEnv)
{
    SoapCtx * soapCtx = NULL;
    char *xmlCnt =NULL; //just for trace

    if(soapEnv)
    {
        soapCtx = soap_ctx_new(soapEnv);

        if(soapCtx)
        {
            xmlBufferPtr buffer = xmlBufferCreate(); //create a xml format string buffer

            xmlDocPtr doc = soapCtx->env->root->doc; //xml doc pointer
            xmlNodePtr cur = soapCtx->env->root;

            xmlNodeDump(buffer, doc, cur, 1, 0 ); //dump xml node into string buffer

            xmlCnt = (char *) xmlBufferContent(buffer); //get xml string buffer pointer
            printf("%s", xmlCnt); //print

            xmlBufferFree(buffer);  //free xml buffer

            Tr069Parse(cur);
        }
    }

    return soapCtx;
}
 
int post(char *content,hresponse_t **response)
{
	char *txHttpPost;
	
	httpc_header_new(HTTP_REQUEST_POST,strlen(content));

	conn->out = http_output_stream_new(&conn->sock,conn->header);

	txHttpPost = (char*) malloc((strlen(content)+strlen(buffer)) * sizeof(char));
	//puts(content);
	sprintf(txHttpPost,"%s%s",buffer,content);
	
	http_output_stream_write_string(conn->out,txHttpPost);

	printf("soap strem:");
	puts(txHttpPost);
	
	httpc_post_end(conn,response);

	http_output_stream_free(conn->out);
	conn->out = NULL;
}


void HttpPostResponse(hresponse_t  * response) 
{
    SoapCtx *m_SoapCtx = NULL;
    
    /* Create Response Context */
    /*
	if(response && response->in &&
        response->in->type != HTTP_TRANSFER_CONNECTION_CLOSE &&
        response->in->content_length != 0 )
	*/
	if (1)
    {
        SoapEnv * soapEnv = NULL;

        soap_env_new_from_stream(response->in, &soapEnv); //new a soap envelope from response stream

        m_SoapCtx = InitSoapCtx(soapEnv);
    }
    else
    {
        SoapEnv * soapEnv = NULL;

        soap_env_new_from_buffer("<empty/>", &soapEnv);

        m_SoapCtx = InitSoapCtx(soapEnv);

        if(response)
        {
            if(response->in)
            {
                printf("TR069:httpPostWithResponse end. (HTTP_TRANSFER_CONNECTION_CLOSE).");
            }
            else
            {
                printf("TR069:httpPostWithResponse failure (No response->in).");
            }
        }
        else
        {
            printf("TR069:httpPostWithResponse failure (No response).");
        }
    }
}

void root_service(httpd_conn_t *m_conn, hrequest_t *req)
{
	httpd_send_header(m_conn,200,"OK");
	hsocket_send(m_conn->sock,"");
	printf("root service\n");
	connectRequest = 1;

	puts("server into choice RPC");
	while (1)
	{
		choiceRPC();
	}

	return;
}

void get_server()
{
	int listenfd;
	int sockfd;
	struct sockaddr_in address;
	
	/*
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		printf("server socket failure\n");
		exit(1);
	}

	memset(&address,0,sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons((unsigned short)7357);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd,(struct sockaddr*)&address,sizeof(struct sockaddr)) == -1)
	{
		printf("server bind failure\n");
		exit(1);
	}


	listen(sockfd,15);

	while(1)
	{
		listenfd = accept(sockfd,(struct sockaddr *)NULL,NULL);
		
		connectRequest = 1;
	}

	*/

	httpd_init(0,NULL);

	if (!httpd_register("/",root_service))
	{
		printf("httpd register failure\n");
		return;
	}

	printf("http server run\n");
	if (httpd_run())
	{
		printf("http server run failuer\n");
		return;
	}
}


int choiceRPC()
{
	int n;
	char *content;
	pthread_t server;
	hresponse_t *res;
	
	
	printf("1:connect\n");
	printf("2:disconnect\n");
	printf("3:inform\n");
	printf("4:post empty\n");
	printf("5:get paramter name\n");
	printf("6:get parameter value\n");
	printf("7:set parameter value\n");
	printf("8:reboot\n");
	printf("9:add object\n");
	printf("10:delete object\n");
	printf("11:inform transfer complete\n");
	printf("12:transfer complete\n");

	printf("input:");
	scanf("%d",&n);
		

	if (!content)
		free(content);
	
	switch(n)
	{
		case 1:
			{
				Connect();
				break;
			}
		case 2:
			{
				disconnect();
				get_server();
				break;
			}
		case 3:
			{
				if (conn) printf("conn is not NULL\n");
				if (conn->sock.sock) printf("scok is not NULL\n");
				char *info;
				//puts(informConnectRequest);
				// the inform connect request must have the ID from the hms send brfore;
				if (connectRequest == 1)
				{
					info = informConnectRequest;
					connectRequest = 0;
					content = (char*)malloc((strlen(info)+strlen(ID)+1)*sizeof(char));
				}
				else 
				{
					info = inform;				
					content = (char*)malloc((strlen(info)+1)*sizeof(char));
				}
				sprintf(content,info,ID);
				//printf("!!!!!!!:%s",content);
				post(content,&res);
				HttpPostResponse(res);
				break;
			}
		case 4:
			{
				post("",&res);
				HttpPostResponse(res);
				break;
			}
		case 5:
			{
				content = (char*)malloc((strlen(getParameterNamesResponse)+strlen(ID)+1)*sizeof(char));
				sprintf(content,getParameterNamesResponse,ID);
				post(content,&res);
				HttpPostResponse(res);
				break;
			}
		case 6:
			{
				content = (char*)malloc((strlen(getParameterValuesResponse)+ strlen(ID)+1)*sizeof(char));
				sprintf(content,getParameterValuesResponse,ID);
				post(content,&res);
				HttpPostResponse(res);
				break;
				}
		case 7:
			{
				user = 1;
				content = (char*)malloc((strlen(setParameterValuesResponse)+strlen(ID)+1)*sizeof(char));
				sprintf(content,setParameterValuesResponse,ID);
				post(content,&res);
				HttpPostResponse(res);
				break;
			}
		// reboot
		case 8:
			{
				content = (char*) malloc ((strlen(rebootResponse)+strlen(ID)+1)*sizeof(char));
				sprintf(content,rebootResponse,ID);
				post(content,&res);
				HttpPostResponse(res);
				break;
			}
		//add object
		case 9:
			{
				content = (char*) malloc ((strlen(addObjectResponse) + strlen(ID) +1 )*sizeof(char));
				sprintf(content,addObjectResponse,ID);
				post(content,&res);
				HttpPostResponse(res);
				break;
			}
		//delete object
		case 10:
			{
				content = (char*) malloc ((strlen(deleteObjectResponse) + strlen(ID) + 1) * sizeof(char));
				sprintf(content,deleteObjectResponse,ID);
				//puts(content);
				post(content,&res);
				HttpPostResponse(res);
				break;
			}
		// inform transfer complete
		case 11:
			{
				content = (char*) malloc (strlen(informTransferComplete) * sizeof(char));
				sprintf(content,informTransferComplete,ID);
				post(content,&res);
				HttpPostResponse(res);
			}
		// transfer complete
		case 12:
			{

			}
		default:
			{	
				break;
			}

	}
}


int main()
{
	int n;
	
	//pthread_create(&server,NULL,(void*)(&get_server),NULL);

	while (1)
	{
		choiceRPC();
	}
}
