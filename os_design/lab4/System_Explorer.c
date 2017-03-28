#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <math.h>

#define MAX_PROC 100	// 能显示的最大进程数量
#define PATH_LEN_MAX 50	// 路径名最大长度
#define NAME_LEN_MAX 256	// status中项名最大长度
// status中各信息行
#define LINE_NAME 1
#define LINE_STATE 2
#define LINE_PID 5
#define LINE_RSS 21

GtkWidget *window;		// 主窗口
GtkWidget *time_label;	// 时间显示

static gchar temp_sel_name[256] = {0};
static gchar *old_sel_name = NULL;


typedef struct _proc_info_st{
	char name[256];
	char state[20];
	char pid[20];
	char rss[20];
	char priority[20];
}proc_info;


void CreateMenuBar(GtkWidget *vbox);	
void Add_notebook(GtkWidget *vbox);
static gboolean get_time(GtkWidget *widget);
void CreateProPage(GtkWidget *notebook);
void CreateCpuPage(GtkWidget *notebook);
void CreateMemPage(GtkWidget *notebook);
static gboolean pro_ref(GtkWidget *clist);
static gboolean cpu_ref(GtkWidget *cpu_label);
static gboolean mem_ref(GtkWidget *mem_label);
void ReadLine(FILE *fp, char *buff, int bufflen, int line);
void sel_func(GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data);

int main(int argc, char **argv)
{
	GtkWidget *vscrollbar, *hscrollbar;	// 定义滚动条
	GtkWidget *main_vbox;
	GtkWidget *status_bar, *menu_bar;
	GtkWidget *file_menu;
	GtkWidget *notebook;
	
	gtk_init(&argc, &argv);				// 初始化在任何构件生成之前完成
	
	// 构建主窗口
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);			// 创建主窗口
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); 	// 设置退出响应
	gtk_window_set_title(GTK_WINDOW(window),"任务管理器");	// 设置窗口标题
	gtk_widget_set_usize(window, 600, 600);					// 设置窗口大小 
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); 		// 窗口居中显示
	gtk_window_set_resizable (GTK_WINDOW (window), TRUE);	// 设置窗口大小可改变
	gtk_container_set_border_width(GTK_CONTAINER(window),5);// 设置窗口边框宽度
	gtk_widget_show(window);

	main_vbox = gtk_vbox_new(FALSE, 1);	// 不强制使用相同大小，构件间隔1像素
	gtk_container_set_border_width(GTK_CONTAINER(main_vbox), 5);
	gtk_container_add(GTK_CONTAINER(window), main_vbox);
	gtk_widget_show(main_vbox);

	// 添加笔记本构件
	notebook = gtk_notebook_new();	
	gtk_box_pack_start(GTK_BOX(main_vbox),notebook,TRUE,TRUE,0); 
	gtk_widget_show(notebook);
	CreateProPage(notebook);
	CreateCpuPage(notebook);
	CreateMemPage(notebook);
	
	// 底部显示时间
	time_label = gtk_label_new("当前时间");
	gtk_box_pack_end(GTK_BOX(main_vbox), time_label, FALSE, FALSE, 0);
	g_timeout_add(1000, (GSourceFunc)get_time, (gpointer)window);  

	gtk_widget_show_all(window);	
	gtk_main();

}

/*void sel_func(GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data)
{
	printf("****\n");
	gtk_clist_get_text(GTK_CLIST(clist), row, 1, &old_sel_name);
	strcpy(temp_sel_name, old_sel_name);
	printf("%s\n", temp_sel_name);
}*/

void CreateProPage(GtkWidget *notebook)
{
	GtkWidget *frame = gtk_frame_new("Process Info");
	gchar *col_name[6] = {"PID", "进程名", "状态", "内存占用", "优先级"};	// 分栏列表列名
	GtkWidget *clist = gtk_clist_new_with_titles(5, col_name);	// 创建分栏列表
	GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);	// 创建滚动窗

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
//	gtk_signal_connect(GTK_OBJECT(clist), "select_row", GTK_SIGNAL_FUNC(sel_func), NULL); // 选中行时做处理

	gtk_clist_set_column_width(GTK_CLIST(clist), 0, 80);
	gtk_clist_set_column_width(GTK_CLIST(clist), 1, 145);
	gtk_clist_set_column_width(GTK_CLIST(clist), 2, 60);
	gtk_clist_set_column_width(GTK_CLIST(clist), 3, 100);
	gtk_clist_set_column_width(GTK_CLIST(clist), 4, 100);

	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (scroll),clist);	// 分栏列表放入滚动窗 

	g_timeout_add(1000, (GSourceFunc)pro_ref, (gpointer)clist);

	gtk_container_set_border_width(GTK_CONTAINER(frame), 10);	// frame设置
	gtk_widget_set_size_request(frame, 100, 335);
	gtk_container_add(GTK_CONTAINER(frame), scroll);

	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame, gtk_label_new("进程信息"));

}

static gboolean pro_ref(GtkWidget *clist)
{
	int i;
	DIR *dir = NULL;
	FILE *fp = NULL;
	struct dirent *ptr;
	char path[PATH_LEN_MAX];
	char name[NAME_LEN_MAX];
	proc_info pro_inf;
	char file[512] = {0};
	char buff[1024] = {0};
	//int cur_row = 0;

	gtk_clist_clear(GTK_CLIST(clist));
	dir = opendir("/proc");
	
	gtk_clist_freeze((GtkCList *)clist);

	while (ptr = readdir(dir))
	{
		if ((ptr->d_name[0] >= '0') && (ptr->d_name[0] <= '9'))	// 数字目录为进程信息
		{
			sprintf(path,"/proc/%s/status", ptr->d_name);
			if (!(fp = fopen(path, "r")))
			{	
				printf("Fail to open file %s !\n", path);
				return ;
			}

			// 获取NAME(第1行)
			fgets(buff, 1024, fp);
			sscanf(buff, "%s %s", name, pro_inf.name);
/*			if (!strcmp(temp_sel_name, pro_inf.name))
			{
				printf("!!\n");
				gtk_clist_select_row(GTK_CLIST(clist), cur_row, 0);
			}
			cur_row++;*/
			// 获取STATE(第2行)
			fgets(buff, 1024, fp);
			sscanf(buff, "%s %s", name, pro_inf.state);
			// 获取PID(第5行)
			for (i = 3; i <= 5; i++)
			{
				fgets(buff, 1024, fp);
			}
			sscanf(buff, "%s %s", name, pro_inf.pid);
			// 获取RSS(第21行)
			for (i = 6; i <= 21; i++)
			{
				fgets(buff, 1024, fp);
			}
			if (buff[0] == 'V')
			{
				sscanf(buff, "%s %s", name, pro_inf.rss);
			}
			else
			{
				strcpy(pro_inf.rss, "0");
			}
			fclose(fp);

			sprintf(file, "/proc/%s/stat", ptr->d_name);                  
			if(!(fp = fopen(file, "r")))
			{
				printf("read file %s wrong! exit\n", path);
				return ;
			}
			fgets(buff, 1024, fp);
			sscanf(buff, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s", pro_inf.priority);
			fclose(fp);
						
			gchar *list[1][6] = {{pro_inf.pid, pro_inf.name, pro_inf.state, pro_inf.rss, pro_inf.priority}};
			gtk_clist_append((GtkCList*) clist, list[0]);
		}
	}
	gtk_clist_thaw((GtkCList *) clist);
	closedir(dir);
	return TRUE;
}

void GetInfo(char *path, char *name, char info[])
{
	int fd = open(path, O_RDONLY);
	char temp[1000];
	int k = 0;
	read(fd, temp, sizeof(temp));
	close(fd);
	char *p = NULL;
	p = strstr(temp, name);
	while(*p != ':')
	p++;
	//p = p + 2;
	p++;
	while(*p == ' ')
	p++;
	while(*p != '\n')
	{
		info[k] = *p;
		p++;
		k++;
	}
	info[k] = '\n';
	info[k+1] = '\0';
}



static gboolean cpu_ref(GtkWidget *label)
{
	FILE *fp;
	char buf[128];
	char name[5];
	char cpuBuffer[2000];
	char buffer[50];
	long int user,nice,system,idle;
	float ratio;
	fp = fopen("/proc/stat","r");
	fgets(buf,sizeof(buf),fp);
	sscanf(buf,"%s%ld%ld%ld%ld",name,&user,&nice,&system,&idle);
	ratio = 100*(float)(user+nice+system)/(float)(user+nice+system+idle);
	sprintf(buffer,"cpu使用率：%%%.6f",ratio);



	char modeName[50], cpuMHz[20], cacheSize[20], cpuCores[20], addrSize[50];
	char *p;
	GetInfo("/proc/cpuinfo", "model name", modeName);
	GetInfo("/proc/cpuinfo", "cache size", cacheSize);
	GetInfo("/proc/cpuinfo", "cpu MHz", cpuMHz);
	GetInfo("/proc/cpuinfo", "cpu cores", cpuCores);
	GetInfo("/proc/cpuinfo", "address sizes", addrSize);
	strcpy(cpuBuffer, "\nCPU型号和主频:\n");
	p = strtok(modeName, "\n");
	strcat(cpuBuffer, p);
	strcat(cpuBuffer, "\n\n寻址位数:");
	p = strtok(addrSize, "\n");
	strcat(cpuBuffer, p);
	strcat(cpuBuffer, "\n\ncpu 主频:");
	p = strtok(cpuMHz, "\n");
	strcat(cpuBuffer, p);
	strcat(cpuBuffer, " MHz");
	strcat(cpuBuffer, "\n\nCache 大小:");
	p = strtok(cacheSize, "\n");
	strcat(cpuBuffer, p);
	strcat(cpuBuffer, "\ncpu核数:");
	p = strtok(cpuCores, "\n");
	strcat(cpuBuffer, p);

	strcat(cpuBuffer, "\n");
	strcat(cpuBuffer, buffer);

	gtk_label_set_text(GTK_LABEL(label), cpuBuffer);
	return TRUE;
}

static gboolean mem_ref(GtkWidget *mem_label)
{
	char buffer0[120];
	char data[30];

	long total = 0, free = 0;
	int counter = 0;
	int fd;
	int i = 0;

	char *buffer;

	fd = open("/proc/meminfo", O_RDONLY);
	read(fd, buffer0, 100);
	buffer = buffer0;

	while (1)
	{
		if (':' == buffer[i])
		{
			counter++;
			i += 2;
			buffer += i;
			i = 0; 
			while ('k' != buffer[i])
			{
				i++;
			}
			buffer[i] = '\0';
			if (1 == counter)
			{
				total = atol(buffer)/1024;
				i += 3;
				buffer += i;
				i = 0;
			}
			else if (2 == counter)
			{
				free = atol(buffer)/1024;	
				break;
			}
				
		}
		else
		{
			i++;
		}
	}

	sprintf(data,"MemTotal:%ldM\nMemFree:%ldM",total,free);
	close(fd);	

	gtk_label_set_text(GTK_LABEL(mem_label), data);

	return TRUE;
}

void CreateCpuPage(GtkWidget *notebook)
{
	GtkWidget *frame = gtk_frame_new("CPU Info");
	gtk_container_set_border_width(GTK_CONTAINER(frame), 10);
	gtk_widget_set_size_request(frame, 100, 335);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame, gtk_label_new("CPU信息"));
	
	GtkWidget *cpu_label = gtk_label_new("cpu");
	g_timeout_add(1000, (GSourceFunc)cpu_ref, (gpointer)cpu_label);
	gtk_container_add(GTK_CONTAINER(frame), cpu_label);

}

void CreateMemPage(GtkWidget *notebook)
{
	GtkWidget *frame = gtk_frame_new("Memory Info");
	gtk_container_set_border_width(GTK_CONTAINER(frame), 10);
	gtk_widget_set_size_request(frame, 100, 335);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame, gtk_label_new("内存信息"));
	
	GtkWidget *mem_label = gtk_label_new("mem");
	g_timeout_add(1000, (GSourceFunc)mem_ref, (gpointer)mem_label);
	gtk_container_add(GTK_CONTAINER(frame), mem_label);
	
}

static gboolean get_time(GtkWidget *widget)
{
	time_t cur_time;
	struct tm *loc_time;
	static char buffer[256];
	cur_time = time(NULL);
	loc_time = localtime(&cur_time);
	sprintf(buffer,"当前时间 : %04d-%02d-%02d %02d:%02d:%02d\n",loc_time->tm_year+1900, loc_time->tm_mon+1,loc_time->tm_mday,loc_time->tm_hour, loc_time->tm_min, loc_time->tm_sec);
	gtk_widget_set_size_request(time_label, 200, 20);
	//gtk_label_set_justify(GTK_LABEL(time_label), GTK_JUSTIFY_RIGHT);	// 设置对齐方式(试了无效)
	gtk_misc_set_alignment(GTK_MISC(time_label), 1, 0);	// 设置对齐(左右、上下)
	gtk_label_set_text(GTK_LABEL(time_label), buffer);

	return TRUE;
}

