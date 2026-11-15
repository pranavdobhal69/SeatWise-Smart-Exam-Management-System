# SeatWise targeted login and back-paper changes

Only the student login and back-paper/room allocation workflow were changed. Regular allocation logic and the rest of the project files were retained, except regular allocation now explicitly excludes rooms designated for back-paper use.

## Student credentials
- Student username is their College ID, e.g. `SW20260032`.
- Initial password is `seatwise@123`.
- First login requires changing the password (minimum 8 characters).
- Passwords are scrypt-hashed.
- Existing linked student accounts are migrated once to College ID + default password when the `must_change_password` column is first introduced. Accounts marked for initial password change remain on the default until they change it. Custom passwords after the change are preserved on future restarts.
- New students added manually or imported through Excel automatically receive an account.

## Dedicated back-paper rooms
- Existing rooms default to regular examination rooms.
- Admin can mark an existing room as a dedicated back-paper room from Room management, or mark it while creating the room.
- Regular allocation and regular invigilator auto-assignment exclude dedicated back-paper rooms.
- Back-paper allocation only accepts rooms marked as dedicated back-paper rooms.
- A room with existing regular seat assignments cannot be converted to a back-paper room until those regular assignments are cleared/reassigned.

## Back-paper seating rules
- Exams with the same date/time are allocated together using the dedicated back-paper room pool.
- A student cannot be registered for two back-paper exams at the same date/time.
- For each room and paper, students are assigned within one checkerboard parity class. This ensures no same-paper students are adjacent horizontally, vertically, or diagonally (all eight surrounding directions).
- If the selected rooms cannot satisfy the strict separation rule, the operation fails without replacing the previous saved back-paper allocation. Add more dedicated rooms or split the session.
- Admin can view the room-wise back-paper seating map, including subject names, student IDs and empty seats.

## Start and verify
From `daaproject/backend`:

```powershell
npm install
npm test
node server.js
```

Keep the existing `backend/.env` file in place. The database migration is applied automatically at server startup; do not drop or recreate the `seatwise` database.
