/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: domelche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/23 13:21:02 by domelche          #+#    #+#             */
/*   Updated: 2018/08/23 13:25:52 by domelche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//static void			ft_update(
//		t_coll *coll,
//		t_object **res_o,
//		float (*dist)[2],
//		t_vector (*pnt)[3])
//{
//	(*pnt)[0] = coll->ucoll_pnt;
//	(*pnt)[1] = coll->coll_pnt;
//	(*pnt)[2] = coll->norm;
//	(*dist)[0] = (*dist)[1];
//	*res_o = coll->o;
//}
//
//static void			ft_init(t_coll *coll, t_object *res_o, t_vector pnt[2])
//{
//	coll->o = res_o;
//	coll->ucoll_pnt = pnt[0];
//	coll->coll_pnt = pnt[1];
//	coll->norm = pnt[2];
//}

static void			ft_init_collision(t_coll *coll, t_list **objs, t_vector *od)
{
	t_list		*node;
//	t_vector	pnt[3];
	t_coll		tmp_coll;
	float		dist[2];
	t_object	*o;

	node = *objs;
	coll->o = NULL;
//	pnt[0] = ft_3_nullpointnew();
//	pnt[1] = ft_3_nullpointnew();
//	pnt[2] = ft_3_nullpointnew();
	dist[0] = FLT_MAX;
	tmp_coll = *coll;
	while (node)
	{
		o = (t_object *)(node->content);
//		if (o->ft_is_reachable(o->fig, od[0], od[1]))
//		{
			dist[1] = o->ft_collide(objs, o, coll, od);
			if (dist[1] < dist[0])
			{
				dist[0] = dist[1];
				tmp_coll = *coll;
			}
//		}
		node = node->next;
	}
	*coll = tmp_coll;
}

static void			ft_refract(t_ray *ray)
{
    float		refr[2];
    float		cos[2];

    ft_get_refrs(ray, &refr);
    ray->coll->trans_vec =
            ft_3_vector_refract(ray->coll->norm, ray->d, refr[0], refr[1]);

//	printf("refrs: %f -> %f\n", refr[0], refr[1]);
//	printf("norm: (%8.3f, %8.3f, %8.3f)\n",
//		   ray->coll->norm[0], ray->coll->norm[1], ray->coll->norm[2]);
//	printf("direction: (%8.3f, %8.3f, %8.3f) -> (%8.3f, %8.3f, %8.3f)\n",
//		   ray->d[0], ray->d[1], ray->d[2],
//		   ray->coll->trans_vec[0], ray->coll->trans_vec[1], ray->coll->trans_vec[2]);

	if (ft_3_isnullpoint(ray->coll->trans_vec))
    {
        ray->coll->fresnel = 1.0f;
        return ;
    }
    cos[0] = fabsf(ft_3_vector_cos(ray->d, ray->coll->norm));
    cos[1] = fabsf(ft_3_vector_cos(ray->coll->trans_vec, ray->coll->norm));
    if (ray->coll->o->spclr)
    {
        ray->coll->fresnel = (powf(
                (refr[1] * cos[0] - refr[0] * cos[1]) /
                (refr[1] * cos[0] + refr[0] * cos[1]),
                2.0f) + powf(
                (refr[0] * cos[1] - refr[1] * cos[0]) /
                (refr[0] * cos[1] + refr[1] * cos[0]),
                2.0f)) / 2.0f;
    }
}

t_coll				ft_get_collision(t_thrarg *arg, t_ray *ray)
{
	t_coll		coll;
	t_vector	od[2];
	fun_tab		func;

//	od[0] = ray->o + ft_3_vector_scale(ray->d, 0.1f);
	od[0] = ray->o;
	od[1] = ray->d;
	ray->coll = &coll;
	ft_init_collision(&coll, &(arg->e->scn->objs), od);

//	printf("norm after coll: (%8.3f, %8.3f, %8.3f)\n",
//		   coll.norm[0], coll.norm[1], coll.norm[2]);

	if (!(coll.o))
		return (coll);
	if (ft_3_vector_cos(coll.norm, ray->d) > 0)
		coll.norm = ft_3_vector_invert(coll.norm);
	if (coll.o->trans)
		ft_refract(ray);
//	coll.coll_pnt += ft_3_vector_scale(coll.norm, 0.1f);
	if (coll.o->spclr)
		coll.spclr_vec = ft_3_vector_reflect(ray->o, coll.coll_pnt, coll.norm);
	func[1] = coll.tex_o->ft_mapping;
	func[2] = coll.tex_o->ft_checker;
	func[3] = coll.tex_o->ft_procedural;
	coll.px_color.val = (coll.o->exposure == 0) ? coll.o->color.val :
		func[coll.o->exposure](coll.tex_o, coll.o->tex_pnt, coll.ucoll_pnt);
	ft_illuminate(arg, &coll);
	return (coll);
}
