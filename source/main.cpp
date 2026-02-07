/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:29:17 by ehode             #+#    #+#             */
/*   Updated: 2026/02/05 21:45:53 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

int	main(int argc, char **argv) {
	if (argc != 2)
	{
		logger << ERROR << "Wrong usage: " << argv[0] << " <*.toml>" << ENDL;
		return (1);
	}
	return (0);
}
