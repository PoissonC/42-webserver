#include "httpRequestParser.hpp"

httpRequestParser::httpRequestParser(const std::string &request) : _req(request)
{
	// constructor will initialize the request variable
}

httpRequestParser::~httpRequestParser()
{
	// destructor
}

void httpRequestParser::parse() // parse the request and store the method, uri, version, headers and body in the class variables
{
	std::istringstream iss(this->_req);
	std::string line;
	bool inLine = true;
	bool inHeaders = false;
	std::stringstream bodyStream;
	while (std::getline(iss, line))
	{
		if (inLine) // first line of the request (Method, URI, Version)
		{
			size_t pos = line.find(" ");
			if (pos != std::string::npos)
			{
				this->_method = line.substr(0, pos);
				line.erase(0, pos + 1);
				pos = line.find(" ");
				if (pos != std::string::npos)
				{
					this->_uri = line.substr(0, pos);
					line.erase(0, pos + 1);
					if (line.empty())
						this->_version = "";
					else
						this->_version = line;
				}
			}
			inLine = false;
			inHeaders = true;
		}
		else if (inHeaders) // hearders of the request in a map variable
		{
			size_t pos = line.find(": ");
			if (pos != std::string::npos)
			{
				std::string key = line.substr(0, pos);
				line.erase(0, pos + 2);
				this->_headers[key] = line;
			}
			if (line == "\r" || line == "\r\n" || line == "\n")
				inHeaders = false;
		}
		else // body of the request
		{
			bodyStream << line << std::endl;
		}
	}
	this->_body = bodyStream.str(); // store the body in the class variable _body
	this->parseUri(); // parse the uri and store the ressource path and the query parameters in the class variables
}

void httpRequestParser::parseUri() // parse the uri and store the ressource path and the query parameters in the class variables
{
	size_t pos = this->_uri.find("?");
	if (pos != std::string::npos)
	{
		this->_ressourcePath = this->_uri.substr(0, pos);
		this->_paramQuery = this->_uri.substr(pos + 1);
	}
	else
	{
		this->_ressourcePath = this->_uri;
		this->_paramQuery = "";
	}
	// Parse and split the uri into its components
	std::string uri = this->_uri;
	size_t posScheme = uri.find("://");
	if (posScheme != std::string::npos) // if the uri contains a scheme
	{
		this->_uriComponents.scheme = uri.substr(0, posScheme);
		uri = uri.substr(posScheme + 3);
	}
	size_t posHost = uri.find(":");
	if (posHost != std::string::npos) // if the uri contains a port
	{
		this->_uriComponents.host = uri.substr(0, posHost);
		uri = uri.substr(posHost + 1);
	}
	size_t posPort = uri.find("/");
	if (posPort != std::string::npos) // if the uri contains a path
	{
		this->_uriComponents.port = uri.substr(0, posPort);
		uri = uri.substr(posPort + 1);
	}
	size_t posPath = uri.find("?");
	if (posPath != std::string::npos) // if the uri contains a query
	{
		this->_uriComponents.path = uri.substr(0, posPath);
		uri = uri.substr(posPath + 1);
	}
	size_t posQuery = uri.find("#");
	if (posQuery != std::string::npos) // if the uri contains a fragment
	{
		this->_uriComponents.query = uri.substr(0, posQuery);
		uri = uri.substr(posQuery + 1);
	}
	this->_uriComponents.fragment = uri;
}

std::string httpRequestParser::getMethod() const
{
	return (this->_method);
}

std::string httpRequestParser::getUri() const
{
	return (this->_uri);
}

std::string httpRequestParser::getVersion() const
{
	return (this->_version);
}

std::map<std::string, std::string> httpRequestParser::getHeaders() const
{
	return (this->_headers);
}

std::string httpRequestParser::getBody() const
{
	return (this->_body);
}

std::string httpRequestParser::getRessourcePath() const
{
	return (this->_ressourcePath);
}

std::string httpRequestParser::getParamQuery() const
{
	return (this->_paramQuery);
}

uriComponents httpRequestParser::getUriComponents() const
{
	return (this->_uriComponents);
}
