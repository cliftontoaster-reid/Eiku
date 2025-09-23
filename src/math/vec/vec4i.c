/* *******************************************************t_vec4i	vec4i_normalize(t_vec4i v)
{
	float	length;
	t_vec4i	result;

	length = vec4i_length(v);
	if (length == 0.0f)
	{
		result.x = 0;
		result.y = 0;
		result.z = 0;
		result.w = 0;
		return (result);
	}
	result.x = (int)(v.x / length);
	result.y = (int)(v.y / length);
	result.z = (int)(v.z / length);
	result.w = (int)(v.w / length);
	return (result);
}

float	vec4i_distance(t_vec4i a, t_vec4i b)
{
	t_vec4i	diff;

	diff = vec4i_sub(a, b);
	return (vec4i_length(diff));
}

float	vec4i_distance_squared(t_vec4i a, t_vec4i b)
{
	t_vec4i	diff;

	diff = vec4i_sub(a, b);
	return ((float)(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z
		+ diff.w * diff.w));
}

bool	vec4i_equal(t_vec4i a, t_vec4i b)
{
	return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}

bool	vec4i_is_zero(t_vec4i v)
{
	return (v.x == 0 && v.y == 0 && v.z == 0 && v.w == 0);
}

t_vec4i	vec4i_zero(void)
{
	t_vec4i	result;

	result.x = 0;
	result.y = 0;
	result.z = 0;
	result.w = 0;
	return (result);
}

t_vec4i	vec4i_one(void)
{
	t_vec4i	result;

	result.x = 1;
	result.y = 1;
	result.z = 1;
	result.w = 1;
	return (result);
}

t_vec4i	vec4i_min(t_vec4i a, t_vec4i b)
{
	t_vec4i	result;

	result.x = (a.x < b.x) ? a.x : b.x;
	result.y = (a.y < b.y) ? a.y : b.y;
	result.z = (a.z < b.z) ? a.z : b.z;
	result.w = (a.w < b.w) ? a.w : b.w;
	return (result);
}

t_vec4i	vec4i_max(t_vec4i a, t_vec4i b)
{
	t_vec4i	result;

	result.x = (a.x > b.x) ? a.x : b.x;
	result.y = (a.y > b.y) ? a.y : b.y;
	result.z = (a.z > b.z) ? a.z : b.z;
	result.w = (a.w > b.w) ? a.w : b.w;
	return (result);
}

t_vec4i	vec4i_abs(t_vec4i v)
{
	t_vec4i	result;

	result.x = (v.x < 0) ? -v.x : v.x;
	result.y = (v.y < 0) ? -v.y : v.y;
	result.z = (v.z < 0) ? -v.z : v.z;
	result.w = (v.w < 0) ? -v.w : v.w;
	return (result);
}*/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4i.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:05:03 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/23 15:10:16 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math/vec.h"
#include <math.h>

t_vec4i	vec4i_add(t_vec4i a, t_vec4i b)
{
	t_vec4i	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	result.w = a.w + b.w;
	return (result);
}

t_vec4i	vec4i_sub(t_vec4i a, t_vec4i b)
{
	t_vec4i	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	result.w = a.w - b.w;
	return (result);
}

t_vec4i	vec4i_scale(t_vec4i v, int scalar)
{
	t_vec4i	result;

	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;
	result.w = v.w * scalar;
	return (result);
}

int	vec4i_dot(t_vec4i a, t_vec4i b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

float	vec4i_length(t_vec4i v)
{
	return (sqrtf((float)(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w)));
}

t_vec4i	vec4i_normalize(t_vec4i v)
{
	float length;
	t_vec4i result;

	length = vec4i_length(v);
	if (length == 0.0f)
	{
		result.x = 0;
		result.y = 0;
		result.z = 0;
		result.w = 0;
		return (result);
	}
	result.x = (int)(v.x / length);
	result.y = (int)(v.y / length);
	result.z = (int)(v.z / length);
	result.w = (int)(v.w / length);
	return (result);
}