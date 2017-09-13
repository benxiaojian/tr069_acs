#include <nanohttp/nanohttp-logging.h>
#include <libcsoap/soap-client.h>

//static const char *url = "http://localhost:10000/hems-web-ui/ws/cwmp/";
static const char *url = "http://172.0.5.14:8080";
static const char *urn = "";
static const char *method = "cwmp:Inform";
char* soapHeader = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<soap-env:Envelope  xmlns:soap-env=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:soap-enc=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:cwmp=\"urn:dslforum-org:cwmp-1-0\">";

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
	char *inform = (char*)malloc((strlen(soapHeader)+strlen(c))*sizeof(char));

	strcat(inform,soapHeader);
	strcat(inform,c);

	SoapEnv * soapEnv = NULL;
	xmlDocPtr env;
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

	printf("response\n");
	soap_xml_doc_print(ctx2->env->root->doc);
	soap_ctx_free(ctx2);
	soap_ctx_free(ctx);

	soap_client_destroy();

	return 0;

}

