#ifndef WAITINGLIST_H
#define WAITINGLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

// Linked List Node for Waiting Queue using official Student struct
typedef struct StudentNode
{
    Student student; // TL ka official Student struct (studentId, name, password)
    int routeId;     // TL ke Route struct se aligned integer routeId
    struct StudentNode *next;
} StudentNode;

// Queue Structure
typedef struct
{
    StudentNode *front;
    StudentNode *rear;
    int count;
} WaitingQueue;

// Function Declarations
WaitingQueue *create_queue(void);
int enqueue_student(WaitingQueue *q, int student_id, const char *name, int route_id);
StudentNode *dequeue_student(WaitingQueue *q);
int save_waiting_list(WaitingQueue *q, const char *filename);
int load_waiting_list(WaitingQueue *q, const char *filename);
void free_queue(WaitingQueue *q);

#endif