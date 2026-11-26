#include <ctype.h>
#include <errno.h>
#include <limits.h>
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

int main(void) {
    int n;
    printf("Enter number of students: ");
    if (readInteger(&n) != 1 || n <= 0 ||
        (size_t) n > SIZE_MAX / sizeof(int)) {
        puts("Enter a valid positive student count.");
        return 1;
    }

    int *marks = malloc(n * sizeof(int));
    if (marks == NULL) {
        puts("Could not allocate memory for the marks.");
        return 1;
    }

    printf("Enter %d marks: ", n);
    for (int i = 0; i < n; i++) {
        if (readInteger(&marks[i]) != 1) {
            puts("Enter valid whole-number marks.");
            free(marks);
            marks = NULL;
            return 1;
        }
    }

    int highest = marks[0];
    int lowest = marks[0];
    double total = 0;
    for (int i = 0; i < n; i++) {
        total += marks[i];
        if (marks[i] > highest) {
            highest = marks[i];
        }
        if (marks[i] < lowest) {
            lowest = marks[i];
        }
    }

    puts("\n--- Result Summary ---");
    printf("Average marks: %.2f\n", total / n);
    printf("Highest marks: %d\n", highest);
    printf("Lowest marks: %d\n", lowest);

    free(marks);
    marks = NULL;
    return 0;
}
