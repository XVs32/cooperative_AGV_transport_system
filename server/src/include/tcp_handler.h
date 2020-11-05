#ifndef TCP_HANDLER
#define TCP_HANDLER

#include <queue>
using namespace std;

typedef struct m_TCP_adapter_arg{
    int* sockfd;
    int max_client;
}TCP_adapter_arg;

extern queue<u_int32_t>  tcp_lis_buf;

void* TCP_accept_adapter(void *);
void* TCP_linstener_adapter(void *);

void TCP_server_init(int*, int);
void TCP_accept(int*, int);
void TCP_linstener(int *);

#endif