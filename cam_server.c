
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include<stdio.h>      /*标准输入输出定义*/
#include<stdlib.h>     /*标准函数库定义*/
#include<unistd.h>     /*Unix 标准函数定义*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>      /*文件控制定义*/
#include<termios.h>    /*PPSIX 终端控制定义*/
#include<errno.h>      /*错误号定义*/
#include<string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <signal.h>

#define	REQBUFS_COUNT	4

struct cam_buf {
	void *start;
	size_t length;
};

struct v4l2_requestbuffers reqbufs;
struct cam_buf bufs[REQBUFS_COUNT];

//摄像头初始化
int camera_init(char *devpath, unsigned int *width, unsigned int *height, unsigned int *size, unsigned int *ismjpeg)
{
	int i;
	int fd = -1;
	int ret;
	struct v4l2_buffer vbuf;
	struct v4l2_format format;
	struct v4l2_capability capability;
	/*open 打开设备文件*/
	if((fd = open(devpath, O_RDWR)) == -1){ //用阻塞模式打开打开摄像头设备
		perror("open:");	
		return -1;		
	}
	/*ioctl 查看支持的驱动*/
	ret = ioctl(fd, VIDIOC_QUERYCAP, &capability);//返回当前设备所支持的功能
	if (ret == -1) {
		perror("camera->init");
		return -1;
	}
	/*判断设备是否支持视频采集*/
	if(!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "camera->init: device can not support V4L2_CAP_VIDEO_CAPTURE\n");
		close(fd);
		return -1;
	}
	/*判断设备是否支持视频流采集*/
	if(!(capability.capabilities & V4L2_CAP_STREAMING)) {
		fprintf(stderr, "camera->init: device can not support V4L2_CAP_STREAMING\n");
		close(fd);
		return -1;
	}
	/*设置捕获的视频格式 MYJPEG*/

	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	format.fmt.pix.width = *width;
	format.fmt.pix.height = *height;
	format.fmt.pix.field = V4L2_FIELD_ANY;
	ret = ioctl(fd, VIDIOC_S_FMT, &format);
	if(ret == -1) {
		perror("camera init");
		return -1;
	} else {
		*ismjpeg = 0;
		fprintf(stdout, "camera->init: picture format is mjpeg\n");
		goto get_fmt;
	}
	/*设置捕获的视频格式 YUYV*/
	memset(&format, 0, sizeof(format));
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;			//永远都是这个类型
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;		//设置采集图片的格式
	format.fmt.pix.width = *width;
	format.fmt.pix.height = *height;
	format.fmt.pix.field = V4L2_FIELD_ANY;				//设置图片一行一行的采集
	ret = ioctl(fd, VIDIOC_S_FMT, &format);				//ioctl	是设置生效
	if(ret == -1)
		perror("camera init");
	else {
		fprintf(stdout, "camera->init: picture format is yuyv\n");
		*ismjpeg = 1;
		goto get_fmt;
	}

get_fmt:
	ret = ioctl(fd, VIDIOC_G_FMT, &format);
	if (ret == -1) {
		perror("camera init");
		return -1;
	}
	/*向驱动申请缓存*/
	memset(&reqbufs, 0, sizeof(struct v4l2_requestbuffers));
	reqbufs.count	= REQBUFS_COUNT;					//缓存区个数
	reqbufs.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbufs.memory	= V4L2_MEMORY_MMAP;					//设置操作申请缓存的方式:映射 MMAP
	ret = ioctl(fd, VIDIOC_REQBUFS, &reqbufs);			
	if (ret == -1) {	
		perror("camera init");
		close(fd);
		return -1;
	}
	/*循环映射并入队*/
	for (i = 0; i < reqbufs.count; i++){
		/*真正获取缓存的地址大小*/
		memset(&vbuf, 0, sizeof(struct v4l2_buffer));
		vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		vbuf.memory = V4L2_MEMORY_MMAP;
		vbuf.index = i;
		ret = ioctl(fd, VIDIOC_QUERYBUF, &vbuf);
		if (ret == -1) {
			perror("camera init");
			close(fd);
			return -1;
		}
		/*映射缓存到用户空间,通过mmap讲内核的缓存地址映射到用户空间,并切和文件描述符fd相关联*/
		bufs[i].length = vbuf.length;
		bufs[i].start = mmap(NULL, vbuf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, vbuf.m.offset);
		if (bufs[i].start == MAP_FAILED) {
			perror("camera init");
			close(fd);
			return -1;
		}
		/*每次映射都会入队,放入缓冲队列*/
		vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		vbuf.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(fd, VIDIOC_QBUF, &vbuf);//VIDIOC_QBUF投放一个空的视频缓冲区到视频缓冲区中
		if (ret == -1) {
			perror("camera init");
			close(fd);
			return -1;
		}
	}
	/*返回真正设置成功的宽.高.大小*/
	*width = format.fmt.pix.width;
	*height = format.fmt.pix.height;
	*size = bufs[0].length;

	return fd;
}

//开始采集
int camera_start(int fd)
{
	int ret;
	
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;//采集窗口参数，设定采集窗口范围
	/*ioctl控制摄像头开始采集*/
	ret = ioctl(fd, VIDIOC_STREAMON, &type);//ioctl设置视频属性
	if (ret == -1) {
		perror("camera->start");
		return -1;
	}
	fprintf(stdout, "camera->start: start capture\n");

	return 0;
}

//出队
int camera_dqbuf(int fd, void **buf, unsigned int *size, unsigned int *index)
{
	int ret;
	fd_set fds;
	struct timeval timeout;
	struct v4l2_buffer vbuf;

	while (1) {
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;
		ret = select(fd + 1, &fds, NULL, NULL, &timeout);	//使用select机制,保证fd有图片的时候才能出对
		if (ret == -1) {
			perror("camera->dbytesusedqbuf");
			if (errno == EINTR)
				continue;
			else
				return -1;
		} else if (ret == 0) {
			fprintf(stderr, "camera->dqbuf: dequeue buffer timeout\n");
			return -1;
		} else {
			vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			vbuf.memory = V4L2_MEMORY_MMAP;
			ret = ioctl(fd, VIDIOC_DQBUF, &vbuf);	//出队,也就是从用户空间取出图片
			if (ret == -1) {
				perror("camera->dqbuf");
				return -1;
			}
			*buf = bufs[vbuf.index].start;
			*size = vbuf.bytesused;
			*index = vbuf.index;

			return 0;
		}
	}
}

//入队
int camera_eqbuf(int fd, unsigned int index)
{
	int ret;
	struct v4l2_buffer vbuf;

	vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	vbuf.memory = V4L2_MEMORY_MMAP;
	vbuf.index = index;
	ret = ioctl(fd, VIDIOC_QBUF, &vbuf);		//入队
	if (ret == -1) {
		perror("camera->eqbuf");
		return -1;
	}

	return 0;
}

//停止采集
int camera_stop(int fd)
{
	int ret;
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	ret = ioctl(fd, VIDIOC_STREAMOFF, &type);
	if (ret == -1) {
		perror("camera->stop");
		return -1;
	}
	fprintf(stdout, "camera->stop: stop capture\n");

	return 0;
}

//退出
int camera_exit(int fd)
{
	int i;
	int ret;
	struct v4l2_buffer vbuf;
	vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	vbuf.memory = V4L2_MEMORY_MMAP;
	for (i = 0; i < reqbufs.count; i++) {
		ret = ioctl(fd, VIDIOC_DQBUF, &vbuf);
		if (ret == -1)
			break;
	}
	for (i = 0; i < reqbufs.count; i++)
		munmap(bufs[i].start, bufs[i].length);
	fprintf(stdout, "camera->exit: camera exit\n");
	return close(fd);
}

int main()
{
	
	//实现服务器创建流程
	int servFd = socket(PF_INET, SOCK_STREAM, 0);
	if (servFd < 0)
	{
		perror("socket error!");
		return -1;
	}
	printf("socket ok!\n");
	//bind
	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;//PF_INET;
	servAddr.sin_port = htons(8888);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int res = bind(servFd, (struct sockaddr *)&servAddr
			, sizeof(servAddr));
	if (res < 0)
	{
		perror("bind error!");
		return -1;
	}
	printf("bind ok!\n");
	//listen
	res = listen(servFd, 5);
	if (res < 0)
	{
		perror("listen error!");
		return -1;
	}
	printf("listening.....\n");
	//accept
	int i;
	int fd;
	int ret;
	unsigned int width;
	unsigned int height;
	unsigned int size;
	unsigned int index;
	unsigned int ismjpeg;

	width = 320;
	height = 240;

	//初始化摄像头
	fd = camera_init("/home/linux/Desktop/-1d59181be817c5c3.png", &width, &height,&size, &ismjpeg);
	if (fd < 0)
	{
		perror("camera_init error");
       // return -1;
	}
		
	
	//开始采集   
	ret = camera_start(fd);
	{
		if (ret < 0)
		{
			perror("camera->start error");
			return -1;
		}
	}
	// 采集几张图片丢弃 
	char *jpeg_ptr = NULL;
	for (i = 0; i < 8; i++) {
		
		//出队
		//char *buff[1024];
		ret = camera_dqbuf(fd,(void**)(&jpeg_ptr), &size, &index);
		{
			if (ret == -1)
			{
               exit(EXIT_FAILURE);
			}
				
		}

		//入队
		ret = camera_eqbuf(fd, index);
		if (ret == -1)
		{
			exit(EXIT_FAILURE);

		}
			
	}
	signal(SIGPIPE, SIG_IGN);
	fprintf(stdout, "init camera success\n");
	while(1)
	{
		printf("wating!\n");
		char buf[1024];
		int clientfd = accept(servFd, NULL, NULL);
		//等待客户端连接
		
		if(clientfd < 0)
		{
			perror("accept");
			return 4;
		}

		/* 循环采集图片 */
		while (1) {
			//出队
			ret = camera_dqbuf(fd,(void **)&jpeg_ptr,&size,&index);
			if (ret == -1)
			{
				perror("dqbuf");
				return -1;
			}

			char sizes[10] = {0};
			sprintf(sizes, "%d", size);
			int ret = send(clientfd, buf, sizeof(size), 0);//发送照片大小
			if(-1 == ret || 0 == ret)
			{
				close(clientfd);
				ret = camera_eqbuf(fd, index);
				break;
			}
			//		printf("write size: %d\n", ret);
	        ret = send(clientfd,buf,sizeof(buf),0);//发送照片内容
			if(-1 == ret || 0 == ret)
			{
				close(clientfd);
				ret = camera_eqbuf(fd, index);
				break;
			}

			ret = camera_eqbuf(fd, index); // 入队
			if (ret == -1)
			{
				perror("eqbuf");
				return -1;
			}
	//		usleep(100000);
		}
		printf("ended !\n");

	}
	close(fd);
	close(servFd);
	
	    //停止采集
	ret = camera_stop(fd);
	if (ret == -1)
	{
        return -1;
	}
			

		//退出
		ret = camera_exit(fd);
		if (ret == -1)
		{
            return -1;
		}
			
}

