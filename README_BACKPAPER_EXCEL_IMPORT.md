# Back-paper Excel registration import

The Back-paper subjects page now includes an admin-only Excel import workflow.

## Excel columns

The first worksheet must contain these headers:

- `College ID` (aliases such as Student ID or Roll No are accepted)
- `Back-paper Subject` (aliases such as Subject or Subject Name are accepted)

The College ID must already exist in the regular SeatWise student database. The student's name and course are fetched from that database. The subject must already be created/scheduled for the student's course. The Excel file does not create student records or back-paper subjects.

## Workflow

1. Create the back-paper subjects and set their exam date/time.
2. In Back-paper subjects, click **Download Excel template** in **Import back-paper students from Excel**.
3. Add one row per student-subject registration and upload the workbook.
4. Click **Preview Excel**. Invalid rows are shown with a reason; only valid rows can be imported.
5. Click **Import valid rows** and confirm.
6. Review the updated Back-paper registrations list and scheduled exam counts, then allocate dedicated back-paper rooms.

Duplicate rows, unknown College IDs, unknown subjects, course mismatches, and already-existing registrations are rejected during preview. The import endpoint revalidates the data in a MySQL transaction before inserting, so a failed import does not partially save registrations.

This feature only inserts into `backpaper_registrations`; it does not create, update, or delete students, user accounts, regular exams, regular seating, or rooms.
