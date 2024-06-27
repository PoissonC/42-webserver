#include "httpResponse.hpp"

httpResponse::httpResponse()
{
	// Constructor
}

httpResponse::~httpResponse()
{
	// Destructor
}

std::string httpResponse::getResponse() const
{
	// Get the response
	std::string response;
	response = "HTTP/1.1 " + std::to_string(this->_code) + " " + this->_statusCodes[this->_code] + "\r\n";
	for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
	{
		response += it->first + ": " + it->second + "\r\n";
	}
	response += "\r\n";
	response += this->_body;
	return (response);
}

void httpResponse::setCode(int code)
{
	// Set the code
	this->_code = code;
}

void httpResponse::setHeader(std::string key, std::string value)
{
	// Set the header
	this->_headers[key] = value;
}

void httpResponse::setBody(std::string body)
{
	// Set the body
	this->_body = body;
}
