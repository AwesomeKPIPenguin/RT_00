/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: domelche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/23 13:25:05 by domelche          #+#    #+#             */
/*   Updated: 2018/08/23 13:25:05 by domelche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
**	x/y[0] - start point, iterator;
**	x/y[1] - end point;
*/

void	*ft_section_handle(void *arg)
{
	t_thrarg	*thrarg;
	int			x[2];
	int			y[2];
	int			y_iter;

	thrarg = (t_thrarg *)arg;
	x[0] = (thrarg->i % 4) * (SCR_WID / (THREADS / 2)) - 1;
	y[0] = (thrarg->i / 4) * (SCR_WID / 2) - 1;
	x[1] = x[0] + SCR_WID / (THREADS / 2) + 1;
	y[1] = y[0] + SCR_HEI / 2 + 1;
	while (++x[0] < x[1])
	{
		y_iter = y[0];
		while (++y_iter < y[1])
//			ft_pixel_put_image(parg->e, x[0], y_iter,
//				ft_trace_ray(parg, x[0], y_iter).val);
	}
	return (NULL);
}

void	ft_render(t_env *e)
{
	pthread_t	threads[THREADS];
	t_thrarg	thrargs[THREADS];
	int			i;

	ft_get_start_refr(e->scn);
	i = -1;
	while (++i < THREADS)
	{
		thrargs[i].i = i;
		thrargs[i].e = e;
	}
	i = -1;
	while (++i < THREADS)
		pthread_create(&threads[i], NULL,
			ft_section_handle, (void *)&thrargs[i]);
	i = -1;
	while (++i < THREADS)
		pthread_join(threads[i], NULL);
//	mlx_put_image_to_window(e->mlx, e->win, e->img->ptr, 0, 0);
}
