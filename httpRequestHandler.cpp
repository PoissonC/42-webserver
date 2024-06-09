#include "httpRequestHandler.hpp"

httpRequestHandler::httpRequestHandler(const std::string &request) : parser(request)
{
	// constructor will initialize the parser variable
	this->parser.parse();
}

httpRequestHandler::~httpRequestHandler()
{
	// destructor
}

void httpRequestHandler::handleRequest()
{
	// handle the request
	int code;
	code = this->checkRequest();
	if (code != 0)
		this->response(code);
	else
	{
		if (this->parser.getMethod() == "GET")
			this->handleGet();
		else if (this->parser.getMethod() == "POST")
			this->handlePost();
		else if (this->parser.getMethod() == "DELETE")
			this->handleDelete();
	}
}

int httpRequestHandler::checkRequest()
{
	std::string method = this->parser.getMethod();
	std::string uri = this->parser.getUri();
	std::string version = this->parser.getVersion();

	if (method.empty() || uri.empty() || version.empty()) // check if the method, uri or version is empty
		return (400);
	if (version != "HTTP/1.1") // check if the version is HTTP/1.1
		return (505);
	if (method != "GET" && method != "POST" && method != "DELETE") // check if the method is GET, POST or DELETE
		return (501);
	int codeUri = this->checkUri();
	if (codeUri != 0) // check the uri
		return (codeUri);
	if (this->parser.getHeaders().find("Host") == this->parser.getHeaders().end()) // check if the Host header is present
		return(400);
	return (0);
}

int httpRequestHandler::checkUri()
{
	// to confirme if it is not more easy to handle this part directly in the method checkRequest
	std::string uri = this->parser.getRessourcePath();
	if (uri.empty()) // check if the ressource path is empty
		return (404);
	if (uri.find("..") != std::string::npos) // check if the ressource path contains ".."
		return (403);
	if (uri.find("~") != std::string::npos) // check if the ressource path contains "~"
		return (403);
	return (0);
}

void httpRequestHandler::handleGet()
{
	// handle the GET request
	if (this->parser.getRessourcePath().find(".php") != std::string::npos || this->parser.getRessourcePath().find(".py") != std::string::npos) // check if the ressource path contains ".php"
	{
		// handle the PHP or Python file via CGI
	}
	else if () // check if the ressource path is a directory authorized to be listed
	{
		// handle the directory
	}
	else
	{
		// 404 Not Found
	}
}

void httpRequestHandler::handlePost()
{
	// handle the POST request
	if (this->parser.getRessourcePath().find(".php") != std::string::npos || this->parser.getRessourcePath().find(".py") != std::string::npos) // check if the ressource path contains ".php"
	{
		// handle the PHP or Python file via CGI
	}
	else if () // check if the ressource path is a directory authorized to be listed
	{
		// handle the directory
	}
	else
	{
		// 404 Not Found
	}
}

void httpRequestHandler::handleDelete()
{
	// handle the DELETE request
	if (this->parser.getRessourcePath().find(".php") != std::string::npos || this->parser.getRessourcePath().find(".py") != std::string::npos) // check if the ressource path contains ".php"
	{
		// handle the PHP or Python file via CGI
	}
	else if () // check if the ressource path is a directory authorized to be listed
	{
		// handle the directory
	}
	else
	{
		// 404 Not Found
	}
}

std::string httpRequestHandler::response(int code)
{
	// send the response https://datatracker.ietf.org/doc/html/rfc2616#page-39
	std::string response;
	std::map<int, std::string> responses = {
		{200, "OK"},
		{201, "Created"},
		{204, "No Content"},
		{400, "Bad Request"},
		{401, "Unauthorized"},
		{403, "Forbidden"},
		{404, "Not Found"},
		{405, "Method Not Allowed"},
		{500, "Internal Server Error"},
		{501, "Not Implemented"},
		{505, "HTTP Version Not Supported"}
	};

	response = "HTTP/1.1 " + std::to_string(code) + " " + responses[code] + "\r\n";
	// Add map for the headers and loop through the headers
	response += "Content-Type: text/html\r\n";
	response += "Content-Length: 0\r\n";
	response += "\r\n";
	return (response);

	// Add option for the body of the response and more
}
