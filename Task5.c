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

void showPlaylist(const char *label, const int playlist[], int count) {
    long long total = 0;
    printf("%s:", label);
    for (int i = 0; i < count; i++) {
        printf(" %d", playlist[i]);
        total += playlist[i];
    }
    printf(" (total %lld minutes)\n", total);
}

int main(void) {
    int n;
    printf("Enter number of songs: ");
    if (readInteger(&n) != 1 || n <= 0 ||
        (size_t) n > SIZE_MAX / sizeof(int)) {
        puts("Enter a valid positive song count.");
        return 1;
    }

    int *playlist = malloc(n * sizeof(int));
    if (playlist == NULL) {
        puts("Could not allocate memory for the playlist.");
        return 1;
    }

    printf("Enter duration (minutes) of %d songs: ", n);
    for (int i = 0; i < n; i++) {
        if (readInteger(&playlist[i]) != 1 || playlist[i] < 0) {
            puts("Enter non-negative whole-number durations.");
            free(playlist);
            playlist = NULL;
            return 1;
        }
    }
    showPlaylist("Playlist", playlist, n);

    int removeCount;
    const char *label = "Playlist unchanged";
    printf("Enter number of songs to remove from the end: ");
    if (readInteger(&removeCount) != 1 || removeCount < 0) {
        puts("Enter a non-negative whole number to remove.");
    } else if (removeCount >= n) {
        puts("Cannot remove that many songs.");
    } else if (removeCount > 0) {
        int remaining = n - removeCount;
        int *smaller = realloc(playlist, remaining * sizeof(int));
        if (smaller == NULL) {
            puts("Could not resize the playlist.");
        } else {
            playlist = smaller;
            n = remaining;
            label = "Playlist after removing";
        }
    }
    showPlaylist(label, playlist, n);

    free(playlist);
    playlist = NULL;
    if (playlist == NULL) {
        puts("\nPlaylist deleted. Nothing left to play.");
    }
    return 0;
}
