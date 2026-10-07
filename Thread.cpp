#include <iostream>
#include "Thread.h"

Thread threads[MAX_THREADS];

int thread_count = 0;
int current_thread = -1;
int context_switches = 0;

LPVOID main_fiber = nullptr;

bool mutex_locked = false;
int mutex_owner = -1;


// ==========================================
// THREAD INITIALIZATION
// ==========================================

void thread_init()
{
    thread_count = 0;
    current_thread = -1;
    context_switches = 0;

    mutex_locked = false;
    mutex_owner = -1;

    main_fiber = ConvertThreadToFiber(nullptr);

    if (main_fiber == nullptr)
    {
        std::cout << "Error: Could not initialize main fiber."
                  << std::endl;
        return;
    }

    for (int i = 0; i < MAX_THREADS; i++)
    {
        threads[i].id = -1;
        threads[i].state = FINISHED;
        threads[i].function = nullptr;
        threads[i].fiber = nullptr;
        threads[i].execution_count = 0;
    }
}


// ==========================================
// THREAD WRAPPER
// ==========================================

void WINAPI thread_wrapper(LPVOID parameter)
{
    int id = (int)(long long)parameter;

    if (id < 0 || id >= thread_count)
    {
        return;
    }

    if (threads[id].function != nullptr)
    {
        threads[id].function();
    }

    threads[id].state = FINISHED;

    thread_exit();
}


// ==========================================
// THREAD CREATE
// ==========================================

int thread_create(void (*function)())
{
    if (function == nullptr)
    {
        std::cout << "Error: Cannot create thread with empty function."
                  << std::endl;

        return -1;
    }

    if (thread_count >= MAX_THREADS)
    {
        std::cout << "Error: Maximum thread limit reached."
                  << std::endl;

        return -1;
    }

    if (main_fiber == nullptr)
    {
        std::cout << "Error: Thread system is not initialized."
                  << std::endl;

        return -1;
    }

    int id = thread_count;

    threads[id].id = id;
    threads[id].state = READY;
    threads[id].function = function;
    threads[id].execution_count = 0;

    threads[id].fiber = CreateFiber(
        STACK_SIZE,
        thread_wrapper,
        (LPVOID)(long long)id
    );

    if (threads[id].fiber == nullptr)
    {
        std::cout << "Error: Could not create fiber for Thread "
                  << id << "."
                  << std::endl;

        threads[id].id = -1;
        threads[id].state = FINISHED;
        threads[id].function = nullptr;

        return -1;
    }

    thread_count++;

    return id;
}


// ==========================================
// THREAD YIELD
// ==========================================

void thread_yield()
{
    if (current_thread < 0 ||
        current_thread >= thread_count)
    {
        return;
    }

    int old_thread = current_thread;

    int next_thread = -1;

    for (int i = 1; i <= thread_count; i++)
    {
        int index = (old_thread + i) % thread_count;

        if (threads[index].state == READY)
        {
            next_thread = index;
            break;
        }
    }

    if (next_thread == -1)
    {
        return;
    }

    threads[old_thread].state = READY;

    current_thread = next_thread;

    threads[current_thread].state = RUNNING;

    context_switches++;

    threads[current_thread].execution_count++;

    SwitchToFiber(threads[current_thread].fiber);
}


// ==========================================
// THREAD EXIT
// ==========================================

void thread_exit()
{
    if (current_thread < 0 ||
        current_thread >= thread_count)
    {
        return;
    }

    if (mutex_owner == current_thread)
    {
        mutex_locked = false;
        mutex_owner = -1;
    }

    threads[current_thread].state = FINISHED;

    int next_thread = -1;

    for (int i = 1; i <= thread_count; i++)
    {
        int index = (current_thread + i) % thread_count;

        if (threads[index].state == READY)
        {
            next_thread = index;
            break;
        }
    }

    if (next_thread == -1)
    {
        current_thread = -1;

        SwitchToFiber(main_fiber);

        return;
    }

    current_thread = next_thread;

    threads[current_thread].state = RUNNING;

    context_switches++;

    threads[current_thread].execution_count++;

    SwitchToFiber(threads[current_thread].fiber);
}


// ==========================================
// START SCHEDULER
// ==========================================

void thread_start()
{
    if (thread_count == 0)
    {
        std::cout << "Error: No threads available."
                  << std::endl;

        return;
    }

    int first_thread = -1;

    for (int i = 0; i < thread_count; i++)
    {
        if (threads[i].state == READY)
        {
            first_thread = i;
            break;
        }
    }

    if (first_thread == -1)
    {
        std::cout << "No READY threads available."
                  << std::endl;

        return;
    }

    current_thread = first_thread;

    threads[current_thread].state = RUNNING;

    threads[current_thread].execution_count++;

    SwitchToFiber(threads[current_thread].fiber);
}


// ==========================================
// THREAD JOIN
// ==========================================

void thread_join(int thread_id)
{
    if (thread_id < 0 || thread_id >= thread_count)
    {
        std::cout << "Error: Invalid thread ID."
                  << std::endl;

        return;
    }

    while (threads[thread_id].state != FINISHED)
    {
        thread_yield();
    }
}


// ==========================================
// MUTEX LOCK
// ==========================================

void thread_lock()
{
    if (current_thread < 0)
    {
        return;
    }

    while (mutex_locked && mutex_owner != current_thread)
    {
        thread_yield();
    }

    mutex_locked = true;
    mutex_owner = current_thread;

    std::cout << "[Mutex] Thread "
              << current_thread
              << " acquired the lock."
              << std::endl;
}


// ==========================================
// MUTEX UNLOCK
// ==========================================

void thread_unlock()
{
    if (mutex_owner == current_thread)
    {
        mutex_locked = false;
        mutex_owner = -1;

        std::cout << "[Mutex] Thread "
                  << current_thread
                  << " released the lock."
                  << std::endl;
    }
    else
    {
        std::cout << "[Mutex] Error: Thread "
                  << current_thread
                  << " does not own the lock."
                  << std::endl;
    }
}


// ==========================================
// THREAD STATUS
// ==========================================

void thread_status()
{
    if (thread_count == 0)
    {
        std::cout << std::endl;
        std::cout << "No threads have been created."
                  << std::endl;

        return;
    }

    std::cout << std::endl;

    std::cout << "========== THREAD STATUS =========="
              << std::endl;

    for (int i = 0; i < thread_count; i++)
    {
        std::cout << "Thread ID: "
                  << threads[i].id;

        std::cout << " | State: ";

        if (threads[i].state == READY)
        {
            std::cout << "READY";
        }
        else if (threads[i].state == RUNNING)
        {
            std::cout << "RUNNING";
        }
        else
        {
            std::cout << "FINISHED";
        }

        std::cout << " | Executions: "
                  << threads[i].execution_count;

        if (i == current_thread)
        {
            std::cout << " | CURRENT";
        }

        std::cout << std::endl;
    }

    std::cout << "==================================="
              << std::endl;
}


// ==========================================
// SCHEDULER STATISTICS
// ==========================================

void show_statistics()
{
    if (thread_count == 0)
    {
        std::cout << std::endl;
        std::cout << "No threads available for statistics."
                  << std::endl;

        return;
    }

    int completed = 0;

    for (int i = 0; i < thread_count; i++)
    {
        if (threads[i].state == FINISHED)
        {
            completed++;
        }
    }

    std::cout << std::endl;

    std::cout << "========== SCHEDULER STATISTICS =========="
              << std::endl;

    std::cout << "Scheduling Algorithm : Round Robin"
              << std::endl;

    std::cout << "Maximum Threads     : "
              << MAX_THREADS
              << std::endl;

    std::cout << "Total Threads       : "
              << thread_count
              << std::endl;

    std::cout << "Completed Threads   : "
              << completed
              << std::endl;

    std::cout << "Context Switches    : "
              << context_switches
              << std::endl;

    std::cout << std::endl;

    for (int i = 0; i < thread_count; i++)
    {
        std::cout << "Thread "
                  << threads[i].id
                  << " Execution Count: "
                  << threads[i].execution_count
                  << std::endl;
    }

    std::cout << "=========================================="
              << std::endl;
}


// ==========================================
// THREAD CLEANUP
// ==========================================

void thread_cleanup()
{
    std::cout << std::endl;

    std::cout << "Cleaning up thread resources..."
              << std::endl;

    for (int i = 0; i < thread_count; i++)
    {
        if (threads[i].fiber != nullptr)
        {
            DeleteFiber(threads[i].fiber);

            threads[i].fiber = nullptr;
        }

        threads[i].id = -1;
        threads[i].state = FINISHED;
        threads[i].function = nullptr;
        threads[i].execution_count = 0;
    }

    thread_count = 0;
    current_thread = -1;

    mutex_locked = false;
    mutex_owner = -1;

    if (main_fiber != nullptr)
    {
        ConvertFiberToThread();

        main_fiber = nullptr;
    }

    std::cout << "Thread resources cleaned successfully."
              << std::endl;
}


// ==========================================
// THREAD RESET
// ==========================================

void thread_reset()
{
    std::cout << std::endl;

    std::cout << "========== RESETTING SCHEDULER =========="
              << std::endl;

    thread_cleanup();

    thread_init();

    std::cout << "Scheduler reset successfully."
              << std::endl;

    std::cout << "You can now create and run new threads."
              << std::endl;

    std::cout << "========================================="
              << std::endl;
}