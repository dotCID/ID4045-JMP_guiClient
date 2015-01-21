/* 	A gui client for the http://www.linuxhowtos.org/C_C++/socket.htm server client 
	for the purpose of JMP. GUI was created with help of YouTube user "thecplusplusguy"'s videos.
	@author Marien Wolthuis */
	
#include <gtk/gtk.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

GtkWidget *cdetIn[3]; // holds connection details

const char* cdet[3][2] = {{"localhost", "Server address"},
						  {"4547", "Server port"}};
						  
const char* commArd[5][2] = {{"blink\n", "Blink LED 13"},
							 {"on\n", "Turn on LED 13"},
							 {"off\n", "Turn off LED 13"},
							 {"extend\n", "Extend the arm"},
							 {"retract\n", "Retract the arm"}};
							 
const char* commSer[2][2] = {{"quit\n", "Close the server"}, 	// it's a zombie that won't die. Dummy 
							 {"echo\n", "Echo this message"}};	

void error(const char *msg)
{
    perror(msg);
    //exit(0);
}

int TCPMessage(const char* message)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    
    portno = atoi(cdet[1][0]);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    server = gethostbyname(cdet[0][0]);
    
    if (server == NULL)
        error("ERROR, no such host\n");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
        
    n = write(sockfd,message,strlen(message));
    if (n < 0) 
         error("ERROR writing to socket");
         
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
         
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}				 


static void setting_response(GtkWidget *button, gpointer data)
{
	const char *label = gtk_button_get_label(GTK_BUTTON(button));
	for(int i=0;i<2;i++){
		if(strcmp(label, cdet[i][1]) ==0){
			cdet[i][0] = gtk_entry_get_text(GTK_ENTRY(data));
			g_print("%s%s%s%s\n", "Set ", cdet[i][0], " as ", cdet[i][1]);
		}		
	}
	//g_print("%s\n", "Setting response completed");
}

static void command_response(GtkWidget *button, gpointer data){
	const char *label = gtk_button_get_label(GTK_BUTTON(button));
	for(int i=0;i<5;i++){
		if(strcmp(label, commArd[i][1]) ==0){
			TCPMessage(commArd[i][0]);
			//g_print("%s\n", commArd[i][0]);
		}else if(i<2 && strcmp(label, commSer[i][1]) ==0){
			TCPMessage(commSer[i][0]);
			//g_print("%s\n", commSer[i][0]);
		}
	}
	//g_print("%s\n", "Command response completed");
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
	for(int i=0;i<2;i++){
		hbox = gtk_hbox_new(false, false);
		button = gtk_button_new_with_label(cdet[i][1]);
		gtk_widget_set_size_request(button, 140, 20);
		cdetIn[i] = gtk_entry_new();
		gtk_entry_set_text(GTK_ENTRY(cdetIn[i]), cdet[i][0]);
		
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
		button = gtk_button_new_with_label(commArd[i][1]);
		gtk_widget_set_size_request(button, 140, 35);
	
		gtk_box_pack_start(GTK_BOX(hbox), button, false, false, 0);
		g_signal_connect(button, "clicked", G_CALLBACK(command_response), &commArd[i][1]);

		if(i<2){
			button = gtk_button_new_with_label(commSer[i][1]);
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
	gtk_window_set_title(GTK_WINDOW(window), "JMP Client software, v0.7");
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

