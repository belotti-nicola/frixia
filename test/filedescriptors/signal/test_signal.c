#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <frixia/frixia.h>

#define TEST_SIGNAL FSIGNAL_INT

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
    
    res = frixia_add_signal(fenv,TEST_SIGNAL);
    if( !frixia_result_is_ok(res) )
    {
        perror("Error adding signal\n");
        return -1;
    }
    int fd = frixia_result_fd(res);
    frixia_register_cb(fenv,fd,TEST_CALLBACK,NULL);

    frixia_start(fenv);
    frixia_environment_destroy(fenv);
    return 0;
}

int main() {
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);

    //MANDATORY PART, UNSKIPPABLE
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, TEST_SIGNAL);    
    int ret = pthread_sigmask(SIG_BLOCK, &mask, NULL);
    if (ret != 0) 
    {
        printf("Error blocking TEST_SIGNAL\n");
        return -1;
    } 

    pid_t pid = fork();

    if (pid == 0) {
        return test();
    }

    sleep(2);
    kill(pid, TEST_SIGNAL);

    int status;
    waitpid(pid, &status, 0);

    if (WIFSIGNALED(status)) 
    {
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