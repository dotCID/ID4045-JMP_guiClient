#include <gtk/gtk.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
namespace local{
#include <arduino-serial-lib.c>
}

int sockfd;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

static void initTCP(int argc, char* argv[]){
	int portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
   //
    //close(sockfd);
    //return 0;	
}

static void sendTCP(char *message){
	char buf[256];
	int n;
	n = write(sockfd,buf,strlen(message));
    if (n < 0) error("ERROR writing to socket");
    bzero(buf,256);
    n = read(sockfd,buf,255);
    if (n < 0) error("ERROR reading from socket");
    printf("%s\n",buf);
}


static void exit_program(GtkWidget* widget, gpointer data){
	close(sockfd);
	gtk_main_quit();
}

static void button_clicked(GtkWidget* callerButton, gpointer data) //automatically get passed when called
{
	char buf[256];
	
	
	g_print("Running Arduino command.\n");
	if(strcmp(gtk_button_get_label(GTK_BUTTON(callerButton)), "Do some fancy blinking") == 0)
	{	
		//sendTCP((char *)"blink");
	}else if(strcmp(gtk_button_get_label(GTK_BUTTON(callerButton)), "Turn LED on") == 0)
	{
		sendTCP((char *)"on");
	}else if(strcmp(gtk_button_get_label(GTK_BUTTON(callerButton)), "Turn LED off") == 0)
	{
		sendTCP((char *)"off");
	}	

	g_print("End of attempt.\n");
}

int main(int argc, char* argv[])
{
	initTCP(argc, argv);
	gtk_init(&argc, &argv);

	GtkWidget *window, *vbox, *button;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	button = gtk_button_new_with_label("Do some fancy blinking"); // this creates a button with a label widget already in it
	vbox = gtk_vbox_new(false, false);
	g_signal_connect(window, "delete-event", G_CALLBACK(exit_program), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
	
	gtk_widget_set_size_request(button, 200,30);
	gtk_box_pack_start(GTK_BOX(vbox), button, false,false, false);
	
	button = gtk_button_new_with_label("Turn LED on");
	g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, false,false, false);
	
	button = gtk_button_new_with_label("Turn LED off");
	g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, false,false, false);
	
	gtk_widget_set_size_request(window, 400, 400);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_window_set_title(GTK_WINDOW(window), "Blink example");
	gtk_widget_show_all(window);

	system("clear");
	gtk_main();

	return 0;
}
