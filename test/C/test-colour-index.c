/* giza - a scientific plotting library built on cairo
 *
 * Copyright (c) 2010      James Wetter and Daniel Price
 * Copyright (c) 2010-2012 Daniel Price
 *
 * This library is free software; and you are welcome to redistribute
 * it under the terms of the GNU General Public License
 * (GPL, see LICENSE file for details) and the provision that
 * this notice remains intact. If you modify this file, please
 * note section 5a) of the GPLv3 states that:
 *
 *  a) The work must carry prominent notices stating that you modified
 *  it, and giving a relevant date.
 *
 * This software is distributed "AS IS", with ABSOLUTELY NO WARRANTY.
 * See the GPL for specific language governing rights and limitations.
 *
 * The Original code is the giza plotting library.
 *
 * Contributor(s):
 *      James Wetter <wetter.j@gmail.com>
 *      Daniel Price <daniel.price@monash.edu> (main contact)
 */

#include <giza.h>

int
main ()
{
  giza_open_device ("?", "colours");

  double xpts[2], ypts[2];

  xpts[0] = 0.;
  xpts[1] = 1.;

  int i;
  for (i = 1; i < 20; i++)
    {
      giza_set_colour_index (i);

      ypts[0] = 0.02 * i;
      ypts[1] = 0.02 * i;

      giza_line (2, xpts, ypts);

    }

  giza_close_device ();

}
