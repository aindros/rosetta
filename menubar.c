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
#include "menubar.h"
#include "dialog.h"

#define UNUSED_ATTR	__attribute__((unused))

GtkWidget	*rosetta_menu_bar_file_menu_new(Rosetta *);
static void	 quit_menu_activated(gpointer *);
static void	 save_menu_activated(gpointer *);

GtkWidget
*rosetta_menu_bar_new(Rosetta *rosetta)
{
  GtkWidget *file_menu;

  rosetta->menubar = gtk_menu_bar_new();
  file_menu = rosetta_menu_bar_file_menu_new(rosetta);

  gtk_menu_shell_append(GTK_MENU_SHELL(rosetta->menubar), file_menu);

  return rosetta->menubar;
}

GtkWidget
*rosetta_menu_bar_file_menu_new(Rosetta *rosetta)
{
  GtkWidget *file_menu = gtk_menu_new();

  GtkWidget *file_menu_item = gtk_menu_item_new_with_mnemonic("_File");
  GtkWidget *save_menu_item = gtk_menu_item_new_with_mnemonic("_Save");
  /* GtkWidget *saveas_menu_item = gtk_menu_item_new_with_mnemonic("S_ave as"); */
  GtkWidget *quit_menu_item = gtk_menu_item_new_with_mnemonic("_Quit");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_menu_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_menu_item);

  /* Add events */
  g_signal_connect_swapped(quit_menu_item, "activate",
                           G_CALLBACK (quit_menu_activated), 
                           (gpointer) rosetta);
  g_signal_connect_swapped(save_menu_item, "activate",
                           G_CALLBACK (save_menu_activated), 
                           (gpointer) rosetta);

  return file_menu_item;
}

static void
quit_menu_activated(gpointer *data)
{
  Rosetta *rosetta = (Rosetta *) data;
  g_application_quit(G_APPLICATION(rosetta->app));
}

static void
save_menu_activated(gpointer *data)
{
  Rosetta *rosetta = (Rosetta *) data;
  rosetta_save_file_dialog(rosetta);
}
