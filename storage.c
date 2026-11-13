/* storage.c - all binary file operations (fopen/fread/fwrite/fseek/fclose) */
#include <stdio.h>
#include <string.h>
#include "sports.h"

/* Linear search of students_sports.dat by enrollment number. */
int storage_find_student(const char *enrol, Student *out, long *offset)
{
    FILE *fp = fopen(STUDENT_FILE, "rb");
    Student s;
    long pos = 0;
    if (!fp) return 0;
    while (fread(&s, sizeof s, 1, fp) == 1) {
        if (strcmp(s.enrollment, enrol) == 0) {
            if (out)    *out = s;
            if (offset) *offset = pos;
            fclose(fp);
            return 1;
        }
        pos += (long)sizeof s;
    }
    fclose(fp);
    return 0;
}

int storage_add_student(const Student *s)
{
    FILE *fp = fopen(STUDENT_FILE, "ab");
    if (!fp) return 0;
    int ok = (fwrite(s, sizeof *s, 1, fp) == 1);
    fclose(fp);
    return ok;
}

int storage_update_student(long offset, const Student *s)
{
    FILE *fp = fopen(STUDENT_FILE, "rb+");
    if (!fp) return 0;
    int ok = (fseek(fp, offset, SEEK_SET) == 0 &&
              fwrite(s, sizeof *s, 1, fp) == 1);
    fclose(fp);
    return ok;
}

/* Used by "view all students": returns 1 while index is valid. */
int storage_get_student_at(int index, Student *out)
{
    FILE *fp = fopen(STUDENT_FILE, "rb");
    if (!fp) return 0;
    int ok = (fseek(fp, (long)index * (long)sizeof *out, SEEK_SET) == 0 &&
              fread(out, sizeof *out, 1, fp) == 1);
    fclose(fp);
    return ok;
}

int storage_add_record(const SportsRecord *r)
{
    FILE *fp = fopen(SPORTS_FILE, "ab");
    if (!fp) return 0;
    int ok = (fwrite(r, sizeof *r, 1, fp) == 1);
    fclose(fp);
    return ok;
}

/* Collects every record of one student (in the order they were entered). */
int storage_get_records(const char *enrol, RecordRef *refs, int max)
{
    FILE *fp = fopen(SPORTS_FILE, "rb");
    SportsRecord r;
    long pos = 0;
    int n = 0;
    if (!fp) return 0;
    while (n < max && fread(&r, sizeof r, 1, fp) == 1) {
        if (strcmp(r.enrollment, enrol) == 0) {
            refs[n].rec = r;
            refs[n].offset = pos;
            n++;
        }
        pos += (long)sizeof r;
    }
    fclose(fp);
    return n;
}

int storage_update_record(long offset, const SportsRecord *r)
{
    FILE *fp = fopen(SPORTS_FILE, "rb+");
    if (!fp) return 0;
    int ok = (fseek(fp, offset, SEEK_SET) == 0 &&
              fwrite(r, sizeof *r, 1, fp) == 1);
    fclose(fp);
    return ok;
}

/* Delete = copy every other record to a temp file, then swap files. */
int storage_delete_record(long offset)
{
    FILE *in = fopen(SPORTS_FILE, "rb");
    FILE *out;
    SportsRecord r;
    long pos = 0;
    if (!in) return 0;
    out = fopen(TEMP_FILE, "wb");
    if (!out) { fclose(in); return 0; }
    while (fread(&r, sizeof r, 1, in) == 1) {
        if (pos != offset) fwrite(&r, sizeof r, 1, out);
        pos += (long)sizeof r;
    }
    fclose(in);
    fclose(out);
    remove(SPORTS_FILE);                 /* needed on Windows */
    return rename(TEMP_FILE, SPORTS_FILE) == 0;
}
