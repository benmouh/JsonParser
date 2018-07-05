#pragma once
/* Helper class to pars and serialize Json object */
class JsonValue
{
public:
	JsonValue():fboolValue(false), fNumber(0), fStrValue("") {};
	~JsonValue() {};
	bool fboolValue;
	int64_t fNumber;
	std::string fStrValue;
	//std::vector<JsonArray> fArray;
	//std::vector<JsonValues> fObject;
	// stters 
	void setBoolean(bool inValue) { fboolValue = inValue; };
	void setNumber(int64_t inNumber) { fNumber = inNumber; };
	void setStringValue(const std::string& inStrValue) { fStrValue = inStrValue; };

	// getters

	bool getBoolean() { return fboolValue; };
	int64_t getNumber() { return fNumber; };
	std::string getStringValue() { return fStrValue; };
	void reset() { fboolValue = false; fNumber = 0; fStrValue = ""; };
};
class  JsonValues {
public:
	JsonValues() {};
	~JsonValues() {};
	std::map<std::string, JsonValue> fjsonValues; // {"key":"val", ... }
	void getProperty(const std::string& inKey, JsonValue& outValue);
	void setProperty(const std::string& inkey, const JsonValue& inValue);
	void removeProperty(const std::string& inKey);
	void ClearProperties();
	int64_t getSize();
};

/* Helper class to parse and serialize Json Array */
class JsonArray {
public:
	JsonArray() {};
	~JsonArray() {};
	std::vector<JsonValue> fArray; // ["val", "val" .... ]
	void getPropety(const int64_t& inIndex, JsonValue& outValue);
	void setProperty(const JsonValue& inValue);
	void clearArrayValues();

};
/* Json value definition according to standard*/
class JsonObject
{
public: 
	JsonObject() {};
	~JsonObject() {};
	std::vector<JsonObject> fObjectValues;
	std::vector<JsonArray> fArrayValues;
	std::vector<JsonValues> fJsonValues;
	void addValueProperty(const JsonValues& inValues);
	void addArrayProperty(const JsonArray& inArray);
	void addObjectProperty(const JsonObject& inValues);
	
	void Clear();
};
/* Helper Class to parse and serializ Data */
class JsonParser
{
public:
	
	void parse(const std::string& injson, JsonObject& outjsonObj);
	void serialize(const JsonObject& injsonObj, std::string& outJsonStr);
	bool isValidJson(const std::string& injson);
	std::shared_ptr<JsonObject> ExtractData(const std::string& inStr);
	bool quickBracesValidate(const std::string& inJsonStr);
	bool quickBracketValidate(const std::string& inJsonStr);
	bool quickQuotValidate(const std::string& inJsonStr);
	bool isObject();
	bool isArray();
	
	JsonParser();
	~JsonParser();
};

