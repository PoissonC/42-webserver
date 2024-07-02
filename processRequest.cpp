#include "httpRequestParser.hpp"
#include "httpResponse.hpp"

static int checkConfigRequest(httpRequestParser &parser, std::vector<ServerConfig> settings)
{
	bool requestValid = false;
	ServerConfig serverValid;
	for (std::vector<ServerConfig>::iterator serverConfig = settings.begin(); serverConfig != settings.end(); serverConfig++)
	{
		std::vector<std::string> serverName = serverConfig->getServerName();
		for (std::vector<std::string>::iterator it = serverName.begin(); serverConfig != settings.end(); serverConfig++)
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
		return (404);

	std::string uri = parser.getUriComponents().path;
	bool locationValid = false;
	std::string tmpPath = parser.getUriComponents().path;
	std::map<std::string, LocationConfig> locationConfig = serverValid.getLocationConfig();
	std::map<std::string, LocationConfig>::iterator tarLocation;
	
	while (1)
	{
		tarLocation = locationConfig.find(tmpPath);
		if (tarLocation != locationConfig.end())
		{
			locationValid = true;
			break;
		}
		else
		{
			tmpPath = tmpPath.substr(0, tmpPath.find_last_of('/'));
			if (tmpPath.empty())
				break;
		}
	}

	bool methodValid = false;
	if(tarLocation->getAllowMethods(parser.getMethod()))
		methodValid = true;

	if (!locationValid || !methodValid)
		return (404);

	setMap(parser, serverValid, mapCGI, client_ip);
	return (200);
}

static void setMap(httpRequestParser &parser, ServerConfig settings, std::map<std::string, std::string> &mapCGI, unsigned char * client_ip)
{
		std::string pathTranslated = settings.getLocation.getRoot() + parser.getUriComponents().path;

		mapCGI["METHOD"] = parser.getMethod();
		mapCGI["CONTENT_TYPE"] = parser.getHeaders()["Content-Type"];
		mapCGI["CONTENT_LENGTH"] = parser.getHeaders()["Content-Length"];
		mapCGI["GATEWAY_INTERFACE"] = "CGI/1.1";
		mapCGI["PATH_INFO"] = parser.getUriComponents().path;
		mapCGI["PATH_TRANSLATED"] = pathTranslated;
		mapCGI["QUERY_STRING"] = parser.getUriComponents().query;
		mapCGI["REMOTE_ADDR"] = client_ip;
		mapCGI["REMOTE_IDENT"] = "";
		mapCGI["REMOTE_USER"] = "";
		mapCGI["REQUEST_METHOD"] = parser.getMethod();
		mapCGI["REQUEST_URI"] = parser.getUri();
		mapCGI["SCRIPT_NAME"] = parser.getUriComponents().path;
		mapCGI["SERVER_NAME"] = parser.getUriComponents().host;
		mapCGI["SERVER_PROTOCOL"] = parser.getVersion();
		mapCGI["SERVER_PORT"] = parser.getUriComponents().port;
		mapCGI["SERVER_SOFTWARE"] = "Webserv";
}

static void	executeRequest(httpRequestParser &parser, httpResponse &response, std::map<std::string, std::string> mapCGI)
{
	if (parser.getUriComponents().path.find(".php") || parser.getUriComponents().path.find(".py"))
	{
		// execute the script and return the output
	}
	else
	{
		if (parser.getMethod() == GET)
		{
			// creat acces to static file
			std::string body = "<html><head><title>200 OK</title></head><body><h1>End-Point of the method GET</h1></body></html>";
			response.setStatusCode(200);
			response.setBody(body);
			response.setHeaders({{"Content-Length", std::to_string(body.size())}, {"Content-Type", "text/html"}});
		}
		else if (parser.getMethod() == POST)
		{
			std::string body = "<html><head><title>200 OK</title></head><body><h1>End-Point of the method POST</h1></body></html>";
			response.setStatusCode(200);
			response.setBody(body);
			response.setHeaders({{"Content-Length", std::to_string(body.size())}, {"Content-Type", "text/html"}});
		}
		else if (parser.getMethod() == DELETE)
		{
			std::string body = "<html><head><title>200 OK</title></head><body><h1>End-Point of the method DELETE</h1></body></html>";
			response.setStatusCode(200);
			response.setBody(body);
			response.setHeaders({{"Content-Length", std::to_string(body.size())}, {"Content-Type", "text/html"}});
		}
	}
}

std::string processRequest(std::string request, std::vector<ServerConfig> settings, unsigned char * client_ip)
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
	std::map<std::string, std::string> mapCGI;
	requestCode =  checkConfigRequest(parser, settings, mapCGI, client_ip);
	if (response.getStatusCode() != 200)
		return response.getResponse();


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
