#include <stdlib.h>
#include "../fevent/frixia_events_queue.h"
#include "../fevent/frixia_event.h"
#include "../filedescriptor/reader/filedescriptor_reader.h"
#include "../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../callback_suite/frixia_callback_main_function.h"
#include "../callback_suite/callback_data/frixia_callback_entry.h"

#include "frixia_thread_pool.h"

void thread_main_loop(frixia_thread_pool_data_t *data)
{
    frixia_events_queue_t     *q  = data->tasks;
    frixia_epoll_t            *ep = data->fepoll;
    while(true)
    {
        frixia_event_t *e = frixia_events_queue_pop(q);
        int event_fd = e->fd;
        if(event_fd == 4)
        {
            break;
        }

        printf("thread main loop!!!%d start iteration\n",event_fd);        

        frixia_fd_t *frixia_fd = search_fepoll(data->fepoll,event_fd);
        if(frixia_fd == NULL)
        {
            printf("frixia_fd == NULL!!!%d  continuing...\n",event_fd);
            continue;
        }  
        int read_size = frixia_fd->read_dim;
        
        FRIXIA_SUPPORTED_PROTOCOL_T p = NO_PROTOCOL;
        simple_list_elem_t *curr      = data->frixia_callbacks->events_callbacks->first;
        while(curr != NULL)
        {
            frixia_callback_entry_t *fcb_curr = (frixia_callback_entry_t *)curr->val;
            curr                              = curr->next;
            if(fcb_curr->fd == event_fd)
            {
                p = fcb_curr->protocol;
                break;
            }
            curr = curr->next;
        }

        frixia_callback_main(e,p,read_size,data->frixia_callbacks);
        printf("thread main loop!!!%d stop  iteration\n",event_fd);
    }

    printf("Exited.\n");
}

frixia_thread_pool_t* create_frixia_thread_pool(int n,frixia_epoll_t *fepoll, frixia_events_queue_t *events,frixia_callbacks_data_structure_t *cbs)
{
    frixia_thread_pool_t *ptr = malloc(sizeof(frixia_thread_pool_t));
    if(ptr == NULL)
    {
        printf("ERROR CREATING FRIXIA THREAD POOL POINTER\n");
        return NULL;
    }

    ptr->tasks = NULL;


    frixia_thread_pool_t *t = (frixia_thread_pool_t*)malloc(n*sizeof(frixia_thread_pool_t));
    if (t == NULL)
    {
        printf("ERROR CREATING thread_pool_t");
        return NULL;
    }

    pthread_t *threads = (pthread_t*)malloc(n*sizeof(pthread_t));
    frixia_events_queue_t **queues = (frixia_events_queue_t **)malloc(n*sizeof(frixia_events_queue_t *));
    for(int i=0;i<n;i++)
    {
        pthread_t th;
        frixia_events_queue_t *q = frixia_events_queue_create();
        frixia_thread_pool_data_t *tpdata = create_frixia_thread_pool_data();
        set_frixia_thread_pool_data_events(tpdata,events);
        set_frixia_thread_pool_data_thread_tasks(tpdata,q);
        set_frixia_thread_pool_data_fepoll(tpdata,fepoll);
        set_frixia_thread_pool_data_callbacks(tpdata,cbs);
        int exit_code = pthread_create(&th,
                                        NULL,
                                        (void *)&thread_main_loop,
                                        tpdata);
        if(exit_code != 0) { exit(-1);}
        threads[i] = th;
        queues[i] = q;
    }
    ptr->th = threads;
    ptr->threads_tasks = queues;
    return ptr;
}

void set_frixia_thread_pool_tasks(frixia_thread_pool_t *ptr, frixia_events_queue_t *tasks)
{
    ptr->tasks = tasks;
}

