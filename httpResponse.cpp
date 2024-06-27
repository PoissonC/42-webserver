#include "httpResponse.hpp"

httpResponse::httpResponse()
{
	// constructor
	this->_statusCode = 0;
	this->_reasonPhrase = "";
	this->_version = "HTTP/1.1";
	this->_headers = std::map<std::string, std::string>();
	this->_body = "";
}

httpResponse::~httpResponse()
{
	// destructor
}

void httpResponse::setStatusCode(int code)
{
	// set the status code
	std::map<std::string, std::string> codePhrase =
	{
		{ "200", "OK" },
		{ "201", "Created" },
		{ "202", "Accepted" },
		{ "204", "No Content" },
		{ "400", "Bad Request" },
		{ "401", "Unauthorized" },
		{ "403", "Forbidden" },
		{ "404", "Not Found" },
		{ "405", "Method Not Allowed" },
		{ "408", "Request Timeout" },
		{ "413", "Payload Too Large" },
		{ "414", "URI Too Long" },
		{ "500", "Internal Server Error" },
		{ "501", "Not Implemented" },
		{ "502", "Bad Gateway" },
		{ "503", "Service Unavailable" },
		{ "505", "HTTP Version Not Supported" }
	};
	this->_statusCode = code;
	this->_reasonPhrase = codePhrase[std::to_string(code)];
	if (this->_statusCode >= 300)
	{
		this-> _body = "<html><head><title>" + std::to_string(this->_statusCode) + " "
			+ this->_reasonPhrase + "</title></head><body><h1>" + std::to_string(this->_statusCode)
			+ " " + this->_reasonPhrase + "</h1></body></html>";
	}
}

int httpResponse::getStatusCode() const
{
	// get the status code
	return this->_statusCode;
}

void httpResponse::setHeaders(const std::map<std::string, std::string> &responseHeaders)
{
	// set the headers
	this->_headers = responseHeaders;
}

void httpResponse::setBody(std::string &responseBody)
{
	// set the body
	this->_body = responseBody;
}

std::string httpResponse::getResponse() const
{
	// get the response
	std::string response = this->_version + " " + std::to_string(this->_statusCode) + " " + this->_reasonPhrase + "\r\n";
	// confirm if we want keep this or not for the moment
	if (this->_statusCode >= 300)
	{
		response += "Content-Length: " + std::to_string(this->_body.size()) + "\r\n";
		response += "Content-Type: text/html\r\n";
	}
	else
	{
		for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
		{
			response += it->first + ": " + it->second + "\r\n";
		}
	}
	response += "\r\n";
	response += this->_body;
	return response;
}
