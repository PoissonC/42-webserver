#include "httpRequestParser.hpp"
#include "httpResponse.hpp"

static void checkConfigRequest(httpRequestParser &parser, httpResponse &response, Settings settings)
{
	bool requestValid = false;
	ServerConfig serverValid;
	for (const ServerConfig& serverConfig : settings.getServerConfig())
	{
		for (const std::string & serverName : serverConfig.getServerNames())
		{
			if (serverName == parser.getUriComponents().host)
			{
				requestValid = true;
				serverValid = serverConfig;
				break;
			}
		}
	}
	if (!requestValid)
	{
		response.setStatusCode(404);
		return;
	}

	std::string uri = parser.getUriComponents().path;
	bool locationValid = false;
	// find root in location config
	if (serverValid.getLocationConfig().find("root") != serverValid.getLocationConfig().end())
	{
		if (serverValid.getLocationConfig()["root"] = parser.getUriComponents().path)
			locationValid = true;
	}
	if (serverValid.getLocationConfig().find("cgi_path") != serverValid.getLocationConfig().end())
	{
		if (serverValid.getLocationConfig()["cgi_path"] = parser.getUriComponents().path)
			locationValid = true;
	}

	std::string method = parser.getMethod();
	bool methodValid = false;
	std::string methodAllowed = serverValid.getLocationConfig()["allow_methods"];
	// split methodAllowed by ' ' and check if method is in the list
	if (methodAllowed.find(method) != std::string::npos)
		methodValid = true;

	if (!locationValid || !methodValid)
	{
		response.setStatusCode(404);
		return;
	}

	response.setStatusCode(200);
}

static void setMapCGI(httpRequestParser &parser, Settings settings, std::map<std::string, std::string> &mapCGI, unsigned char * client_ip)
{
	// find the server name in the settings
	// if not found, return 404 Not Found
	// if found, verify if the request is valid
		// if not valid, return 404 Not Found
		// if valid, return 200 OK
}

static void	executeRequest(httpRequestParser &parser, httpResponse &response, std::map<std::string, std::string> mapCGI)
{
	if (parser.getUriComponents().path.find(".php") || parser.getUriComponents().path.find(".py"))
	{
		// execute the script and return the output
	}
	else
	{
		if (parser.getMethod() == "GET")
		{
			std::string body = "<html><head><title>200 OK</title></head><body><h1>End-Point of the method GET</h1></body></html>";
			response.setStatusCode(200);
			response.setBody(body);
			response.setHeaders({{"Content-Length", std::to_string(body.size())}, {"Content-Type", "text/html"}});
		}
		else if (parser.getMethod() == "POST")
		{
			std::string body = "<html><head><title>200 OK</title></head><body><h1>End-Point of the method POST</h1></body></html>";
			response.setStatusCode(200);
			response.setBody(body);
			response.setHeaders({{"Content-Length", std::to_string(body.size())}, {"Content-Type", "text/html"}});
		}
		else if (parser.getMethod() == "DELETE")
		{
			std::string body = "<html><head><title>200 OK</title></head><body><h1>End-Point of the method DELETE</h1></body></html>";
			response.setStatusCode(200);
			response.setBody(body);
			response.setHeaders({{"Content-Length", std::to_string(body.size())}, {"Content-Type", "text/html"}});
		}
	}
}

std::string processRequest(std::string request, Settings settings, unsigned char * client_ip)
{
	// parse the request
	httpRequestParser parser(request);
	httpResponse response;
	parser.parse();


	// verify if the request is valid
	int requestCode = parser.checkRequest();
	if (requestCode != 200)
	{
		response.setStatusCode(requestCode);
		return response.getResponse();
	}

	// find the server name in the settings
	// if not found, return 404 Not Found
	checkConfigRequest(parser, response, server);
	if (response.getStatusCode() != 200)
		return response.getResponse();


	// setmap for CGI
	std::map<std::string, std::string> mapCGI;
	setMapCGI(parser, server, mapCGI, client_ip);


	// process the request
	executeRequest(parser, response, mapCGI);


	// return the response
	return response.getResponse();

}

	/*
	How to access data in the vector:
		- parse every server name to find the good one with host.
		- then in the index I go to the map of serverconfig and compare the kay words with path of URI
		- then I go the the value of the map and I can access to the data of the serverconfig

		inside location config have
				- allow methods.
				- the path of the location (root variable)
				- cgi_path
				- allow methodes
	*/

	// parse the request and verify if it is valid (request line, headers, body)
		// if not valid, return 400 Bad Request
		// verification to be done in the parser
			// request line (method, uri, version)
			// headers (Host, Content-Length, Content-Type, etc.)
			// body (if any and if required and the length is correct)

	// use the config of server to verify if the request is valid
		// if not valid, return 404 Not Found
		// verification to be done in the handler
			// method (GET, POST, PUT, DELETE, etc.)
			// uri (path, query, fragment)
			// version (HTTP/1.1)
			// headers (Host, Content-Length, Content-Type, etc.)
			// body (if any and if required)

	// process the request (GET, POST or DELETE) in static content or dynamic content (CGI)
		// GET: return the content of the file
		// POST: create a new file with the content
		// DELETE: delete the file
		// CGI: execute the script and return the output

	// if valid, return the response
		// response line (version, status code, status message)
		// headers (Content-Length, Content-Type, etc.)
		// body (if any)
