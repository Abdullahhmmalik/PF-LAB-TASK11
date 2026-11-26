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
    int candidates;
    int voters;
    printf("Enter number of candidates: ");
    if (readInteger(&candidates) != 1 || candidates <= 0 ||
        (size_t) candidates > SIZE_MAX / sizeof(int)) {
        puts("Enter a valid positive candidate count.");
        return 1;
    }
    printf("Enter number of voters: ");
    if (readInteger(&voters) != 1 || voters < 0) {
        puts("Enter a valid non-negative voter count.");
        return 1;
    }

    int *votes = calloc(candidates, sizeof(int));
    if (votes == NULL) {
        puts("Could not allocate the vote counters.");
        return 1;
    }

    printf("Initial vote counts:");
    for (int i = 0; i < candidates; i++) {
        printf(" %d", votes[i]);
    }
    putchar('\n');

    if (voters > 0) {
        printf("Enter %d votes (1-%d): ", voters, candidates);
    }
    for (int i = 0; i < voters; i++) {
        int candidate;
        int status = readInteger(&candidate);
        if (status == -1) {
            puts("The vote list ended early.");
            free(votes);
            votes = NULL;
            return 1;
        }
        if (status == 1 && candidate >= 1 && candidate <= candidates) {
            votes[candidate - 1]++;
        }
    }

    puts("\n--- Election Result ---");
    int winner = 0;
    int tied = 0;
    for (int i = 0; i < candidates; i++) {
        printf("Candidate %d: %d vote(s)\n", i + 1, votes[i]);
        if (votes[i] > votes[winner]) {
            winner = i;
            tied = 0;
        } else if (i != winner && votes[i] == votes[winner]) {
            tied = 1;
        }
    }

    if (votes[winner] == 0) {
        puts("No valid votes. No winner.");
    } else if (tied) {
        printf("Tie for first place with %d vote(s) each.\n", votes[winner]);
    } else {
        printf("Winner: Candidate %d\n", winner + 1);
    }

    free(votes);
    votes = NULL;
    return 0;
}
