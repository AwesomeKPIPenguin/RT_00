/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_3_vector_cos.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: domelche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/19 11:42:44 by domelche          #+#    #+#             */
/*   Updated: 2018/09/19 11:43:05 by domelche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libpnt.h"

float			ft_3_vector_cos(t_vector vec_1, t_vector vec_2)
{
	return ((vec_1[0] * vec_2[0] + vec_1[1] * vec_2[1] + vec_1[2] * vec_2[2]) /
		(ft_3_vector_len(vec_1) * ft_3_vector_len(vec_2)));
}
