# Devtrack-student-development-system
Hello

This is where we will be showing our logics and critical thinking skills as a 1st year 









# Sports & Tournament Module - Student Development System

Pure C (C99) console module with two portals:

| Module  | File               | Access                                                     |
|---------|--------------------|------------------------------------------------------------|
| Admin   | `admin_module.c`   | Password protected. Add/edit students, add/edit/delete sports records, view all |
| Student | `student_module.c` | **View only.** Enter Enrollment No. -> see Name, Class, Section, past tournaments |

## Build & run
```
./sports_module  
```
Default admin password: `admin123` (change `ADMIN_PASSWORD` in `include/sports.h`).

## Files
```
include/sports.h      structs, constants, prototypes
src/main.c            role selection menu
src/admin_module.c    admin menu + data entry/editing
src/student_module.c  read-only student view
src/storage.c         binary file handling (fopen/fread/fwrite/fseek)
src/utils.c           safe input + validation helpers
docs/sports_module_flowchart.pdf   flowchart
```
## Data
- `students_sports.dat` - `struct Student` (enrollment, name, class_name, section)
- `sports.dat` - `struct SportsRecord` (enrollment, sport, tournament, year, position, matches_played, achievement)

Both are linked by the enrollment number and are created automatically.
