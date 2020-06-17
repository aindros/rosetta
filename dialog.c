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
#include "dialog.h"
#include "editor.h"

void
rosetta_save_file_dialog(Rosetta *rosetta)
{
  GtkWidget *dialog;
  GtkFileChooser *chooser;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Save File",
                                        GTK_WINDOW(rosetta->window),
                                        action,
                                        "_Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "_Save",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);
  chooser = GTK_FILE_CHOOSER (dialog);

  gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

  /*
  if (user_edited_a_new_document)
    gtk_file_chooser_set_current_name (chooser, "Untitled document");
  else
    gtk_file_chooser_set_filename (chooser,
                                   existing_filename);
  */
  gtk_file_chooser_set_current_name (chooser, "Untitled document");

  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT) {
    char *filename;

    filename = gtk_file_chooser_get_filename (chooser);

    rosetta_editor_save_file(SCINTILLA(rosetta->editor), filename);

    g_free (filename);
  }

  gtk_widget_destroy (dialog);
}
