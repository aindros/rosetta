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
#include "window.h"
#include "menubar.h"
#include "editor.h"

#include <stdlib.h>

GtkWidget
*rosetta_window_new(Rosetta *rosetta, const char *app_title)
{
  char *filename = rosetta->argc >= 1 ? rosetta->argv[1] : NULL;
  rosetta->editor = rosetta_editor_new(filename);

  /* Main window */
  rosetta->window = gtk_application_window_new(rosetta->app);

  /* Containers */
  rosetta->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  rosetta->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  /* Menubar */
  rosetta_menu_bar_new(rosetta);

  gtk_window_set_title(GTK_WINDOW(rosetta->window), app_title);
  gtk_window_set_default_size(GTK_WINDOW(rosetta->window), 760, 490);
  
  gtk_container_add(GTK_CONTAINER(rosetta->window), rosetta->vbox);

  gtk_box_pack_start(GTK_BOX(rosetta->vbox), rosetta->menubar, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(rosetta->vbox), rosetta->hbox, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(rosetta->hbox), rosetta->editor, TRUE, TRUE, 0);

  gtk_widget_show_all(rosetta->window);
  gtk_widget_grab_focus(rosetta->editor);

  return rosetta->window;
}

