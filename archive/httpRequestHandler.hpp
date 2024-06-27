#ifndef HTTPREQUESTHANDLER_HPP
# define HTTPREQUESTHANDLER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include "httpRequestParser.hpp"

class httpRequestHandler
{
	public:
		httpRequestHandler(const std::string &request);
		~httpRequestHandler();
		void handleRequest(); // handle the request
		std::string getResponse(); // get the response

	private:
		int checkRequest(); // check if the request is valid
		int checkUri(); // check if the URI is valid
		void handleGet(); // handle the GET request
		void handlePost(); // handle the POST request
		void handleDelete(); // handle the DELETE request

		httpRequestParser parser;
		std::string response(int code, const std::map<std::string, std::string>& _headers, const std::string& body);

};

#endif // HTTPREQUESTHANDLER_HPP
