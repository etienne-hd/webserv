/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:29:17 by ehode             #+#    #+#             */
/*   Updated: 2026/02/05 00:15:39 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

int	main(void) {
	logger.debug("Hello, World");
	logger.info("Hello, World");
	logger.warning("Hello, World");
	logger.error("Hello, World");
	logger.critical("Hello, World");
	return (0);
}