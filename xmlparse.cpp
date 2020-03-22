#include "tinyxml2.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>

using namespace tinyxml2;
using namespace std;

void GetAllElementValue(XMLElement * element)
{
	for (XMLElement* currentele = element->FirstChildElement(); currentele; currentele = currentele->NextSiblingElement())
	{
	    XMLElement* tmpele = currentele;
	    if (currentele->GetText() != NULL)
            cout << currentele->Name() << ":" << currentele->GetText() << endl;	
	    if (!tmpele->NoChildren())
		{
			GetAllElementValue(tmpele);
		}
	}
}

XMLElement *FindFirstMatchedElement(XMLElement * element, const char *elementName)
{
	for (XMLElement* currentele = element->FirstChildElement(); currentele; currentele = currentele->NextSiblingElement())
	{
		if (XMLUtil::StringEqual(currentele->Name(), elementName))
		{
			cout << "matched element: " << " - "<< currentele->Name() << endl;
			return currentele;
		}

	    if (!currentele->NoChildren())
		{
			XMLElement * matchedElement = FindFirstMatchedElement(currentele, elementName);
			if (matchedElement)
			{
				return matchedElement;
			}
		}
	}

	return nullptr;
}

XMLElement *FindFirstMatchedElementWithAttribute(XMLElement * element, const char *elementName, const char *attributeName, const char *attributeValue)
{
	for (XMLElement* currentele = element->FirstChildElement(); currentele; currentele = currentele->NextSiblingElement())
	{
		if (XMLUtil::StringEqual(currentele->Name(), elementName))
		{
			const XMLAttribute * attribute = currentele->FindAttribute(attributeName);
			
			if ( (attribute != nullptr) && (XMLUtil::StringEqual(attribute->Value(), attributeValue)) )
			{
				cout << "matched element: " << " - "<< currentele->Name() << " with attribute: " << attributeName << ", " << attributeValue << endl;
				return currentele;					
			}
		}

	    if (!currentele->NoChildren())
		{
			XMLElement * matchedElement = FindFirstMatchedElementWithAttribute(currentele, elementName, attributeName, attributeValue);
			if (matchedElement)
			{
				return matchedElement;
			}
		}
	}

	return nullptr;
}

// command: xmlparse [xmlfile] [countername]
int main( int argc, const char **argv)
{
	if (argc > 1)
	{		
		XMLDocument doc;
		doc.LoadFile(argv[1]);
		int errorID = doc.ErrorID();
		
		if (errorID)
		{
			printf("Parse failed, error id=%d!\n", errorID);
		}
		else
		{
			if (argc <= 2)
			{
				GetAllElementValue(doc.FirstChildElement());
			}
			else
			{
				XMLElement *matchedElement = nullptr;
				
				if (0)
				{
					// Find directly the PM counter
					matchedElement = FindFirstMatchedElement(doc.FirstChildElement()->FirstChildElement(), argv[2]);
				}
				else
				{
					// Find the paranet element (NE-WBTS_1.0, "measurementType" "LTE_eNB_Load") and then the counter in argv[2]
					XMLElement *ParentElement = FindFirstMatchedElementWithAttribute(doc.FirstChildElement()->FirstChildElement(), "NE-WBTS_1.0", "measurementType", "LTE_eNB_Load");
					if (ParentElement)
					{
						matchedElement=ParentElement->FirstChildElement(argv[2]);
					}
				}

				if (matchedElement)
				{
					cout << "Find matched Element: " << matchedElement->Name() << ":" << matchedElement->GetText() << endl;
				}
				else
				{
					cout << "No matched element found." << endl;
				}

			}
		}
	}
	else
	{
		printf("argument not enough! please input filename, and field name\n");
	}

	return 0;
}