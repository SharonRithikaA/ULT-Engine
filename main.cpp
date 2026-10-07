#include <iostream>
#include "Thread.h"

using namespace std;

int shared_counter = 0;


// ==========================================
// THREAD 1
// ==========================================

void task1()
{
    for (int i = 1; i <= 5; i++)
    {
        thread_lock();

        shared_counter++;

        cout << "[Thread 1] "
             << "Iteration " << i
             << " | Shared Counter = "
             << shared_counter
             << endl;

        thread_unlock();

        thread_yield();
    }
}


// ==========================================
// THREAD 2
// ==========================================

void task2()
{
    for (int i = 1; i <= 5; i++)
    {
        thread_lock();

        shared_counter++;

        cout << "[Thread 2] "
             << "Iteration " << i
             << " | Shared Counter = "
             << shared_counter
             << endl;

        thread_unlock();

        thread_yield();
    }
}


// ==========================================
// THREAD 3
// ==========================================

void task3()
{
    for (int i = 1; i <= 5; i++)
    {
        thread_lock();

        shared_counter++;

        cout << "[Thread 3] "
             << "Iteration " << i
             << " | Shared Counter = "
             << shared_counter
             << endl;

        thread_unlock();

        thread_yield();
    }
}


// ==========================================
// PROJECT INFORMATION
// ==========================================

void show_project_info()
{
    cout << endl;

    cout << "=================================================="
         << endl;

    cout << "              ULT ENGINE INFORMATION"
         << endl;

    cout << "=================================================="
         << endl;

    cout << "Project : User Level Threading Engine"
         << endl;

    cout << "Language: C++"
         << endl;

    cout << "Platform: Windows"
         << endl;

    cout << "Context : Windows Fibers"
         << endl;

    cout << "Scheduler: Cooperative Round Robin"
         << endl;

    cout << "Maximum Threads: "
         << MAX_THREADS
         << endl;

    cout << "Synchronization: Mutex"
         << endl;

    cout << "Scheduling Type: User-Level"
         << endl;

    cout << "=================================================="
         << endl;
}


// ==========================================
// MAIN
// ==========================================

int main()
{
    int choice;

    cout << endl;

    cout << "=================================================="
         << endl;

    cout << "          USER LEVEL THREADING ENGINE"
         << endl;

    cout << "=================================================="
         << endl;

    cout << "        Design and Implementation of ULT"
         << endl;

    cout << "=================================================="
         << endl;

    thread_init();

    do
    {
        cout << endl;

        cout << "==================== MAIN MENU ==================="
             << endl;

        cout << "1. Create Threads"
             << endl;

        cout << "2. View Thread Status"
             << endl;

        cout << "3. Start Scheduler"
             << endl;

        cout << "4. View Shared Counter"
             << endl;

        cout << "5. View Scheduler Statistics"
             << endl;

        cout << "6. Reset Scheduler"
             << endl;

        cout << "7. Project Information"
             << endl;

        cout << "8. Exit"
             << endl;

        cout << "=================================================="
             << endl;

        cout << "Enter your choice: ";

        cin >> choice;


        // ==========================================
        // INPUT VALIDATION
        // ==========================================

        if (cin.fail())
        {
            cin.clear();

            cin.ignore(1000, '\n');

            cout << endl;

            cout << "ERROR: Please enter a valid number."
                 << endl;

            continue;
        }


        // ==========================================
        // MENU
        // ==========================================

        switch (choice)
        {
            // --------------------------------------
            // CREATE THREADS
            // --------------------------------------

            case 1:
            {
                if (thread_count == 0)
                {
                    int t1 = thread_create(task1);
                    int t2 = thread_create(task2);
                    int t3 = thread_create(task3);

                    cout << endl;

                    if (t1 != -1 &&
                        t2 != -1 &&
                        t3 != -1)
                    {
                        cout << "SUCCESS: Threads created."
                             << endl;

                        cout << "----------------------------------"
                             << endl;

                        cout << "Thread 1 ID : "
                             << t1
                             << endl;

                        cout << "Thread 2 ID : "
                             << t2
                             << endl;

                        cout << "Thread 3 ID : "
                             << t3
                             << endl;

                        cout << "----------------------------------"
                             << endl;

                        cout << "Total Threads: "
                             << thread_count
                             << endl;
                    }

                    break;
                }

                cout << endl;

                cout << "WARNING: Threads already exist."
                     << endl;

                cout << "Use option 6 to reset the scheduler."
                     << endl;

                break;
            }


            // --------------------------------------
            // THREAD STATUS
            // --------------------------------------

            case 2:
            {
                thread_status();

                break;
            }


            // --------------------------------------
            // START SCHEDULER
            // --------------------------------------

            case 3:
            {
                if (thread_count == 0)
                {
                    cout << endl;

                    cout << "ERROR: No threads available."
                         << endl;

                    cout << "Please create threads first."
                         << endl;
                }
                else
                {
                    cout << endl;

                    cout << "=================================================="
                         << endl;

                    cout << "       STARTING ROUND ROBIN SCHEDULER"
                         << endl;

                    cout << "=================================================="
                         << endl;

                    cout << endl;

                    thread_start();

                    cout << endl;

                    cout << "=================================================="
                         << endl;

                    cout << "             SCHEDULER COMPLETED"
                         << endl;

                    cout << "=================================================="
                         << endl;
                }

                break;
            }


            // --------------------------------------
            // SHARED COUNTER
            // --------------------------------------

            case 4:
            {
                cout << endl;

                cout << "=================================================="
                     << endl;

                cout << "                SHARED RESOURCE"
                     << endl;

                cout << "=================================================="
                     << endl;

                cout << "Shared Counter Value : "
                     << shared_counter
                     << endl;

                cout << "Expected Final Value : 15"
                     << endl;

                cout << "Mutex Protection     : ENABLED"
                     << endl;

                cout << "=================================================="
                     << endl;

                break;
            }


            // --------------------------------------
            // STATISTICS
            // --------------------------------------

            case 5:
            {
                show_statistics();

                break;
            }


            // --------------------------------------
            // RESET
            // --------------------------------------

            case 6:
            {
                thread_reset();

                shared_counter = 0;

                break;
            }


            // --------------------------------------
            // PROJECT INFORMATION
            // --------------------------------------

            case 7:
            {
                show_project_info();

                break;
            }


            // --------------------------------------
            // EXIT
            // --------------------------------------

            case 8:
            {
                cout << endl;

                cout << "=================================================="
                     << endl;

                cout << "             SHUTTING DOWN ULT ENGINE"
                     << endl;

                cout << "=================================================="
                     << endl;

                thread_cleanup();

                cout << endl;

                cout << "ULT Engine terminated successfully."
                     << endl;

                cout << "Thank you."
                     << endl;

                cout << "=================================================="
                     << endl;

                break;
            }


            // --------------------------------------
            // INVALID
            // --------------------------------------

            default:
            {
                cout << endl;

                cout << "ERROR: Invalid menu choice."
                     << endl;

                cout << "Please select an option from 1 to 8."
                     << endl;

                break;
            }
        }

    } while (choice != 8);

    return 0;
}