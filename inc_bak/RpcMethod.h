#ifndef __RPC_METHOD_H__
#define __RPC_METHOD_H__

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>
using namespace std;

string inform_response();
string get_parameter_names(string &paramPath, string &nextLevel);
string get_parameter_values(set<string> &paramName);
string set_parameter_values(map<string,string> &paramList, string &paramKey);
string add_object(string objectName, string parameterKey);
string delete_object(string objectName, string parameterkey);


#endif
