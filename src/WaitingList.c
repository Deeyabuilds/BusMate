#include "../include/WaitingList.h"

WaitingQueue *create_queue(void)
{
    WaitingQueue *q = (WaitingQueue *)malloc(sizeof(WaitingQueue));
    if (!q)
        return NULL;
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
    return q;
}

int enqueue_student(WaitingQueue *q, int student_id, const char *name, int route_id)
{
    if (!q)
        return 0;

    StudentNode *new_node = (StudentNode *)malloc(sizeof(StudentNode));
    if (!new_node)
        return 0;

    new_node->student.studentId = student_id;
    strncpy(new_node->student.name, name, sizeof(new_node->student.name) - 1);
    new_node->student.name[sizeof(new_node->student.name) - 1] = '\0';
    new_node->student.password[0] = '\0'; // Default empty password

    new_node->routeId = route_id;
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
    return temp; // Freeing caller side par hoga
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
        // Direct binary write matching official Student & Route structures
        fwrite(&(current->student), sizeof(Student), 1, fp);
        fwrite(&(current->routeId), sizeof(int), 1, fp);
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

    Student st;
    int r_id;
    while (fread(&st, sizeof(Student), 1, fp) == 1 &&
           fread(&r_id, sizeof(int), 1, fp) == 1)
    {
        enqueue_student(q, st.studentId, st.name, r_id);
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