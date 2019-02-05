/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 17:26:37 by vbespalk          #+#    #+#             */
/*   Updated: 2018/08/02 17:26:39 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	translate(Uint32 key, t_vector *pos, int cam)
{
	float	i;

	i = 1.5;
	if (key == SDLK_w)
		(*pos)[1] += i;
	if (key == SDLK_d)
		(*pos)[0] += i;
	if (key == SDLK_a)
		(*pos)[0] -= i;
	if (key == SDLK_s)
		(*pos)[1] -= i;
	i = cam ? -1.5 : 1.5;
	if (key == SDLK_LSHIFT)
		(*pos)[2] += i;
	if (key == SDLK_LCTRL)
		(*pos)[2] -= i;
}

void	rotate(Uint32 key, t_vector *dir)
{
	float		k;
	t_matrix	m;

	k = DEG_TO_RAD(1);
	if (key == SDLK_UP)
		x_rotate(&m, k);
	if (key == SDLK_DOWN)
		x_rotate(&m, -k);
	if (key == SDLK_LEFT)
		y_rotate(&m, k);
	if (key == SDLK_RIGHT)
		y_rotate(&m, -k);
	if (key == SDLK_PAGEUP)
		z_rotate(&m, k);
	if (key == SDLK_PAGEDOWN)
		z_rotate(&m, -k);
	dir_multipl(&m, dir);
	normilize_vec(dir);
}

void	cam_rotate(Uint32 key, t_vector *angles)
{
	float k;

	k = DEG_TO_RAD(1);
	if (key == SDLK_UP)
		(*angles)[0] += k;
	if (key == SDLK_DOWN)
		(*angles)[0] -= k;
	if (key == SDLK_LEFT)
		(*angles)[1] -= k;
	if (key == SDLK_RIGHT)
		(*angles)[1] += k;
	if (key == SDLK_PAGEUP)
		(*angles)[2] -= k;
	if (key == SDLK_PAGEDOWN)
		(*angles)[2] += k;
}

void	scale(Uint32 key, float *siz, int cam)
{
	float	i;

	i = cam ? 0.1 : -1.0f;
	if (key == SDLK_z)
		*siz = ((*siz - i) < DEG_TO_RAD(0) && cam) ? DEG_TO_RAD(0) : *siz - i;
	if (key == SDLK_x)
	{
		if (((*siz + i) > 0 && !cam) || ((*siz + i) < DEG_TO_RAD(160) && cam))
			*siz += i;
		else
		{
			if (cam)
				*siz = DEG_TO_RAD(160);
			else
				*siz = 0;
		}
	}
}

void	reset(t_env *e)
{
	t_object	*objs;
	t_scene	*scene;

	scene = e->scn;
	scene->cam->cam_transl = scene->cam->origin;
	scene->cam->angles = (t_vector){0.0f, 0.0f, 0.0f};
	scene->cam->fov = FOV;
	e->selected = NULL;
	objs = e->obj;
	while (objs)
	{
		objs->translate = objs->pos;
		objs->rotate = objs->rot;
		objs->scale = objs->size;
		objs = objs->next;
	}
}

void	delete_obj(t_object **obj_lst, int id)
{
	t_object *temp;
	t_object *prev;

	if (!obj_lst || !(*obj_lst))
		return ;
	if ((temp = *obj_lst) && temp->id == id)
	{
		*obj_lst = temp->next;
		ft_memdel((void **)&temp);
		return ;
	}
	while (temp)
	{
		prev = temp;
		temp = temp->next;
		if (temp->id == id)
		{
			prev->next = temp->next;
			ft_memdel((void **)&temp);
			return ;
		}
	}

}