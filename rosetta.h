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
#ifndef __ROSETTA_H__
#define __ROSETTA_H__

#include <gtk/gtk.h>

#include <Scintilla.h>
#include <SciLexer.h>
#include <ScintillaWidget.h>

#define UNUSED_ATTR	__attribute__((unused))

typedef struct Rosetta {
  int			  argc;
  char			**argv;
  GtkApplication	 *app;
  GtkWidget		 *window;
  GtkWidget 		 *menubar;
  GtkWidget 		 *vbox;
  GtkWidget 		 *hbox;
  GtkWidget		 *editor;
} Rosetta;

Rosetta		*rosetta_init(GtkApplication *, int, char **);

#endif
