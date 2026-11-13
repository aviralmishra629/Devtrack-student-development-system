/*
 * sports.h - Sports & Tournament Module (Student Development System)
 * Shared declarations for the Admin module and the Student module.
 */
#ifndef SPORTS_H
#define SPORTS_H

#include <stddef.h>

/* ---------- Configuration ---------- */
#define STUDENT_FILE   "students_sports.dat"   /* student profiles        */
#define SPORTS_FILE    "sports.dat"            /* sports/tournament rows  */
#define TEMP_FILE      "sports.tmp"            /* used while deleting     */
#define ADMIN_PASSWORD "admin123"              /* CHANGE before submitting */
#define MAX_PASS_TRIES 3
#define MAX_RECORDS    100                     /* per-student display cap */

/* ---------- Data structures ---------- */
typedef struct {
    char enrollment[16];   /* unique key, digits only */
    char name[50];
    char class_name[30];   /* e.g. "B.Tech CSE"       */
    char section[8];       /* e.g. "A1"               */
} Student;

typedef struct {
    char enrollment[16];   /* links record to a Student */
    char sport[30];
    char tournament[60];
    int  year;
    char position[30];     /* playing position / role   */
    int  matches_played;
    char achievement[60];  /* e.g. "Winner", "Runner-up" */
} SportsRecord;

/* A record plus where it sits in sports.dat (needed for edit/delete) */
typedef struct {
    SportsRecord rec;
    long         offset;
} RecordRef;

/* ---------- utils.c : safe input helpers ---------- */
void read_line(const char *prompt, char *buf, size_t size);
void read_nonempty(const char *prompt, char *buf, size_t size);
int  read_int(const char *prompt, int min, int max);
void read_enrollment(const char *prompt, char *buf, size_t size);
int  confirm(const char *prompt);
void edit_string(const char *label, char *field, size_t size);
void edit_int(const char *label, int *field, int min, int max);

/* ---------- storage.c : binary file handling ---------- */
int storage_find_student(const char *enrol, Student *out, long *offset);
int storage_add_student(const Student *s);
int storage_update_student(long offset, const Student *s);
int storage_get_student_at(int index, Student *out);
int storage_add_record(const SportsRecord *r);
int storage_get_records(const char *enrol, RecordRef *refs, int max);
int storage_update_record(long offset, const SportsRecord *r);
int storage_delete_record(long offset);

/* ---------- Modules ---------- */
void run_admin_module(void);     /* admin_module.c   : read + write */
void run_student_module(void);   /* student_module.c : READ ONLY    */
void print_record(int no, const SportsRecord *r);
void print_profile(const Student *s, const RecordRef *refs, int n);

#endif
