/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcavalle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/17 01:01:52 by lcavalle          #+#    #+#             */
/*   Updated: 2018/06/10 01:08:45 by lcavalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	ft_min_pos(double t1, double t2)
{
	if ((t1 < t2 || t2 < 0) && t1 > 0)
		return (t1);
	if ((t2 < t1 || t1 < 0) && t2 > 0)
		return (t2);
	else
		return (-1);
}

/*
** quadratic equation. simpliefied a, b and c. also simplified formula
** (wikipedia line-sphere intersection)
*/
int					intersect_sphere(t_line line, t_object obj, double sols[MAX_DEGREE])
{
	t_quadratic	equa;
	double		radic;

	equa = (t_quadratic){.a = dotprod(line.v, line.v), .b = 2 *
		dotprod(line.o, line.v),
			.c = dotprod(line.o, line.o) - (obj.object_union.sphere.radius *
					obj.object_union.sphere.radius)};
	radic = (equa.b * equa.b) - (4 * equa.a * equa.c);
	if (radic < 0.0)
		return (0);
	sols[0] = (-equa.b + sqrt(radic)) / (2.0 * equa.a);
	sols[1] = (-equa.b - sqrt(radic)) / (2.0 * equa.a);
	return (2);
}

int					intersect_paraboloid(t_line line, t_object obj, double sols[MAX_DEGREE])
{
	t_quadratic	equa;
	double		radic;
	(void)obj;
	equa = (t_quadratic){.a = line.v.y * line.v.y + line.v.z * line.v.z,
	   	.b = 2 * (line.o.y * line.v.y + line.v.z * line.o.z) - line.v.x,
			.c = line.o.y * line.o.y + line.o.z * line.o.z - line.o.x};
	radic = (equa.b * equa.b) - (4 * equa.a * equa.c);
	if (radic < 0.0)
		return (0);
	sols[0] = (-equa.b + sqrt(radic)) / (2.0 * equa.a);
	sols[1] = (-equa.b - sqrt(radic)) / (2.0 * equa.a);
	return (2);
}
int					intersect_hyperboloid(t_line line, t_object obj, double sols[MAX_DEGREE])
{
	t_quadratic	equa;
	double		radic;
	double rad;

	rad = obj.object_union.hyperboloid.radius;
	(void)obj;
	equa = (t_quadratic){.a = line.v.y * line.v.y + line.v.z * line.v.z -
		line.v.x * line.v.x,
	   	.b = 2 * (line.o.y * line.v.y + line.o.z * line.v.z - line.o.x * line.v.x),
			.c = line.o.y * line.o.y + line.o.z * line.o.z - 
				line.o.x * line.o.x - rad * rad};
	radic = (equa.b * equa.b) - (4 * equa.a * equa.c);
	if (radic < 0.0)
		return (0);
	sols[0] = (-equa.b + sqrt(radic)) / (2.0 * equa.a);
	sols[1] = (-equa.b - sqrt(radic)) / (2.0 * equa.a);
	return (2);
}

static t_quadratic get_quadratic_ellipsoid(t_line line, t_object obj)
{
	t_point3d	abc;
	t_quadratic res;

	abc = obj.object_union.ellipsoid.abc;
	res.a = ((line.v.x * line.v.x) / (abc.x * abc.x)) +
		((line.v.y * line.v.y) / (abc.y * abc.y)) +
		((line.v.z * line.v.z) / (abc.z * abc.z));
	res.b = 2 *(((line.v.x * line.o.x) / (abc.x * abc.x)) +
			((line.v.y * line.o.y) / (abc.y * abc.y)) +
			((line.v.z * line.o.z) / (abc.z * abc.z)));
	res.c = ((line.o.x * line.o.x) / (abc.x * abc.x)) +
		((line.o.y * line.o.y) / (abc.y * abc.y)) +
		((line.o.z * line.o.z) / (abc.z * abc.z)) -
		(obj.object_union.ellipsoid.radius *
		 obj.object_union.ellipsoid.radius);
	return res;
}

int					intersect_ellipsoid(t_line line, t_object obj, double sols[MAX_DEGREE])
{
	t_quadratic	equa;
	double		radic;

	equa = get_quadratic_ellipsoid(line, obj);
	radic = (equa.b * equa.b) - (4 * equa.a * equa.c);
	if (radic < 0.0)
		return (0);
	sols[0] = (-equa.b + sqrt(radic)) / (2.0 * equa.a);
	sols[1] = (-equa.b - sqrt(radic)) / (2.0 * equa.a);
	return (2);
}

void	noquartics(double complex qsols[MAX_DEGREE])
{
	int i;

	i = 0;
	while (i < MAX_DEGREE)
		qsols[i++] = -1;
}

t_cubic				ft_quartic_as_cubic(t_quartic quartic)
{
	t_cubic res;

	res.a = quartic.b;
	res.b = quartic.c;
	res.c = quartic.d;
	res.d = quartic.e;
	res.debug = quartic.debug;
	return (res);
}

long complex double		ft_cbrt(long complex double z, int i)
{
	return (cbrtl(cabsl(z)) * cexpl(I * (cargl(z) + 2 * i * M_PI) / 3.0));
}

t_quadratic			ft_cubic_as_quadratic(t_cubic cubic)
{
	t_quadratic res;

	res.a = cubic.b;
	res.b = cubic.c;
	res.c = cubic.d;
	res.debug = cubic.debug;
	return (res);
}

t_affine		ft_quadratic_as_affine(t_quadratic quadratic)
{
	t_affine res;

	res.a = quadratic.b;
	res.b = quadratic.c;
	res.debug = quadratic.debug;
	return (res);
}

int			resolve_affine(t_affine equa, double complex qsols[MAX_DEGREE])
{
	noquartics(qsols);

	if (equa.a == 0)
		return (0);
	qsols[0] = -equa.b / equa.a;
	return (1);
}

int			resolve_quadratic(t_quadratic equa, double complex qsols[MAX_DEGREE])
{
	if (equa.a == 0)
		return resolve_affine(ft_quadratic_as_affine(equa), qsols);
	noquartics(qsols);
	equa.radic = equa.b * equa.b - 4 * equa.a * equa.c;
	if (equa.radic < 0)
		return (0);
	else
	{
		if (equa.radic == 0)
		{
			qsols[0] = - equa.b / (2 * equa.a);
			return (1);
		}
		else
		{
			qsols[0] = (-equa.b - sqrt(equa.radic)) / (2 * equa.a);
			qsols[1] = (-equa.b + sqrt(equa.radic)) / (2 * equa.a);
			return (2);
		}
	}
}

int					ft_is_zero(long double complex z)
{
	return (fabs(crealf(z)) < EPSILON4 && fabs(cimagf(z)) < EPSILON4);
}


int		resolve_cubic(t_cubic equa, double complex qsols[MAX_DEGREE])
{
	t_auxcubic	x;
	if (fabsl(equa.a) <  0.001)
		return resolve_quadratic(ft_cubic_as_quadratic(equa), qsols);
	noquartics(qsols);

	equa.b /= equa.a;
	equa.c /= equa.a;
	equa.d /= equa.a;

	x.s = equa.b / 3.0;
	x.d = (equa.c / 3.0) - (x.s * x.s);
	x.e = (x.s * x.s * x.s) + ((equa.d - (x.s * equa.c)) / 2.0);
	x.sqrt = csqrt(x.e * x.e + x.d * x.d * x.d);
	x.f = -x.sqrt - x.e;
	if (ft_is_zero(x.f))
		x.f = x.sqrt - x.e;
	x.g = ft_cbrt(x.f, 0);
	qsols[0] = x.g - (x.d / x.g) - x.s;
	x.g = ft_cbrt(x.f, 1);
	qsols[1] = x.g - (x.d / x.g) - x.s;
	x.g = ft_cbrt(x.f, 2);
	qsols[2] = x.g - (x.d / x.g) - x.s;
	return (3);
}

int				resolve_quartic(t_quartic equa, double complex qsols[MAX_DEGREE])
{
	t_auxquartic x;

	if (fabsl(equa.a) < EPSILON2 )
		return resolve_cubic(ft_quartic_as_cubic(equa), qsols);
	equa.b /= equa.a;
	equa.c /= equa.a;
	equa.d /= equa.a;
	equa.e /= equa.a;
	x.alpha = - ((3.0 * equa.b * equa.b) / (8.0)) + (equa.c);
	x.beta = ((equa.b * equa.b * equa.b) / (8.0)) - ((equa.b * equa.c) / (2.0)) + (equa.d);
	x.gamma = - ((3.0 * equa.b * equa.b * equa.b * equa.b) / (256.0)) + ((equa.c * equa.b * equa.b) / (16.0)) -
		((equa.b * equa.d) / (4.0)) + (equa.e);
	x.t = -(equa.b / (4.0));
	if (ft_is_zero(x.beta))
	{
		x.rad = csqrt((x.alpha * x.alpha) - 4 * x.gamma);
		x.r1 = csqrt((-x.alpha + x.rad) / 2.0);
		x.r2 = csqrt((-x.alpha - x.rad) / 2.0);
		qsols[0] = x.t + x.r1;
		qsols[1] = x.t - x.r1;
		qsols[2] = x.t + x.r2;
		qsols[3] = x.t - x.r2;
	}
	x.p = - ((x.alpha * x.alpha) / 12.0 ) - x.gamma;
	x.q = - ((x.alpha * x.alpha * x.alpha) / 108.0) +
		((x.alpha * x.gamma) / 3.0) - ((x.beta * x.beta) / 8.0);
	x.r = - (x.q / 2.0) + csqrt(((x.q * x.q) / 4.0) + ((x.p * x.p * x.p) / 27.0));
	x.u = cpowl(x.r, 1.0 / 3.0);
	x.y = -((5.0 * x.alpha) / 6.0) + x.u;
	if (ft_is_zero(x.u))
		x.y -= cpowl(x.q, 1.0 / 3.0);
	else
		x.y -= (x.p / (3.0 * x.u));
	x.w = csqrtl(x.alpha + 2 * x.y);
	x.r1 = csqrtl(-((3.0 * x.alpha) + (2.0 * x.y) + ((2.0 * x.beta) / x.w)));
	x.r2 = csqrtl(-((3.0 * x.alpha) + (2.0 * x.y) - ((2.0 * x.beta) / x.w)));
	qsols[0] = x.t + ((x.w - x.r1) / 2.0);
	qsols[1] = x.t + ((x.w + x.r1) / 2.0);
	qsols[2] = x.t + ((-x.w - x.r2) / 2.0);
	qsols[3] = x.t + ((-x.w + x.r2) / 2.0);
	return (4);
}

void			ft_init_aux(t_auxquart_init *g, t_line line)
{
	g->ox2 = line.o.x * line.o.x;
	g->ox3 = g->ox2 * line.o.x;
	g->ox4 = g->ox2 * g->ox2;
	g->vx2 = line.v.x * line.v.x;
	g->vx3 = g->vx2 * line.v.x;
	g->vx4 = g->vx2 * g->vx2;
	g->oy2 = line.o.y * line.o.y;
	g->oy3 = g->oy2 * line.o.y;
	g->oy4 = g->oy2 * g->oy2;
	g->vy2 = line.v.y * line.v.y;
	g->vy3 = g->vy2 * line.v.y;
	g->vy4 = g->vy2 * g->vy2;
	g->oz2 = line.o.z * line.o.z;
	g->oz3 = g->oz2 * line.o.z;
	g->oz4 = g->oz2 * g->oz2;
	g->vz2 = line.v.z * line.v.z;
	g->vz3 = g->vz2 * line.v.z;
	g->vz4 = g->vz2 * g->vz2;
}

int					ft_transfer_real_roots(double complex qsols[MAX_DEGREE], int nbqsols, 
			double sols[MAX_DEGREE])
{
	int i;
	int res;

	i = 0;
	res = 0;
	while (i < nbqsols)
	{
		if (fabsl(cimagl(qsols[i])) < EPSILON3)
			sols[res++] = creal(qsols[i]);
		i++;
	}
	return (res);
}

int					intersect_torus(t_line line, t_object obj,
		double sols[MAX_DEGREE])
{
	t_quartic equa;
	double complex qsols[4];
	int	nbqsols;

	equa = get_quartic_torus(line, obj);
	nbqsols = resolve_quartic(equa, qsols);
	return (ft_transfer_real_roots(qsols, nbqsols, sols));
}

int					intersect_goursat(t_line line, t_object obj,
		double sols[MAX_DEGREE])
{
	t_quartic equa;
	double complex qsols[MAX_DEGREE];
	int nbqsols;

	equa = get_quartic_goursat(line, obj);
	nbqsols = resolve_quartic(equa, qsols);
	return (ft_transfer_real_roots(qsols, nbqsols, sols));
}

void				ft_cut_as_strip(double sols[MAX_DEGREE], int nbsols, t_line line)
{
	int i;
	t_point3d point;

	i = 0;
	while (i < nbsols)
	{
		point = ft_point3d_add(line.o, ft_point3d_scalar(line.v, sols[i]));
		if (point.x * point.x + point.y * point.y + point.z * point.z > 0.65
				|| point.x * point.x + point.y * point.y + point.z * point.z < 0.25)
			sols[i] = -1;
		i++;
	}
}

int					intersect_mobius(t_line line, t_object obj,
		double sols[MAX_DEGREE])
{
	t_quartic equa;
	int nbqsols;
	double complex qsols[4];
	int ret;

	(void)obj;
	equa = get_quartic_mobius(line, obj);
	nbqsols = resolve_quartic(equa, qsols);
	ret = ft_transfer_real_roots(qsols, nbqsols, sols);
	ft_cut_as_strip(sols, ret, line);
	return (ret);
}

int					intersect_lemniscate(t_line line, t_object obj,
		double sols[MAX_DEGREE])
{
	t_quartic equa;
	int nbqsols;
	double complex qsols[4];

	(void)obj;
	equa = get_quartic_lemniscate(line);
	nbqsols = resolve_quartic(equa, qsols);
	return (ft_transfer_real_roots(qsols, nbqsols, sols));
}

int					intersect_roman(t_line line, t_object obj,
		double sols[MAX_DEGREE])
{
	t_quartic equa;
	double complex qsols[4];
	int nbqsols;	
	
	(void)obj;
	equa = get_quartic_roman(line);
	nbqsols = resolve_quartic(equa, qsols);
	return (ft_transfer_real_roots(qsols, nbqsols, sols));
}

int					intersect_piriform(t_line line, t_object obj,
		double sols[MAX_DEGREE])
{
	t_quartic equa;
	double complex qsols[4];
	int nbqsols;

	(void)obj;
	equa = get_quartic_piriform(line);
	nbqsols = resolve_quartic(equa, qsols);
	return (ft_transfer_real_roots(qsols, nbqsols, sols));
}

static t_auxcone	getauxcone(t_line line, t_object obj)
{
	t_auxcone	aux;

	(void) line;
	aux.sqcos = cos(obj.object_union.cone.angle) *
		cos(obj.object_union.cone.angle);
	aux.sqsin = sin(obj.object_union.cone.angle) *
		sin(obj.object_union.cone.angle);
	return (aux);
}

int					intersect_cone(t_line line, t_object obj,
		double sols[MAX_DEGREE])
{
	t_auxcone	aux;
	t_quadratic	equa;
	double		radic;

	aux = getauxcone(line, obj);
	equa.a = aux.sqcos * ((line.v.y * line.v.y) + (line.v.z * line.v.z))
		- aux.sqsin * (line.v.x * line.v.x);
	equa.b = 2 * (aux.sqcos * ((line.o.y * line.v.y) + (line.o.z * line.v.z))
			- aux.sqsin * (line.o.x * line.v.x));
	equa.c = aux.sqcos * ((line.o.y * line.o.y) + (line.o.z * line.o.z))
		- aux.sqsin * (line.o.x * line.o.x);
	radic = (equa.b * equa.b) - (4 * equa.a * equa.c);
	if (radic < 0.0)
		return (0);
	sols[0] = (-equa.b + sqrt(radic)) / (2.0 * equa.a);
	sols[1] = (-equa.b - sqrt(radic)) / (2.0 * equa.a);
	return (2);
}

int					intersect_plane(t_line line, t_object obj,
		double sols[MAX_DEGREE])
{
	(void)obj;
	if (line.v.y < EPSILON && line.v.y > -EPSILON)
		return (0);
	sols[0] = -line.o.y / line.v.y;
	return (1);
}

int					intersect_cylinder(t_line line, t_object obj,
			double sols[MAX_DEGREE])
{
	t_quadratic	equa;
	double		radic;

	equa = (t_quadratic){.a = (line.v.z * line.v.z) + (line.v.y * line.v.y),
		.b = 2 * ((line.o.z * line.v.z) + (line.o.y * line.v.y)),
		.c = (line.o.z * line.o.z) + (line.o.y * line.o.y) -
			(obj.object_union.cylinder.radius * obj.object_union.cylinder.radius)};
	radic = (equa.b * equa.b) - (4 * equa.a * equa.c);
	if (radic < 0.0)
		return (0);
	sols[0] = (-equa.b + sqrt(radic)) / (2.0 * equa.a);
	sols[1] = (-equa.b - sqrt(radic)) / (2.0 * equa.a);
	return (2);
}
