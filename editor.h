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
#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "rosetta.h"

#define SSM(m, w, l) scintilla_send_message(sci, (m), (w), (l))

GtkWidget	*rosetta_editor_new(char *);
void		 rosetta_editor_save_file(ScintillaObject *, char *);
void		 rosetta_editor_open_file(ScintillaObject *, char *);

#endif
