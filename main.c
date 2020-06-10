#include <gtk/gtk.h>

#include <Scintilla.h>
#include <SciLexer.h>
#include <ScintillaWidget.h>

#define UNUSE_ATTR	__attribute__((unused))

static void sci_setup(ScintillaObject *sci) {
	scintilla_set_id(sci, 0);
#define SSM(m, w, l) scintilla_send_message(sci, (m), (w), (l))

	SSM(SCI_SETCODEPAGE, SC_CP_UTF8, 0);
	SSM(SCI_SETIMEINTERACTION, SC_IME_WINDOWED, 0);
	//SSM(SCI_SETIMEINTERACTION, SC_IME_INLINE, 0);

	SSM(SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, SC_CHARSET_DEFAULT);
	SSM(SCI_STYLESETFONT, STYLE_DEFAULT, (sptr_t)"Monospace");
	SSM(SCI_STYLESETSIZEFRACTIONAL, STYLE_DEFAULT, 11*SC_FONT_SIZE_MULTIPLIER);
	SSM(SCI_STYLECLEARALL, 0, 0);

	SSM(SCI_SETTABWIDTH, 4, 0);
	// show line number margin
	SSM(SCI_SETMARGINWIDTHN, 0, (int)SSM(SCI_TEXTWIDTH, STYLE_LINENUMBER, (sptr_t)" 99"));
	// hide bookmark and selection margin
	SSM(SCI_SETMARGINWIDTHN, 1, 0);
	// word wrap
	SSM(SCI_SETWRAPMODE, SC_WRAP_WORD, 0);
	SSM(SCI_SETWRAPVISUALFLAGS, SC_WRAPVISUALFLAG_END, 0);
	SSM(SCI_SETWRAPINDENTMODE, SC_WRAPINDENT_INDENT, 0);

	SSM(SCI_SETLEXER, SCLEX_CPP, 0);
	SSM(SCI_SETKEYWORDS, 0, (sptr_t)"int char return");
	SSM(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x0080ff);
	SSM(SCI_STYLESETFORE, SCE_C_COMMENT, 0x008000);
	SSM(SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x008000);
	SSM(SCI_STYLESETFORE, SCE_C_NUMBER, 0x808000);
	SSM(SCI_STYLESETFORE, SCE_C_WORD, 0xff0000);
	SSM(SCI_STYLESETFORE, SCE_C_STRING, 0x800080);
	SSM(SCI_STYLESETBOLD, SCE_C_OPERATOR, TRUE);
	SSM(SCI_SETPROPERTY, (uptr_t)"styling.within.preprocessor", (sptr_t)"1");

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

	char buf[128];
	int length = snprintf(buf, sizeof(buf) - 1, "// GTK %d.%d.%d\n", GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);
	SSM(SCI_APPENDTEXT, length, (sptr_t)buf);
}

static void setup_scintilla_window(GtkWidget *window) {
	gtk_window_set_title(GTK_WINDOW(window), "Scintilla GTK Test");
	gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);

	GtkWidget *editor = scintilla_new();
	gtk_container_add(GTK_CONTAINER(window), editor);
	sci_setup(SCINTILLA(editor));

	gtk_widget_show_all(window);
	gtk_widget_grab_focus(editor);
}

#if GTK_CHECK_VERSION(3, 0, 0)
// Hello World in GTK3
// https://developer.gnome.org/gtk3/stable/gtk-getting-started.html
static void app_activate(GtkApplication *app, UNUSE_ATTR gpointer user_data) {
	GtkWidget *window = gtk_application_window_new(app);
	setup_scintilla_window(window);
}

int main(int argc, char **argv) {
	GtkApplication *app = gtk_application_new("org.scintilla.example.gtk", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}

#else
// Hello World in GTK2
// https://developer.gnome.org/gtk-tutorial/stable/c39.html
static int exit_app(UNUSE_ATTR GtkWidget *w, UNUSE_ATTR GdkEventAny *e, UNUSE_ATTR gpointer p) {
	gtk_main_quit();
	return 1;
}

int main(int argc, char **argv) {
	gtk_init(&argc, &argv);
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(exit_app), NULL);

	setup_scintilla_window(window);
	gtk_main();

	return 0;
}

#endif
