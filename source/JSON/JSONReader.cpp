#include "JSONReader.hpp"

#include <string>
#include <cstdlib>
#include <vector>

JSONReader::JSONReader(std::string content, std::string content_base) {
	_data = content;
	_base_data = content_base;
}

JSONReader::JSONReader(std::string data) {
	if (!JSONReader::isValidJSON(data))
		throw JSONReader::InvalidJSON();

	bool inScope = false;
	for (std::string::iterator it = data.begin(); it != data.end(); it++) {
		if (*it == '"' && *(it - 1) != '\\')
			inScope = !inScope;
		else if (inScope == false && ((*it >= 8 && *it <= 13) || *it == ' ' || *it == '\n'))
			data.erase(it--);
	}
	
	_data = data;
	_base_data = data;
}

JSONReader JSONReader::getValue(std::string::iterator &it) {
	std::string value;
	bool inScope = false;

	// Array / Dict
	if (*it == '{' || *it == '[') {
		char openChar = *it == '{' ? '{' : '[';
		char closeChar = *it == '{' ? '}' : ']';
		int depth = 0;
		while (it != _data.end() - 1)
		{
			if (*it == '"' && *(it - 1) != '\\')
				inScope = !inScope;
			else if (inScope == false && *it == openChar)
				depth++;
			else if (inScope == false && *it == closeChar)
				depth--;
			value += *it;
			it++;
			if (depth == 0)
				break;
		}
	}
	// Basic (e.g: "a": 1)
	else {
		while (!(*it == ',' && inScope == false) && it != _data.end() - 1) {
			if (*it == '"' && *(it - 1) != '\\')
				inScope = !inScope;
			value += *it;
			it++;
		}
	}
	return (JSONReader(value, _base_data));
}

std::string JSONReader::getKey(std::string::iterator &it) {
	std::string key;
	bool inScope = false;

	while (!(*it == ':' && inScope == false) && it != _data.end() - 1) {
		if (*it == '"' && *(it - 1) != '\\')
			inScope = !inScope;
		else
			key += *it;
		it++;
	}
	return (key);
}

JSONReader JSONReader::get(std::string key) {
	if (!(*_data.begin() == '{' && *(_data.end() - 1) == '}'))
		throw JSONReader::NotADict();

	std::string::iterator it = _data.begin() + 1;

	while (it != _data.end()) {
		std::string currentKey = this->getKey(it);
		it++; // skip ':'

		JSONReader currentValue = this->getValue(it);
		it++; // skip ','
		
		if (key == currentKey)
			return (currentValue);
	}
	throw JSONReader::KeyNotFound();
}

JSONReader JSONReader::get(unsigned int index) {
	std::vector<JSONReader> values = this->values(index + 1);
	if (values.size() > index) {
		return (values[index]);
	}
	throw JSONReader::OutOfRange();
}

JSONReader JSONReader::operator[](unsigned int index) {
	return(this->get(index));
}

JSONReader JSONReader::operator[](std::string key) {
	return(this->get(key));
}

std::vector<JSONReader> JSONReader::values(int n) {
	std::vector<JSONReader> values;

	if (!this->isArray())
		throw JSONReader::NotAnArray();

	std::string::iterator it = _data.begin() + 1;
	
	int currentIndex = 0;
	while (it < _data.end() - 1 && (currentIndex < n || n == -1)) {
		JSONReader currentValue = this->getValue(it);
		it++; // skip ','

		values.push_back(currentValue);
		currentIndex--;
	}
	return (values);
}

std::vector<std::string> JSONReader::keys(void) {
	std::vector<std::string> keys;

	if (!this->isDict())
		throw JSONReader::NotADict();

	std::string::iterator it = _data.begin() + 1;

	while (it < _data.end() - 1) {
		std::string currentKey = this->getKey(it);
		it++; // skip ':'

		JSONReader currentValue = this->getValue(it);
		it++; // skip ','
		
		keys.push_back(currentKey);
	}
	
	return (keys);
}

std::string JSONReader::toString(void) const {
	if (!this->isString())
		throw JSONReader::InvalidConversion();
	return std::string(_data.begin() + 1, _data.end() - 1);
}

int JSONReader::toInt(void) const {
	if (!this->isNumber())
		throw JSONReader::InvalidConversion();
	return (std::atoi(_data.c_str()));
}

long JSONReader::toLong(void) const {
	if (!this->isNumber())
		throw JSONReader::InvalidConversion();
	return (std::atol(_data.c_str()));
}

long long JSONReader::toLongLong(void) const {
	if (!this->isNumber())
		throw JSONReader::InvalidConversion();
	return (std::atoll(_data.c_str()));
}

float JSONReader::toFloat(void) const {
	if (!this->isNumber())
		throw JSONReader::InvalidConversion();
	return (std::atof(_data.c_str()));
}

double JSONReader::toDouble(void) const {
	if (!this->isNumber())
		throw JSONReader::InvalidConversion();
	return (std::strtod(_data.c_str(), __null));
}

bool JSONReader::toBool(void) const {
	if (!this->isBool())
		throw JSONReader::InvalidConversion();
	return (_data == "true");
}

std::string JSONReader::raw(void) const {
	return (_data);
}

bool JSONReader::isArray(void) const {
	return (*_data.begin() == '[');
}

bool JSONReader::isDict(void) const {
	return (*_data.begin() == '{');
}

bool JSONReader::isNumber(void) const {
	return (std::isdigit(*_data.begin()) || *_data.begin() == '-');
}

bool JSONReader::isString(void) const {
	return (*_data.begin() == '"');
}

bool JSONReader::isBool(void) const {
	return (_data == "true" || _data == "false");
}

bool JSONReader::isNull(void) const {
	return (_data == "null");
}

unsigned int JSONReader::length(void) {
	return (this->values().size());
}

const char *JSONReader::InvalidJSON::what(void) const throw() {
	return ("Provided JSON is invalid.");
}
const char *JSONReader::InvalidConversion::what(void) const throw() {
	return ("Can't convert data to this type.");
}
const char *JSONReader::NotADict::what(void) const throw() {
	return ("Not a dict.");
}
const char *JSONReader::NotAnArray::what(void) const throw() {
	return ("Not an array.");
}
const char *JSONReader::KeyNotFound::what(void) const throw() {
	return ("Key not found.");
}
const char *JSONReader::OutOfRange::what(void) const throw() {
	return ("Index out of range.");
}

std::ostream &operator<<(std::ostream &stream, JSONReader reader) {
	stream << reader.raw();
	return (stream);
}