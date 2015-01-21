#include <gtk/gtk.h>
#include <string>
#include <cstring>

static void menu_response(GtkWidget *menu_item, gpointer data)
{
	if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)), "Exit") == 0)
	{
		gtk_main_quit();
	}else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)), "About") == 0)
	{
		g_print("About dialog here\n");
	}
}

int main(int argc, char* argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget *window, *event_box, 
			  *menu_bar, *menu_item, *file_menu, *help_menu, *vbox,
		 	  *notebook, *label, *label2;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
	
	vbox = gtk_vbox_new(false, false);
	
	/* Notebook */
	notebook = gtk_notebook_new();
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook),GTK_POS_TOP);

	std::string s = "This is where the connection settings will be set";
	label = gtk_label_new(s.c_str()); //converts to c characters?
	gtk_box_pack_start(GTK_BOX(vbox), label, true, false, 0);
	
	s="Settings";
	label2 = gtk_label_new(s.c_str()); //converts to c characters?		
	
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),vbox, label2);
	
	//gtk_box_pack_start(GTK_BOX(vbox), notebook, true, false, 0);
	/* End of notebook */
	
	
	gtk_container_add(GTK_CONTAINER(window),notebook);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

