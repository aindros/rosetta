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
#include "window.h"

#include <Scintilla.h>
#include <SciLexer.h>
#include <ScintillaWidget.h>

#include <stdlib.h>

#define UNUSED_ATTR	__attribute__((unused))
#define CHUNK 1024 /* read 1024 bytes at a time */
#define SSM(m, w, l) scintilla_send_message(sci, (m), (w), (l))

GtkWidget	*rosetta_menu_bar_new(GtkApplication *);
GtkWidget	*rosetta_menu_bar_file_menu_new(GtkApplication *);
GtkWidget	*rosetta_editor_new(char *);
void		 rosetta_editor_setup(ScintillaObject *, char *);
void		 rosetta_editor_show_default_text(ScintillaObject *);
void		 rosetta_editor_open_file(ScintillaObject *, char *);
void		 rosetta_editor_save_file(ScintillaObject *, char *);
static void	 quit_menu_activated(gpointer *);
static void	 save_menu_activated(gpointer *);

void open_fd();

ScintillaObject *sci;
GtkWidget *parent_window;

GtkWidget
*rosetta_window_new(GtkApplication *app, const char *app_title, char *filename)
{
  GtkWidget *window = gtk_application_window_new(app);
  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget *menubar = rosetta_menu_bar_new(app);
  GtkWidget *editor = rosetta_editor_new(filename);

  parent_window = window;

  sci = SCINTILLA(editor);

  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), editor, TRUE, TRUE, 0);

  gtk_window_set_title(GTK_WINDOW(window), app_title);
  gtk_window_set_default_size(GTK_WINDOW(window), 760, 490);

  gtk_widget_show_all(window);
  gtk_widget_grab_focus(editor);

  return window;
}

GtkWidget
*rosetta_menu_bar_new(GtkApplication *app)
{
  GtkWidget *menu_bar = gtk_menu_bar_new();
  GtkWidget *file_menu = rosetta_menu_bar_file_menu_new(app);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu);

  return menu_bar;
}

GtkWidget
*rosetta_menu_bar_file_menu_new(GtkApplication *app)
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
                           (gpointer) app);
  g_signal_connect_swapped(save_menu_item, "activate",
                           G_CALLBACK (save_menu_activated), 
                           (gpointer) NULL);

  return file_menu_item;
}

static void
quit_menu_activated(gpointer *data)
{
  g_application_quit(G_APPLICATION(data));
}

static void
save_menu_activated(UNUSED_ATTR gpointer *data)
{
  open_fd();
  rosetta_editor_save_file(sci, "test2.txt"); 
}

GtkWidget
*rosetta_editor_new(char *filename)
{
  GtkWidget *editor = scintilla_new();
  rosetta_editor_setup(SCINTILLA(editor), filename);

  return editor;
}

void
rosetta_editor_setup(ScintillaObject *sci, char *filename)
{
  scintilla_set_id(sci, 0);

  SSM(SCI_SETCODEPAGE, SC_CP_UTF8, 0);
  SSM(SCI_SETIMEINTERACTION, SC_IME_WINDOWED, 0);
  /* SSM(SCI_SETIMEINTERACTION, SC_IME_INLINE, 0); */

  SSM(SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, SC_CHARSET_DEFAULT);
  SSM(SCI_STYLESETFONT, STYLE_DEFAULT, (sptr_t)"Monospace");
  SSM(SCI_STYLESETSIZEFRACTIONAL, STYLE_DEFAULT, 8 * SC_FONT_SIZE_MULTIPLIER);
  SSM(SCI_STYLECLEARALL, 0, 0);

  SSM(SCI_SETTABWIDTH, 4, 0);
  SSM(SCI_SETUSETABS, FALSE, 0);

  /* show line number margin */
  SSM(SCI_SETMARGINWIDTHN, 0, (int)SSM(SCI_TEXTWIDTH, STYLE_LINENUMBER, (sptr_t)" 9999"));

  /* hide bookmark and selection margin */
  SSM(SCI_SETMARGINWIDTHN, 1, 0);

  /* word wrap */
  SSM(SCI_SETWRAPMODE, SC_WRAP_WORD, 0);
  SSM(SCI_SETWRAPVISUALFLAGS, SC_WRAPVISUALFLAG_END, 0);
  SSM(SCI_SETWRAPINDENTMODE, SC_WRAPINDENT_INDENT, 0);

  SSM(SCI_SETLEXER, SCLEX_CPP, 0);
  SSM(SCI_SETKEYWORDS, 0, (sptr_t)"int char double long return");
  SSM(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x0080ff);
  SSM(SCI_STYLESETFORE, SCE_C_COMMENT, 0x008000);
  SSM(SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x008000);
  SSM(SCI_STYLESETFORE, SCE_C_NUMBER, 0x808000);
  SSM(SCI_STYLESETFORE, SCE_C_WORD, 0xff0000);
  SSM(SCI_STYLESETFORE, SCE_C_STRING, 0x800080);
  SSM(SCI_STYLESETBOLD, SCE_C_OPERATOR, TRUE);
  SSM(SCI_SETPROPERTY, (uptr_t)"styling.within.preprocessor", (sptr_t)"1");

  if (filename != NULL) {
    rosetta_editor_open_file(sci, filename);
  } else {
    rosetta_editor_show_default_text(sci);
  }
}

void
rosetta_editor_show_default_text(ScintillaObject *sci)
{
  char buf[128];
  int length;

  SSM(SCI_INSERTTEXT, 0, (sptr_t)
      "#include <gtk/gtk.h>\n"
      "\n"
      "int main(int argc, char **argv) {\n"
      "	// show GTK version\n"
      "	printf(\"GTK %d.%d.%d\\n\", GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);\n"
      "	return 0;\n"
      "}\n"
      "\n"
  );
  length = snprintf(buf,
                    sizeof(buf) - 1,
                    "// GTK %d.%d.%d\n",
                    GTK_MAJOR_VERSION,
                    GTK_MINOR_VERSION,
                    GTK_MICRO_VERSION);

  SSM(SCI_APPENDTEXT, length, (sptr_t)buf);
}

void
rosetta_editor_open_file(ScintillaObject *sci, char *filename)
{
  char buf[CHUNK];
  FILE *file;
  size_t nread;

  file = fopen(filename, "r");
  if (file) {
    while ((nread = fread(buf, 1, sizeof buf, file)) > 0) {
      /* fwrite(buf, 1, nread, stdout); */
      SSM(SCI_APPENDTEXT, nread, (sptr_t)buf);
    }

    if (ferror(file)) {
      /* deal with error */
    }

    fclose(file);
  }
}

void
open_fd()
{
  GtkWidget *dialog;
  GtkFileChooser *chooser;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Save File",
                                        GTK_WINDOW(parent_window),
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

    rosetta_editor_save_file(sci, filename);

    g_free (filename);
  }

  gtk_widget_destroy (dialog);
}

void
rosetta_editor_save_file(ScintillaObject *sci, char *filename)
{
  FILE *file = fopen(filename, "w");
  int n_len = (int) SSM(SCI_GETLENGTH, 0, (sptr_t) NULL);
  char *buf = (char *) malloc(sizeof(char) * (n_len + 1));

  SSM(SCI_GETTEXT, n_len+1, (sptr_t)buf);
  
  if (file) {
    fwrite(buf, 1, n_len, file);
    if (ferror(file)) {
      /* deal with error */
    }

    fclose(file);
  }
}
