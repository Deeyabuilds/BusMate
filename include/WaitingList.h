#ifndef WAITING_LIST_H
#define WAITING_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Student node info
typedef struct StudentNode
{
    char student_id[20];
    char name[50];
    char route_id[20];
    struct StudentNode *next;
} StudentNode;

// Queue structure
typedef struct
{
    StudentNode *front;
    StudentNode *rear;
    int count;
} WaitingQueue;

// Function prototypes
WaitingQueue *create_queue();
int enqueue_student(WaitingQueue *q, const char *id, const char *name, const char *route_id);
StudentNode *dequeue_student(WaitingQueue *q);
void free_queue(WaitingQueue *q);
int save_waiting_list(WaitingQueue *q, const char *filename);
int load_waiting_list(WaitingQueue *q, const char *filename);

#endif // WAITING_LIST_H