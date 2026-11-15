# SeatWise login and back-paper update

## Student login
- Username: student's College ID (for example `SW20260032`).
- Initial password: `seatwise@123`.
- Students must change the initial password on first login.
- Passwords are stored as scrypt hashes.
- New students added manually or imported from Excel automatically receive a login.
- Existing linked student accounts are migrated to College ID usernames and receive the initial password once when the `must_change_password` column is first added. Existing custom passwords are not reset on subsequent server starts.

## Room types
1. Open Admin → Room management.
2. For each room reserved for back-paper examinations, click **Set as back-paper room** (or tick **Dedicated back-paper room** while creating it).
3. Regular allocation only uses regular rooms. Back-paper allocation only uses rooms marked as back-paper rooms.
4. A room with regular seats already assigned cannot be converted to a back-paper room until its regular allocation is cleared/reassigned.

## Back-paper allocation
- Students register for subjects matching their course.
- Admin selects dedicated back-paper rooms and runs allocation.
- Subjects scheduled at the same date and time share the selected back-paper room pool. Different time slots may reuse those rooms.
- The back-paper allocator uses four checkerboard parity classes. All students for one subject in a given room are placed in the same parity class, so no two students taking the same subject are horizontally, vertically, or diagonally adjacent.
- If the strict separation rule cannot fit all students in the selected rooms, allocation fails without replacing the previous saved allocation. Add dedicated rooms or split the session.
- The back-paper seating map is available in Admin → Back-paper allocation.

## Run
From `daaproject/backend`:

```powershell
npm install
npm test
node server.js
```

Keep your existing `.env` file in `daaproject/backend`. Do not share it because it contains database credentials.
