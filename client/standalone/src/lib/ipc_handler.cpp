#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "ipc_handler.h"
#include "log.h"

#define QLEN 10
#define TMP_PATH "/var/tmp/" 

int mos_ipc[2];
int qr_ipc;
int camera_ipc;
int command_ipc;


void ipc_clear(int token){
	int tmp;
	while(recv(token, &tmp, sizeof(int), MSG_DONTWAIT)>0){}
	
	#ifdef DEBUG_LV_2
		write_log("ipc_clear");
	#endif
	return;
}

int ipc_accept(int listenfd){
	
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

int ipc_listen(){
	return serv_listen("ipc_socket");
}

int serv_listen(const char *name){
	
	int fd, len, err, rval;
	struct sockaddr_un un;

	/* create a UNIX domain stream socket */
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		printf("Error: Socket creat fail. Keep going.\n");
		return(-1);
	}
	unlink(name);/* in case socket file already exists */

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



int ipc_connect(){
	return cli_conn("ipc_socket");
}

int ipc_recv(int token, void* buf, int size){
	int ret = read(token, buf, size);
	#ifdef DEBUG_LV_2
		write_log("DEBUG: ipc_recv");
	#endif
	return ret;
}

int ipc_send(int token, void* buf, int size){
	
	int ret = write(token, buf, size);
	#ifdef DEBUG_LV_2
		write_log("DEBUG: ipc_send");
	#endif
	return ret;
}

int ipc_int_send(int token, int data){
	int ret = write(token, &data, sizeof(int));
	#ifdef DEBUG_LV_2
		write_log("DEBUG: ipc_int_send");
	#endif
	return ret;
}

int ipc_int_recv(int token, int *data){
	int ret = read(token, data, sizeof(int));
	#ifdef DEBUG_LV_2
		write_log("DEBUG: ipc_int_send");
	#endif
	return ret;
}

/*
 * Create a client endpoint and connect to a server.
 * Returns fd if all OK, <0 on error.
 */
int cli_conn(const char *name){
	
	int fd, len, err, rval;
	struct sockaddr_un un;
	
	/* create a UNIX domain stream socket */
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		write_log("Error: Socket creat fail. Keep going.");
		return -1;
	}
	
	// fill socket address structure with our address
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	sprintf(un.sun_path, "%s%05d", TMP_PATH, getpid());// +5 for pid = 14 chars 
	len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	
	unlink(un.sun_path);//in case it already exists
	
	bind(fd, (struct sockaddr *)&un, len);
	
	/* fill socket address structure with server's address */
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
	
	int retry = 30;
	while(retry--){
		if (connect(fd, (struct sockaddr *)&un, len) >= 0){
			break;
		}
		sleep(1);
	}
	
	return fd;
}

