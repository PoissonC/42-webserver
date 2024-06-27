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
		this->response(code, {}, ""); // to setup the response with the code and the headers and the body
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

std::string httpRequestHandler::getResponse()
{
	// get the response
	std::string response;
	response = this->parser.getRequest();
	return (response);
}

int httpRequestHandler::checkRequest()
{
	std::string method = this->parser.getMethod();
	std::string uri = this->parser.getUri();
	std::string version = this->parser.getVersion();
	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "text/html";

	if (method.empty() || uri.empty() || version.empty()) // check if the method, uri or version is empty
	{
		std::string body = "<html><body><h1>400 Bad Request</h1></body></html>";
		std::string response = this->response(400, headers, body);
		return (1);
	}
	if (version != "HTTP/1.1") // check if the version is HTTP/1.1
	{
		std::string body = "<html><body><h1>505 HTTP Version Not Supported</h1></body></html>";
		std::string response = this->response(505, headers, body);
		return (1);
	}
	if (this->parser.getHeaders().find("Host") == this->parser.getHeaders().end()) // check if the Host header is present
	{
		std::string body = "<html><body><h1>400 Bad Request</h1></body></html>";
		std::string response = this->response(400, headers, body);
		return(1);
	}
	if (method != "GET" && method != "POST" && method != "DELETE") // check if the method is GET, POST or DELETE
	{
		std::string body = "<html><body><h1>405 Method Not Allowed</h1></body></html>";
		std::string response = this->response(405, headers, body);
		return (1);
	}
	int codeUri = this->checkUri();
	if (codeUri != 0) // check the uri
		return (1);
	return (0);
}

int httpRequestHandler::checkUri()
{
	// to confirme if it is not more easy to handle this part directly in the method checkRequest
	std::string uri = this->parser.getRessourcePath();
	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "text/html";

	if (uri.empty()) // check if the ressource path is empty
	{
		std::string body = "<html><body><h1>400 Bad Request</h1></body></html>";
		std::string response = this->response(400, headers, body);
		return (1);
	}
	if (uri.find("..") != std::string::npos) // check if the ressource path contains ".."
	{
		std::string body = "<html><body><h1>403 Forbidden</h1></body></html>";
		std::string response = this->response(403, headers, body);
		return (1);
	}
	if (uri.find("~") != std::string::npos) // check if the ressource path contains "~"
	{
		std::string body = "<html><body><h1>403 Forbidden</h1></body></html>";
		std::string response = this->response(403, headers, body);
		return (1);
	}
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

std::string httpRequestHandler::response(int code, const std::map<std::string, std::string>& _headers, const std::string& body)
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
	response = "HTTP/1.1 " + std::to_string(code) + " " + responses[code] + "\r\n"; // status line
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) // headers
	{
		response += it->first + ": " + it->second + "\r\n";
	}
	response += "\r\n";
	response += body; // body
	return (response);
}
