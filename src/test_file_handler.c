#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "file_handler.h"

int main(void)
{
    Student student = {101, "Deeya", "test123"};
    Student updatedStudent = {101, "Deeya Pandey", "new123"};

    void *records = NULL;
    size_t count = 0;
    int updated = 0;
    int deleted = 0;

    printf("\nTesting BusMate File Handler\n\n");

    if (!appendRecord("data/test_students.dat",
                      sizeof(Student), &student))
    {
        printf("Save FAILED\n");
        return 1;
    }
    printf("1. Save: PASSED\n");

    if (!readRecords("data/test_students.dat",
                     sizeof(Student), &records, &count))
    {
        printf("Read FAILED\n");
        return 1;
    }

    if (count != 1)
    {
        printf("Read FAILED: unexpected record count\n");
        free(records);
        return 1;
    }

    printf("2. Read: PASSED - %s\n", ((Student *)records)[0].name);
    free(records);
    records = NULL;

    if (!updateRecordById("data/test_students.dat",
                          sizeof(Student),
                          offsetof(Student, studentId),
                          101, &updatedStudent, &updated))
    {
        printf("Update FAILED\n");
        return 1;
    }

    if (!updated)
    {
        printf("Update FAILED: record not found\n");
        return 1;
    }
    printf("3. Update: PASSED\n");

    if (!deleteRecordById("data/test_students.dat",
                          sizeof(Student),
                          offsetof(Student, studentId),
                          101, &deleted))
    {
        printf("Delete FAILED\n");
        return 1;
    }

    if (!deleted)
    {
        printf("Delete FAILED: record not found\n");
        return 1;
    }
    printf("4. Delete: PASSED\n");

    if (!readRecords("data/test_students.dat",
                     sizeof(Student), &records, &count))
    {
        printf("Verification FAILED\n");
        return 1;
    }

    printf("5. Verify deletion: %s\n",
           count == 0 ? "PASSED" : "FAILED");

    free(records);
    remove("data/test_students.dat");

    printf("\nTesting completed!\n");

    return count == 0 ? 0 : 1;
}