/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu <yu@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:04:57 by ychen2            #+#    #+#             */
/*   Updated: 2024/05/10 12:52:25 by yu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
#include <iostream>


int main () {
	try{
		std::vector<Settings> settings;
		settings.push_back(Settings(8080)); 
		settings.push_back(Settings(12345)); 
		Server server(settings);
		
	}
	catch (std::exception & e){
		if (errno != 0)
			perror(e.what());
		else
			std::cerr << e.what() << std::endl;
	}

	return 0;
}