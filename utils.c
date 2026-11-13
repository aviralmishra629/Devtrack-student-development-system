#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sports.h"

static void trim(char *s)
{
    size_t len = strlen(s), start = 0;
    while (len > 0 && isspace((unsigned char)s[len - 1])) s[--len] = '\0';
    while (s[start] && isspace((unsigned char)s[start])) start++;
    if (start > 0) memmove(s, s + start, strlen(s + start) + 1);
}

/* Reads one line safely (no buffer overflow), trims spaces. */
void read_line(const char *prompt, char *buf, size_t size)
{
    if (prompt) { printf("%s", prompt); fflush(stdout); }
    if (!fgets(buf, (int)size, stdin)) {
        printf("\nInput closed. Exiting.\n");
        exit(0);
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {                       /* line longer than buffer: discard the rest */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    trim(buf);
}

void read_nonempty(const char *prompt, char *buf, size_t size)
{
    for (;;) {
        read_line(prompt, buf, size);
        if (buf[0] != '\0') return;
        printf("  ! This field cannot be empty.\n");
    }
}

int read_int(const char *prompt, int min, int max)
{
    char buf[32], *end;
    for (;;) {
        read_line(prompt, buf, sizeof buf);
        long v = strtol(buf, &end, 10);
        if (buf[0] != '\0' && *end == '\0' && v >= min && v <= max)
            return (int)v;
        printf("  ! Enter a number between %d and %d.\n", min, max);
    }
}

void read_enrollment(const char *prompt, char *buf, size_t size)
{
    for (;;) {
        read_line(prompt, buf, size);
        size_t i, len = strlen(buf);
        int ok = (len > 0);
        for (i = 0; i < len && ok; i++)
            if (!isdigit((unsigned char)buf[i])) ok = 0;
        if (ok) return;
        printf("  ! Enrollment number must contain digits only (max %d).\n",
               (int)size - 1);
    }
}

int confirm(const char *prompt)
{
    char buf[8];
    for (;;) {
        read_line(prompt, buf, sizeof buf);
        if (tolower((unsigned char)buf[0]) == 'y') return 1;
        if (tolower((unsigned char)buf[0]) == 'n') return 0;
        printf("  ! Please type y or n.\n");
    }
}

/* Edit helpers: pressing Enter keeps the current value. */
void edit_string(const char *label, char *field, size_t size)
{
    char buf[128], prompt[128];
    snprintf(prompt, sizeof prompt, "  %s [%s]: ", label, field);
    read_line(prompt, buf, sizeof buf);
    if (buf[0] != '\0') {
        strncpy(field, buf, size - 1);
        field[size - 1] = '\0';
    }
}

void edit_int(const char *label, int *field, int min, int max)
{
    char buf[32], prompt[128], *end;
    for (;;) {
        snprintf(prompt, sizeof prompt, "  %s [%d]: ", label, *field);
        read_line(prompt, buf, sizeof buf);
        if (buf[0] == '\0') return;
        long v = strtol(buf, &end, 10);
        if (*end == '\0' && v >= min && v <= max) { *field = (int)v; return; }
        printf("  ! Enter a number between %d and %d.\n", min, max);
    }
}
