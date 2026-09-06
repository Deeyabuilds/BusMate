#ifndef SEAT_MANAGER_H
#define SEAT_MANAGER_H

#include <iostream>
#include <fstream>
#include <queue>

class SeatManager
{
private:
    int seats[10][4];            // 2D Array: 10 Rows x 4 Columns (0 = Vacant, 1 = Booked)
    std::queue<int> waitingList; // Queue for waiting list

public:
    SeatManager();
    void displaySeats();
    bool bookSeat(int row, int col);
    void addToWaitingList(int studentID);

    // .txt File handling functions
    void saveSeatsToFile(const std::string &filename);
    void loadSeatsFromFile(const std::string &filename);
};

#endif