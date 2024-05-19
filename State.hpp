/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   State.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu <yu@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:14:22 by yu                #+#    #+#             */
/*   Updated: 2024/05/16 15:40:55 by yu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

typedef struct s_state {
	// If the size of buffer is too large, we can consider to put this into a external file
	std::string			buffer;
	// std::ifstream		file;
} t_state;

void	fill_write_buffer(t_state *state, std::string const &response);