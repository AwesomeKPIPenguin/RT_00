
#include "rt.h"

static t_vector	ft_parse_point(char **content)
{
	float		coords[3];
	t_vector	point;

	coords[0] = 0.0f; //coords[0] = 1;
	coords[1] = 0.0f;
	coords[2] = 0.0f;
	ft_get_attr(content, "x", &(coords[0]), DT_FLOAT);
	ft_get_attr(content, "y", &(coords[1]), DT_FLOAT);
	ft_get_attr(content, "z", &(coords[2]), DT_FLOAT);
	point = (t_vector){ coords[0], coords[1], coords[2] };
	return (point);
}

void			ft_parse_str
					(char **content, char **data,
					void *dst, t_datatype datatype)
{
	++(*data);
	if (datatype == DT_STRING)
		*((char **)dst) = ft_strsub(
			*data, 0, ft_strchr(*data, '\"') - *data);
	else if (datatype == DT_COLOR)
	{
		(*((Uint32 *) dst)) = SDL_Swap32(ft_limit_uint(
				0, 0xffffff, (unsigned int) ft_atoi_base(*data, 16)) << 8);
//		(*((Uint32 *) dst)) = SDL_Swap32((*((Uint32 *) dst)) << 8);
	}
	else
		ft_parse_warning_datatype(content[0], *data, datatype);
}

void			ft_parse_json_object
					(char **content, char **data,
					void *dst, t_datatype datatype)
{
	char	*(new_content[2]);

	new_content[0] = content[0];
	new_content[1] = *data;
	if (datatype == DT_POINT)
		*((t_vector *)dst) = ft_parse_point(new_content);
	else if (datatype == DT_CAMERA)
		ft_parse_camera(new_content, *((t_camera **) dst));
	else if (datatype == DT_SKYBOX)
		ft_parse_skybox(new_content, ((t_skybox **) dst));
	else if (datatype == DT_PROCEDURAL)
	{
		ft_parse_procedural(new_content, ((t_procedural **) dst));
	}
	else if (datatype == DT_CHECKER)
	{
		ft_parse_checker(new_content, ((t_checkbrd **) dst));
	}
	else
		ft_parse_warning_datatype(content[0], *data, datatype);
}

static void		ft_parse_array
					(char **content, char **data, t_list **list,
					void (*ft_parse)(char **, t_list **, Uint32))
{
	int		scope;
	int		i;
	int		is_in_str;
	Uint32	id;
	char	*(new_content[2]);

	scope = 0;
	i = 0;
	is_in_str = 0;
	id = 0;
	while (scope != 0 || (*data)[i] != ']' || is_in_str)
	{
		if ((*data)[i] == '\"' && (*data)[i - 1] != '\\')
			is_in_str = !is_in_str;
		if (is_in_str)
		{
			++i;
			continue ;
		}
		if ((*data)[i] == '{')
		{
			new_content[0] = content[0];
			new_content[1] = &((*data)[i]);
			if (scope == 0)
				ft_parse(new_content, list, id++);
			++scope;
		}
		else if ((*data)[i] == '}')
			--scope;
		++i;
	}
}

void			ft_parse_json_array
					(char **content, char **data,
					void *dst, t_datatype datatype)
{
	if (datatype == DT_LIGHT_ARR || datatype == DT_OBJECT_ARR)
		ft_parse_array(
			content,
			data,
			(t_list **)dst,
			(datatype == DT_LIGHT_ARR) ? ft_parse_light : ft_parse_object
		);
	else
		ft_parse_warning_datatype(content[0], *data, datatype);
}

void			ft_parse_num
					(char **content, char **data,
					void *dst, t_datatype datatype)
{
	if (datatype == DT_FLOAT)
		*((float *)dst) = (float)ft_atod(*data);
	else if (datatype == DT_COEF)
		*((float *)dst) = (float)ft_limitf(0.0, 1.0, (float)ft_atod(*data));
	else
		ft_parse_warning_datatype(content[0], *data, datatype);
}

void			ft_parse_bool
					(char **content, char **data,
					void *dst, t_datatype datatype)
{
	if (datatype == DT_BOOL)
		*((int *)dst) = (**data == 't') ? 1 : 0;
	else
		ft_parse_warning_datatype(content[0], *data, datatype);
}