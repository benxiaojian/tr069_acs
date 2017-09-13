#include <stdio.h>
#include <string.h>
#include <nanohttp/nanohttp-client.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <nanohttp/nanohttp-logging.h>
#include <libcsoap/soap-client.h>

//static const char *url = "http://localhost:10000/hems-web-ui/ws/cwmp/";
static const char *url = "http://172.0.11.29:8080/hems-web-ui/ws/cwmp/";
//static const char *url = "http://172.0.5.14:8080";
static const char *urn = "";
static const char *method = "cwmp:Inform";
char* soapHeader = /*"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\*/
"<soap-env:Envelope  xmlns:soap-env=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:soap-enc=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:cwmp=\"urn:dslforum-org:cwmp-1-0\">";

char *d="<soap-env:Envelope  xmlns:soap-env=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:soap-enc=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:cwmp=\"urn:dslforum-org:cwmp-1-0\"><soap-env:Header/><soap-env:Body><cwmp:Inform><DeviceId><Manufacturer>Radisys</Manufacturer><OUI>000050</OUI><ProductClass>WCDMA HNB</ProductClass><SerialNumber>2113100007</SerialNumber></DeviceId><Event soap-enc:arrayType=\"cwmp:EventStruct[2]\"><EventStruct><EventCode>1 BOOT</EventCode><CommandKey/></EventStruct><EventStruct><EventCode>4 VALUE CHANGE</EventCode><CommandKey/></EventStruct></Event><MaxEnvelopes>1</MaxEnvelopes><CurrentTime>2016-12-26T14:03:14+05:30</CurrentTime><RetryCount>0</RetryCount><ParameterList soap-enc:arrayType=\"cwmp:ParameterValueStruct[9]\"><ParameterValueStruct><Name>InternetGatewayDevice.DeviceInfo.HardwareVersion</Name><Value>HW5.1</Value></ParameterValueStruct><ParameterValueStruct><Name>InternetGatewayDevice.DeviceInfo.ProvisioningCode</Name><Value>FE3T.RSYS</Value></ParameterValueStruct><ParameterValueStruct><Name>InternetGatewayDevice.DeviceInfo.SoftwareVersion</Name><Value>SW2.0.0</Value></ParameterValueStruct><ParameterValueStruct><Name>InternetGatewayDevice.DeviceSummary</Name><Value>InternetGatewayDevice:1.0[](Basline:1, FAPService:1.0[1](Baseline:1, ACL:1, UMTSCellConfigBaseline:1)</Value></ParameterValueStruct><ParameterValueStruct><Name>InternetGatewayDevice.ManagementServer.ConnectionRequestURL</Name><Value>http://172.0.10.148:7547/rand0mur1g3n3rati0nfai13d26-12-2016_08-32-55.832142</Value></ParameterValueStruct><ParameterValueStruct><Name>InternetGatewayDevice.ManagementServer.ParameterKey</Name><Value/></ParameterValueStruct><ParameterValueStruct><Name>InternetGatewayDevice.Services.FAPService.1.FAPControl.OpState</Name><Value>false</Value></ParameterValueStruct><ParameterValueStruct><Name>InternetGatewayDevice.Services.FAPService.1.FAPControl.RFTxStatus</Name><Value>false</Value></ParameterValueStruct><ParameterValueStruct><Name>InternetGatewayDevice.Services.FAPService.1.FAPControl.X_RADISYS_COM_AlarmStatus</Name><Value>Cleared</Value></ParameterValueStruct></ParameterList></cwmp:Inform></soap-env:Body></soap-env:Envelope>";

char *c = "\n<soap-env:Header/>\
 <soap-env:Body>\
  <cwmp:Inform>\
   <DeviceId>\
    <Manufacturer>\
     Radisys</Manufacturer>\
    <OUI>\
     000050</OUI>\
    <ProductClass>\
     WCDMA HNB</ProductClass>\
    <SerialNumber>\
     2113100007</SerialNumber>\
    </DeviceId>\
   <Event soap-enc:arrayType=\"cwmp:EventStruct[2]\">\
    <EventStruct>\
     <EventCode>\
      1 BOOT</EventCode>\
     <CommandKey/>\
     </EventStruct>\
    <EventStruct>\
     <EventCode>\
      4 VALUE CHANGE</EventCode>\
     <CommandKey/>\
     </EventStruct>\
    </Event>\
   <MaxEnvelopes>\
    1</MaxEnvelopes>\
   <CurrentTime>\
    2016-12-05T11:55:36+05:30</CurrentTime>\
   <RetryCount>\
    0</RetryCount>\
   <ParameterList soap-enc:arrayType=\"cwmp:ParameterValueStruct[9]\">\
    <ParameterValueStruct>\
     <Name>\
      InternetGatewayDevice.DeviceInfo.HardwareVersion</Name>\
     <Value>\
      HW5.1</Value>\
     </ParameterValueStruct>\
    <ParameterValueStruct>\
     <Name>\
      InternetGatewayDevice.DeviceInfo.ProvisioningCode</Name>\
     <Value>\
      FE3T.RSYS</Value>\
     </ParameterValueStruct>\
    <ParameterValueStruct>\
     <Name>\
      InternetGatewayDevice.DeviceInfo.SoftwareVersion</Name>\
     <Value>\
      SW2.0.0</Value>\
     </ParameterValueStruct>\
    <ParameterValueStruct>\
     <Name>\
      InternetGatewayDevice.DeviceSummary</Name>\
     <Value>\
      InternetGatewayDevice:1.0[](Basline:1, FAPService:1.0[1](Baseline:1, ACL:1, UMTSCellConfigBaseline:1)</Value>\
     </ParameterValueStruct>\
    <ParameterValueStruct>\
     <Name>\
      InternetGatewayDevice.ManagementServer.ConnectionRequestURL</Name>\
     <Value>\
      http://192.168.57.64:7547/58981aef0858e357517ddbcfac6c052c</Value>\
     </ParameterValueStruct>\
    <ParameterValueStruct>\
     <Name>\
      InternetGatewayDevice.ManagementServer.ParameterKey</Name>\
     <Value/>\
     </ParameterValueStruct>\
    <ParameterValueStruct>\
     <Name>\
      InternetGatewayDevice.Services.FAPService.1.FAPControl.OpState</Name>\
     <Value>\
      false</Value>\
     </ParameterValueStruct>\
    <ParameterValueStruct>\
     <Name>\
      InternetGatewayDevice.Services.FAPService.1.FAPControl.RFTxStatus</Name>\
     <Value>\
      false</Value>\
     </ParameterValueStruct>\
    <ParameterValueStruct>\
     <Name>\
      InternetGatewayDevice.Services.FAPService.1.FAPControl.X_RADISYS_COM_AlarmStatus</Name>\
     <Value>\
      Cleared</Value>\
     </ParameterValueStruct>\
    </ParameterList>\
   </cwmp:Inform>\
  </soap-env:Body>\
 </soap-env:Envelope>";

static herror_t soap_client_invoke_empty(SoapCtx * call, SoapCtx ** response, const char *url,
                   const char *soap_action);
int main(int argc,char *argv[])
{
	SoapCtx *ctx,*ctx2;
	herror_t err;

	err = soap_client_init_args(argc,argv);
	if (err != H_OK)
	{
		 log_error4("%s():%s [%d]", herror_func(err), herror_message(err),herror_code(err));
		 herror_release(err);
		 return 1;
	}
	//char *inform = (char*)malloc((strlen(soapHeader)+strlen(c))*sizeof(char));
	char *inform=(char *)malloc(strlen(d)+1);

	//strcat(inform,soapHeader);
	//strcat(inform,c);
	strcat(inform,d);

	SoapEnv * soapEnv = NULL;
	//xmlDocPtr env;
	soap_env_new_from_buffer(inform,&soapEnv);
	ctx = soap_ctx_new(soapEnv);
//	soap_env_new_from_buffer(inform,&soapEnv);
        //soap_env_new_from_buffer("<empty/>", &soapEnv);

//	ctx=soap_ctx_new(soapEnv);
/*
	err = soap_ctx_new_with_method(urn,method,&ctx);
	if (err != H_OK)
	{
		 log_error4("%s():%s [%d]", herror_func(err), herror_message(err),herror_code(err));
		 herror_release(err);
		 return 1;
	}

	soap_env_add_item(ctx->env, "", "MaxEnvelopes","1");
	soap_env_add_item(ctx->env, "", "MaxEnvelopes1","11");
soap_env_push_item(ctx->env, "", "MaxEnvelopes1_1");
	soap_env_add_item(ctx->env, "", "MaxEnvelopes1a","aa");
	soap_env_pop_item(ctx->env);
	soap_env_add_item(ctx->env, "", "MaxEnvelopes2","22");
*/

	soap_xml_doc_print(ctx->env->root->doc);

	

	if (argc > 1)
		err = soap_client_invoke(ctx,&ctx2,argv[1],"");
	else
		err = soap_client_invoke(ctx,&ctx2,url,"");
	
	if (err != H_OK)
	{
		log_error4("%s():%s [%d]", herror_func(err), herror_message(err),herror_code(err));
		herror_release(err);
		soap_ctx_free(ctx);
		printf("error\n");
		return 1;
	}

	printf("response:\n");
	soap_xml_doc_print(ctx2->env->root->doc);

//////////////////

        //SoapEnv * soapEnv1 = NULL;

#if 1
        soap_env_new_from_buffer("<empty/>", &soapEnv);


	//SoapCtx *ctx3;
	ctx = soap_ctx_new(soapEnv);

	printf("print:\n");
	soap_xml_doc_print(ctx->env->root->doc);
	printf("end print\n");
	err = soap_client_invoke_empty(ctx,NULL,url,"");


#else


#endif


printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);

	soap_ctx_free(ctx);
	soap_ctx_free(ctx2);
	//soap_ctx_free(ctx);

	soap_client_destroy();

	return 0;

}

static herror_t
__soap_client_build_result(hresponse_t * res, SoapEnv ** env)
{
  log_verbose2("Building result (%p)", res);

  if (res == NULL)
    return herror_new("_soap_client_build_result",
                      GENERAL_INVALID_PARAM, "hresponse_t is NULL");


  if (res->in == NULL)
    return herror_new("_soap_client_build_result",
                      GENERAL_INVALID_PARAM, "Empty response from server");

  if (res->errcode != 200)
    return herror_new("_soap_client_build_result",
                      GENERAL_INVALID_PARAM, "HTTP code is not OK (%i)", res->errcode);

  return soap_env_new_from_stream(res->in, env);
}

static herror_t soap_client_invoke_empty(SoapCtx * call, SoapCtx ** response, const char *url,
                   const char *soap_action)
{
printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);
  /* Status */
  herror_t status;

  /* Result document */
  SoapEnv *res_env;

  /* Buffer variables */
//  xmlBufferPtr buffer;
  //char *content;
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
  //buffer = xmlBufferCreate();
  //xmlNodeDump(buffer, call->env->root->doc, call->env->root, 1, 0);
  //content = (char *) xmlBufferContent(buffer);

  /* Transport via HTTP */
  if (!(conn = httpc_new()))
  {
    return herror_new("soap_client_invoke", SOAP_ERROR_CLIENT_INIT,
                      "Unable to create SOAP client!");
  }

  /* Set soap action */
  if (soap_action != NULL)
    httpc_set_header(conn, "SoapAction", soap_action);

  httpc_set_header(conn, HEADER_CONNECTION, "Close");

  /* check for attachments */
  if (!call->attachments)
  {
    /* content-type is always 'text/xml' */
printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);
    //httpc_set_header(conn, HEADER_CONTENT_TYPE, "text/xml");

    sprintf(tmp, "%d", 0);
    httpc_set_header(conn, HEADER_CONTENT_LENGTH, tmp);

    if ((status = httpc_post_begin(conn, url)) != H_OK)
    {
      httpc_close_free(conn);
      //xmlBufferFree(buffer);
      return status;
    }

printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);
    if ((status = http_output_stream_write_string(conn->out, "")) != H_OK)
    {
      httpc_close_free(conn);
      //xmlBufferFree(buffer);
      return status;
    }

printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);
    if ((status = httpc_post_end(conn, &res)) != H_OK)
    {
      httpc_close_free(conn);
      //xmlBufferFree(buffer);
      return status;
    }
  }

  /* Free buffer */
  //xmlBufferFree(buffer);

  /* Build result */
#if 1
  __soap_client_build_result(res, &res_env);
#endif

printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);
  /* Create Context */
  *response = soap_ctx_new(res_env);
/*	soap_ctx_add_files(*response, res->attachments);*/


  hresponse_free(res);
  httpc_close_free(conn);

printf("...................add by lyb .............func:%s,line:%d\n",__func__,__LINE__);
  return H_OK;
}
