# SeatWise Admin Excel Imports

Two admin-only Excel import workflows have been added to the Exam Schedule page.

## Invigilators

Required headers (case-insensitive): `name`, `employee_id`

Example:

| name | employee_id |
|---|---|
| Amit Joshi | EMP0001 |
| Neha Sharma | EMP0002 |

Employee IDs already present in `invigilators` and duplicates within the workbook are marked invalid. Import inserts only valid preview rows into the existing `invigilators` table.

## Exams

Required headers (case-insensitive): `exam_name`, `exam_datetime`, `course`

Example:

| exam_name | exam_datetime | course |
|---|---|---|
| DBMS | 2026-11-02 09:30:00 | CSE |
| OS | 2026-11-03 14:00:00 | IT |

`exam_datetime` must be a valid date/time in `YYYY-MM-DD HH:MM:SS` format. `exam_name` is the subject name only (for example, `DBMS`, not `DBMS Mid Term`). Existing records with the same exam name, date/time and course are flagged as duplicates.

## Workflow

1. Open Admin → Exam Schedule.
2. Download the relevant template or prepare an Excel workbook with the exact required columns.
3. Select the file and click **Preview Excel**.
4. Review valid and invalid rows. Only valid rows are offered for import.
5. Click **Import** and confirm.

The backend accepts `.xlsx` and `.xls` files up to 5 MB and up to 5,000 rows. Imports require an admin session. No new database tables are required; records are inserted into the existing `invigilators` and `exams` tables. Import is transactional: if a duplicate is detected between preview and import, the batch is rolled back.
