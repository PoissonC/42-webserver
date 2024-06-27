#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include <string>
#include <map>
#include <sstream>

class httpResponse
{
public:
	httpResponse();
	~httpResponse();
	std::string getResponse() const;
	void setCode(int code);
	void setHeader(std::string key, std::string value);
	void setBody(std::string body);

private:
	int _code;
	std::map<std::string, std::string> _headers;
	std::string _body;
};


#endif // HTTPRESPONSE_HPP
