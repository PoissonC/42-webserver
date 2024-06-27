#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <iostream>
#include <string>
#include <map>

class httpResponse
{
public:
	httpResponse();
	~httpResponse();
	void setStatusCode(int code);
	void setHeaders(const std::map<std::string, std::string> &responseHeaders);
	void setBody(std::string &responseBody);
	int getStatusCode() const;
	std::string getResponse() const;

private:
	int _statusCode;
	std::string _reasonPhrase;
	std::string _version;
	std::map<std::string, std::string> _headers;
	std::string _body;
};

#endif
