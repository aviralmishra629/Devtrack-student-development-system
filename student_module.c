/*
 * student_module.c - STUDENT MODULE (view only)
 * Opens the data files in read mode only: a student cannot add,
 * change or delete anything from here.
 */
#include <stdio.h>
#include "sports.h"

void print_record(int no, const SportsRecord *r)
{
    printf("  #%d  %s - %s (%d)\n", no, r->sport, r->tournament, r->year);
    printf("      Position    : %s\n", r->position);
    printf("      Matches     : %d\n", r->matches_played);
    printf("      Achievement : %s\n", r->achievement);
}

void print_profile(const Student *s, const RecordRef *refs, int n)
{
    int i, total_matches = 0;

    printf("\n==================================================\n");
    printf("            STUDENT SPORTS PROFILE\n");
    printf("==================================================\n");
    printf(" Enrollment No. : %s\n", s->enrollment);
    printf(" Name           : %s\n", s->name);
    printf(" Class          : %s\n", s->class_name);
    printf(" Section        : %s\n", s->section);
    printf("--------------------------------------------------\n");
    printf(" PAST TOURNAMENTS & SPORTS ACTIVITY\n");
    printf("--------------------------------------------------\n");

    if (n == 0) {
        printf("  No sports or tournament records found.\n");
    } else {
        for (i = 0; i < n; i++) {
            print_record(i + 1, &refs[i].rec);
            total_matches += refs[i].rec.matches_played;
            if (i < n - 1) printf("\n");
        }
        printf("--------------------------------------------------\n");
        printf(" Total tournaments: %d   Total matches: %d\n",
               n, total_matches);
    }
    printf("==================================================\n");
}

void run_student_module(void)
{
    char enrol[16];
    Student s;
    RecordRef refs[MAX_RECORDS];
    int n;

    printf("\n------------- STUDENT PORTAL (View Only) -------------\n");
    read_enrollment("Enter your Enrollment No.: ", enrol, sizeof enrol);

    if (!storage_find_student(enrol, &s, NULL)) {
        printf("\n  ! No student found with Enrollment No. %s\n", enrol);
        printf("  Please contact your sports coordinator.\n");
        return;
    }
    n = storage_get_records(enrol, refs, MAX_RECORDS);
    print_profile(&s, refs, n);
}
