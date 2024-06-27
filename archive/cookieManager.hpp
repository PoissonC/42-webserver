#ifndef COOKIE_MANAGER_HPP
#define COOKIE_MANAGER_HPP

#include <iostream>
#include <map>
#include <string>


class CookieManager
{
public:
	CookieManager();
	~CookieManager();

private:
	std::map<std::string, std::string> _cookies;
};

#endif
