#include <dirent.h>
#include <sstream>
#include <string>
#include <iostream>

std::string listDirectoryContents(const std::string& path) {
	std::ostringstream html;
	DIR* dir = opendir(path.c_str());
	struct dirent* entry;

	if (dir == nullptr) {
		return "<html><body><h1>Error: Unable to open directory</h1></body></html>";
	}

	html << "<html><head><title>Directory Listing</title></head><body>";
	html << "<h1>Directory Listing of " << path << "</h1><ul>";

	while ((entry = readdir(dir)) != nullptr) {
		if (entry->d_name[0] == '.')
			continue;
		html << "<li><a href=\"" << entry->d_name << "\">" << entry->d_name << "</a></li>";
	}
	html << "</ul></body></html>";

	closedir(dir);

	return html.str();
}

// int main () {
// 	std::string path = "./";
// 	std::string html = listDirectoryContents(path);
// 	std::cout << html << std::endl;
// 	return 0;
// }