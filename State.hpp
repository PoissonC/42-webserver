/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   State.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu <yu@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:14:22 by yu                #+#    #+#             */
/*   Updated: 2024/05/10 14:30:17 by yu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <queue>

typedef struct s_state {
	enum {
		READ,
		WRITE,
		END,
	} state;
	std::string			read_buffer;
	std::queue<char>	write_buffer;
} t_state;

void	fill_write_buffer(t_state *state, std::string const &response);