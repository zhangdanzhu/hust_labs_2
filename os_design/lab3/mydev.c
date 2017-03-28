#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/device.h>
#include <asm/uaccess.h>

#define MAJOR_NUM 168

int BUSY_SIG = 0;

struct mydev
{
	int len;
	char buffer[50];
	struct cdev cdev;
	loff_t *r_ppos;
}my_dev0;

static ssize_t mydev_open(struct inode *inode,struct file *file)
{
	struct mydev *my_dev;
	/*if (BUSY_SIG)
	{
		return EBUSY;
	}
	BUSY_SIG++;*/
	my_dev0.r_ppos = 0;
	printk("mydev open successfully!\n");
	my_dev = container_of(inode->i_cdev,struct mydev, cdev);
	file->private_data = my_dev;
	
	return 0;
}

static ssize_t mydev_release(struct inode *inode, struct file *file)
{
	/*if (!BUSY_SIG)
	{
		return -1;
	}
	BUSY_SIG = 0;*/
	return 0;
}

static ssize_t mydev_read(struct file *file, char __user *buf_usr, size_t size, loff_t *ppos)
{
	int n, ret;
	char *buf_ker;

	struct mydev *my_dev = file->private_data;
	
	printk("read *ppos : %lld\n", *ppos);

	if (*ppos == my_dev->len)
	{
		return 0;
	}

	if (size > (my_dev->len - *ppos))
	{
		n = my_dev->len - *ppos;
	}
	else
	{
		n = size;
	}

	printk("n = %d\n", n);

	buf_ker = my_dev->buffer + *ppos;

	ret = copy_to_user(buf_usr, buf_ker, n);
	if (ret != 0)
	{
		return EFAULT;
	}

	*ppos += n;

	printk("mydev read successfully\n");
	// return n;
}

static ssize_t mydev_write(struct file *file, const char __user *buf_usr, size_t size, loff_t *ppos)
{
	int n, ret;
	char *buf_ker;
	struct mydev *my_dev = file->private_data;

	printk("write *ppos : %lld\n", *ppos);

	// 已经到buf尾部
	if (*ppos == sizeof(my_dev->buffer))
	{
		return -1;
	}

	if (size > (sizeof(my_dev->buffer) - *ppos))
	{
		n = sizeof(my_dev->buffer) - *ppos;
	}
	else
	{
		n = size;
	}

	buf_ker = my_dev->buffer + *ppos;

	ret = copy_from_user(buf_ker, buf_usr, n);
	if (ret != 0)
	{
		return EFAULT;
	}
	// 更新文件位置指针的值
	*ppos += n;
	
	// 更新my_dev.len
	my_dev->len += n;

	printk("mydev write successfully\n");
}

static const struct file_operations mydev_fops = {
	.owner = THIS_MODULE,
	.read = mydev_read,
	.write = mydev_write,
//	.ioctl = mydev_ioctl,
	.open = mydev_open,
	.release = mydev_release
};

int __init mydev_init(void)
{
	int ret;
	dev_t dev_num;

	// 初始化字符设备
	cdev_init(&my_dev0.cdev, &mydev_fops);
	// 设备号(主设备号12bit，次设备号20bit)
	dev_num = MKDEV(MAJOR_NUM,0);

	// 注册设备号
	ret = register_chrdev_region(dev_num,1,"my_dev");
	if (ret < 0) // 若静态注册失败则改用动态注册
	{
		ret = alloc_chrdev_region(&dev_num, 0, 1, "my_dev");
		if (ret < 0)
		{
			printk("Fail to register_chrdev_region!\n");
			return EIO;
		}
	}

	// 添加设备到操作系统
	ret = cdev_add(&my_dev0.cdev, dev_num, 1);
	if (ret < 0)
	{	
		printk("Fail to cdev_add");
		goto unregister_chrdev;
	}

	printk("Register my_dev to system successfully!\n");
	return 0;

	unregister_chrdev:
	unregister_chrdev_region(dev_num, 1);
}

void __exit mydev_exit(void)
{
	// 从系统中删除设备的字符设备
	cdev_del(&my_dev0.cdev);
	// 释放设备申请号
	unregister_chrdev_region(MKDEV(MAJOR_NUM,0), 1);
	printk("Exit my_dev successfully!\n");
	return ;
}

module_init(mydev_init);
module_exit(mydev_exit);
