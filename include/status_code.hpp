/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_code.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 21:30:17 by ehode             #+#    #+#             */
/*   Updated: 2026/02/15 18:51:07 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUS_CODE_HPP
# define STATUS_CODE_HPP

# define RESPONSE_CONTINUE							100
# define RESPONSE_SWITCHING_PROTOCOLS				101
# define RESPONSE_PROCESSING						102
# define RESPONSE_EARLY_HINTS						103

# define RESPONSE_OK								200
# define RESPONSE_CREATED							201
# define RESPONSE_ACCEPT							202
# define RESPONSE_NON_AUTHORITATIVE_INFORMATION		203
# define RESPONSE_NO_CONTENT						204
# define RESPONSE_RESET_CONTENT						205
# define RESPONSE_PARTIAL_CONTENT					206
# define RESPONSE_MULTI_STATUS						207
# define RESPONSE_ALREADY_REPORTED					208
# define RESPONSE_IM_USED							226

# define RESPONSE_MULTIPLE_CHOICES					300
# define RESPONSE_MOVED_PERMANENTLY					301
# define RESPONSE_FOUND								302
# define RESPONSE_SEE_OTHER							303
# define RESPONSE_NOT_MODIFIED						304
# define RESPONSE_USE_PROXY							305
# define RESPONSE_UNUSED							306
# define RESPONSE_TEMPORARY_REDIRECT				307
# define RESPONSE_PERMANENT_REDIRECT				308

# define RESPONSE_BAD_REQUEST						400
# define RESPONSE_UNAUTHORIZED						401
# define RESPONSE_PAYMENT_REQUIRED					402
# define RESPONSE_FORBIDDEN							403
# define RESPONSE_NOT_FOUND							404
# define RESPONSE_METHOD_NOT_ALLOWED				405
# define RESPONSE_NOT_ACCEPTABLE					406
# define RESPONSE_PROXY_AUTHENTICATION_REQUIRED		407
# define RESPONSE_REQUEST_TIMEOUT					408
# define RESPONSE_CONFLICT							409
# define RESPONSE_GONE								410
# define RESPONSE_LENGTH_REQUIRED					411
# define RESPONSE_PRECONDITION_FAILED				412
# define RESPONSE_CONTENT_TOO_LARGE					413
# define RESPONSE_URI_TOO_LONG						414
# define RESPONSE_UNSUPPORTED_MEDIA_TYPE			415
# define RESPONSE_RANGE_NOT_SATISFIABLE				416
# define RESPONSE_EXPECTATION_FAILED				417
# define RESPONSE_IM_A_TEAPOT						418
# define RESPONSE_MISDIRECTED_REQUEST				421
# define RESPONSE_UNPROCESSABLE_CONTENT				422
# define RESPONSE_LOCKED							423
# define RESPONSE_FAILED_DEPENDENCY					424
# define RESPONSE_TOO_EARLY							425
# define RESPONSE_UPGRADE_REQUIRED					426
# define RESPONSE_PRECONDITION_REQUIRED				428
# define RESPONSE_TOO_MANY_REQUESTS					429
# define RESPONSE_REQUEST_HEADER_FILEDS_TOO_LARGE	431
# define RESPONSE_UNAVAILABLE_FOR_LEGAL_REASONS		451

# define RESPONSE_INTERNAL_SERVER_ERROR				500
# define RESPONSE_NOT_IMPLEMENTED					501
# define RESPONSE_BAD_GATEWAY						502
# define RESPONSE_SERVICE_UNAVAILABLE				503
# define RESPONSE_GATEWAY_TIMEOUT					504
# define RESPONSE_HTTP_VERSION_NOT_SUPPORTED		505
# define RESPONSE_VARIANT_ALSO_NEGOTIATES			506
# define RESPONSE_INSUFFICIENT_STORAGE				507
# define RESPONSE_LOOP_DETECTED						508
# define RESPONSE_NOT_EXTENDED						510
# define RESPONSE_NETWORK_AUTHENTICATION_REQUIRED	511


#endif