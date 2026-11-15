# SeatWise Phase 2 — Setup

## Included
- Existing SeatWise frontend, Express backend, MySQL integration and DAA allocation modules.
- OS-style allocation job history, allocation queue/critical section and transactional seating replacement.
- Login page, hashed passwords (Node.js scrypt), role-based Admin/Student/Invigilator access, logout and session expiry.
- Student accounts can only view the seat associated with their own student record.

## 1. Configure MySQL
Use your existing SeatWise database and schema. Do not create a second database if your current app already uses one. Ensure the MySQL account has permission to create the `system_jobs` and `users` tables. The backend creates those two tables automatically at startup.

## 2. Configure backend
1. Open `backend` in a terminal.
2. Copy `.env.example` to `.env`.
3. Enter your own DB host, username, password and database name.
4. Change `ADMIN_PASSWORD` to a strong password of at least 8 characters before starting. Keep `.env` private and never commit it.
5. Run `npm install`, then `npm start`.

The first startup creates the admin user using `ADMIN_USERNAME` and `ADMIN_PASSWORD` if that username does not already exist. If an admin account already exists, changing the environment password will not reset it; update the account password through your database/admin procedure.

## 3. Open frontend
Open `frontend/index.html` using VS Code Live Server or another local static server. The default API URL is `http://localhost:8080/api`. If your backend uses another port, update `API_BASE_URL` at the top of `frontend/app.js`.

## 4. Log in and create student accounts
- Admin: use the username/password from your `.env` on first setup.
- Add the student in Student Management first.
- In **User accounts → Create student login**, enter the student's college/student ID and a password of at least 8 characters.
- Student username is the college/student ID. Give the username/password privately to that student.
- Student login sees only their own seat page; admin-only APIs return 403 for student sessions.
- Use Log out to end the session. Sessions expire after 8 hours and are cleared when the backend restarts.

## Notes
- Session tokens are kept in server memory; this is suitable for a college project/demo, not a horizontally scaled production deployment.
- For a public production deployment, add HTTPS, rate limiting, persistent/revocable sessions, password reset and account lockout policies.
- Back up the database before testing allocation changes. The allocation code uses a transaction to replace the active seating layout.


## Course matching, passwords, room maps and invigilators

- Keep your existing `backend/.env` file and its database credentials unchanged. The example `.env.example` is also unchanged.
- On startup, the backend adds the `course` columns to existing `students` and `exams` tables when missing, and creates the invigilator assignment tables.
- When adding a student, enter a unique college/student ID, group and course. The college ID is the student's login username.
- Add an exam with a course name that matches the student's course (case and surrounding spaces are ignored). Students see matching exams in their student portal.
- Admins can create or reset a student's login password from Students → Create student login. The username is the college ID. Passwords must be at least 8 characters.
- Signed-in admins and students can change their own password using the Change password button. For security, a password cannot be reset from the login screen without authenticating; students should ask an administrator to reset it.
- Register invigilators under Exam schedule, then choose an exam and invigilator to assign them.
- Under Rooms, select a room or click View map to see the current seating arrangement. The map shows the latest seating allocation, not a separate allocation per exam.


## Phase 2 additions

On startup, the backend creates/upgrades the invigilator account field and back-paper tables automatically. Keep your existing `backend/.env` unchanged; it still supplies the same MySQL connection and admin login settings.

- **Invigilators:** Add a staff record in Exam Schedule, then create/reset a login using that employee ID and a password of at least 8 characters. The invigilator signs in with the employee ID and sees assigned exam dates/times.
- **Back papers:** Add course-specific subjects in Back-paper Management. Students see subjects matching their saved course and can register their back papers. Admin can review registrations, select rooms and allocate a separate back-paper seating plan.
- **Room selection:** Check the rooms you want to use under Allocation Rooms. If none are checked, regular allocation uses all rooms.
- **Move seats:** On the Overview room map, drag an occupied student seat onto another room card. The backend assigns an available seat in that destination room. Admin can also search by name/college ID and use the room selector.
- **Database:** New tables are `backpaper_subjects`, `backpaper_registrations`, and `backpaper_seating`; these are separate from regular `seating`.

Use a unique employee ID per invigilator. Back-paper subjects must be entered using the same course label as the student's course (for example, `B.Tech CSE`, `MBA`, or `BCA`) so the course match works.


## Latest Phase 2 improvements
- Admin seating map renders every row/column for each room. Drag a student directly onto an empty seat to select the exact room, row and column; occupied destinations are rejected.
- Admin actions are split into focused sidebar tabs for adding students, user accounts, student list, student search, rooms, exams, invigilators, back-paper subjects and back-paper allocation.
- Exam dates/times can be changed from the scheduled exams list.
- Back-paper subjects can be assigned an exam date/time. Allocation is performed separately for each subject, so a student registered for DBMS and OS gets one assignment for each subject's exam.
- New database tables/columns are created on backend startup. Keep a database backup before running a new build against important data.


## Phase 2 fixes included in this build
- **Room-specific invigilation:** Admin assigns an exam + room + invigilator. The `exam_room_invigilators` table stores all three, and the invigilator portal displays the assigned room, course, date/time, and room capacity. Existing assignments in the older `exam_invigilators` table do not contain room information. The auto-assign action migrates compatible legacy duties into the first available room, then fills missing room duties; if a legacy duty conflicts with an overlapping assignment, it is skipped and the normal allocator chooses a free invigilator.
- **Scheduled back-paper list:** The Back-paper subjects tab shows scheduled subjects by course, date/time, number of registered students, and seat-allocation status.
- **Course-first back-paper setup:** Choose a course from the dropdown, then choose an existing subject for that course or select “Add a new subject…”. Set the exam date/time and save.
- **Student portal separated into tabs:** My seat, My exams, Back-paper registration, and My back-paper seats are separate student-only tabs.
- **Seat-allocation status in Student list:** Shows “Seat allotted” when a student has a regular or subject-wise back-paper seat, otherwise “Not allotted”. The status refreshes after allocation.

### After updating
1. Keep your existing `backend/.env` file and credentials. This ZIP intentionally does not include `.env`.
2. Stop the old backend process and start the backend again so startup creates the new `exam_room_invigilators` table.
3. Log in as admin and reassign invigilators with the room selector. Old exam-only assignments cannot identify a correct room automatically.
4. Test one scheduled back-paper subject, registration counts, allocation status, and student seat visibility against your own MySQL database.


## Corrected build: edge cases and requested updates

- **Back-paper schedule editing/removal:** the scheduled-exam list now has Edit schedule and Remove actions. Removing a subject also removes its registrations and subject-specific seating rows; the action asks for confirmation.
- **Back-paper registration visibility:** student checkboxes reflect saved registrations. A separate “Back papers I have applied for” list shows the exam date/time and seat status. Saving an empty selection removes all registrations intentionally; a missing/malformed selection is rejected.
- **Back-paper seat correctness:** allocation validates course membership, requires a date/time for registered exams, rejects a student registered in two back papers at the same date/time, and avoids assigning the same physical seat to different simultaneous subjects. Insufficient room capacity fails without replacing the previous allocation.
- **Regular seat allocation fallback:** the database group column is mapped to the algorithm's expected `group` field. The CSP still tries to separate groups, but if that is impossible it fills remaining physical seats rather than silently leaving students without seats. The response reports any students left unallocated because total room capacity is insufficient.
- **Course-wise student filter:** the Student list has an All courses/course dropdown filter and shows a seat status for each student.
- **Invigilators:** Auto-assign missing room duties assigns available invigilators without overwriting room duties already assigned manually. Admin can choose an exam and room, see the current assignment, and assign/update it to change an invigilator. If there are not enough staff for overlapping duties, auto-assignment reports the shortage and rolls back rather than saving a partial result.
- **User accounts:** Admin’s User accounts tab lists usernames, roles, active status and linked student/staff records; creating/resetting a login refreshes the list. Password hashes are never returned to the UI.
- **Student login edge cases:** account creation checks username/linked-student conflicts before updating, to avoid accidentally changing another user's account.
- **Automated tests:** run `npm test` inside `backend` for algorithm boundary tests and `npm run check` for JavaScript syntax checks.

### Notes on testing

The automated test suite exercises the allocation algorithms, same-group constraint fallback, seat uniqueness, multi-room capacity, insufficient capacity, and invalid room dimensions. The project was not connected to your local MySQL instance during this build, so you should still run the smoke-test checklist below against a backup of your own database after installing dependencies.

### Smoke-test checklist

1. Sign in as admin and confirm the User accounts list loads.
2. Add a student, create/reset their login, then confirm the user appears in the list.
3. Filter Student list by each course; verify the list is empty only when no students match.
4. Run regular allocation with enough capacity, including several students in the same group; confirm allocated + unallocated equals total students and no student/seat is duplicated.
5. Run allocation with too few seats; verify unallocated count is reported and existing seating is preserved if the allocation fails.
6. Add a back-paper subject, edit its date/time, remove it, and confirm the list and registrations update.
7. Register one student for two back papers at different times, allocate seats, and verify both show in the student portal.
8. Try registering that student for two back papers at the same date/time; allocation should reject the conflict.
9. Try allocating a registered back-paper subject without a date/time or with insufficient rooms; allocation should fail without deleting the previous allocation.
10. Auto-assign invigilators with enough staff, then change one assignment manually. Try an overlapping schedule with insufficient staff; auto-assignment should roll back and show a useful error.
11. Sign in as a student and verify they can see only their own saved registrations and seat assignments; admin-only user and allocation APIs should be denied.

## Phase 2 operations and seating optimization

- **Room subset optimization:** the allocator uses an exact subset-sum search for ordinary room inventories to choose the smallest total capacity that can seat the cohort. This reduces unused seats and avoids opening unnecessary rooms. Large inventories use a bounded best-fit fallback.
- **Course-aware seating:** students are placed in checkerboard order, choosing seats that avoid adjacent students from the same course where possible. Small/medium rooms get a bounded local-swap optimization pass. If a hard separation is mathematically impossible (for example, one course fills the entire room), the allocator still seats everyone and reports remaining adjacency conflicts.
- **Preview before applying:** admins can preview the room subset, capacity, unseated count, empty seats and same-course adjacency conflicts without modifying the database.
- **Allocation quality dashboard:** shows seated/unseated counts, occupied rooms, empty seats and capacity utilization.
- **Reports and recovery:** export a seating CSV, print a report, and download/restore a JSON backup of the regular seating layout. Restore validates IDs, duplicate seats and room bounds before replacing the current layout in a transaction.
- **Conflict checks:** reports adjacent same-course students, students left unseated, and exams sharing an exact stored date/time. The current schema stores a single exam timestamp, not start/end durations, so it cannot detect partial time overlaps.
- **Attendance:** admins and assigned invigilators can mark students present/absent for their course's exam. Invigilators are restricted to students seated in their assigned room.
- **Notifications and audit trail:** admins can send in-app notifications to all users or a role; important allocation, seat-move, attendance, notification, restore and invigilator-assignment actions are logged.
- **Back-paper seating:** subjects with the same exam timestamp are allocated as one combined cohort so they cannot occupy the same physical seat; seats can be reused by exams at different times. The same room-subset and course-aware placement algorithm is used.

New tables (`audit_logs`, `notifications`, `exam_attendance`) are created automatically on backend startup. Keep your existing `backend/.env` file; the ZIP includes only `.env.example`.
