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

int readDonations(int donations[], int start, int end) {
    for (int i = start; i < end; i++) {
        if (readInteger(&donations[i]) != 1 || donations[i] < 0) {
            puts("Enter non-negative whole-number donation amounts.");
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int n;
    printf("Enter number of donations: ");
    if (readInteger(&n) != 1 || n <= 0 ||
        (size_t) n > SIZE_MAX / sizeof(int)) {
        puts("Enter a valid positive donation count.");
        return 1;
    }

    int *donations = malloc(n * sizeof(int));
    if (donations == NULL) {
        puts("Could not allocate memory for donations.");
        return 1;
    }

    printf("Enter %d donation amounts: ", n);
    if (!readDonations(donations, 0, n)) {
        free(donations);
        donations = NULL;
        return 1;
    }

    int extra;
    printf("Enter number of additional donations: ");
    if (readInteger(&extra) != 1 || extra < 0 || extra > INT_MAX - n ||
        (size_t) (n + extra) > SIZE_MAX / sizeof(int)) {
        puts("Enter a valid number of additional donations.");
        free(donations);
        donations = NULL;
        return 1;
    }

    if (extra > 0) {
        int *larger = realloc(donations, (n + extra) * sizeof(int));
        if (larger == NULL) {
            puts("Could not expand the donation list.");
            free(donations);
            donations = NULL;
            return 1;
        }
        donations = larger;

        printf("Enter %d more amounts: ", extra);
        if (!readDonations(donations, n, n + extra)) {
            free(donations);
            donations = NULL;
            return 1;
        }
    }

    long long total = 0;
    printf("\nAll donations:");
    for (int i = 0; i < n + extra; i++) {
        printf(" %d", donations[i]);
        total += donations[i];
    }
    printf("\nTotal collected: %lld\n", total);

    free(donations);
    donations = NULL;
    return 0;
}
