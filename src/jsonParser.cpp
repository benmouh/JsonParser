// JsonParser.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <regex>
#include <cstdint>
#include <memory>
#include <cassert>
#include "jsonParser.h"
enum jsonValue 
{
	jsonString,
	jsonNumber,
	jsonNull,
	jsonBool,
	jsonArray,
	jsonObject
} JSVALUE;
struct keyWords
{
	char OPENOBJECT = '{';
	char CLOSEOBJECT = '}';
	char OPENARRAY = '[';
	char CLOSEARRAY = ']';
	char VALUEDELIMITER = ',';
	char OPENVALUE = '"';
	char CLOSEVALUE = '"';
	char SEPARATOR = ':';
	char LF = '\n';
	char CR = '\r';

} JSTRUCT;

void JsonValues::setProperty(const std::string& inkey, const JsonValue& inValue) 
{
	fjsonValues[inkey] = inValue;
}

void JsonValues::getProperty(const std::string& inKey, JsonValue& outValue)
{
	outValue = fjsonValues[inKey];
}
void JsonValues::removeProperty(const std::string& inKey)
{
	fjsonValues.erase(inKey);
}
void JsonValues::ClearProperties()
{
	fjsonValues.clear();
}
int64_t JsonValues::getSize()
{
	return fjsonValues.size();
}
void JsonObject::addValueProperty(const JsonValues& inValues)
{
	fJsonValues.push_back(inValues);
}
void JsonObject::addArrayProperty(const JsonArray& inArray)
{
	fArrayValues.push_back(inArray);
}
void JsonObject::addObjectProperty(const JsonObject& inObject)
{
	fObjectValues.push_back(inObject);
}
void JsonObject::Clear()
{
	fObjectValues.clear();
	fArrayValues.clear();
	fJsonValues.clear();
}

void JsonArray::getPropety(const int64_t& inIndex, JsonValue& outValue) 
{
	if(inIndex <= fArray.size() ) outValue = fArray[inIndex];
}
void JsonArray::setProperty(const JsonValue& inValue) 
{
	fArray.push_back(inValue);
}
void JsonArray::clearArrayValues() 
{
	fArray.clear();
}

JsonParser::JsonParser()
{
	
}

JsonParser::~JsonParser()
{

}
std::shared_ptr<JsonObject> JsonParser::ExtractData(const std::string& inStr)
{
	std::shared_ptr<JsonObject> jsonObj(new JsonObject());
	std::string tmpString = inStr;
	JsonValues jsonValues;
	JsonArray jsonArray;
	std::vector<std::string> keys;
	std::vector<std::string> values;
	JsonValue jsonValue;
	for (size_t i = 0; i < inStr.length(); ++i)
	{
		//std::cout << "caracter at " << i << inStr.at(i) << std::endl;
		if (inStr.at(i) == JSTRUCT.OPENOBJECT)
		{

			std::cout << "new object open " << tmpString.at(1) << std::endl;
			continue; // do the same for new subobject 
		}
		else if (inStr.at(i) == JSTRUCT.OPENARRAY)
		{
			
			std::cout << "new array " << std::endl;
			break;
		
		}
		else if (inStr.at(i) == JSTRUCT.OPENVALUE) {
			
			//std::cout << "test " << inStr.at(i + 1) << std::endl;
			//std::cout << "test " << tmpString << std::endl;
		
			if (inStr.at(i + 1) != JSTRUCT.SEPARATOR && inStr.at(i + 1) != JSTRUCT.VALUEDELIMITER && inStr.at(i + 1) != JSTRUCT.CLOSEOBJECT)
			{
				// key
				//std::cout << "test " << tmpString.at(0) << std::endl;
				size_t openVal = tmpString.find_first_of(JSTRUCT.OPENVALUE);
				if (tmpString.at(0) != JSTRUCT.SEPARATOR) // keys are delimited by (:)
				{
					size_t SepPos = tmpString.find_first_of(JSTRUCT.SEPARATOR);
					std::string key = tmpString.substr(openVal + 1, SepPos - openVal - 2);
					keys.push_back(key);
				}
				else if (tmpString.at(0) != JSTRUCT.VALUEDELIMITER) // values are delimited by (,)
				{
					size_t comma = tmpString.find_first_of(JSTRUCT.VALUEDELIMITER);
					size_t CloseSep = tmpString.find_first_of(JSTRUCT.CLOSEOBJECT);
					std::string val = tmpString.substr(openVal + 1, comma - openVal - 2);
					rsize_t lastValue = val.find_first_of(JSTRUCT.CLOSEOBJECT); // "val"}
					if (lastValue) val = val.substr(0, lastValue - 1);
					values.push_back(val); 
					
				}
				
			}
			
			
			
		} else if (inStr.at(i) == JSTRUCT.SEPARATOR){

			// no ""
			//std::cout << "test " << tmpString << std::endl;
			if (inStr.at(i+1) != JSTRUCT.OPENVALUE && inStr.at(i + 1) != JSTRUCT.OPENARRAY)
			{
				std::string NoStrVal;
				size_t SepPos = tmpString.find_first_of(JSTRUCT.SEPARATOR); // :
				size_t comma = tmpString.find_first_of(JSTRUCT.VALUEDELIMITER); // ,
				size_t closeObj = tmpString.find_first_of(JSTRUCT.CLOSEOBJECT); // }
				if(comma != std::string::npos) NoStrVal = tmpString.substr(SepPos+1, comma - SepPos-1);
				else  NoStrVal = tmpString.substr(SepPos + 1, closeObj - SepPos - 1);
				values.push_back(NoStrVal);
			}
			else if(inStr.at(i + 1) == JSTRUCT.OPENARRAY) {
				
				std::cout << "test " << tmpString << std::endl;
				std::string consumed = tmpString;
				for (int j = 0; j < tmpString.length(); ++j) {
					size_t openAr = consumed.find_first_of(JSTRUCT.OPENARRAY); // [
					size_t comma = consumed.find_first_of(JSTRUCT.VALUEDELIMITER); // ,
					size_t closeAr = consumed.find_first_of(JSTRUCT.CLOSEARRAY); // ]
					std::string value;
					if (comma != std::string::npos) value = consumed.substr(openAr + 1, comma - openAr - 1);
					else value = consumed.substr(openAr + 1, closeAr - openAr - 1);
					std::cout << "----- " << value << std::endl;
					consumed = tmpString.substr(j, tmpString.length());
				}
				
			}
			
		}
		tmpString = inStr.substr(i, inStr.length());
		//std::cout << "consumed " << tmpString << std::endl;
		
	}
	int i = 0;
	for (auto k : keys) 
	{
		//assert(i < values.size());
		if(i < values.size()) jsonValue.setStringValue(values[i]);
		jsonValues.setProperty(k, jsonValue);
		jsonValue.reset();
		++i;
	}
	
	jsonObj->addValueProperty(jsonValues);
	return jsonObj;
}
// count { }
bool JsonParser::quickBracesValidate(const std::string& inJsonStr)
{
	int bracketCount = 0;
	int closedBracket = 0;
	
	for (size_t i = 0; i < inJsonStr.length(); ++i)
	{
		if (inJsonStr.at(i) == JSTRUCT.OPENOBJECT) ++bracketCount;
		if (inJsonStr.at(i) == JSTRUCT.CLOSEOBJECT) ++closedBracket;
	}
	if (bracketCount == closedBracket) return true;
	return false;
}
// count []
bool JsonParser::quickBracketValidate(const std::string& inJsonStr)
{
	int openCount = 0;
	int closeCount = 0;
	for (size_t i = 0; i < inJsonStr.length(); ++i)
	{
		if (inJsonStr.at(i) == JSTRUCT.OPENARRAY) ++openCount;
		if (inJsonStr.at(i) == JSTRUCT.CLOSEARRAY) ++closeCount;
	}
	if (openCount == closeCount) return true;
	return false;
}

// count ""
bool JsonParser::quickQuotValidate(const std::string& inJsonStr)
{
	int quotCount = 0;
	for (size_t i = 0; i < inJsonStr.length(); ++i)
	{
		if (inJsonStr.at(i) == JSTRUCT.OPENVALUE) ++quotCount;
	}
	if (quotCount %2 == 0) return true;
	return false;
}
bool JsonParser::isValidJson(const std::string& injson)
{
	
	// Quick validate
	if (injson.at(0) != JSTRUCT.OPENOBJECT) return false;
	if (!quickBracesValidate(injson)) return false;
	if (!quickBracketValidate(injson)) return false;
	if (!quickQuotValidate(injson)) return false;
	// check for order
	try {
		//{.*:{.*:.*}} | {.*:.*} | {.*:[.*:.*]}
		//std::string regxPattern = "{.*:{.*:.*}}"; // {"key": "val", "key": "val", {"key":"val"}, [{"key":"val"}, {"k":"v"}]}
		std::string regxPattern = "\\{.*\\:.*\\}|\\{.*\\:\\{.*\\:.*\\}\\}|\\{.*\\:\\[\\{.*\\:.*\\}\\]\\}";
		std::regex regx(regxPattern, std::regex_constants::icase);
		if (std::regex_match(injson, regx))
		return true;
	} catch (std::exception e) {
		
		std::cerr << e.what() << std::endl;
	}
	
	return false;
}

void JsonParser::parse(const std::string& injson, JsonObject& outjsonObj)
{
	if (isValidJson(injson)) 
	{
		try {
			outjsonObj = *ExtractData(injson);
		}
		catch(std::exception e) {
			std::cerr << e.what() << std::endl;
		}
	}
}
void JsonParser::serialize(const JsonObject& injsonObj, std::string& outJsonStr) 
{

}

bool JsonParser::isObject() { return true; }

bool JsonParser::isArray() { return true;  }

int main()
{		/* ------------------------------------- Unit Test --------------------------------------------------------------- */	std::string json = "{";
		json += '"';
		json += "key1";
		json += '"';
		json += ":";
		json += '"';
		json += "value1";
		json += '"';
		json += ",";
		json += '"';
		json += "key2";
		json += '"';
		json += ":";
		//json += '"';
		json += "value2";
		//json += '"';
		//json += "}";

		json += ",";
		json += '"';
		json += "key3";
		json += '"';
		json += ":";
		//json += '"';
		json += "value3";
		//json += '"';



		json += ",";
		json += '"';
		json += "kay4";
		json += '"';
		json += ":";
		json += "[";
		json += '"';
		json += "v1";
		json += '"';
		json += ",";
		json += '"';
		json += "v2";
		json += '"';
		json += "]";

		json += "}";


		//"{\"key1\":\"value1\",\"key2\":\"value2\"}"; // exemple of simple json string
	//std::string json = "{\"key1\":\"value1\",\"key\":[{\"k\":\"v\"}]}"; // exemple of json with object array 
	//std::string json = "{\"key1\":{\"k\":\"v\",\"k\":\"v\"}}"; // exemple of json with object 
	//std::string json = "{\"k\":[{\"k\":\"v\",\"k\":\"v\"}],\"K\":{\"k\":\"v\"}}"; // exemple of json with both object and array 
	//std::string json = "{gsdsgsdg";
	JsonParser jsonparser;
	JsonObject obj;
	if (jsonparser.isValidJson(json)) {
		
		std::cout << "---- Test passed ---- " << std::endl;
		jsonparser.parse(json, obj);
		
		//JsonValue val;
		//jsonValues.getProperty("key1", val);
		//std::cout << "from container " << val.getStringValue() << std::endl;
		// 	//assert(jsonValues.getSize() == 3);

	}
	else std::cout << " !!!!!!! Test Failed !!!!!!!! " << std::endl;

	return getchar();
}
