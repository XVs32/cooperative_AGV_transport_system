#ifndef IPC_HANDLER
#define IPC_HANDLER

int ipc_accept(int listenfd);
int ipc_listen();
int ipc_connect();

int ipc_recv(int token, char* buf, int size);
int ipc_int_recv(int token, int *data);

int ipc_send(int token, char* buf, int size);
int ipc_int_send(int token, int data);

void ipc_clear(int token);


int serv_listen(const char *name);

int cli_conn(const char *name);

#endif