/* main.c - entry point: role selection menu */
#include <stdio.h>
#include "sports.h"

int main(void)
{
    int choice;
    do {
        printf("\n==================================================\n");
        printf("   STUDENT DEVELOPMENT SYSTEM - SPORTS & TOURNAMENTS\n");
        printf("==================================================\n");
        printf(" 1. Admin Portal   (data entry & editing)\n");
        printf(" 2. Student Portal (view only)\n");
        printf(" 3. Exit\n");
        choice = read_int("Choice: ", 1, 3);

        switch (choice) {
            case 1: run_admin_module();   break;
            case 2: run_student_module(); break;
            case 3: printf("Goodbye!\n");  break;
        }
    } while (choice != 3);
    return 0;
}
