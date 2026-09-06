#include "../include/SeatManager.h"

SeatManager::SeatManager()
{
    // Default: Saari seats 0 (Khali) initialize karein
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            seats[i][j] = 0;
        }
    }
}

void SeatManager::displaySeats()
{
    std::cout << "\n--- BUS SEAT MATRIX (0 = Khali, 1 = Booked) ---\n";
    for (int i = 0; i < 10; i++)
    {
        std::cout << "Row " << i + 1 << ": ";
        for (int j = 0; j < 4; j++)
        {
            std::cout << seats[i][j] << " ";
        }
        std::cout << "\n";
    }
}

bool SeatManager::bookSeat(int row, int col)
{
    if (row < 0 row >= 10 col < 0 || col >= 4)
    {
        std::cout << "Invalid seat position!\n";
        return false;
    }
    if (seats[row][col] == 0)
    {
        seats[row][col] = 1;
        std::cout << "Seat successfully booked!\n";
        return true;
    }
    std::cout << "Seat pehle se booked hai!\n";
    return false;
}

void SeatManager::addToWaitingList(int studentID)
{
    waitingList.push(studentID);
    std::cout << "Student ID " << studentID << " Waiting list mein add ho gaya.\n";
}

// .txt File mein data save karne ka function
void SeatManager::saveSeatsToFile(const std::string &filename)
{
    std::ofstream outFile(filename);
    if (!outFile)
    {
        std::cerr << "File open karne me error!\n";
        return;
    }
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            outFile << seats[i][j] << " ";
        }
        outFile << "\n";
    }
    outFile.close();
    std::cout << "Seats data successfully .txt file me save ho gaya!\n";
}

// .txt File se data load karne ka function
void SeatManager::loadSeatsFromFile(const std::string &filename)
{
    std::ifstream inFile(filename);
    if (!inFile)
    {
        std::cerr << "File nahi mili, default setup loading...\n";
        return;
    }
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            inFile >> seats[i][j];
        }
    }
    inFile.close();
    std::cout << "Seats data successfully .txt file se load ho gaya!\n";
}
