#include <fstream>
#include <iostream>
#include "Settings.hpp"

std::string readFile(const std::string &filename) {
    std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + filename);
	}
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    std::string filename = "config.txt";
    std::vector<std::string> tokens = tokenize(readFile(filename));
	std::cout << "Start parsing config" << std::endl;
    std::vector<ServerConfig> servers;
    parseConfig(tokens, servers);
	std::cout << "Finish parsing config" << std::endl;
    // Print parsed configuration for verification
    for (std::vector<ServerConfig>::iterator it = servers.begin(); it != servers.end(); ++it) {
        const ServerConfig &server = *it;
        std::cout << "Server: ";
		std::vector<std::string> server_names = server.getServerNames();
		for (std::vector<std::string>::const_iterator s_it = server_names.begin(); s_it != server_names.end(); ++s_it) {
			std::cout << *s_it << " ";
		}
		std::cout << "\n";
        std::cout << "Listen: " << server.getListen() << "\n";
        std::cout << "Root: " << server.getRoot() << "\n";
		std::cout << "Error Pages:\n";
		std::map<int, std::string> error_pages = server.getErrorPages();
		for (std::map<int, std::string>::const_iterator e_it = error_pages.begin(); e_it != error_pages.end(); ++e_it) {
			std::cout << "  " << e_it->first << ": " << e_it->second << "\n";
		}
        std::cout << "\nLocations:\n";
		std::map<std::string, LocationConfig> locations = server.getLocations();
        for (std::map<std::string, LocationConfig>::const_iterator l_it = locations.begin(); l_it != locations.end(); ++l_it) {
            const LocationConfig &location = l_it->second;
            std::cout << "  Path: " << l_it->first << "\n";
            std::cout << "  Root: " << location.getRoot() << "\n";
			std::cout << "  Index: ";
			std::vector<std::string> index = location.getIndex();
			for (std::vector<std::string>::const_iterator i_it = index.begin(); i_it != index.end(); ++i_it) {
				std::cout << *i_it << " ";
			}
			std::cout << "\n";
            std::cout << "  CGI Pass: " << location.getCgiPass() << "\n";
			std::cout << "  Autoindex: " << location.getAutoindex() << "\n";
			std::cout << "  Redirect: " << location.getRedir().first << " " << location.getRedir().second << "\n";
			std::cout << "  Client Upload: " << location.getClientUpload() << "\n";
            std::cout << "  Client Body Buffer Size: " << location.getClientBodyBufferSize() << "\n";
            std::cout << "  Allow Methods: ";
            if (location.getAllowMethods(GET))
				std::cout << "GET ";
			if (location.getAllowMethods(POST))
				std::cout << "POST ";	
			if (location.getAllowMethods(DELETE))
				std::cout << "DELETE ";
            std::cout << "\n";
        }
    }
    return 0;
}