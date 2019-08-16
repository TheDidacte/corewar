/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stream.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 16:30:32 by mbenjell          #+#    #+#             */
/*   Updated: 2019/05/08 19:58:43 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int		init_stream(t_stream *s, int fd)
{
	memset(s, 0, sizeof(t_stream));
	s->fd = fd;
	s->nread = read(s->fd, s->buff, BUFFER_SIZE);
	if (s->nread == ERROR || s->nread == 0)
		return (ERROR);
	return (SUCCESS);
}

int		flusher(t_stream *s)
{
	if (s->i >= s->nread)
	{
		s->i = 0;
		memset(s->buff, 0, BUFFER_SIZE);
		s->nread = read(s->fd, s->buff, BUFFER_SIZE);
		if (s->nread == ERROR || s->nread == 0)
			return (0);
	}
	return (1);
}

int		ft_getc(t_stream *s)
{
	if (s->nread > 0 && flusher(s))
		return (s->buff[s->i++]);
	else
		return (0);
}

int		ft_getchar(t_stream *s)
{
	if (s->nread > 0 && flusher(s))
		return (s->buff[s->i]);
	else
		return (0);
}

int		skip_space(t_stream *s)
{
	int	c;

	c = ft_getchar(s);
	if (!strchr(SPACE, c))
		return (SUCCESS);
	while (memchr(SPACE, c, sizeof(SPACE) - 1))
		c = ft_getc(s);
	if (s->i > 0 && !strchr(SPACE, c))
		s->i--;
	return (SUCCESS);
}
