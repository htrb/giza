/* giza - a scientific plotting layer built on cairo
 *
 * This file is (or was) part of GIZA, a scientific plotting
 * layer built on cairo.
 * GIZA comes with ABSOLUTELY NO WARRANTY.
 * This is free software; and you are welcome to redistribute
 * it under the terms of the GNU General Public License
 * (see LICENSE file for details) and the provision that
 * this notice remains intact. If you modify this file, please
 * note section 2a) of the GPLv2 states that:
 *
 *  a) You must cause the modified files to carry prominent notices
 *     stating that you changed the files and the date of any change.
 *
 * Copyright (C) 2010 James Wetter and Daniel Price. All rights reserved.
 * Contact: wetter.j@gmail.com
 *          daniel.price@sci.monash.edu.au
 *
 * ChangeLog:
 * 22/4/10 (DJP) added drawchar routine, centres character correctly on x,y position
 * 23/4/10 (DJP) added polygon, star, triangle, arrow and general circle routines
 * 23/4/10 (DJP) now handles all PGPLOT symbols from -8->127
 */

#include "giza-private.h"
#include "giza-transforms-private.h"
#include "giza-stroke-private.h"
#include "giza-io-private.h"
#include <giza.h>
#include <math.h>

/* Internal functions */
static void _giza_point        (double x, double y);
static void _giza_rect         (double x, double y, int fill);
static void _giza_plus         (double x, double y);
static void _giza_triangle     (double x, double y, int fill);
static void _giza_diamond      (double x, double y, int fill);
static void _giza_polygon      (double x, double y, int nsides , int fill);
static void _giza_star         (double x, double y, int npoints, double ratio, int fill);
static void _giza_circle       (double x, double y);
static void _giza_circle_size  (double x, double y, double size, int fill);
static void _giza_cross        (double x, double y);
static void _giza_arrow        (double x, double y, double angle);
static void _giza_char         (int symbol, double x, double y);
static void _giza_drawchar     (const char *string, double x, double y);
static void _giza_start_draw_symbols (int *oldTrans, int *oldLineStyle, int *oldLineCap,
                                      double *oldLineWidth, double *oldCh);
static void _giza_end_draw_symbols   (int oldTrans, int oldLineStyle, int oldLineCap,
                                      double oldLineWidth, double oldCh);
static void _giza_draw_symbol (double xd, double yd, int symbol);

/* Stores the height of the markers */
double markerHeight;

/**
 * Drawing: giza_points
 *
 * Synopsis: Plot n points at x[n], y[n] in world coords.
 *
 * Input:
 *  -n      :- the number of points
 *  -x      :- the x-coordinates of the points in world coords
 *  -y      :- the y-coordinates of the points in world coords
 *  -symbol :- the type of marker to use
 *
 * Symbols:
 *  -default :- small point (same as 1)
 *  -0       :- square
 *  -1       :- .
 *  -2       :- +
 *  -3       :- *
 *  -4       :- o
 *  -5       :- x
 *  ->31    :- from the Unicode table
 *  
 */
void
giza_points (int n, double x[n], double y[n], int symbol)
{
  if (!_giza_check_device_ready ("giza_points"))
    return;
  if (n < 1)
    {
      _giza_warning ("giza_points", "Invalid number of points.");
      return;
    }

  int oldTrans,oldLineStyle,oldLineCap;
  double oldLineWidth,oldCh;

  // initialise symbol drawing
  _giza_start_draw_symbols (&oldTrans,&oldLineStyle,&oldLineCap,&oldLineWidth,&oldCh);

  // for each point find where to put each marker
  int i;
  double xd, yd;
  for (i = 0; i < n; i++)
    {
      // convert world coords to device coords
      _giza_set_trans (GIZA_TRANS_WORLD);
      xd = x[i];
      yd = y[i];
      cairo_user_to_device (context, &xd, &yd);
      _giza_set_trans (GIZA_TRANS_IDEN);

      // draw the symbol
      _giza_draw_symbol(xd, yd, symbol);
    }

  _giza_stroke ();
  if (!Sets.buf)
    {
      giza_flush_device ();
    }

  // restore old setting
  _giza_end_draw_symbols (oldTrans,oldLineStyle,oldLineCap,oldLineWidth,oldCh);
}

/**
 * Drawing: giza_points_float
 *
 * Synopsis: Same functionality as giza_points but takes floats.
 *
 * See Also: giza_points
 */
void
giza_points_float (int n, float x[n], float y[n], int symbol)
{
  if (!_giza_check_device_ready ("giza_points"))
    return;
  if (n < 1)
    {
      _giza_warning ("giza_points_float", "Invalid number of points.");
    }

  int oldTrans,oldLineStyle,oldLineCap;
  double oldLineWidth,oldCh;

  // initialise symbol drawing
  _giza_start_draw_symbols (&oldTrans,&oldLineStyle,&oldLineCap,&oldLineWidth,&oldCh);

  // for each point find where to put each marker
  double xd, yd;
  int i;
  for (i = 0; i < n; i++)
    {
      xd = (double) x[i];
      yd = (double) y[i];

      // convert world coords to device coords
      _giza_set_trans (GIZA_TRANS_WORLD);
      cairo_user_to_device (context, &xd, &yd);
      _giza_set_trans (GIZA_TRANS_IDEN);
      
      // draw the symbol
      _giza_draw_symbol(xd, yd, symbol);
    }

  giza_end_buffer ();
  _giza_stroke ();
  if (!Sets.buf)
    {
      giza_flush_device ();
    }

  // restore old settings
  _giza_end_draw_symbols (oldTrans,oldLineStyle,oldLineCap,oldLineWidth,oldCh);
}

/**
 * Drawing: giza_single_point
 *
 * Synopsis: Plots a single point at x, y in world coords.
 *
 * Input:
 *  -x      :- the x-coordinate of the point in world coords
 *  -y      :- the y-coordinate of the point in world coords
 *  -symbol :- the type of marker to use
 *
 */
void
giza_single_point (double x, double y, int symbol)
{
  double xpts[1], ypts[1];
  xpts[0] = x;
  ypts[0] = y;
  giza_points (1, xpts, ypts, symbol);
}

/**
 * Drawing: giza_single_point_float
 *
 * Synopsis: Same functionality as giza_single_point,
 * but uses floats.
 *
 * See Also: giza_single_point
 */
void
giza_single_point_float (float x, float y, int symbol)
{
  double xpts[1], ypts[1];
  xpts[0] = (double) x;
  ypts[0] = (double) y;
  giza_points (1, xpts, ypts, symbol);
}

/**
 * DJP: Internal routine to initialise drawing of symbols
 *      (mainly to avoid repeated code in float/double versions)
 */
void
_giza_start_draw_symbols (int *oldTrans, int *oldLineStyle, int *oldLineCap,
                          double *oldLineWidth, double *oldCh)
{
  // store the old trans and line width
  *oldTrans = _giza_get_trans ();
  giza_get_line_style       (oldLineStyle);
  giza_get_line_cap         (oldLineCap);
  giza_get_line_width       (oldLineWidth);
  giza_get_character_height (oldCh);

  // Set the height for manually drawn markers
  markerHeight = Sets.fontExtents.max_x_advance * 0.2;

  // Set the line width for manually drawn markers
  //_giza_set_trans (GIZA_TRANS_IDEN);
  
  giza_set_line_width(1.5);
  giza_set_line_style(1);
  giza_set_line_cap(0);

  // Set the character size to draw the Unicode markers at
  giza_set_character_height (*oldCh * 0.8);

}

void
_giza_end_draw_symbols (int oldTrans, int oldLineStyle, int oldLineCap,
                        double oldLineWidth, double oldCh)
{
  // restore old settings
  _giza_set_trans (oldTrans);
  giza_set_line_style       (oldLineStyle);
  giza_set_line_cap         (oldLineCap);
  giza_set_line_width       (oldLineWidth);
  giza_set_character_height (oldCh);
}
/**
 * DJP: Internal routine to draw a single point, 
 * assuming initialisation routines have been called
 * (so cannot be called externally)
 *
 * This is mainly to avoid repeated code in the
 *  float and double routines
 *
 * Input:
 *  -x, y   :- the coordinate of the point in USER coords
 *  -symbol :- the type of marker to use
 */
void
_giza_draw_symbol (double xd, double yd, int symbol)
{

  // Use Unicode to draw the marker
  if (symbol > 31)
    {
      _giza_char (symbol, xd, yd);
    }
  // Manually draw the marker
  else
    {
      switch (symbol)
	{
        case 31: // down arrow
          _giza_arrow (xd, yd, 0.5*M_PI);
          break;
        case 30: // up arrow
          _giza_arrow (xd, yd, -0.5*M_PI);
          break;
        case 29: // right arrow
          _giza_arrow (xd, yd, 0.);
          break;
        case 28: // left arrow
          _giza_arrow (xd, yd, M_PI);
          break;
        case 27: // hollow circles of various sizes
        case 26:
        case 25:
        case 24:
        case 23:
        case 22:
        case 21:
          _giza_circle_size (xd, yd, 0.33*fabs(symbol-20), 0);
          break;
        case 20: // 7 pointed star
          _giza_star (xd, yd, 7, 0.25, 0);
          break;
        case 19: // hexagon with cross
          _giza_polygon (xd, yd, 6, 0);
          _giza_cross (xd, yd);
          break;
        case 18: // filled diamond
          _giza_diamond (xd, yd, 1);
          break;
	case 17: // solid circle
	  _giza_circle_size (xd, yd, 0.75, 1);
          break;
        case 16: // filled square
	  _giza_rect (xd, yd, 1);
          break;
        case 15: // hollow triangle up
          _giza_polygon (xd, yd, 3, 0);
          break;
        case 14: // pentagon
          _giza_polygon (xd, yd, 5, 0);
          break;
        case 13: // solid triangle
          _giza_triangle(xd, yd, 1);
          break;
        case 12: // five-pointed star
          _giza_star (xd, yd, 5, 0.5, 0);
          break;
        case 11: // hollow diamond
          _giza_diamond (xd, yd, 0);
          break;
        case 10: // asterisk made from combined + and x
          _giza_cross(xd, yd);
          _giza_plus(xd, yd);
          break;
        case 9: // circle with small dot (like Sun symbol)
          _giza_point (xd, yd);
	  _giza_circle_size (xd, yd, 1.25, 0);
          break;
        case 8: // circle and plus
          _giza_circle_size (xd, yd, 1.25, 0);
          _giza_plus (xd, yd);
          break;
        case 7: // hollow downward-pointing triangle
          _giza_triangle(xd, yd, 0);
          break;
	case 5: // cross (x)
	  _giza_cross (xd, yd);
          break;
	case 4: // hollow circle
	  _giza_circle (xd, yd);
	  break;
	case 3: // asterisk
	  _giza_drawchar ("*",xd, yd);
	  break;
	case 2: // plus
	  //_giza_drawchar ("+",xd, yd);
	  _giza_plus (xd, yd);
	  break;
	case 1: // single small point
	  _giza_point (xd, yd);
	  break;
        case 6: // hollow square
	case 0:
	  _giza_rect (xd, yd, 0);
	  break;
	case -1: // solid circles of various sizes
	case -2:
	case -3:
	case -4:
	case -5:
	case -6:
	case -7:
	case -8:
          _giza_circle_size (xd, yd, 0.15*fabs(symbol), 1);
          break;
	default:
	  _giza_point (xd, yd);
	  break;
	}
    }
}

/**
 * Draws a small circle at x, y
 */
static void
_giza_point (double x, double y)
{
  cairo_arc (context, x, y, 1.0, 0., 2.* M_PI);
  cairo_fill (context);
  _giza_stroke();
}

/**
 * Draw a rectangle centred at x, y
 */
static void
_giza_rect (double x, double y, int fill)
{
  cairo_rectangle (context, x - markerHeight * 0.5, y - markerHeight * 0.5, markerHeight,
		   markerHeight);
  if (fill) { cairo_fill(context); }

}

/**
 * Draw a plus centred at x, y
 */
static void
_giza_plus (double x, double y)
{
  cairo_move_to (context, x - markerHeight * 0.5, y);
  cairo_line_to (context, x + markerHeight * 0.5, y);
  cairo_move_to (context, x, y - markerHeight * 0.5);
  cairo_line_to (context, x, y + markerHeight * 0.5);
  _giza_stroke ();
}

/**
 * Draws a hollow circle at x, y
 */
static void
_giza_circle (double x, double y)
{
  cairo_arc (context, x, y, markerHeight * 0.5, 0., 2. * M_PI);
  _giza_stroke ();
}

/**
 * Draws a hollow circle at x, y, with size and fill arguments
 */
static void
_giza_circle_size (double x, double y, double size, int fill)
{
  cairo_arc (context, x, y, size * markerHeight * 0.5, 0., 2. * M_PI);
  if (fill) { cairo_fill(context); }
  _giza_stroke ();
}

/**
 * Draws a downward pointing triangle at x, y, either hollow or solid
 */
static void
_giza_triangle(double x, double y, int fill)
{
  cairo_move_to (context, x - markerHeight * 0.5, y - markerHeight * 0.5);
  cairo_line_to (context, x + markerHeight * 0.5, y - markerHeight * 0.5);
  cairo_line_to (context, x, y + markerHeight * 0.5);
  cairo_close_path (context);
  if (fill) { cairo_fill(context); }
  _giza_stroke ();
}

/**
 * Draws a diamond at x, y, either hollow or solid
 */
static void
_giza_diamond(double x, double y, int fill)
{
  cairo_move_to (context, x - markerHeight*0.5, y );
  cairo_line_to (context, x, y + markerHeight * 0.625  );
  cairo_line_to (context, x + markerHeight*0.5, y );
  cairo_line_to (context, x, y - markerHeight * 0.625);
  cairo_close_path (context);
  if (fill) { cairo_fill(context); }
  _giza_stroke ();
}


/**
 * Draws a cross at x, y
 */
static void
_giza_cross (double x, double y)
{
  cairo_move_to (context, x, y);
  double dx = 1./sqrt(2.)*markerHeight;
  cairo_rel_move_to (context, -dx * 0.5, -dx * 0.5);
  cairo_rel_line_to (context, dx, dx);
  cairo_rel_move_to (context, -dx, 0);
  cairo_rel_line_to (context, dx, -dx);
  _giza_stroke ();
}

/**
 * Draws an arrow at x, y
 */
static void
_giza_arrow (double x, double y, double angle)
{
  double headwidth  = 0.25*markerHeight;
  double headlength = 0.25*markerHeight;
  double r = 0.5*markerHeight;
  double cosa = cos(angle); double sina = sin(angle);
  cairo_move_to (context, x - r*cosa, y - r*sina);
  cairo_line_to (context, x + r*cosa, y + r*sina);
  cairo_rel_line_to (context, - headlength*cosa + headwidth*sina, headwidth*cosa - headlength*sina);
  cairo_move_to (context, x + r*cosa, y + r*sina);
  cairo_rel_line_to (context, - headlength*cosa - headwidth*sina, -headwidth*cosa - headlength*sina);
  _giza_stroke ();
}


/**
 * Draws a general polygon at x, y
 */
static void
_giza_polygon (double x, double y, int nsides, int fill)
{
 // Define radius
 double r = 0.5 * markerHeight;

 // Set first vertex so that shape appears flat-bottomed
 double alpha = (0.5 + 1./nsides)* M_PI;
 cairo_move_to (context, x + r * cos(alpha), y + r * sin(alpha));

 // Define other vertexes
 double alpha_step = 2 * M_PI / nsides;
 int i;
 for (i = 1; i < nsides; i++)
 {
  alpha += alpha_step;
  cairo_line_to (context, x + r * cos(alpha), y + r * sin(alpha));
 }
 cairo_close_path(context);
 if (fill) { cairo_fill(context); }
  _giza_stroke ();

}

/**
 * Draws an n-pointed star at x,y
 */
static void
_giza_star (double x, double y, int npoints, double ratio, int fill)
{
 // Define outer and inner radius
 double r = 0.5 * markerHeight;
 double ri = ratio * r;

 // Set first vertex so that shape appears flat-bottomed
 double alpha = (0.5 + 1./npoints)* M_PI;
 cairo_move_to (context, x + r * cos(alpha), y + r * sin(alpha));

 // Define other vertexes
 double alpha_step = 2 * M_PI / npoints;
 int i;
 for (i = 1; i < npoints; i++)
 {
  alpha += 0.5*alpha_step;
  cairo_line_to (context, x + ri * cos(alpha), y + ri * sin(alpha));
  alpha += 0.5*alpha_step;
  cairo_line_to (context, x + r * cos(alpha), y + r * sin(alpha));
 }
 alpha += 0.5*alpha_step;
 cairo_line_to (context, x + ri * cos(alpha), y + ri * sin(alpha));
 cairo_close_path(context);
 if (fill) { cairo_fill(context); }
  _giza_stroke ();

}

/**
 * Draws the Unicode symbol at x, y.
 * NOTE: Only supports ASCII now.
 */
static void
_giza_char (int symbol, double x, double y)
{
  char str[4];
//  cairo_text_extents_t extents;
  if (symbol <= 127)
    {
      str[0] = (char) symbol;
      str[1] = '\0';
    }
  else
    {
      str[0] = 'A';
      str[1] = '\0';
    }
  _giza_drawchar(str,x,y);
}

/**
 * Draws a character at x, y.
 */
static void
_giza_drawchar (const char *str, double x, double y)
{
  cairo_text_extents_t extents;
  cairo_text_extents (context, str, &extents);
  /* 
   * use information about the glyph to centre
   * the character on the x, y position
   */
  cairo_move_to (context, x - 0.5*extents.width  - extents.x_bearing,
                          y - 0.5*extents.height - extents.y_bearing);
  cairo_show_text (context, str);
}