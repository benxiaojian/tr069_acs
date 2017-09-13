#include <stdio.h>
#include <string.h>
#include <nanohttp/nanohttp-client.h>
#include <libcsoap/soap-server.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "soap_common.h"
#include <pthread.h>



hurl_t url;
char *G_ID;
char buffer[4096];
herror_t status;
int ssl;
httpc_conn_t *conn;

#define AliasSize  64
#define FtpServerSize 64
#define UsernameSize 64
#define PasswordSize 64
#define cmdSize 1024


static herror_t
__soap_client_build_result(hresponse_t * res, SoapEnv ** env);
int Connect();

void httpc_header_free();
void httpc_header_add_date(void);

void httpc_header_new(hreq_method_t method,int txContentLength);
SoapEnv * post(char *content,hresponse_t **response);
SoapCtx * post_res(char *content,hresponse_t **response);

herror_t soap_client_invoke_connect(SoapCtx * call, SoapCtx ** response, const char *url,const char *soap_action);
void show_response(hresponse_t *res);

char *ctx_get_ID(SoapCtx *ctx3);
int send_ctx_ID(SoapCtx *ctx,SoapCtx **ctx2,char *xml_str,char *ID);

static void root_service_root(httpd_conn_t *conn, hrequest_t *req);

void *HeartBeatThread(void *arg);

void heart_beat_fun();

void *HandlePMFileUpload(void *arg);


xmlNodeSetPtr
soap_xpath_find_set(xmlDocPtr doc, const char *xpath);

void print_Node_set( xmlNodeSetPtr nodeset);

int main()
{

	char *inform;
	hresponse_t *response;

	Connect();

	inform = (char*)malloc(strlen(d)+sizeof(char));
	strcat(inform,d);

	post(inform,&response);
	usleep(1000);

	SoapEnv * soapEnv = NULL;
	SoapCtx *ctx,*ctx2;
	ctx = post_res("",&response);
	sleep(2);

	//hresponse_free(response);
	printf("doc:\n");
	 if(ctx->env->root->doc)
        soap_xml_doc_print(ctx->env->root->doc);
	
{
	xmlNodeSetPtr set=soap_xpath_find_set(ctx->env->root->doc, "//Name");
	print_Node_set(set);
}
	
	//printf("method:%s...............\n",xmlNodeGetContent(soap_env_get_method(ctx->env)));
	//printf("method:%s...............\n",xmlNodeGetContent(soap_env_get_body(ctx->env)));
	printf("method:%s...............\n",xmlNodeGetContent(soap_env_get_method(ctx->env)));
	


	char *ID=ctx_get_ID(ctx);

	printf("%s\n",ID);

	soap_ctx_free(ctx);

#if 1
{

	send_ctx_ID(ctx,&ctx2,SetParameterValuesResponse,ID);

	free(ID);
	ID=ctx_get_ID(ctx2);
        printf("response....:\n");
	if(ctx2->env->root->doc)
        soap_xml_doc_print(ctx2->env->root->doc);
}
#endif


	send_ctx_ID(ctx,&ctx2,name,ID);


        printf("response:\n");
	if(ctx2->env->root->doc)
        soap_xml_doc_print(ctx2->env->root->doc);

	
printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);

	
	free(ID);
	ID=ctx_get_ID(ctx2);

	
	send_ctx_ID(ctx,&ctx2,GetParameterValuesResponse,ID);
	//soap_ctx_free(ctx); //error
	free(ID);
	G_ID=ctx_get_ID(ctx2);

	
	soap_ctx_free(ctx2);

	pthread_t tid;
	int ret=0;
	ret=pthread_create(&tid,NULL,HeartBeatThread,NULL);    
   	if(ret!=0)    
        return -1;

	ret=pthread_create(&tid,NULL,HandlePMFileUpload,NULL);    
   	if(ret!=0)    
        return -1;
	
/*http server*/
	httpd_init(0, NULL);
	httpd_register("/", root_service_root);

	httpd_run();
	


	
}
void *HeartBeatThread(void *arg)
{

	while(1)
	{
	         printf("send heart beat.\n");
	 	 heart_beat_fun();
                sleep(2);
	}
}

void *HandlePMFileUpload(void *arg)
{
	int Performance_Enable=0;
	char  Performance_Alias[AliasSize]="";
	char  FtpServer[FtpServerSize]="172.0.5.14";
	char  Username[UsernameSize]="liuyibin";
        char Password[PasswordSize]="123456";
	char *local_filname="./up.txt";
	char *remote_filname="up.txt";
	int Periodic=1;//minute
	char cmd[cmdSize];
	//./a.out flag ftp_user ftp_password ftp_server_ip local_filname remote_filname
	sprintf(cmd,"./a.out 1 %s %s %s %s %s\n",Username,Password,FtpServer,local_filname,remote_filname);
	printf("cmd:%s",cmd);
	system(cmd);
}

void heart_beat_fun()
{
		SoapCtx *ctx,*ctx2;
		send_ctx_ID(ctx, &ctx2, heart_beat, "");
		char *ID=ctx_get_ID(ctx2);
		send_ctx_ID(ctx, &ctx2,CasaPost , ID);

		//CasaPost
		
}

static herror_t
__soap_client_build_result(hresponse_t * res, SoapEnv ** env)
{
  //log_verbose2("Building result (%p)", res);

  if (res == NULL)
    return herror_new("__soap_client_build_result",
                      GENERAL_INVALID_PARAM, "hresponse_t is NULL");


  if (res->in == NULL)
    return herror_new("__soap_client_build_result",
                      GENERAL_INVALID_PARAM, "Empty response from server");

  if (res->errcode != 200)
    return herror_new("__soap_client_build_result",
                      GENERAL_INVALID_PARAM, "HTTP code is not OK (%i)", res->errcode);

  return soap_env_new_from_stream(res->in, env);
}


herror_t
soap_client_invoke_connect(SoapCtx * call, SoapCtx ** response, const char *url,
                   const char *soap_action)
{
  /* Status */
  herror_t status;

  /* Result document */
  SoapEnv *res_env;

  /* Buffer variables */
  xmlBufferPtr buffer;
  char *content;
  char tmp[15];

  /* Transport variables */
  httpc_conn_t *conn;
  hresponse_t *res;

  /* multipart/related start id */
  char start_id[150];
  static int counter = 1;
  part_t *part;

  /* for copy attachments */
  char href[MAX_HREF_SIZE];

  /* Create buffer */
  buffer = xmlBufferCreate();
  xmlNodeDump(buffer, call->env->root->doc, call->env->root, 1, 0);
  content = (char *) xmlBufferContent(buffer);

  /* Transport via HTTP */
  if (!(conn = httpc_new()))
  {
    //return herror_new("soap_client_invoke", SOAP_ERROR_CLIENT_INIT,
     //                 "Unable to create SOAP client!");
  }

  /* Set soap action */
  if (soap_action != NULL)
    httpc_set_header(conn, "SoapAction", soap_action);

  httpc_set_header(conn, HEADER_CONNECTION, "keep-alive");
       httpc_add_header(conn, "Cookie", "Cookie");//key = Cookie, is valid but Value = Cookie, is just a dummy value
       httpc_set_basic_authorization(conn, "casa", "casa");
       httpc_add_header(conn, "Password", "casa");
       httpc_add_header(conn, "Username", "casa");

  /* check for attachments */
  if (!call->attachments)
  {
    /* content-type is always 'text/xml' */
    httpc_set_header(conn, HEADER_CONTENT_TYPE, "text/xml");

    sprintf(tmp, "%d", (int) strlen(content));
    httpc_set_header(conn, HEADER_CONTENT_LENGTH, tmp);

    if ((status = httpc_post_begin(conn, url)) != H_OK)
    {
      httpc_close_free(conn);
      xmlBufferFree(buffer);
      return status;
    }

    if ((status = http_output_stream_write_string(conn->out, content)) != H_OK)
    {
      httpc_close_free(conn);
      xmlBufferFree(buffer);
      return status;
    }

    if ((status = httpc_post_end(conn, &res)) != H_OK)
    {
      httpc_close_free(conn);
      xmlBufferFree(buffer);
      return status;
    }
  }
  else
  {

    /* Use chunked transport */
    httpc_set_header(conn, HEADER_TRANSFER_ENCODING,
                     TRANSFER_ENCODING_CHUNKED);

    sprintf(start_id, "289247829121218%d", counter++);
    if ((status = httpc_mime_begin(conn, url, start_id, "", "text/xml")) != H_OK)
    {
      httpc_close_free(conn);
      xmlBufferFree(buffer);
      return status;
    }

    if ((status = httpc_mime_next(conn, start_id, "text/xml", "binary")) != H_OK)
    {
      httpc_close_free(conn);
      xmlBufferFree(buffer);
      return status;
    }

    if ((status = http_output_stream_write(conn->out, content, strlen(content))) != H_OK)
    {
      httpc_close_free(conn);
      xmlBufferFree(buffer);
      return status;
    }


    for (part = call->attachments->parts; part; part = part->next)
    {
      status = httpc_mime_send_file(conn, part->id,
                                    part->content_type,
                                    part->transfer_encoding, part->filename);
      if (status != H_OK)
      {
        //log_error2("Send file failed. Status:%d", status);
        httpc_close_free(conn);
        xmlBufferFree(buffer);
        return status;
      }
    }

    if ((status = httpc_mime_end(conn, &res)) != H_OK)
    {
      httpc_close_free(conn);
      xmlBufferFree(buffer);
      return status;
    }
  }

  /* Free buffer */
  xmlBufferFree(buffer);

  /* Build result */
  if ((status = __soap_client_build_result(res, &res_env)) != H_OK)
  {
    hresponse_free(res);
    httpc_close_free(conn);
    return status;
  }

  /* Create Context */
  *response = soap_ctx_new(res_env);
/*	soap_ctx_add_files(*response, res->attachments);*/

  if (res->attachments != NULL)
  {
    part = res->attachments->parts;
    while (part)
    {
      soap_ctx_add_file(*response, part->filename, part->content_type, href);
      part->deleteOnExit = 0;
      part = part->next;
    }
    part = (*response)->attachments->parts;
    while (part)
    {
      part->deleteOnExit = 1;
      part = part->next;
    }
  }

//  hresponse_free(res);
//  httpc_close_free(conn);

  return H_OK;
}
typedef unsigned char byte_t;
void show_response(hresponse_t *res)
{
  hpair_t *pair;
  unsigned char buffer[1024+1];
  int read;
	if (res == NULL) 
	{
//	 log_error1("Response is NULL!");
	 return;
	}
	

  pair = res->header;
  while (pair != NULL) {
//		log_debug3("%s: %s", pair->key, pair->value);
		pair = pair->next;
	}

	if (res->in == NULL)
	{
	  //log_warn1("No input stream!");
	  return;
	}

	
	while (http_input_stream_is_ready(res->in))
	{
	  read = http_input_stream_read(res->in, buffer, 1025);
	  buffer[read] = '\0';
	  puts(buffer);
	}

}
int Connect()
{
	if (H_OK == hurl_parse(&url,path))
	{
		conn = httpc_new();
		
		if (conn)
		{
			ssl = url.protocol == PROTOCOL_HTTPS ? 1 : 0;

			if (hsocket_open(&conn->sock,url.host,url.port,ssl) != H_OK)
			{
				return -1;
			}
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
       httpc_add_header(conn, "Cookie", "Cookie");//key = Cookie, is valid but Value = Cookie, is just a dummy value
       httpc_set_basic_authorization(conn, "casa", "casa");
       httpc_add_header(conn, "Password", "casa");
       httpc_add_header(conn, "Username", "casa");


	char tmp[15];
	sprintf(tmp,"%d",(int)txContentLength);
	httpc_add_header(conn,HEADER_CONTENT_LENGTH,tmp);

	httpc_header_add_date();

	httpc_add_header(conn,HEADER_HOST,url.host);

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
}
SoapEnv * post(char *content,hresponse_t **response)
{
	httpc_header_new(HTTP_REQUEST_POST,strlen(content));

	conn->out = http_output_stream_new(&conn->sock,conn->header);

	http_output_stream_write_string(conn->out,content);

	httpc_post_end(conn,response);

}



SoapCtx *post_res(char *content,hresponse_t **response)
{
	httpc_header_new(HTTP_REQUEST_POST,strlen(content));

	conn->out = http_output_stream_new(&conn->sock,conn->header);

	http_output_stream_write_string(conn->out,content);

	httpc_post_end(conn,response);

//
if(1)
{
	SoapEnv *res_env;
	//httpc_mime_end(conn, response);
	hresponse_new_from_socket(&(conn->sock), response);

	__soap_client_build_result(*response , &res_env);
	http_output_stream_free(conn->out);
	conn->out = NULL;

	return soap_ctx_new(res_env);;
}
	
//

	http_output_stream_free(conn->out);
	conn->out = NULL;
	return NULL;


}
char *ctx_get_ID(SoapCtx *ctx3)
{
	xmlNodePtr cur=soap_xml_get_children(soap_env_get_header(ctx3->env));
	char *ID=xmlNodeGetContent(cur);
	//printf("id:%s................\n",cur->name);
	return ID;
}
int send_ctx_ID(SoapCtx *ctx,SoapCtx **ctx2,char *xml_str,char *ID)
{
	SoapEnv * soapEnv = NULL;
	char *buf=(char *)malloc(strlen(xml_str)+strlen(ID)+1);
	sprintf(buf,xml_str,ID); 

	soap_env_new_from_buffer(buf,&soapEnv);
	ctx = soap_ctx_new(soapEnv);
	//soap_xml_doc_print(ctx->env->root->doc);
	//soap_client_invoke(ctx,ctx2,path,"");
	soap_client_invoke_connect(ctx,ctx2,path,"");
	free(buf);
	
	//free soapEnv ??
	soapEnv=NULL;
	usleep(1000);


	return 0;
}
static void root_service_root(httpd_conn_t *conn, hrequest_t *req)
{
	
	hresponse_t *response;
	printf("in root_service\n");

	httpd_send_header(conn, 200, "OK");
#if 1
	hsocket_send(conn->sock,
		"<html>"
			"<head>"
				"<title>nanoHTTP server examples</title>"
			"</head>"
			"<body>"
				"<h1>nanoHTTP server examples</h1>"
			"</body>"
		"</html>");
#endif

	SoapCtx *ctx,*ctx2;
	//send_ctx_ID(ctx,&ctx2,inform_begin,G_ID);
	char *inform;
	//hresponse_t *response;
#if 1
	Connect();

	inform = (char*)malloc(strlen(inform_begin)+strlen(G_ID)+1);
	sprintf(inform,inform_begin,G_ID);
//	sprintf(inform,inform_begin,"1234");

	post(inform,&response);
	usleep(1000);

#else
	SoapEnv * soapEnv = NULL;
	char *buf=(char *)malloc(strlen(inform_begin)+strlen(G_ID)+1);
	sprintf(buf,inform_begin,G_ID); 

	soap_env_new_from_buffer(buf,&soapEnv);
	ctx = soap_ctx_new(soapEnv);
	soap_client_invoke_connect(ctx,&ctx2,path,"");
	free(buf);

#endif
	sleep(1);
	ctx = post_res("",&response);
	printf("res:\n");
      // if(ctx->env->root->doc)
	//   soap_xml_doc_print(ctx->env->root->doc);
	//if(ctx->http)
	   if (1)
	  {
		char *rpcname;
		xmlNodePtr  node=soap_env_get_body(ctx->env);
		rpcname=(char *)soap_xml_get_children(node)->name;
		printf("rpcname:%s...........................\n",rpcname);
	
		G_ID=ctx_get_ID(ctx);
		printf("%s\n",G_ID);

		if(!strcmp(rpcname,"SetParameterValues"))
		{
			send_ctx_ID(ctx,&ctx2,SetParameterValuesResponse,G_ID);
			
		        printf("response:\n");
			//if(ctx2->env->root->doc)
		        //soap_xml_doc_print(ctx2->env->root->doc);
			printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);

			
		}
		else if(!strcmp(rpcname,"GetParameterValues"))
		{
			
		}
		else if(!strcmp(rpcname,"GetParameterNames"))
		{
			send_ctx_ID(ctx,&ctx2,GetParameterNamesResponse,G_ID);
			printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);


		}
		else if(!strcmp(rpcname,"AddObject"))
		{
			//AddObjectResponse
			send_ctx_ID(ctx,&ctx2,AddObjectResponse,G_ID);
			printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);

			
		}
		else if(!strcmp(rpcname,"DeleteObject"))
		{//RebootResponse
			send_ctx_ID(ctx,&ctx2,DeleteObjectResponse,G_ID);
			printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);


		}
		else if(!strcmp(rpcname,"Reboot"))
		{
			send_ctx_ID(ctx,&ctx2,RebootResponse,G_ID);
			printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);

		}
		else if(!strcmp(rpcname,"Download"))
		{

		}	
		else if(!strcmp(rpcname,"Upload"))
		{

		}	
		else
		{
			printf("unkonw rpc method\n");
		}		
	}
		else
			printf("length is  null\n");

	
	printf("exit root_service_root....\n");

}


xmlNodeSetPtr
soap_xpath_find_set(xmlDocPtr doc, const char *xpath)
{
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;

  context = xmlXPathNewContext(doc);
  result = xmlXPathEvalExpression(BAD_CAST xpath, context);
  if (xmlXPathNodeSetIsEmpty(result->nodesetval))
  {
    /* no result */
	printf("no result");
    return NULL;
  }

  xmlXPathFreeContext(context);
  xmlNodeSetPtr nodeset=result->nodesetval;

  
 
  return nodeset;
}

void print_Node_set( xmlNodeSetPtr nodeset)
{
	int i;
	xmlNodePtr temp;
	if(nodeset == NULL)
	{
		printf("nodeset is null\n");
		return ;
	}
	if(nodeset->nodeNr == 0)
	{
		printf("nodeset is empty\n");
		return;
	}
	for(i=0;i<nodeset->nodeNr;i++)
	{
	 temp= nodeset->nodeTab[i];
	printf("name:%s,value:%s\n",temp->name,(char *)xmlNodeGetContent(temp));
	
	}

}

