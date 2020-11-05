#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h> 

#define QLEN 10

int serv_listen(const char *name){
	
	int fd, len, err, rval;
	struct sockaddr_un un;

	/* create a UNIX domain stream socket */
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		printf("Error: Socket creat fail. Keep going.\n");
		return(-1);
	}
	unlink(name);   /* in case socket file already exists */

	/* fill in socket address structure */
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(name);

	/* bind the name to the descriptor */
	bind(fd, (struct sockaddr *)&un, len);
	listen(fd, QLEN);
	return(fd);
}


int serv_accept(int listenfd){
	
	int clifd, len, err, rval;
	time_t staletime;
	struct sockaddr_un un;
	struct stat         statbuf;

	len = sizeof(un);
	if ((clifd = accept(listenfd, (struct sockaddr *)&un, (socklen_t*)&len)) < 0){
		return(-1);     /* often errno=EINTR, if signal caught */
	}
	
	/* obtain the client's uid from its calling address */
	len -= offsetof(struct sockaddr_un, sun_path); /* len of pathname */
	un.sun_path[len] = 0;           /* null terminate */
	unlink(un.sun_path);        /* we're done with pathname now */
	
	return(clifd);
}

#define TMP_PATH    "/var/tmp/"      /* +5 for pid = 14 chars */

/*
 * Create a client endpoint and connect to a server.
 * Returns fd if all OK, <0 on error.
 */
void* cli_conn(void *arg){
	
	const char *name = (char*)arg;
	
	int fd, len, err, rval;
	struct sockaddr_un un;
	int retry = 30;

	/* create a UNIX domain stream socket */
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		printf("Error: Socket creat fail. Keep going.\n");
		pthread_exit(0);
	}

		//return(-1);

	/* fill socket address structure with our address */
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	sprintf(un.sun_path, "%s%05d", TMP_PATH, getpid());
	len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	
	unlink(un.sun_path);        /* in case it already exists */
	
	bind(fd, (struct sockaddr *)&un, len);
	
	/* fill socket address structure with server's address */
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
	
	while(retry--){
		if (connect(fd, (struct sockaddr *)&un, len) >= 0) {
			printf("connected\n"); 
			break;
		}
		sleep(1);
	}
	
	if(retry == 0){
		rval = -4;
		goto cli_errout;
	}
	
	retry = 0;
	while(1){
		write (fd, &retry,sizeof(int));
		retry++;
		sleep(1);
		write (fd, &retry,sizeof(int));
		retry++;
		sleep(1);
	}
	
	pthread_exit(0);
	//return(fd);

cli_errout:
	err = errno;
	close(fd);
	errno = err;
	pthread_exit(0);
	//return(rval);
}


int main(void)
{
	char arg[]="hello";
	
	int serv_fd = serv_listen(arg);
	
	pthread_t t_client;
	int pth;
	pth = pthread_create(&t_client, NULL, cli_conn, (void*)arg);
	if(pth<0){
		printf("cannot create thread 't_mos_reader_0', exit\n");
		exit(1);
	}
	

	int cli_fd =serv_accept(serv_fd);
	
	printf("cli_fd %d\n",cli_fd);
	
	/*if(send(cli_fd,"hello from server",sizeof(char)*18,0)<0){
		printf("Error: Fail to send data. exit.");
		//exit(0);
	}*/
	printf("send done");
	int buf;
	while(1){
		//close(cli_fd);
		//open(cli_fd);
		read(cli_fd, &buf, sizeof(int));
		printf("recv done %d\n",buf);
		sleep(1);
		
		sleep(1);
	}

	
	exit(0);
}