///////////////////////////////////////////////////////////////////////////////
//
// Tr069XmlStringUtil.h
//
// Copyright radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __Tr069XmlStringUtil_h_
#define __Tr069XmlStringUtil_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <string>
#include <libxml/parser.h>
#include <tr1/memory>


///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////


using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Forward Declarations
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Functions / Classes
///////////////////////////////////////////////////////////////////////////////



class XmlStringUtils
{
private:
    XmlStringUtils();

public:
    static string Filter( const string & inputString )
    {
        string output(inputString);

        size_t position = output.find_first_of("&<>\'\"");
        while (position != string::npos)
        {
            switch(output.at(position))
            {
                case '&': output.replace(position, 1, "&amp;"); break;
                case '<': output.replace(position, 1, "&lt;"); break;
                case '>': output.replace(position, 1, "&gt;"); break;
                case '\"': output.replace(position, 1, "&quot;"); break;
                case '\'': output.replace(position, 1, "&apos;"); break;

            }

            position = output.find_first_of("&<>\'\"",position+1);
        }

        return output;
    }

    static string ExtractFromNode( xmlNodePtr node )
    {
        string xmlString;
        xmlChar * xmlNodeListString = xmlNodeListGetString( node->doc, node->xmlChildrenNode, 1);

        if(xmlNodeListString)
        {
            xmlString = (const char *)xmlNodeListString;
            xmlFree(xmlNodeListString);
        }

        return xmlString;
    }

    static string ToElementString( const string & tag, const string & value = "", const string & attributes = "")
    {
        stringstream ss;

        if(!attributes.empty())
        {
            ss << "<" << tag << " " << attributes;
        }
        else
        {
            ss << "<" << tag;
        }

        if(!value.empty())
        {
            ss << ">" << value << "</" << tag << ">";
        }
        else
        {
            ss << "/>";
        }

        return ss.str();
    }

};


#endif
