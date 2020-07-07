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
#include "editor.h"

#define CHUNK 1024 /* read 1024 bytes at a time */

void		 rosetta_editor_setup(ScintillaObject *, char *);
void		 rosetta_editor_show_default_text(ScintillaObject *);

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

  // Set the save point, clear undos
  SSM(SCI_SETSAVEPOINT, 0, 0); /* For Scintilla there are no more edits in the buffer */
  SSM(SCI_EMPTYUNDOBUFFER, 0, 0); /* If you press CTRL+Z happens nothing */
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
    /* Set the save point, clear undos */
    SSM(SCI_SETSAVEPOINT, 0, 0); /* For Scintilla there are no more edits in the buffer */
    SSM(SCI_EMPTYUNDOBUFFER, 0, 0); /* If you press CTRL+Z happens nothing */
  }
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
    /* Set the save point, clear undos */
    SSM(SCI_SETSAVEPOINT, 0, 0); /* For Scintilla there are no more edits in the buffer */
    SSM(SCI_EMPTYUNDOBUFFER, 0, 0); /* If you press CTRL+Z happens nothing */
  }
}
