/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2i.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:17:57 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/23 15:18:14 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math/vec.h"
#include <math.h>

t_vec2i	vec2i_add(t_vec2i a, t_vec2i b)
{
	t_vec2i	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return (result);
}

t_vec2i	vec2i_sub(t_vec2i a, t_vec2i b)
{
	t_vec2i	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return (result);
}

t_vec2i	vec2i_scale(t_vec2i v, int scalar)
{
	t_vec2i	result;

	result.x = v.x * scalar;
	result.y = v.y * scalar;
	return (result);
}

int	vec2i_dot(t_vec2i a, t_vec2i b)
{
	return (a.x * b.x + a.y * b.y);
}

float	vec2i_length(t_vec2i v)
{
	return (sqrtf((float)(v.x * v.x + v.y * v.y)));
}

t_vec2i	vec2i_normalize(t_vec2i v)
{
	float	length;
	t_vec2i	result;

	length = vec2i_length(v);
	if (length == 0.0f)
	{
		result.x = 0;
		result.y = 0;
		return (result);
	}
	result.x = (int)(v.x / length);
	result.y = (int)(v.y / length);
	return (result);
}

float	vec2i_distance(t_vec2i a, t_vec2i b)
{
	t_vec2i	diff;

	diff = vec2i_sub(a, b);
	return (vec2i_length(diff));
}

float	vec2i_distance_squared(t_vec2i a, t_vec2i b)
{
	t_vec2i	diff;

	diff = vec2i_sub(a, b);
	return ((float)(diff.x * diff.x + diff.y * diff.y));
}

bool	vec2i_equal(t_vec2i a, t_vec2i b)
{
	return (a.x == b.x && a.y == b.y);
}

bool	vec2i_is_zero(t_vec2i v)
{
	return (v.x == 0 && v.y == 0);
}

t_vec2i	vec2i_zero(void)
{
	t_vec2i	result;

	result.x = 0;
	result.y = 0;
	return (result);
}

t_vec2i	vec2i_one(void)
{
	t_vec2i	result;

	result.x = 1;
	result.y = 1;
	return (result);
}

t_vec2i	vec2i_unit_x(void)
{
	t_vec2i	result;

	result.x = 1;
	result.y = 0;
	return (result);
}

t_vec2i	vec2i_unit_y(void)
{
	t_vec2i	result;

	result.x = 0;
	result.y = 1;
	return (result);
}

t_vec2i	vec2i_min(t_vec2i a, t_vec2i b)
{
	t_vec2i	result;

	result.x = (a.x < b.x) ? a.x : b.x;
	result.y = (a.y < b.y) ? a.y : b.y;
	return (result);
}

t_vec2i	vec2i_max(t_vec2i a, t_vec2i b)
{
	t_vec2i	result;

	result.x = (a.x > b.x) ? a.x : b.x;
	result.y = (a.y > b.y) ? a.y : b.y;
	return (result);
}

t_vec2i	vec2i_abs(t_vec2i v)
{
	t_vec2i result;

	result.x = (v.x < 0) ? -v.x : v.x;
	result.y = (v.y < 0) ? -v.y : v.y;
	return (result);
}