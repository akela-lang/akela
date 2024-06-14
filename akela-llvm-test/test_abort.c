#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

jmp_buf env;

void on_sigabrt (int signum)
{
    signal (signum, SIG_DFL);
    longjmp (env, 1);
}

void try_and_catch_abort (void (*func)(void))
{
    if (setjmp (env) == 0) {
        signal(SIGABRT, &on_sigabrt);
        (*func)();
        signal (SIGABRT, SIG_DFL);
        printf("after");
    }
    else {
        printf("aborted\n");
    }
}

void do_stuff_aborted ()
{
    printf("one\n");
    abort();
    printf("two\n");
}

void do_stuff ()
{
    printf("three\n");
    printf("four\n");
}

int main()
{
    try_and_catch_abort (&do_stuff_aborted);
    try_and_catch_abort (&do_stuff);
}