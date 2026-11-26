#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readInteger(int *number) {
    char text[64];
    char *end;
    if (scanf("%63s", text) != 1) {
        return -1;
    }
    if (strlen(text) == sizeof text - 1) {
        int ch = getchar();
        while (ch != EOF && !isspace(ch)) {
            ch = getchar();
        }
        return 0;
    }

    errno = 0;
    long value = strtol(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0' ||
        value < INT_MIN || value > INT_MAX) {
        return 0;
    }
    *number = (int) value;
    return 1;
}

struct Employee {
    int id;
    char name[31];
    float salary;
};

int readEmployee(struct Employee *employee) {
    if (readInteger(&employee->id) != 1 ||
        scanf("%30s", employee->name) != 1) {
        return 0;
    }

    int separator = getchar();
    if (separator == EOF || !isspace(separator)) {
        return 0;
    }

    char text[64];
    char *end;
    if (scanf("%63s", text) != 1 || strlen(text) == sizeof text - 1) {
        return 0;
    }
    errno = 0;
    float salary = strtof(text, &end);
    if (errno != 0 || end == text || *end != '\0' ||
        !isfinite(salary) || salary < 0) {
        return 0;
    }
    employee->salary = salary;
    return 1;
}

int main(void) {
    int n;
    printf("Enter number of employees: ");
    if (readInteger(&n) != 1 || n <= 0 ||
        (size_t) n > SIZE_MAX / sizeof(struct Employee)) {
        puts("Enter a valid positive employee count.");
        return 1;
    }

    struct Employee *employees = malloc(n * sizeof(struct Employee));
    if (employees == NULL) {
        puts("Could not allocate memory for employees.");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Enter ID, Name, Salary of employee %d: ", i + 1);
        if (!readEmployee(&employees[i])) {
            puts("Enter a valid ID, single-word name, and non-negative salary.");
            free(employees);
            employees = NULL;
            return 1;
        }
    }

    double total = 0;
    int highest = 0;
    puts("\n--- Employee List ---");
    for (int i = 0; i < n; i++) {
        printf("%d %s Rs.%.2f\n", employees[i].id,
               employees[i].name, employees[i].salary);
        total += employees[i].salary;
        if (employees[i].salary > employees[highest].salary) {
            highest = i;
        }
    }
    printf("\nTotal payroll: Rs.%.2f\n", total);
    printf("Highest salary: %s (Rs.%.2f)\n",
           employees[highest].name, employees[highest].salary);

    free(employees);
    employees = NULL;
    return 0;
}
