/**********System Includes****************/
#include <stdio.h>  
#include <string.h> 
#include <stdlib.h>
#include <unistd.h>  
#include <assert.h>  
  
#include <libxml/parser.h>  
#include <libxml/tree.h>  
#include <libxml/xmlmemory.h>  

#include <libcsoap/soap-server.h>
#include <nanohttp/nanohttp-client.h>

/********** Local Includes****************/

/********** Macro****************/

/********** Typedefs****************/


/**********************************************************
                           parse tr069 xml packet
**********************************************************/

/***********tool functions*****************/
static char * ExtractFromNode( xmlNodePtr node )
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





/***********SetParameterValues RPC begin*****************/
ParameterValueStruct(xmlNodePtr cur)
{
    printf("TR069-Parse:" "ParameterValueStruct");

    char *name = NULL; //just for trace
    char *value = NULL; //just for trace

    /* We don't care what the top level element name is */
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
	 if(!strcmp((const char*)cur->name, "Name"))
        {
            name = ExtractFromNode(cur);

            printf("TR069-Parse:" "Name=%s", name);
	      xmlFree((xmlChar *)name); //free buffer
        }
	 else if(!strcmp((const char*)cur->name, "Value"))
        {
            value = ExtractFromNode(cur);

            printf("TR069-Parse:" "Value=%s", value);
	      xmlFree((xmlChar *)value); //free buffer
        }
        else
        {
            printf("TR069-Parse:" "Unhandled %s", (const char*)cur->name);
        }
        cur = cur->next;
    }
}


void ParameterList(xmlNodePtr cur)
{
    /* We don't care what the top level element name is */
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
	 if(!strcmp((const char*)cur->name, "ParameterValueStruct"))
        {
            ParameterValueStruct(cur);
        }
        else
        {
            printf("TR069-Parse:" "Unhandled %s", (const char*)cur->name);
        }

        cur = cur->next;
    }
}


void SetParameterValues(xmlNodePtr cur)
{
    printf("TR069-Parse:" "SetParameterValues");
    char *parameterKey = NULL; //just for trace

    /* We don't care what the top level element name is */
    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
	 if(!strcmp((const char*)cur->name, "ParameterList"))
        {
            ParameterList(cur);
        }
	  else if(!strcmp((const char*)cur->name, "ParameterKey"))	
        {
            parameterKey = ExtractFromNode(cur);

            printf("TR069-Parse:" "ParameterKey=%s", parameterKey );
	      xmlFree((xmlChar *)parameterKey); //free buffer
        }
        else
        {
            printf("TR069-Parse:" "Unhandled %s", (const char*)cur->name);
        }

        cur = cur->next;
    }
}

/***********SetParameterValues RPC end*****************/


/***********InformResponse RPC begin*****************/
Tr069ParseInformResponse(xmlNodePtr cur)
{
    printf("TR069-Parse:" "InformResponse");
    char *maxEnvelopes = NULL; //just for trace

    /* We don't care what the top level element name is */
    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
        if(!strcmp((const char*)cur->name, "MaxEnvelopes"))
        {
            maxEnvelopes = ExtractFromNode(cur);

            printf("TR069-Parse:" "MaxEnvelopes=%s", maxEnvelopes);
	      xmlFree((xmlChar *)maxEnvelopes); //free buffer
        }
        else
        {
            printf("TR069-Parse:" "Unhandled %s", (const char*)cur->name);
        }

        cur = cur->next;
    }
}

/***********InformResponse RPC end*****************/


void Tr069ParseBody(xmlNodePtr cur)
{
    printf("TR069-Parse:" "Body");

    /* We don't care what the top level element name is */
    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
        if(!strcmp((const char*)cur->name, "AddObject"))
        {
            //addObject = shared_ptr<AddObject>(new AddObject(cur));
        }
        else if(!strcmp((const char*)cur->name, "SetParameterValues"))
        {
            SetParameterValues(cur);
        }
        else if(!strcmp((const char*)cur->name, "SetParameterAttributes"))
        {
            //setParameterAttributes = shared_ptr<Tr069ParseSetParameterAttributes>(new Tr069ParseSetParameterAttributes(cur));
        }
        else if(!strcmp((const char*)cur->name, "InformResponse"))
        {
            Tr069ParseInformResponse(cur);
        }
        else if(!strcmp((const char*)cur->name, "GetParameterValues"))
        {
            //getParameterValues = shared_ptr<Tr069ParseGetParameterValues>(new Tr069ParseGetParameterValues(cur));
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
        }
        else if(!strcmp((const char*)cur->name, "Upload"))
        {
            //upload = shared_ptr<Tr069ParseUpload>(new Tr069ParseUpload(cur));
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
        if(!strcmp((const char*)cur->name, "HoldRequests"))
        {
            holdRequests = ExtractFromNode(cur);

            printf("TR069-Parse:" "HoldRequests=%s", holdRequests );
	      xmlFree((xmlChar *)holdRequests); //free buffer
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

    /* We don't care what the top level element name is */
    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
        if(!strcmp((const char*)cur->name, "Body"))
        {
            Tr069ParseBody(cur);
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



/* handle response packet from HMS */
void HttpPostResponse(hresponse_t  * response) 
{
    SoapCtx *m_SoapCtx = NULL;
    
    /* Create Response Context */
    if( response &&
        response->in &&
        response->in->type != HTTP_TRANSFER_CONNECTION_CLOSE &&
        response->in->content_length != 0 )
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





int main(int argc, char *argv[])  
{  
    hresponse_t  response;
	
    HttpPostResponse(&response);
  
    return 0;  
}




