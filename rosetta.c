/*
 * A new IDE
 * Copyright (C) 2020  Alessandro Iezzi <info@alessandroiezzi.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "rosetta.h"
#include <stdlib.h>

Rosetta
*rosetta_init(GtkApplication *app, int argc, char **argv)
{
  Rosetta *rosetta = (Rosetta *) malloc(sizeof(Rosetta));
  rosetta->app = app;
  rosetta->argc = argc;
  rosetta->argv = argv;

  return rosetta;
}
