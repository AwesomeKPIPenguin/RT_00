/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 12:33:06 by vbespalk          #+#    #+#             */
/*   Updated: 2019/02/20 12:33:12 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int			ft_solve_sqr_(float a, float b, float c, float (*res)[2])
{
	float	d;

	d = (float)pow(b, 2) - 4.0f * a * c;
	if (d < 0)
		return (0);
	d = sqrtf(d);
	(*res)[0] = (-b + d) / (2.0f * a);
	(*res)[1] = (-b - d) / (2.0f * a);
	if ((*res)[0] > (*res)[1] || (*res)[0] < FLT_MIN)
		ft_swap_float(&(*res)[0], &(*res)[1]);
	return (1);
}

static t_vector	get_closer_pnt(t_vector origin, t_vector *coll)
{
	float t[2];

	if (ft_3_isnullpoint(coll[0]))
		return (coll[1]);
	if (ft_3_isnullpoint(coll[1]))
		return (coll[0]);
	t[0] = ft_3_vector_len(coll[0] - origin);
	t[1] = ft_3_vector_len(coll[1] - origin);
	return (t[0] < t[1] ? coll[0] : coll[1]);
}

/*
** ADD 1e-1 TO t TO AVOID MISSES BETW SIDES AND CAPS
*/

static t_vector	get_cides_coll(t_vector origin, t_vector direct, float *res, t_cone *con)
{
	t_vector	coll[2];
	float 		m[2];

	coll[0] = origin + ft_3_vector_scale(direct, res[0]);
	m[0] = ft_3_vector_dot(con->v, coll[0] - con->o);
	coll[1] = origin + ft_3_vector_scale(direct, res[1]);
	m[1] = ft_3_vector_dot(con->v, coll[1] - con->o);
	if (con->maxh == FLT_MAX && con->minh == FLT_MIN)
	{
		if (res[0] >= FLT_MIN)
			return (coll[0]);
		else if (res[1] >= FLT_MIN)
			return (coll[1]);
		return (ft_3_nullpointnew());
	}
	if (res[0] + 1e-1 >= FLT_MIN && IN_RANGE(m[0], con->minh, con->maxh))
		return (coll[0]);
	if (res[1] + 1e-1 >= FLT_MIN && IN_RANGE(m[1], con->minh, con->maxh))
		return (coll[1]);
	return (ft_3_nullpointnew());
}

static t_vector	get_caps_coll(t_vector origin, t_vector direct, t_cone *con, float dot)
{
	float		t[2];
	t_vector	coll[2];
	t_vector 	o[2];

	o[0] = con->o + ft_3_vector_scale(con->v, con->minh);
	o[1] = con->o + ft_3_vector_scale(con->v, con->maxh);
	t[0] = (ft_3_vector_dot(o[0], con->v) - ft_3_vector_dot(origin, con->v)) * dot;
	t[1] = (ft_3_vector_dot(o[1], con->v) - ft_3_vector_dot(origin, con->v)) * dot;
	if (t[0] < 1e-6 && t[1] < 1e-6)
		return (ft_3_nullpointnew());
	coll[0] = ft_3_nullpointnew();
	coll[1] = ft_3_nullpointnew();
	if (t[0] >= FLT_MIN && (con->minh != FLT_MIN || con->minh != 0.0f))
	{
		coll[0] = origin + ft_3_vector_scale(direct, t[0]);
		if (ft_3_vector_dot(coll[0] - o[0], coll[0] - o[0]) > (con->r[0]) * (con->r[0]))
			coll[0] = ft_3_nullpointnew();
	}
	if (t[1] >= FLT_MIN  && con->maxh != FLT_MAX)
	{
		coll[1] = origin + ft_3_vector_scale(direct, t[1]);
		if (ft_3_vector_dot(coll[1] - o[1], coll[1]	- o[1]) > (con->r[1]) * (con->r[1]))
			coll[1] = ft_3_nullpointnew();
	}
	if (t[0] < FLT_MIN)
		return (coll[1]);
	else if (t[1] < FLT_MIN)
		return (coll[0]);
	return ((t[0] < t[1] && !ft_3_isnullpoint(coll[0])) ? coll[0] : coll[1]);
}

int					ft_is_reachable_cone(void *fig, t_vector origin, t_vector direct)
{
	return (1);
}

t_vector			ft_collide_cone(t_list **objs, t_object *obj, t_coll *coll, t_vector od[2])
{
	t_cone	*con;
	t_vector	coll_pnt[2];
	float		res[2];
	float 		dot_dv;
	float 		dot_vx;

	con = (t_cone *)(obj->fig);
	dot_dv = ft_3_vector_dot(od[1], con->v);
	dot_vx = ft_3_vector_dot(od[0] - con->o, con->v);
	if (!ft_solve_sqr_(ft_3_vector_dot(od[1], od[1]) - (1 + con->tan * con->tan) * dot_dv * dot_dv,
		2.0f * (ft_3_vector_dot(od[0] - con->o, od[1]) - (1 + con->tan * con->tan) * dot_dv * dot_vx),
		ft_3_vector_dot(od[0] - con->o, od[0] - con->o) - (1 + con->tan * con->tan) * dot_vx * dot_vx,
		&res))
		return (fabsf(dot_dv) < 1e-6 ? ft_3_nullpointnew() :
			get_caps_coll(od[0], od[1], con, 1.0f / dot_dv));
	coll_pnt[0] = get_cides_coll(od[0], od[1], res, con);
	if ((con->maxh == FLT_MAX && con->minh == FLT_MIN) || fabsf(dot_dv) < 1e-6)
		return (coll_pnt[0]);
	coll_pnt[1] = get_caps_coll(od[0], od[1], con, 1.0f / dot_dv);
	return (get_closer_pnt(od[0], coll_pnt));
}

//int			ft_is_inside_cone(void *fig, t_vector point)
//{
//	t_cone		*cone;
//	float 		hei;
//
//	cone = (t_cone *)fig;
//	hei = ft_3_vector_dot(cone->v, point - cone->o);
//	if (!IN_RANGE(hei, cone->minh, cone->maxh) && cone->maxh != FLT_MAX)
//		return (0);
//	return ((ft_3_vector_dot(point - cone->o, point - cone->o) <
//			 hei * hei + hei * cone->tan * hei * cone->tan) ? 1 : 0);
//}

int			ft_is_inside_cone(void *fig, t_vector point)
{
	t_cone		*cone;
	float 		hei;
	float		rad;

	cone = (t_cone *)fig;
	hei = ft_3_vector_dot(cone->v, point - cone->o);
	if (!IN_RANGE(hei, cone->minh, cone->maxh) && cone->maxh != FLT_MAX)
		return (0);
	rad = hei * cone->tan;
	if ((ft_3_vector_dot(point - cone->o, point - cone->o) < hei * hei + rad * rad))
	{
//		printf("INSIDE\n");
		return (1);
	}
//	printf("OUTSIDE\n");
	return (0);
}

//
// CHECK RETURN |
//              V

t_vector	ft_get_norm_cone(void *fig, t_vector coll)
{
	t_cone 		*con;

	con = (t_cone *)fig;
	con->h = ft_3_vector_dot(con->v, coll - con->o);
	if (con->maxh != FLT_MAX && con->h >= con->maxh - 1e-2)
		return (con->v);
	if (con->minh != FLT_MIN && con->h <= con->minh + 1e-2)
		return (-con->v);
	return (ft_3_tounitvector(
		coll - ((t_cone *)fig)->o - ft_3_vector_scale(
			con->v, con->h * (1.0f + con->tan * con->tan))));
}