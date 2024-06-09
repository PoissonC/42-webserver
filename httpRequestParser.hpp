#ifndef HTTPREQUESTPARSER_HPP
# define HTTPREQUESTPARSER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <map>

struct uriComponents
{
	std::string scheme;
	std::string host;
	std::string port;
	std::string path;
	std::string query;
	std::string fragment;
};

class httpRequestParser
{
	public:
		httpRequestParser(const std::string &request);
		~httpRequestParser();
		void parse();
		void parseUri();
		std::string getMethod() const;
		std::string getUri() const;
		std::string getVersion() const;
		std::map<std::string, std::string> getHeaders() const;
		std::string getBody() const;
		std::string getRessourcePath() const;
		std::string getParamQuery() const;
		uriComponents getUriComponents() const;

	private:
		std::string _method;
		std::string _uri;
		std::string _version;
		std::map<std::string, std::string> _headers;
		std::string _body;
		std::string _req;
		std::string _ressourcePath;
		std::string _paramQuery;
		uriComponents _uriComponents;
};

#endif // HTTPREQUESTPARSER_HPP
