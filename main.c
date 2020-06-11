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
#include <gtk/gtk.h>
#include "window.h"

#define UNUSE_ATTR	__attribute__((unused))

static void	 app_activate(GtkApplication *app, gpointer user_data);

const char *app_title = "Rosetta IDE v0.0.0";

/*
 * Entry point
 */
int main(int argc, char **argv) {
  int status;
  GtkApplication *app = gtk_application_new("rosetta.ide", G_APPLICATION_FLAGS_NONE);

  if (argc <= 1) {
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
  } else if (argc == 2) {
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), argv[1]);
  } else {
    fprintf(stderr, "Too many arguments\n");
    return 1;
  }

  /* status = g_application_run(G_APPLICATION(app), argc, argv); */
  status = g_application_run(G_APPLICATION(app), 0, NULL);
  g_object_unref(app);

  return status;
}

static void app_activate(GtkApplication *app, gpointer data) {
  rosetta_window_new(app, app_title, (char *) data);
}
