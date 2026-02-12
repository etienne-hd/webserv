/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 02:45:56 by ehode             #+#    #+#             */
/*   Updated: 2026/02/12 21:42:34 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <sys/socket.h>

Request::Request(void) {
	_segment_count = 0;
	_segment_timeout = -1;
	_pre_response_status_code = -1;
}