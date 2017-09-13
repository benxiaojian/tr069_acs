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
using namespace std;

class DeleteObjectResponse
{
public:
	DeleteObjectResponse(xmlNodePtr cur);
	~DeleteObjectResponse();

	string status;
};

class AddObjectResponse
{
public:
	AddObjectResponse(xmlNodePtr cur);
	~AddObjectResponse();

	string instanceNumber;
	string status;
};

class GetParamterValuesResponse
{
public:
	GetParamterValuesResponse(xmlNodePtr cur);
	~GetParamterValuesResponse();

	map<string,string> parameterName;
};

class GetParameterNamesResponse
{
public:
	GetParameterNamesResponse(xmlNodePtr cur);
	~GetParameterNamesResponse();

	map<string,string> parameterList;
};

class SetParameterValuesResponse
{
public:
	SetParameterValuesResponse(xmlNodePtr cur);
	~SetParameterValuesResponse();

	string status;
};

class Inform
{
public:
	Inform(xmlNodePtr cur);
	~Inform();
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