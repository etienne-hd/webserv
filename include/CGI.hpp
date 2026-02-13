/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 18:53:27 by ehode             #+#    #+#             */
/*   Updated: 2026/02/13 21:28:24 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <string>

class CGI {
	public:
		bool		is_running;
		int			fd;
		int			pid;
		std::string	output;
		long		start_at;
		bool		eof;

		CGI(void);
};

#endif