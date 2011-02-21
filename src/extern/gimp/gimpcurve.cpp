/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


// Comment by SpaecDeminer Developers: The Part not needed by us is not included.
// the following codelines are implemented by us to intigrade the gimp_curve_plot
// function into our project

#include "./../../tools/PlanetGenerator/Model.hpp"
typedef Curve GimpCurve;
#define gint gsize

// the following Line is taken from the gimpmath.h file of the Gimp source
#define ROUND(x) ((int) ((x) + 0.5))

// the following code is taken from the gimpcurve.c file of the Gimp source (except of "Curve::" and the removed "static")

/*
 * This function calculates the curve values between the control points
 * p2 and p3, taking the potentially existing neighbors p1 and p4 into
 * account.
 *
 * This function uses a cubic bezier curve for the individual segments and
 * calculates the necessary intermediate control points depending on the
 * neighbor curve control points.
 */
/*[static] commented out by the Space developer team*/void
Curve::gimp_curve_plot (GimpCurve *curve, // Curve:: added by the Space developer team
                 gint       p1,
                 gint       p2,
                 gint       p3,
                 gint       p4)
{
  gint    i;
  gdouble x0, x3;
  gdouble y0, y1, y2, y3;
  gdouble dx, dy;
  gdouble slope;

  /* the outer control points for the bezier curve. */
  x0 = curve->points[p2].x;
  y0 = curve->points[p2].y;
  x3 = curve->points[p3].x;
  y3 = curve->points[p3].y;

  /*
   * the x values of the inner control points are fixed at
   * x1 = 2/3*x0 + 1/3*x3   and  x2 = 1/3*x0 + 2/3*x3
   * this ensures that the x values increase linearily with the
   * parameter t and enables us to skip the calculation of the x
   * values altogehter - just calculate y(t) evenly spaced.
   */

  dx = x3 - x0;
  dy = y3 - y0;

  g_return_if_fail (dx > 0);

  if (p1 == p2 && p3 == p4)
    {
      /* No information about the neighbors,
       * calculate y1 and y2 to get a straight line
       */
      y1 = y0 + dy / 3.0;
      y2 = y0 + dy * 2.0 / 3.0;
    }
  else if (p1 == p2 && p3 != p4)
    {
      /* only the right neighbor is available. Make the tangent at the
       * right endpoint parallel to the line between the left endpoint
       * and the right neighbor. Then point the tangent at the left towards
       * the control handle of the right tangent, to ensure that the curve
       * does not have an inflection point.
       */
      slope = (curve->points[p4].y - y0) / (curve->points[p4].x - x0);

      y2 = y3 - slope * dx / 3.0;
      y1 = y0 + (y2 - y0) / 2.0;
    }
  else if (p1 != p2 && p3 == p4)
    {
      /* see previous case */
      slope = (y3 - curve->points[p1].y) / (x3 - curve->points[p1].x);

      y1 = y0 + slope * dx / 3.0;
      y2 = y3 + (y1 - y3) / 2.0;
    }
  else /* (p1 != p2 && p3 != p4) */
    {
      /* Both neighbors are available. Make the tangents at the endpoints
       * parallel to the line between the opposite endpoint and the adjacent
       * neighbor.
       */
      slope = (y3 - curve->points[p1].y) / (x3 - curve->points[p1].x);

      y1 = y0 + slope * dx / 3.0;

      slope = (curve->points[p4].y - y0) / (curve->points[p4].x - x0);

      y2 = y3 - slope * dx / 3.0;
    }

  /*
   * finally calculate the y(t) values for the given bezier values. We can
   * use homogenously distributed values for t, since x(t) increases linearily.
   */
  for (i = 0; i <= ROUND (dx * (gdouble) (curve->n_samples - 1)); i++)
    {
      gdouble y, t;
      gint    index;

      t = i / dx / (gdouble) (curve->n_samples - 1);
      y =     y0 * (1-t) * (1-t) * (1-t) +
          3 * y1 * (1-t) * (1-t) * t     +
          3 * y2 * (1-t) * t     * t     +
              y3 * t     * t     * t;

      index = i + ROUND (x0 * (gdouble) (curve->n_samples - 1));

      if (index < curve->n_samples)
        curve->samples[index] = CLAMP (y, 0.0, 1.0);
    }
}
