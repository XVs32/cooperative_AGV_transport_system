#ifndef TCP_HANDLER
#define TCP_HANDLER

typedef struct m_TCP_adapter_arg{
    int* sockfd;
    int max_client;
}TCP_adapter_arg;

typedef struct t_id_table{
    uint32_t socket;
    uint16_t team;
    uint16_t agv;
}id_table;

void* TCP_accept_adapter(void *);
void* TCP_linstener_adapter(void *);

void TCP_server_init(int*, int);
void TCP_accept(int*, int);
void TCP_linstener(id_table *);

#endif
