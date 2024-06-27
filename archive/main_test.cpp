#include <iostream>
#include "httpRequestParser.hpp"

void testRequest(const std::string &request)
{
    httpRequestParser parser(request);
    parser.parse();

    std::cout << "Method: " << parser.getMethod() << std::endl;
    std::cout << "URI: " << parser.getUri() << std::endl;
	std::cout << "  Uri Components: " << std::endl;
	std::cout << "    Scheme: " << parser.getUriComponents().scheme << std::endl;
	std::cout << "    Host: " << parser.getUriComponents().host << std::endl;
	std::cout << "    Port: " << parser.getUriComponents().port << std::endl;
	std::cout << "    Path: " << parser.getUriComponents().path << std::endl;
	std::cout << "    Query: " << parser.getUriComponents().query << std::endl;
	std::cout << "    Fragment: " << parser.getUriComponents().fragment << std::endl;
    std::cout << "Version: " << parser.getVersion() << std::endl;
    std::cout << "Headers: " << std::endl;

    std::map<std::string, std::string> headers = parser.getHeaders();
    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
    {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }

    std::cout << "Body: " << parser.getBody() << std::endl;
    std::cout << "---------------------------" << std::endl;
}

int main()
{
    std::string request1 = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: curl/7.68.0\r\nAccept: */*\r\n\r\n";
    std::string request2 = "POST /submit-form HTTP/1.1\r\nHost: www.example.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nname=John&age=30&city=Paris";
	std::string request4 = "/index.html?name=John&age=30&city=Paris#section1 HTTP/1.1\r\nHost: www.example.com:8080\r\nUser-Agent: curl/7.68.0\r\nAccept: */*\r\n\\n";
	std::string request5 = "GET http://www.example.com:8080/index.html?name=John&age=30&city=Paris#section1 HTTP/1.1\r\nHost: www.example.com:8080\r\nUser-Agent: curl/7.68.0\r\nAccept: */*\r\n\r\n";
	// test with body
	std::string request6 = "POST /submit-form HTTP/1.1\r\nHost: www.example.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nname=John&age=30&city=Paris";
	std::string request7 = "POST /submit-form HTTP/1.1\r\nHost: www.example.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nname=John&age=30&city=Paris";
	std::string request8 = "POST http://www.example.com:8080/submit-form?name=John&age=30&city=Paris#section1 HTTP/1.1\r\n"
                          "Host: www.example.com:80\r\n"
                          "Content-Type: application/x-www-form-urlencoded\r\n"
                          "Content-Length: 27\r\n"
                          "\r\n"
                          "name=John&age=30&city=Paris";

    //std::cout << "Testing request 1:" << std::endl;
    //testRequest(request1);

    //std::cout << "Testing request 2:" << std::endl;
    //testRequest(request2);

	//std::cout << "Testing request 4:" << std::endl;
	//testRequest(request4);

	//std::cout << "Testing request 5:" << std::endl;
	//testRequest(request5);

	//std::cout << "Testing request 6:" << std::endl;
	//testRequest(request6);

	//std::cout << "Testing request 7:" << std::endl;
	//testRequest(request7);

	std::cout << "Testing request 8:" << std::endl;
	testRequest(request8);


    return 0;
}
