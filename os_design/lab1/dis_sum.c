#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>


static char buffer[256];
GtkWidget *label;

static gboolean get_sum(GtkWidget *widget)
{
	static int sum = 0;
	static int i = 1;
	
	if (i <= 100)
	{
		sum = sum+i;
		i++;
		sprintf(buffer,"累加和：%d",sum);
		gtk_label_set_text(GTK_LABEL(label), buffer);
	}
	return TRUE;
}


int main(int argc, char **argv)
{
	gtk_init(&argc,&argv);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	label = gtk_label_new("sum");
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);

	gtk_window_set_title(GTK_WINDOW(window),"Sum");
	gtk_widget_set_usize(window, 300, 300); // 设置窗口大小

	gtk_container_add(GTK_CONTAINER(window),label);
	g_timeout_add(3000, (GSourceFunc) get_sum, (gpointer) window);
	
	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
