#include "../include/WaitingList.h"

WaitingQueue *create_queue()
{
    WaitingQueue *q = (WaitingQueue *)malloc(sizeof(WaitingQueue));
    if (!q)
        return NULL;
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
    return q;
}

int enqueue_student(WaitingQueue *q, const char *id, const char *name, const char *route_id)
{
    if (!q)
        return 0;

    StudentNode *new_node = (StudentNode *)malloc(sizeof(StudentNode));
    if (!new_node)
        return 0;

    strncpy(new_node->student_id, id, sizeof(new_node->student_id) - 1);
    strncpy(new_node->name, name, sizeof(new_node->name) - 1);
    strncpy(new_node->route_id, route_id, sizeof(new_node->route_id) - 1);
    new_node->next = NULL;

    if (q->rear == NULL)
    {
        q->front = q->rear = new_node;
    }
    else
    {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->count++;
    return 1;
}

StudentNode *dequeue_student(WaitingQueue *q)
{
    if (!q || q->front == NULL)
        return NULL;

    StudentNode *temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL)
    {
        q->rear = NULL;
    }

    q->count--;
    return temp; // Free calling side par karein
}

int save_waiting_list(WaitingQueue *q, const char *filename)
{
    if (!q || !filename)
        return 0;
    FILE *fp = fopen(filename, "wb");
    if (!fp)
        return 0;

    StudentNode *current = q->front;
    while (current != NULL)
    {
        fwrite(current->student_id, sizeof(char), 20, fp);
        fwrite(current->name, sizeof(char), 50, fp);
        fwrite(current->route_id, sizeof(char), 20, fp);
        current = current->next;
    }

    fclose(fp);
    return 1;
}

int load_waiting_list(WaitingQueue *q, const char *filename)
{
    if (!q || !filename)
        return 0;
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return 0;

    char id[20], name[50], route[20];
    while (fread(id, sizeof(char), 20, fp) == 20 &&
           fread(name, sizeof(char), 50, fp) == 50 &&
           fread(route, sizeof(char), 20, fp) == 20)
    {
        enqueue_student(q, id, name, route);
    }

    fclose(fp);
    return 1;
}

void free_queue(WaitingQueue *q)
{
    if (!q)
        return;
    StudentNode *current = q->front;
    while (current != NULL)
    {
        StudentNode *temp = current;
        current = current->next;
        free(temp);
    }
    free(q);
}