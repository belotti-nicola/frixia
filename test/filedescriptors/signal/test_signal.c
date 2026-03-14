#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <frixia/frixia.h>

void *TEST_CALLBACK(FRIXIA_CALLBACK_CTX *ctx)
{
    frixia_environment_t *fenv = ctx->fenv;
    frixia_stop(fenv);
    return NULL;
}

int test()
{
    FRIXIA_RESULT res;
    frixia_environment_t *fenv = frixia_environment_create(10);
    
    res = frixia_add_signal(fenv,FSIGNAL_TERM);
    if( !frixia_result_is_ok(res) )
    {
        perror("Error adding signal");
        return -1;
    }
    int fd = frixia_result_fd(res);
    frixia_register_cb(fenv,fd,TEST_CALLBACK,NULL);

    frixia_start(fenv);
    frixia_environment_destroy(fenv);
    return 0;
}

int main() {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    
    int ret = pthread_sigmask(SIG_BLOCK, &mask, NULL);
    if (ret != 0) 
    {
        printf("Error blocking sigint\n");
        return -1;
    }


    setbuf(stderr, NULL);
    setbuf(stdout, NULL);


    pid_t pid = fork();

    if (pid == 0) {
        return test();
    }

    sleep(2);
    kill(pid, SIGINT);

    int status;
    waitpid(pid, &status, 0);

    if (WIFSIGNALED(status)) {
        printf("Process killed by signal: %d\n", WTERMSIG(status));
        return 1;
    }

    if (!WIFEXITED(status))
    {
        printf("Error WIFEXITED");
        return 1;
    }
    if (WEXITSTATUS(status) != 0)
    {
        printf("Error WEXITSTATUS");
        return 1;
    }

    return 0;
}