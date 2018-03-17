#ifndef __PARSE_H__
#define __PARSE_H__


#include <iostream>
#include <algorithm>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <tr1/memory>
#include <map>
#include <nanohttp/nanohttp-server.h>
#include <nanohttp/nanohttp-client.h>
#include <libcsoap/soap-server.h>
#include <libcsoap/soap-env.h>
#include <libcsoap/soap-ctx.h>

#include "Tr069XmlStringUtils.h"
using namespace std;

class ParseDeleteObjectResponse
{
public:
	ParseDeleteObjectResponse(xmlNodePtr cur);
	~ParseDeleteObjectResponse();

	string status;
};

class ParseAddObjectResponse
{
public:
	ParseAddObjectResponse(xmlNodePtr cur);
	~ParseAddObjectResponse();

	string instanceNumber;
	string status;
};

class ParseGetParamterValuesResponse
{
public:
	ParseGetParamterValuesResponse(xmlNodePtr cur);
	~ParseGetParamterValuesResponse();

	map<string,string> parameterName;
};

class ParseGetParameterNamesResponse
{
public:
	ParseGetParameterNamesResponse(xmlNodePtr cur);
	~ParseGetParameterNamesResponse();

	map<string,string> parameterList;
};

class ParseSetParameterValuesResponse
{
public:
	ParseSetParameterValuesResponse(xmlNodePtr cur);
	~ParseSetParameterValuesResponse();

	string status;
};

class Inform
{
public:
	Inform(xmlNodePtr cur);
	~Inform();

	class ParameterList
	{
	public:
		ParameterList(xmlNodePtr cur);
		~ParameterList();
		
		void ParseParameterValueStruct(xmlNodePtr cur);
		string mConnectionUrl;
	};

	ParameterList *parameterList;
};

class ParseBody
{
public:
	ParseBody(xmlNodePtr cur);
	~ParseBody();

	ParseSetParameterValuesResponse* setParameterValuesResponse;
	ParseGetParameterNamesResponse* getParameterNamesResponse;
	ParseGetParamterValuesResponse* getParamterValuesResponse;
	ParseAddObjectResponse* addObjectResponse;
	ParseDeleteObjectResponse* deleteObjectResponse;
	Inform* inform;
};

class ParseHeader
{
public:
	ParseHeader(xmlNodePtr cur);
	~ParseHeader();
};

class Parse
{
public:
	Parse(xmlNodePtr cur);
	~Parse();
	
	ParseBody* body;
	ParseHeader* header;
};

class HttpPostRequest
{
public:
	HttpPostRequest(hrequest_t *request);
	~HttpPostRequest();

	Parse* parse;
};

#endif
