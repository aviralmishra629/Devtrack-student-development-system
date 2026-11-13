/*
 * admin_module.c - ADMIN MODULE (data entry + editing)
 * Password protected. Can add / edit students and add / edit / delete
 * sports & tournament records.
 */
#include <stdio.h>
#include <string.h>
#include "sports.h"

static int admin_login(void)
{
    char pass[64];
    int t;
    for (t = 1; t <= MAX_PASS_TRIES; t++) {
        read_line("Enter admin password: ", pass, sizeof pass);
        if (strcmp(pass, ADMIN_PASSWORD) == 0) return 1;
        printf("  ! Wrong password (%d/%d)\n", t, MAX_PASS_TRIES);
    }
    return 0;
}

/* Asks for an enrollment no. and loads that student; 0 if not found. */
static int ask_student(Student *s, long *offset)
{
    char enrol[16];
    read_enrollment("Enrollment No.: ", enrol, sizeof enrol);
    if (!storage_find_student(enrol, s, offset)) {
        printf("  ! Student %s not found. Add the student first.\n", enrol);
        return 0;
    }
    return 1;
}

static void add_student(void)
{
    Student s;
    printf("\n--- Add New Student ---\n");
    read_enrollment("Enrollment No.: ", s.enrollment, sizeof s.enrollment);
    if (storage_find_student(s.enrollment, NULL, NULL)) {
        printf("  ! Enrollment No. %s already exists.\n", s.enrollment);
        return;
    }
    read_nonempty("Name           : ", s.name, sizeof s.name);
    read_nonempty("Class          : ", s.class_name, sizeof s.class_name);
    read_nonempty("Section        : ", s.section, sizeof s.section);

    if (storage_add_student(&s)) printf("  + Student saved.\n");
    else                         printf("  ! Could not write to file.\n");
}

static void edit_student(void)
{
    Student s;
    long off;
    printf("\n--- Edit Student (Enter = keep current) ---\n");
    if (!ask_student(&s, &off)) return;

    edit_string("Name   ", s.name, sizeof s.name);
    edit_string("Class  ", s.class_name, sizeof s.class_name);
    edit_string("Section", s.section, sizeof s.section);

    if (storage_update_student(off, &s)) printf("  + Student updated.\n");
    else                                 printf("  ! Update failed.\n");
}

static void add_record(void)
{
    Student s;
    SportsRecord r;
    printf("\n--- Add Sports / Tournament Record ---\n");
    if (!ask_student(&s, NULL)) return;
    printf("  Student: %s (%s-%s)\n", s.name, s.class_name, s.section);

    strcpy(r.enrollment, s.enrollment);
    read_nonempty("Sport                  : ", r.sport, sizeof r.sport);
    read_nonempty("Tournament name         : ", r.tournament, sizeof r.tournament);
    r.year           = read_int("Year (1990-2100)       : ", 1990, 2100);
    read_nonempty("Position / role         : ", r.position, sizeof r.position);
    r.matches_played = read_int("Matches played (0-1000) : ", 0, 1000);
    read_nonempty("Achievement             : ", r.achievement, sizeof r.achievement);

    if (storage_add_record(&r)) printf("  + Record saved.\n");
    else                        printf("  ! Could not write to file.\n");
}

/* Lists a student's records and lets admin pick one. Returns index or -1. */
static int pick_record(const Student *s, RecordRef *refs, int *count)
{
    int i, choice;
    *count = storage_get_records(s->enrollment, refs, MAX_RECORDS);
    if (*count == 0) {
        printf("  ! %s has no sports records yet.\n", s->name);
        return -1;
    }
    printf("\n  Records of %s:\n", s->name);
    for (i = 0; i < *count; i++) print_record(i + 1, &refs[i].rec);
    choice = read_int("Select record number (0 to cancel): ", 0, *count);
    return choice - 1;               /* -1 when cancelled */
}

static void edit_record(void)
{
    Student s;
    RecordRef refs[MAX_RECORDS];
    SportsRecord r;
    int count, idx;
    printf("\n--- Edit Sports Record ---\n");
    if (!ask_student(&s, NULL)) return;
    idx = pick_record(&s, refs, &count);
    if (idx < 0) return;

    r = refs[idx].rec;
    printf("\n  (Enter = keep current)\n");
    edit_string("Sport      ", r.sport, sizeof r.sport);
    edit_string("Tournament ", r.tournament, sizeof r.tournament);
    edit_int   ("Year       ", &r.year, 1990, 2100);
    edit_string("Position   ", r.position, sizeof r.position);
    edit_int   ("Matches    ", &r.matches_played, 0, 1000);
    edit_string("Achievement", r.achievement, sizeof r.achievement);

    if (storage_update_record(refs[idx].offset, &r))
        printf("  + Record updated.\n");
    else
        printf("  ! Update failed.\n");
}

static void delete_record(void)
{
    Student s;
    RecordRef refs[MAX_RECORDS];
    int count, idx;
    printf("\n--- Delete Sports Record ---\n");
    if (!ask_student(&s, NULL)) return;
    idx = pick_record(&s, refs, &count);
    if (idx < 0) return;

    if (confirm("Really delete this record? (y/n): ")) {
        if (storage_delete_record(refs[idx].offset))
            printf("  + Record deleted.\n");
        else
            printf("  ! Delete failed.\n");
    } else {
        printf("  Cancelled.\n");
    }
}

static void view_student(void)
{
    Student s;
    RecordRef refs[MAX_RECORDS];
    int n;
    printf("\n--- View Student Profile ---\n");
    if (!ask_student(&s, NULL)) return;
    n = storage_get_records(s.enrollment, refs, MAX_RECORDS);
    print_profile(&s, refs, n);
}

static void view_all_students(void)
{
    Student s;
    int i = 0;
    printf("\n--- All Registered Students ---\n");
    printf("%-16s %-24s %-14s %s\n", "Enrollment", "Name", "Class", "Sec");
    printf("--------------------------------------------------------------\n");
    while (storage_get_student_at(i, &s)) {
        printf("%-16s %-24s %-14s %s\n",
               s.enrollment, s.name, s.class_name, s.section);
        i++;
    }
    if (i == 0) printf("  No students registered yet.\n");
    else        printf("  Total: %d\n", i);
}

void run_admin_module(void)
{
    int choice;
    printf("\n------------------- ADMIN PORTAL -------------------\n");
    if (!admin_login()) {
        printf("  Too many wrong attempts. Returning to main menu.\n");
        return;
    }
    do {
        printf("\n============== ADMIN MENU ==============\n");
        printf(" 1. Add student\n");
        printf(" 2. Edit student\n");
        printf(" 3. Add sports / tournament record\n");
        printf(" 4. Edit sports record\n");
        printf(" 5. Delete sports record\n");
        printf(" 6. View a student's profile\n");
        printf(" 7. View all students\n");
        printf(" 0. Logout\n");
        choice = read_int("Choice: ", 0, 7);

        switch (choice) {
            case 1: add_student();        break;
            case 2: edit_student();       break;
            case 3: add_record();         break;
            case 4: edit_record();        break;
            case 5: delete_record();      break;
            case 6: view_student();       break;
            case 7: view_all_students();  break;
            case 0: printf("  Logged out.\n"); break;
        }
    } while (choice != 0);
}
