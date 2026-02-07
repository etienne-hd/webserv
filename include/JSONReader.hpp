#ifndef JSON_HPP
# define JSON_HPP

# include <exception>
# include <ostream>
# include <string>
# include <vector>

class JSONReader {
	private:
		std::string		_data;
		std::string		_base_data;

		JSONReader getValue(std::string::iterator &it);
		std::string getKey(std::string::iterator &it);
		explicit JSONReader(std::string data, std::string base_data);
	public:
		explicit JSONReader(std::string data);

		static bool isValidJSON(std::string data);
		
		JSONReader get(std::string key);
		JSONReader get(unsigned int index);
		JSONReader operator[](unsigned int index);
		JSONReader operator[](std::string key);
		std::vector<JSONReader> values(int n = -1);
		std::vector<std::string> keys(void);

		std::string toString(void) const;
		int toInt(void) const;
		long toLong(void) const;
		long long toLongLong(void) const;
		float toFloat(void) const;
		double toDouble(void) const;
		bool toBool(void) const;

		std::string raw(void) const;
		
		bool isArray(void) const;
		bool isDict(void) const;
		bool isNumber(void) const;
		bool isString(void) const;
		bool isBool(void) const;
		bool isNull(void) const;
		unsigned int length(void);

		class JSONReaderError: public std::exception {};
		class InvalidJSON: public JSONReaderError {
			virtual const char *what(void) const throw();
		};
		class InvalidConversion: public JSONReaderError {
			virtual const char *what(void) const throw();
		};
		class NotADict: public JSONReaderError {
			virtual const char *what(void) const throw();
		};
		class NotAnArray: public JSONReaderError {
			virtual const char *what(void) const throw();
		};
		class KeyNotFound: public JSONReaderError {
			virtual const char *what(void) const throw();
		};
		class OutOfRange: public JSONReaderError {
			virtual const char *what(void) const throw();
		};
};

std::ostream &operator<<(std::ostream &stream, JSONReader reader);

#endif