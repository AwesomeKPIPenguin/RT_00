/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paraboloid.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 18:12:39 by vbespalk          #+#    #+#             */
/*   Updated: 2019/02/19 18:12:42 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_prbld		*ft_prbldnew(void)
{
	t_prbld	*par;

	par = ft_smemalloc(sizeof(t_prbld), "ft_prbldnew");
	par->o = (t_vector){0.0f, 0.0f, 0.0f};
	par->d = (t_vector){0.0f, 1.0f, 0.0f};
	par->sk = 10.f;
	par->maxh = 200.0f;
	return (par);
}

char		*ft_parse_prbld(char *attr, t_scene *scn, unsigned int id)
{
	t_object	*obj;
	t_prbld		*par;

	// obj = ft_parse_object(attr);
	obj = ft_objectnew(id);
	obj->id = id;
	obj->ft_collide = ft_collide_prbld;
	obj->ft_is_reachable = ft_is_reachable_prbld;
	obj->ft_is_inside = ft_is_inside_prbld;
	obj->ft_get_norm = ft_get_norm_prbld;
	obj->ft_translate = ft_translate_prbld;
	obj->ft_rotate = ft_rotate_prbld;
	obj->ft_scale = ft_scale_prbld;
	obj->spclr = 0.8f;
	obj->s_blur = 1.0f;
	par = ft_prbldnew();
	par->n = ft_3_nullpointnew();
	par->d = ft_3_tounitvector(par->d);
	// attr = ft_get_curve(attr, '{');
	// ft_get_attr_in_scope(attr, "origin:", (void *)(&(par->origin_ini)), PNT);
	// ft_get_attr_in_scope(attr, "radius:", (void *)(&(par->radius_ini)), FLT);
	obj->fig = par;
	ft_lstpush(&(scn->objs), ft_nodenew((void *)obj, sizeof(obj)));
	// return (ft_get_curve(attr, '}'));
	printf("INITIALISED PARABOLOID\n");
	return (NULL);
}

void		ft_translate_prbld(Uint32 key, void *fig, t_vector *transl)
{
	t_prbld	*par;

	par = (t_prbld *)fig;
	*transl = (t_vector){0,0,0};
	if (!fig)
		return ;
	if (key == SDLK_d)
		(*transl)[2] += TRANS_F;
	if (key == SDLK_a)
		(*transl)[2] -= TRANS_F;
	if (key == SDLK_w)
		(*transl)[1] += TRANS_F;
	if (key == SDLK_s)
		(*transl)[1] -= TRANS_F;
	if (key == SDLK_e)
		(*transl)[0] += TRANS_F;
	if (key == SDLK_q)
		(*transl)[0] -= TRANS_F;
	par->o = par->o + *(transl);
}

void		ft_rotate_prbld(Uint32 key, void *fig, t_vector *rot)
{
	t_prbld *par;

	par = (t_prbld *)fig;
	*rot = (t_vector){0,0,0};
	if (!fig)
		return ;
	if (key == SDLK_DOWN)
		(*rot)[2] += ROTAT_F;
	else if (key == SDLK_UP)
		(*rot)[2] -= ROTAT_F;
	else if (key == SDLK_LEFT)
		(*rot)[1] -= ROTAT_F;
	else if (key == SDLK_RIGHT)
		(*rot)[1] += ROTAT_F;
	else if (key == SDLK_PAGEDOWN)
		(*rot)[0] += ROTAT_F;
	else if (key == SDLK_PAGEUP)
		(*rot)[0] -= ROTAT_F;
	par->d = ft_3_vector_rotate(par->d, (*rot)[0], (*rot)[1], (*rot)[2]);

}

void		ft_scale_prbld(Uint32 key, void *fig, float *scale)
{
	t_prbld *par;

	par = (t_prbld *)fig;
	*scale = 1;
	if (!fig)
		return ;
	if (key == SDLK_z)
		*scale += SCALE_F;
	else if (key == SDLK_x && *scale >= 0.0f)
		*scale -= SCALE_F;
	else
		*scale = 0;
	par->sk = par->sk * *scale;
	par->maxh = par->maxh * *scale;
}
