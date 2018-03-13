#include "HttpPostRequest.h"

ParseSetParameterValuesResponse::ParseSetParameterValuesResponse(xmlNodePtr cur)
{
	cout << __FUNCTION__ << endl;
}

ParseGetParameterNamesResponse::ParseGetParameterNamesResponse(xmlNodePtr cur)
{
	cout << __FUNCTION__ << endl;
}

ParseGetParamterValuesResponse::ParseGetParamterValuesResponse(xmlNodePtr cur)
{
	cout << __FUNCTION__ << endl;
}

ParseAddObjectResponse::ParseAddObjectResponse(xmlNodePtr cur)
{
	cout << __FUNCTION__ << endl;
}

ParseDeleteObjectResponse::ParseDeleteObjectResponse(xmlNodePtr cur)
{
	cout << __FUNCTION__ << endl;
}

Inform::Inform(xmlNodePtr cur)
{
	cout << __FUNCTION__ << endl;
	cur = cur->xmlChildrenNode;
	while (cur != NULL )
	{
		if (!strcmp((const char*)cur->name, "ParameterList"))
		{
			parameterList = new ParameterList(cur);
		}
		
		cur = cur->next;
	}
}

Inform::ParameterList::ParameterList(xmlNodePtr cur)
{
	cout << __FUNCTION__ << endl;
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if (!strcmp((const char*)cur->name, "ParameterValueStruct"))
		{
			ParseParameterValueStruct(cur);
		}

		cur = cur->next;
	}
}

void Inform::ParameterList::ParseParameterValueStruct(xmlNodePtr cur)
{
	cout << __FUNCTION__ << endl;
	cur = cur->xmlChildrenNode;

	string mName,mValue;
	while (cur != NULL)
	{
		if (!strcmp((const char*)cur->name, "Name"))
		{
			mName = XmlStringUtils::ExtractFromNode(cur);
		}
		else if (!strcmp((const char*)cur->name,"Value"))
		{
			mValue = XmlStringUtils::ExtractFromNode(cur);
		}
		
		if (mName == "InternetGatewayDevice.ManagementServer.ConnectionRequestURL")
		{
			cout << "ConnectionRequestURL = " << mValue << endl;
			mConnectionUrl = mValue;
		}

		cur = cur->next;
	}
}

ParseBody::ParseBody(xmlNodePtr cur):
  inform(NULL),
  setParameterValuesResponse(NULL),
  getParameterNamesResponse(NULL),
  getParamterValuesResponse(NULL),
  addObjectResponse(NULL),
  deleteObjectResponse(NULL)
{
	cout << __FUNCTION__ << endl;

	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if (!strcmp((const char*)cur->name, "SetParameterValuesResponse"))
		{
			setParameterValuesResponse = new ParseSetParameterValuesResponse(cur);
		}
		else if (!strcmp((const char*)cur->name, "GetParameterNamesResponse"))
		{
			getParameterNamesResponse = new ParseGetParameterNamesResponse(cur);
		}
		else if (!strcmp((const char*)cur->name, "GetParamterValuesResponse"))
		{
			getParamterValuesResponse = new ParseGetParamterValuesResponse(cur);
		}
		else if (!strcmp((const char*)cur->name, "AddObjectResponse"))
		{
			addObjectResponse = new ParseAddObjectResponse(cur);
		}
		else if (!strcmp((const char*)cur->name, "DeleteObjectResponse"))
		{
			deleteObjectResponse = new ParseDeleteObjectResponse(cur);
		}
		else if (!strcmp((const char*)cur->name, "Inform"))
		{
			inform = new Inform(cur);
		}
		else
		{
			cout << "parse unhandle " << cur->name << endl;
		}

		cur = cur->next;
	}
}

ParseHeader::ParseHeader(xmlNodePtr cur)
{
	cout << __FUNCTION__ << endl;
}

Parse::Parse(xmlNodePtr cur):
  body(NULL),
  header(NULL)
{
	cout << __FUNCTION__ << endl;

	cur = cur->xmlChildrenNode;

	while (cur != NULL)
	{
		if (!strcmp((const char*)cur->name, "Body"))
		{
			body = new ParseBody(cur);
		}
		else if(!strcmp((const char*)cur->name, "Header"))
		{
			header = new ParseHeader(cur);
		}
		else
		{
			cout << "parse unhandle " << cur->name << endl;
		}

		cur = cur->next;
	}
}

HttpPostRequest::HttpPostRequest(hrequest_t* request):
  parse(NULL)
{
	cout << __FUNCTION__ << endl;

	string m_post;
	if (request &&
		request->in &&
		request->in->content_length != 0)
	{
		SoapEnv *soapEnv = NULL;
		soap_env_new_from_stream(request->in, &soapEnv);

		SoapCtx *soapCtx = NULL;
		soapCtx = soap_ctx_new(soapEnv);

		xmlBufferPtr buffer = xmlBufferCreate();

        xmlDocPtr doc = soapCtx->env->root->doc;
        xmlNodePtr cur = soapCtx->env->root;
 
        xmlNodeDump(buffer, doc, cur, 1, 0 );

        m_post += (const char *) xmlBufferContent(buffer);

        xmlBufferFree(buffer);

        parse = new Parse(cur);

	}
	// post empty
	else
	{

	}
}

HttpPostRequest::~HttpPostRequest()
{
	cout << __FUNCTION__ << endl;
	if (parse) delete parse;
}

Parse::~Parse()
{
	cout << __FUNCTION__ << endl;
	if (header) delete header;
	if (body) delete body;
}

ParseHeader::~ParseHeader()
{
	cout << __FUNCTION__ << endl;
}

ParseBody::~ParseBody()
{
	cout << __FUNCTION__ << endl;
	if (inform) delete inform;
	if (setParameterValuesResponse) delete setParameterValuesResponse;
	if (getParamterValuesResponse) delete getParamterValuesResponse;
	if (getParameterNamesResponse) delete getParameterNamesResponse;
	if (addObjectResponse) delete addObjectResponse;
	if (deleteObjectResponse) delete deleteObjectResponse;
}

Inform::~Inform()
{
	cout << __FUNCTION__ << endl;
	if (parameterList) delete parameterList;
}

Inform::ParameterList::~ParameterList()
{
	cout << __FUNCTION__ << endl;
}

ParseSetParameterValuesResponse::~ParseSetParameterValuesResponse()
{
	cout << __FUNCTION__ << endl;
}

ParseGetParameterNamesResponse::~ParseGetParameterNamesResponse()
{
	cout << __FUNCTION__ << endl;
}

ParseGetParamterValuesResponse::~ParseGetParamterValuesResponse()
{
	cout << __FUNCTION__ << endl;
}

ParseAddObjectResponse::~ParseAddObjectResponse()
{
	cout << __FUNCTION__ << endl;
}

ParseDeleteObjectResponse::~ParseDeleteObjectResponse()
{
	cout << __FUNCTION__ << endl;
}

