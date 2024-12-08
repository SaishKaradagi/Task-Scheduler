#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // for isspace function
#include <time.h>  // for nanosleep function

#define MAX_SIZE 100

struct Task {
    int priority;
    char name[50];
    char due_date[20];
    char notes[100];
    int completed; // 1 if completed, 0 if not
};

struct PriorityQueue {
    struct Task tasks[MAX_SIZE];
    int size;
};

void initPriorityQueue(struct PriorityQueue *pq) {
    pq->size = 0;
}

int isEmpty(struct PriorityQueue *pq) {
    return pq->size == 0;
}

int isFull(struct PriorityQueue *pq) {
    return pq->size == MAX_SIZE;
}

// Function to insert task into priority queue
void enqueue(struct PriorityQueue *pq, struct Task new_task) {
    if (!isFull(pq)) {
        int i = pq->size;
        while (i > 0 && new_task.priority < pq->tasks[i - 1].priority) {
            pq->tasks[i] = pq->tasks[i - 1];
            i--;
        }
        pq->tasks[i] = new_task;
        pq->size++;
    } else {
        printf("\033[1;31mPriority queue is full\n\033[0m");
    }
}

// Function to remove task from priority queue
void dequeue(struct PriorityQueue *pq, int index) {
    if (!isEmpty(pq)) {
        for (int i = index - 1; i < pq->size - 1; i++) {
            pq->tasks[i] = pq->tasks[i + 1];
        }
        pq->size--;
    } else {
        printf("\033[1;31mPriority queue is empty\n\033[0m");
    }
}

// Function to display tasks in priority queue
void display(struct PriorityQueue *pq) {
    if (!isEmpty(pq)) {
        printf("\n\033[1;36m-------------------------------------------------------------------------------------------------------------\n");
        printf("| \033[1;33m%-30s\033[1;36m | \033[1;33m%-10s\033[1;36m | \033[1;33m%-15s\033[1;36m | \033[1;33m%-50s\033[1;36m |\n", "Task", "Priority", "Due Date", "Notes");
        printf("-------------------------------------------------------------------------------------------------------------\n");
        for (int i = 0; i < pq->size; i++) {
            printf("| \033[1;37m%-30s\033[1;36m | \033[1;37m%-10d\033[1;36m | \033[1;37m%-15s\033[1;36m | \033[1;37m%-50s\033[1;36m |\n", pq->tasks[i].name, pq->tasks[i].priority, pq->tasks[i].due_date, pq->tasks[i].notes);
        }
        printf("-------------------------------------------------------------------------------------------------------------\n\033[0m");
    } else {
        printf("\033[1;31mPriority queue is empty\n\033[0m");
    }
}

// Function to provide user with helpful tips
void tips() {
    printf("\033[1;35m\nTips for Effective Task Management:\n");
    printf("1. Prioritize your tasks based on importance and urgency.\n");
    printf("2. Break down large tasks into smaller, manageable subtasks.\n");
    printf("3. Set deadlines for each task to stay focused and organized.\n");
    printf("4. Use a task scheduler like this one to keep track of your tasks.\n");
    printf("5. Take regular breaks to maintain productivity and avoid burnout.\n\033[0m");
}

// Function to handle user input and process tasks
int main() {
    struct PriorityQueue pq;
    initPriorityQueue(&pq);

    int choice;
    do {
        printf("\n\033[1;34mTask Scheduler Menu:\033[0m\n");
        printf("\033[1;36m1.\033[0m Add Task\n");
        printf("\033[1;36m2.\033[0m Mark Task as Completed\n");
        printf("\033[1;36m3.\033[0m Delete Task\n");
        printf("\033[1;36m4.\033[0m Display Tasks\n");
        printf("\033[1;36m5.\033[0m View Completed Tasks and Delete\n");
        printf("\033[1;36m6.\033[0m Tips\n");
        printf("\033[1;36m7.\033[0m Exit\n");
        printf("\033[1;34mEnter your choice: \033[0m");

        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                struct Task new_task;
                printf("Enter task name: ");
                scanf(" %[^\n]", new_task.name); // Read entire line including spaces
                printf("Enter priority: ");
                scanf("%d", &new_task.priority);
                printf("Enter due date (dd/mm/yyyy): ");
                scanf("%s", new_task.due_date);
                printf("Enter notes: ");
                scanf(" %[^\n]", new_task.notes); // Read entire line including spaces
                new_task.completed = 0;
                enqueue(&pq, new_task);
                printf("\033[1;32mTask added successfully!\n\033[0m");
                break;
            }
            case 2: {
                printf("Enter index of task to mark as completed: ");
                int index;
                scanf("%d", &index);
                if (index > 0 && index <= pq.size) {
                    pq.tasks[index - 1].completed = 1;
                    printf("\033[1;32mTask marked as completed!\n\033[0m");
                } else {
                    printf("\033[1;31mInvalid index.\n\033[0m");
                }
                break;
            }
            case 3: {
                printf("Enter number of tasks to delete: ");
                int num_tasks;
                scanf("%d", &num_tasks);

                if (num_tasks <= 0) {
                    printf("\033[1;31mInvalid number of tasks.\n\033[0m");
                    break;
                }

                printf("Enter the indices of tasks to delete separated by spaces: ");
                int indices[num_tasks];
                for (int i = 0; i < num_tasks; i++) {
                    scanf("%d", &indices[i]);
                    if (indices[i] <= 0 || indices[i] > pq.size) {
                        printf("\033[1;31mInvalid index %d.\n\033[0m", indices[i]);
                        return 1;
                    }
                }

                printf("\033[1;34mTasks to be deleted:\n\033[0m");
                for (int i = 0; i < num_tasks; i++) {
                    printf("Index %d: %s\n", indices[i], pq.tasks[indices[i] - 1].name);
                }

                printf("Are you sure you want to delete these tasks? (yes/no): ");
                char confirmation[10];
                scanf("%s", confirmation);

                if (strcmp(confirmation, "yes") == 0) {
                    printf("\033[1;34mDeleted tasks:\n\033[0m");
                    for (int i = 0; i < num_tasks; i++) {
                        printf("%s\n", pq.tasks[indices[i] - 1].name);
                        dequeue(&pq, indices[i]);
                    }
                    printf("\033[1;32mTasks deleted successfully!\n\033[0m");
                } else {
                    printf("\033[1;32mDeletion cancelled.\n\033[0m");
                }
                break;
            }
            case 4: {
                display(&pq);
                
                printf("Do you want to delete any of these tasks? (yes/no): ");
                char response[10];
                scanf("%s", response);
                
                if (strcmp(response, "yes") == 0) {
                    printf("Enter number of tasks to delete: ");
                    int num_tasks;
                    scanf("%d", &num_tasks);

                    if (num_tasks <= 0) {
                        printf("\033[1;31mInvalid number of tasks.\n\033[0m");
                        break;
                    }

                    printf("Enter the indices of tasks to delete separated by spaces: ");
                    int indices[num_tasks];
                    for (int i = 0; i < num_tasks; i++) {
                        scanf("%d", &indices[i]);
                        if (indices[i] <= 0 || indices[i] > pq.size) {
                            printf("\033[1;31mInvalid index %d.\n\033[0m", indices[i]);
                            return 1;
                        }
                    }

                    printf("\033[1;34mTasks to be deleted:\n\033[0m");
                    for (int i = 0; i < num_tasks; i++) {
                        printf("Index %d: %s\n", indices[i], pq.tasks[indices[i] - 1].name);
                    }

                    printf("Are you sure you want to delete these tasks? (yes/no): ");
                    char confirmation[10];
                    scanf("%s", confirmation);

                    if (strcmp(confirmation, "yes") == 0) {
                        printf("\033[1;34mDeleted tasks:\n\033[0m");
                        for (int i = 0; i < num_tasks; i++) {
                            printf("%s\n", pq.tasks[indices[i] - 1].name);
                            dequeue(&pq, indices[i]);
                        }
                        printf("\033[1;32mTasks deleted successfully!\n\033[0m");
                    } else {
                        printf("\033[1;32mDeletion cancelled.\n\033[0m");
                    }
                }
                break;
            }
            case 5: {
                int completed_tasks_count = 0;
                printf("\n\033[1;36mCompleted Tasks:\033[0m\n");
                printf("\033[1;36m-------------------------------------------------------------------------------------------------------------\n");
                printf("| \033[1;33m%-30s\033[1;36m | \033[1;33m%-10s\033[1;36m | \033[1;33m%-15s\033[1;36m | \033[1;33m%-50s\033[1;36m |\n", "Task", "Priority", "Due Date", "Notes");
                printf("-------------------------------------------------------------------------------------------------------------\n");
                for (int i = 0; i < pq.size; i++) {
                    if (pq.tasks[i].completed == 1) {
                        printf("| \033[1;37m%-30s\033[1;36m | \033[1;37m%-10d\033[1;36m | \033[1;37m%-15s\033[1;36m | \033[1;37m%-50s\033[1;36m |\n", pq.tasks[i].name, pq.tasks[i].priority, pq.tasks[i].due_date, pq.tasks[i].notes);
                        completed_tasks_count++;
                    }
                }
                printf("-------------------------------------------------------------------------------------------------------------\n\033[0m");
                
                if (completed_tasks_count == 0) {
                    printf("\033[1;33mNo completed tasks to show.\n\033[0m");
                } else {
                    printf("Do you want to delete any of these tasks? (yes/no): ");
                    char response[10];
                    scanf("%s", response);

                    if (strcmp(response, "yes") == 0) {
                        printf("Enter number of tasks to delete: ");
                        int num_tasks;
                        scanf("%d", &num_tasks);

                        if (num_tasks <= 0) {
                            printf("\033[1;31mInvalid number of tasks.\n\033[0m");
                            break;
                        }

                        printf("Enter the indices of tasks to delete separated by spaces: ");
                        int indices[num_tasks];
                        for (int i = 0; i < num_tasks; i++) {
                            scanf("%d", &indices[i]);
                            if (indices[i] <= 0 || indices[i] > pq.size) {
                                printf("\033[1;31mInvalid index %d.\n\033[0m", indices[i]);
                                return 1;
                            }
                        }

                        printf("\033[1;34mTasks to be deleted:\n\033[0m");
                        for (int i = 0; i < num_tasks; i++) {
                            printf("Index %d: %s\n", indices[i], pq.tasks[indices[i] - 1].name);
                        }

                        printf("Are you sure you want to delete these tasks? (yes/no): ");
                        char confirmation[10];
                        scanf("%s", confirmation);

                        if (strcmp(confirmation, "yes") == 0) {
                            printf("\033[1;34mDeleted tasks:\n\033[0m");
                            for (int i = 0; i < num_tasks; i++) {
                                printf("%s\n", pq.tasks[indices[i] - 1].name);
                                dequeue(&pq, indices[i]);
                            }
                            printf("\033[1;32mTasks deleted successfully!\n\033[0m");
                        } else {
                            printf("\033[1;32mDeletion cancelled.\n\033[0m");
                        }
                    }
                }
                break;
            }
            case 6: {
                tips();
                break;
            }
            case 7:
                printf("\033[1;33mExiting...\n\033[0m");
                break;
            default:
                printf("\033[1;31mInvalid choice. Please try again.\n\033[0m");
        }
        // Add a delay for a little dramatic effect
        struct timespec ts = {0, 500000000L}; // 0.5 seconds delay
        nanosleep(&ts, NULL);
    } while (choice != 7);

    return 0;
}