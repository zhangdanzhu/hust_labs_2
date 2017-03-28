#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>


static char buffer[256];
GtkWidget *label;

static gboolean get_cpu(GtkWidget *widget)
{
	FILE *fp;
	char buf[128];
	char name[5];
	long int user,nice,system,idle;
	float ratio;
	fp = fopen("/proc/stat","r");
	fgets(buf,sizeof(buf),fp);
	sscanf(buf,"%s%ld%ld%ld%ld",name,&user,&nice,&system,&idle);
	ratio = 100*(float)(user+nice+system)/(float)(user+nice+system+idle);
	sprintf(buffer,"cpu使用率：%%%.6f",ratio);
	gtk_label_set_text(GTK_LABEL(label), buffer);
	return TRUE;
}


int main(int argc, char **argv)
{
	gtk_init(&argc,&argv);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	label = gtk_label_new("cpu");
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);

	gtk_window_set_title(GTK_WINDOW(window),"Cpu");
	gtk_widget_set_usize(window, 300, 300); // 设置窗口大小

	gtk_container_add(GTK_CONTAINER(window),label);
	g_timeout_add(3000, (GSourceFunc) get_cpu, (gpointer) window);
	
	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
