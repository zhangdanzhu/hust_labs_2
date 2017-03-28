#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>


static char buffer[256];
GtkWidget *label;



static gboolean get_time(GtkWidget *widget)
{
	time_t cur_time;
	struct tm *loc_time;

	cur_time = time(NULL);
	loc_time = localtime(&cur_time);
	sprintf(buffer,"now time : %04d-%02d-%02d %02d:%02d:%02d\n",loc_time->tm_year+1900, loc_time->tm_mon+1,loc_time->tm_mday,loc_time->tm_hour, loc_time->tm_min, loc_time->tm_sec);
	gtk_label_set_text(GTK_LABEL(label), buffer);
	return TRUE;
}


int main(int argc, char **argv)
{
	gtk_init(&argc,&argv);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	label = gtk_label_new("time");
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
//	GtkWidget *box = gtk_vbox_new(TRUE,100);

//	gtk_container_add(GTK_CONTAINER(window),box);

	gtk_window_set_title(GTK_WINDOW(window),"Now Time");
	gtk_widget_set_usize(window, 300, 300); // 设置窗口大小

	
	gtk_container_add(GTK_CONTAINER(window),label);
	g_timeout_add(1000, (GSourceFunc) get_time, (gpointer) window);
	
	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
