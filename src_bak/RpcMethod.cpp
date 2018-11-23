#include "../inc_bak/RpcMethod.h"

string inform_response()
{
	std::ostringstream response;
		response <<
		"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" " <<
		"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" " <<
		"xmlns:cwmp=\"urn:dslforum-org:cwmp-1-0\" " <<
		"xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\" " <<
		"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" <<
		"<SOAP-ENV:Header>" <<
		"<cwmp:ID SOAP-ENV:mustUnderstand=\"1\">" <<
		"123" <<
		"</cwmp:ID>" <<
		"</SOAP-ENV:Header>" <<
		"<SOAP-ENV:Body>" <<
		"<cwmp:InformResponse>" <<
		"<MaxEnvelopes>" <<
		"1" <<
		"</MaxEnvelopes>" <<
		"</cwmp:InformResponse>" <<
		"</SOAP-ENV:Body>" <<
		"</SOAP-ENV:Envelope>";

	return response.str();
}

string get_parameter_names(string &paramPath , string &nextLevel)
{
	std::ostringstream response;
		response <<
		"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" " <<
		"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" " <<
		"xmlns:cwmp=\"urn:dslforum-org:cwmp-1-0\" " <<
		"xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\" " <<
		"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" <<
		"<SOAP-ENV:Header>" <<
		"<cwmp:ID SOAP-ENV:mustUnderstand=\"1\">" <<
		"123" <<
		"</cwmp:ID>" <<
		"</SOAP-ENV:Header>" <<
		"<SOAP-ENV:Body>" <<
		"<cwmp:GetParameterNames>" <<
		"<ParameterPath>" <<
		paramPath <<
		"</ParameterPath>" <<
		"<NextLevel>" <<
		nextLevel <<
		"</NextLevel>" <<
		"</cwmp:GetParameterNames>" <<
		"</SOAP-ENV:Body>" <<
		"</SOAP-ENV:Envelope>";

	return response.str();
}

string get_parameter_values(set<string> &paramName)
{
	std::ostringstream response;
		response <<
		"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" " <<
		"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" " <<
		"xmlns:cwmp=\"urn:dslforum-org:cwmp-1-0\" " <<
		"xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\" " <<
		"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" <<
		"<SOAP-ENV:Header>" <<
		"<cwmp:ID SOAP-ENV:mustUnderstand=\"1\">" <<
		"123" <<
		"</cwmp:ID>" <<
		"</SOAP-ENV:Header>" <<
		"<SOAP-ENV:Body>" <<
		"<cwmp:GetParameterValues>" <<
		"<ParameterNames SOAP-ENC:arrayType=\"string[" <<
		paramName.size() << 
		"]\">";
		set<string>::iterator it;
		for (it = paramName.begin(); it != paramName.end(); ++it)
		{
			response <<
			"<string>" <<
			*it <<
			"</string>";
		}
		response <<
		"</ParameterNames>" <<
		"</cwmp:GetParameterValues>" <<
		"</SOAP-ENV:Body>" <<
		"</SOAP-ENV:Envelope>";

	return response.str();
}

string set_parameter_values(map<string,string> &paramList, string &paramKey)
{
	std::ostringstream response;
		response <<
		"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" " <<
		"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" " <<
		"xmlns:cwmp=\"urn:dslforum-org:cwmp-1-0\" " <<
		"xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\" " <<
		"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" <<
		"<SOAP-ENV:Header>" <<
		"<cwmp:ID SOAP-ENV:mustUnderstand=\"1\">" <<
		"123" <<
		"</cwmp:ID>" <<
		"</SOAP-ENV:Header>" <<
		"<SOAP-ENV:Body>" <<
		"<cwmp:SetParameterValues>" << 
		"<ParamterList SOAP-ENC:arrayType=\"cwmp:EventStruct[" <<
		paramList.size()  <<
		"]\">";

		map<string,string>::iterator it;
		for (it = paramList.begin(); it != paramList.end(); ++it)
		{
			response <<
			"<ParameterValueStruct>" <<
			"<Name>" <<
			it->first.c_str() <<
			"</Name>" <<
			"<Value>" <<
			it->second.c_str() <<
			"</Value>" <<
			"</ParameterValueStruct>";

		}

		response <<
		"</ParamterList>" <<
		"<ParamterKey>" <<
		paramKey.c_str() <<
		"</ParamterKey>" <<
		"</cwmp:SetParameterValues>" <<
		"</SOAP-ENV:Body>" <<
		"</SOAP-ENV:Envelope>";

	return response.str();
}

string add_object(string objectName, string parameterKey)
{
	std::ostringstream response;
		response <<
		"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" " <<
		"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" " <<
		"xmlns:cwmp=\"urn:dslforum-org:cwmp-1-0\" " <<
		"xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\" " <<
		"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" <<
		"<SOAP-ENV:Header>" <<
		"<cwmp:ID SOAP-ENV:mustUnderstand=\"1\">" <<
		"123" <<
		"</cwmp:ID>" <<
		"</SOAP-ENV:Header>" <<
		"<SOAP-ENV:Body>" <<
		"<cwmp:AddObject>" <<
		"<ObjectName>" <<
		objectName <<
		"</ObjectName>" << 
		"<ParameterKey>" <<
		parameterKey <<
		"</ParameterKey>" <<
		"</cwmp:AddObject>" <<
		"</SOAP-ENV:Body>" <<
		"</SOAP-ENV:Envelope>";

	return response.str();
}

string delete_object(string objectName, string parameterKey)
{
	std::ostringstream response;
		response <<
		"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" " <<
		"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" " <<
		"xmlns:cwmp=\"urn:dslforum-org:cwmp-1-0\" " <<
		"xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\" " <<
		"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" <<
		"<SOAP-ENV:Header>" <<
		"<cwmp:ID SOAP-ENV:mustUnderstand=\"1\">" <<
		"123" <<
		"</cwmp:ID>" <<
		"</SOAP-ENV:Header>" <<
		"<SOAP-ENV:Body>" <<
		"<cwmp:DeleteObject>" <<
		"<ObjectName>" <<
		objectName <<
		"</ObjectName>" << 
		"<ParameterKey>" <<
		parameterKey <<
		"</ParameterKey>" <<
		"</cwmp:DeleteObject>" <<
		"</SOAP-ENV:Body>" <<
		"</SOAP-ENV:Envelope>";

	return response.str();
}
