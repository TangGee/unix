
#define NLLOC 10


/**
 * 每次申请10个
 */
static void client_alloc(void)
{
	int i;

	if(client ==NULL)
		client=malloc(NLLOC *sizeof(Client));
	else 
		client = realloc(client,(client_size+NLLOC)*sizeof(Client));
	if(client==NULL)
		err_sys("can't to initialize the new client array");
	for(i=0;i<client_size+NLLOC;i++)
		client[i].fd=-1;

	client_size+=NLLOC;

	
}



int 
client_add(int fd, uid_t uid)
{
	int i;

	if(client==NULL)
		client_alloc();
again:
	for(i=0;i<client_size;i++){
		if(client[i].fd==-1){
			client[i].fd=fd;
			client[i].uid=uid;
			return i;
		}

	}

	client_alloc();
	goto again;

}

void client_del(int fd)
{
	int i;

	for(i=0;i<client_size;i++){
		if(client[i].fd==fd){
			client[i].fd=-1;
			return ;
		}

	}
	log_quit("can't find client entry for fd %d",fd);
}
