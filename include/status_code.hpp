/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_code.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 21:30:17 by ehode             #+#    #+#             */
/*   Updated: 2026/02/12 21:38:35 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUS_CODE_HPP
# define STATUS_CODE_HPP

# define RESPONSE_OK							200
# define RESPONSE_NO_CONTENT					204
# define RESPONSE_CREATED						201

# define RESPONSE_MOVED_PERMANENTLY				301

# define RESPONSE_BAD_REQUEST					400
# define RESPONSE_UNAUTHORIZED					401
# define RESPONSE_FORBIDDEN						403
# define RESPONSE_NOT_FOUND						404
# define RESPONSE_REQUEST_TIMEOUT				408
# define RESPONSE_CONTENT_TOO_LARGE				413
# define RESPONSE_TOO_MANY_REQUESTS				429

# define RESPONSE_INTERNAL_SERVER_ERROR			500
# define RESPONSE_NOT_IMPLEMENTED				501
# define RESPONSE_BAD_GATEWAY					502
# define RESPONSE_SERVICE_UNAVAILABLE			503
# define RESPONSE_HTTP_VERSION_NOT_SUPPORTED	505

#endif