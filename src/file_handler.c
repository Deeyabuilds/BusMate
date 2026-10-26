#include <stdio.h>
#include <string.h>
#include "../include/file_handler.h"

#define STUDENT_FILE "data/students.dat"

/* Save a student */
void saveStudent(Student student)
{
    FILE *fp = fopen(STUDENT_FILE, "ab");

    if (fp == NULL)
    {
        printf("Error opening student file!\n");
        return;
    }

    fwrite(&student, sizeof(Student), 1, fp);
    fclose(fp);
}

/* Display all students */
void viewStudents(void)
{
    FILE *fp = fopen(STUDENT_FILE, "rb");
    Student student;

    if (fp == NULL)
    {
        printf("No student records found.\n");
        return;
    }

    printf("\n--- Student Records ---\n");

    while (fread(&student, sizeof(Student), 1, fp) == 1)
    {
        printf("Student ID: %s\n", student.student_id);
        printf("Name: %s\n", student.name);
        printf("-----------------------\n");
    }

    fclose(fp);
}

/* Update a student using Student ID */
void updateStudent(Student updatedStudent)
{
    FILE *fp = fopen(STUDENT_FILE, "rb+");
    Student student;
    int found = 0;

    if (fp == NULL)
    {
        printf("No student records found.\n");
        return;
    }

    while (fread(&student, sizeof(Student), 1, fp) == 1)
    {
        if (strcmp(student.student_id, updatedStudent.student_id) == 0)
        {
            fseek(fp, -(long)sizeof(Student), SEEK_CUR);
            fwrite(&updatedStudent, sizeof(Student), 1, fp);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found)
        printf("Student updated successfully.\n");
    else
        printf("Student not found.\n");
}

/* Delete a student using Student ID */
void deleteStudent(char student_id[])
{
    FILE *fp = fopen(STUDENT_FILE, "rb");
    FILE *temp = fopen("data/temp.dat", "wb");

    Student student;
    int found = 0;

    if (fp == NULL)
    {
        if (temp != NULL)
            fclose(temp);

        printf("No student records found.\n");
        return;
    }

    if (temp == NULL)
    {
        fclose(fp);
        printf("Error creating temporary file.\n");
        return;
    }

    while (fread(&student, sizeof(Student), 1, fp) == 1)
    {
        if (strcmp(student.student_id, student_id) == 0)
        {
            found = 1;
        }
        else
        {
            fwrite(&student, sizeof(Student), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (remove(STUDENT_FILE) != 0)
    {
        printf("Error replacing student file.\n");
        remove("data/temp.dat");
        return;
    }

    if (rename("data/temp.dat", STUDENT_FILE) != 0)
    {
        printf("Error renaming temporary file.\n");
        return;
    }

    if (found)
        printf("Student deleted successfully.\n");
    else
        printf("Student not found.\n");
}