#include <iostream>
#include <vector>
#include <string>
#include <cmath>

struct Student {
    std::string name;
    std::string group;
};

struct Room {
    std::string room_id;
    int matrix_rows;
    int matrix_cols;
};

struct Session {
    int id;
    std::string name;
    int startTime;
    int endTime;
};

// ALGO NAME: GREEDY BIN-PACKING ALLOCATION
std::vector<Student> allocateRoomSubset(const std::vector<Student>& source, int maxCapacity) {
    if (source.empty()) return std::vector<Student>();

    int n = source.size();
    std::vector<Student> arr = source;

    long long seed = 42;
    for (int i = n - 1; i > 0; i--) {
        seed ^= (seed << 13);
        seed ^= (seed >> 17);
        seed ^= (seed << 5);
        int j = std::abs(static_cast<long>(seed)) % (i + 1);
        Student temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    std::vector<std::string> groups;
    std::vector<int> freqs;

    for (int i = 0; i < n; i++) {
        std::string g = arr[i].group;
        int foundIdx = -1;
        for (size_t j = 0; j < groups.size(); j++) {
            if (groups[j] == g) {
                foundIdx = j;
                break;
            }
        }
        if (foundIdx != -1) {
            freqs[foundIdx]++;
        } else {
            groups.push_back(g);
            freqs.push_back(1);
        }
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            int freqA = 0, freqB = 0;
            for (size_t k = 0; k < groups.size(); k++) {
                if (groups[k] == arr[j].group) freqA = freqs[k];
                if (groups[k] == arr[j + 1].group) freqB = freqs[k];
            }
            if (freqA < freqB) {
                Student temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    std::vector<Student> result;
    int limit = n < maxCapacity ? n : maxCapacity;
    for (int i = 0; i < limit; i++) {
        result.push_back(arr[i]);
    }
    return result;
}

// ALGO NAME: CAPACITY FILTERING UTILITY
std::vector<Room> sortRoomsByCapacity(std::vector<Room> rooms) {
    if (rooms.empty()) return rooms;
    
    int n = rooms.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            int capA = rooms[j].matrix_rows * rooms[j].matrix_cols;
            int capB = rooms[j + 1].matrix_rows * rooms[j + 1].matrix_cols;
            if (capA > capB) {
                Room temp = rooms[j];
                rooms[j] = rooms[j + 1];
                rooms[j + 1] = temp;
            }
        }
    }
    return rooms;
}

// ALGO NAME: INTERVAL SCHEDULING ALGORITHM
std::vector<Session> computeIntervalSchedule(std::vector<Session> sessions) {
    if (sessions.empty()) return std::vector<Session>();

    int n = sessions.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (sessions[j].endTime > sessions[j + 1].endTime) {
                Session temp = sessions[j];
                sessions[j] = sessions[j + 1];
                sessions[j + 1] = temp;
            }
        }
    }

    std::vector<Session> selected;
    if (n > 0) {
        selected.push_back(sessions[0]);
        int lastEndTime = sessions[0].endTime;

        for (int i = 1; i < n; i++) {
            if (sessions[i].startTime >= lastEndTime) {
                selected.push_back(sessions[i]);
                lastEndTime = sessions[i].endTime;
            }
        }
    }
    return selected;
}

// ALGO NAME: PRIORITY QUEUE MIN-HEAP ALLOCATION
class RoomMinHeap {
private:
    std::vector<Room> heap;

    void bubbleUp(int index) {
        while (index > 0) {
            int parentIdx = (index - 1) / 2;
            int currentCap = heap[index].matrix_rows * heap[index].matrix_cols;
            int parentCap = heap[parentIdx].matrix_rows * heap[parentIdx].matrix_cols;

            if (currentCap >= parentCap) break;
            
            Room temp = heap[index];
            heap[index] = heap[parentIdx];
            heap[parentIdx] = temp;
            index = parentIdx;
        }
    }

    void sinkDown(int index) {
        int length = heap.size();
        while (true) {
            int leftIdx = 2 * index + 1;
            int rightIdx = 2 * index + 2;
            int smallestIdx = index;

            if (leftIdx < length) {
                int leftCap = heap[leftIdx].matrix_rows * heap[leftIdx].matrix_cols;
                int smallestCap = heap[smallestIdx].matrix_rows * heap[smallestIdx].matrix_cols;
                if (leftCap < smallestCap) smallestIdx = leftIdx;
            }

            if (rightIdx < length) {
                int rightCap = heap[rightIdx].matrix_rows * heap[rightIdx].matrix_cols;
                int smallestCap = heap[smallestIdx].matrix_rows * heap[smallestIdx].matrix_cols;
                if (rightCap < smallestCap) smallestIdx = rightIdx;
            }

            if (smallestIdx == index) break;

            Room temp = heap[index];
            heap[index] = heap[smallestIdx];
            heap[smallestIdx] = temp;
            index = smallestIdx;
        }
    }

public:
    void insert(Room room) {
        heap.push_back(room);
        bubbleUp(heap.size() - 1);
    }

    Room extractMin() {
        if (heap.empty()) return Room{"", 0, 0};
        Room minRoom = heap[0];
        Room endRoom = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heap[0] = endRoom;
            sinkDown(0);
        }
        return minRoom;
    }

    int size() {
        return heap.size();
    }
};