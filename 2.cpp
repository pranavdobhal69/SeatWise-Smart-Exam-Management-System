#include <iostream>
#include <vector>
#include <string>
#include <cmath>

struct Student {
    std::string name;
    std::string group;
};

struct UtilizationStats {
    int totalSeats;
    int occupiedSeats;
    int emptySeats;
    double utilizationPercentage;
};

// ALGO NAME: CONSTRAINT SATISFACTION PROBLEM (CSP) BACKTRACKING
bool isSafe(const std::vector<std::vector<Student*>>& classroom, int row, int col, Student* student, int rows, int cols) {
    if (col > 0) {
        Student* leftStudent = classroom[row][col - 1];
        if (leftStudent != nullptr && leftStudent->group == student->group) {
            return false;
        }
    }

    if (col < cols - 1) {
        Student* rightStudent = classroom[row][col + 1];
        if (rightStudent != nullptr && rightStudent->group == student->group) {
            return false;
        }
    }

    if (row > 0) {
        Student* frontStudent = classroom[row - 1][col];
        if (frontStudent != nullptr && frontStudent->group == student->group) {
            return false;
        }
    }

    if (row < rows - 1) {
        Student* backStudent = classroom[row + 1][col];
        if (backStudent != nullptr && backStudent->group == student->group) {
            return false;
        }
    }

    return true;
}

bool fillRoomMatrix(std::vector<std::vector<Student*>>& classroom, const std::vector<Student*>& students, std::vector<bool>& used, int index, int rows, int cols) {
    if (index == rows * cols) {
        return true;
    }

    int row = index / cols;
    int col = index % cols;

    for (size_t i = 0; i < students.size(); i++) {
        if (!used[i]) {
            Student* candidateStudent = students[i];

            if (isSafe(classroom, row, col, candidateStudent, rows, cols)) {
                classroom[row][col] = candidateStudent;
                used[i] = true;

                if (fillRoomMatrix(classroom, students, used, index + 1, rows, cols)) {
                    return true;
                }

                classroom[row][col] = nullptr;
                used[i] = false;
            }
        }
    }

    if (fillRoomMatrix(classroom, students, used, index + 1, rows, cols)) {
        return true;
    }

    return false;
}

bool runPreAllocationSanityCheck(const std::vector<Student*>& students, int rows, int cols) {
    if (students.empty()) return false;
    if (rows <= 0 || cols <= 0) return false;
    return true;
}

// ALGO NAME: LOCAL RE-OPTIMIZATION STRATEGY
int countConflicts(const std::vector<std::vector<Student*>>& classroom, int rows, int cols) {
    int totalConflictsFound = 0;
    int dynamicRowOffsets[2] = {0, 1};
    int dynamicColOffsets[2] = {1, 0};

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Student* currentSeatHolder = classroom[i][j];
            if (currentSeatHolder != nullptr) {
                for (int k = 0; k < 2; k++) {
                    int neighborRowIndex = i + dynamicRowOffsets[k];
                    int neighborColIndex = j + dynamicColOffsets[k];

                    if (neighborRowIndex < rows && neighborColIndex < cols) {
                        Student* targetNeighbor = classroom[neighborRowIndex][neighborColIndex];
                        if (targetNeighbor != nullptr && currentSeatHolder->group == targetNeighbor->group) {
                            totalConflictsFound = totalConflictsFound + 1;
                        }
                    }
                }
            }
        }
    }
    return totalConflictsFound;
}

std::vector<std::vector<Student*>> optimizeRoomLayout(std::vector<std::vector<Student*>> classroom, int rows, int cols) {
    int baselineConflictMetrics = countConflicts(classroom, rows, cols);
    if (baselineConflictMetrics == 0) {
        return classroom;
    }

    for (int r1 = 0; r1 < rows; r1++) {
        for (int c1 = 0; c1 < cols; c1++) {
            for (int r2 = 0; r2 < rows; r2++) {
                for (int c2 = 0; c2 < cols; c2++) {
                    if (r1 == r2 && c1 == c2) continue;

                    Student* studentNodeA = classroom[r1][c1];
                    Student* studentNodeB = classroom[r2][c2];

                    if (studentNodeA != nullptr || studentNodeB != nullptr) {
                        Student* temporaryHolder = classroom[r1][c1];
                        classroom[r1][c1] = classroom[r2][c2];
                        classroom[r2][c2] = temporaryHolder;

                        int evaluatedConflictMetrics = countConflicts(classroom, rows, cols);

                        if (evaluatedConflictMetrics < baselineConflictMetrics) {
                            baselineConflictMetrics = evaluatedConflictMetrics;
                        } else {
                            Student* rollbackHolder = classroom[r1][c1];
                            classroom[r1][c1] = classroom[r2][c2];
                            classroom[r2][c2] = rollbackHolder;
                        }
                    }
                }
            }
        }
    }
    return classroom;
}

UtilizationStats computeRoomUtilizationStatistics(const std::vector<std::vector<Student*>>& classroom, int rows, int cols) {
    int occupiedSeatsCounter = 0;
    int maximumCapacitySeats = rows * cols;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (classroom[r][c] != nullptr) {
                occupiedSeatsCounter = occupiedSeatsCounter + 1;
            }
        }
    }

    double percentageRatioValue = 0;
    if (maximumCapacitySeats > 0) {
        percentageRatioValue = (static_cast<double>(occupiedSeatsCounter) / maximumCapacitySeats) * 100.0;
    }

    UtilizationStats stats;
    stats.totalSeats = maximumCapacitySeats;
    stats.occupiedSeats = occupiedSeatsCounter;
    stats.emptySeats = maximumCapacitySeats - occupiedSeatsCounter;
    stats.utilizationPercentage = percentageRatioValue;

    return stats;
}