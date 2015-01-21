#include <gtk/gtk.h>
#include <string>
#include <cstring>

GtkWidget *cdetIn[3]; // holds connection details
std::string cdet[3][2] = {{"/dev/ttyACM3", "Arduino address"},
						  {"localhost", "Server address"},
						  {"4547", "Server port"}};
						  
std::string commArd[5][2] = {{"blink", "Blink LED 13"},
							 {"on", "Turn on LED 13"},
							 {"off", "Turn off LED 13"},
							 {"extend", "Extend the arm"},
							 {"retract", "Retract the arm"}};
							 
std::string commSer[2][2] = {{"quit", "Close the server"},
							 {"echo", "Echo this message"}};

static void setting_response(GtkWidget *button, gpointer data)
{
	const char *label = gtk_button_get_label(GTK_BUTTON(button));
	for(int i=0;i<3;i++){
		if(strcmp(label, cdet[i][1].c_str()) ==0){
			cdet[i][0] = gtk_entry_get_text(GTK_ENTRY(data));
			g_print("%s\n", cdet[i][0].c_str());
		}		
	}
	g_print("%s\n", "Setting response completed");
}

static void command_response(GtkWidget *button, gpointer data){
	const char *label = gtk_button_get_label(GTK_BUTTON(button));
	for(int i=0;i<5;i++){
		if(strcmp(label, commArd[i][1].c_str()) ==0){
			g_print("%s\n", commArd[i][0].c_str());
		}else if(i<2 && strcmp(label, commSer[i][1].c_str()) ==0){
			g_print("%s\n", commSer[i][0].c_str());
		}
	}
	g_print("%s\n", "Command response completed");
}


int main(int argc, char* argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget *window, *hbox, *button, *vbox,
			  *menu_bar, *menu_item, *file_menu, *help_menu, 
		 	  *notebook, *label, *label2, *separator;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
	
	vbox = gtk_vbox_new(false, false);
	
	notebook = gtk_notebook_new();
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook),GTK_POS_TOP);
	
	/* Settings page */
	for(int i=0;i < 3;i++){
		hbox = gtk_hbox_new(false, false);
		button = gtk_button_new_with_label(cdet[i][1].c_str());
		gtk_widget_set_size_request(button, 140, 20);
		cdetIn[i] = gtk_entry_new();
		gtk_entry_set_text(GTK_ENTRY(cdetIn[i]), cdet[i][0].c_str());
		
		gtk_box_pack_start(GTK_BOX(hbox), cdetIn[i], false, false, 0);
		gtk_box_pack_start(GTK_BOX(hbox), button, false, false, 0);
		g_signal_connect(button, "clicked", G_CALLBACK(setting_response), cdetIn[i]);
		gtk_box_pack_start(GTK_BOX(vbox), hbox, false, false, 0);
		
	}
	
	
	label = gtk_label_new("Settings");		
	
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),vbox, label);
	/* End of settings */
	
	/* Command page */
	vbox = gtk_vbox_new(false, 0);
	
	hbox = gtk_hbox_new(false, false);
	label = gtk_label_new("Arduino Commands");
	gtk_widget_set_size_request(label, 140, 25);
	label2 = gtk_label_new("Server Commands");
	gtk_widget_set_size_request(label2, 140, 25);
	gtk_box_pack_start(GTK_BOX(hbox), label, false, false , 0);
	gtk_box_pack_start(GTK_BOX(hbox), label2, false, false , 20);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, false, false, 0);
	
	hbox = gtk_hbox_new(false, false);
	separator = gtk_hseparator_new();
	gtk_widget_set_size_request(separator, 300, 10);
	gtk_box_pack_start(GTK_BOX(hbox), separator, false, false , 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, false, false, 0);
	
	for(int i=0;i < 5;i++){
		hbox = gtk_hbox_new(false, false);
		button = gtk_button_new_with_label(commArd[i][1].c_str());
		gtk_widget_set_size_request(button, 140, 35);
	
		gtk_box_pack_start(GTK_BOX(hbox), button, false, false, 0);
		g_signal_connect(button, "clicked", G_CALLBACK(command_response), &commArd[i][1]);

		if(i<2){
			button = gtk_button_new_with_label(commSer[i][1].c_str());
			gtk_widget_set_size_request(button, 140, 35);
			
			gtk_box_pack_start(GTK_BOX(hbox), button, false, false, 20);
			g_signal_connect(button, "clicked", G_CALLBACK(command_response), &commSer[i][1]);
		}
	

		gtk_box_pack_start(GTK_BOX(vbox), hbox, false, false, 0);
	}
	
	label = gtk_label_new("Commands");		
	
	gtk_notebook_prepend_page(GTK_NOTEBOOK(notebook),vbox, label);
	/* End of command */
	
	
	gtk_container_add(GTK_CONTAINER(window),notebook);
	gtk_widget_set_size_request(window, 500, 500);
	gtk_window_set_title(GTK_WINDOW(window), "JMP Client software, v0.6");
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

