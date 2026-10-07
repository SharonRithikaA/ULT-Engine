#ifndef THREAD_H
#define THREAD_H

#include <windows.h>

#define MAX_THREADS 10
#define STACK_SIZE 65536

enum ThreadState
{
    READY,
    RUNNING,
    FINISHED
};

struct Thread
{
    int id;
    ThreadState state;
    void (*function)();
    LPVOID fiber;
    int execution_count;
};

extern Thread threads[MAX_THREADS];

extern int thread_count;
extern int current_thread;
extern int context_switches;

void thread_init();

int thread_create(void (*function)());

void thread_yield();

void thread_exit();

void thread_start();

void thread_join(int thread_id);

void thread_lock();

void thread_unlock();

void thread_status();

void show_statistics();

void thread_cleanup();

void thread_reset();

#endif