#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ranges>

/*
#ifdef _WIN32
#include <GetOpt.h>
#else
#include <unistd.h>
#endif
*/

/*
 * @brief flight is a struct containing information about a one specific flight
 * and setter / getter functions for said flight
 */
class Flight
{
private:
    int id;
    std::string departure;
    std::string destination;
    std::string date;
    std::string time;
    int fClassSeats;
    int bClassSeats;
    int eClassSeats;
    int occupiedSeats = 0;

public:
    // setter functions
    void setId(int id) { this->id = id; };
    void setDeparture(std::string departure) { this->departure = departure; };
    void setDestination(std::string destination) { this->destination = destination; };
    void setDate(std::string date) { this->date = date; };
    void setTime(std::string time) { this->time = time; };
    void setFClassSeats(int amount) { fClassSeats = amount; };
    void setBClassSeats(int amount) { bClassSeats = amount; };
    void setEClassSeats(int amount) { eClassSeats = amount; };
    void addOccupiedSeat() { occupiedSeats++; };

    // getter functions
    int getId() { return id; };
    std::string getDeparture() { return departure; };
    std::string getDestination() { return destination; };
    std::string getDate() { return date; };
    std::string getTime() { return time; };
    int getFClassSeats() { return fClassSeats; };
    int getBClassSeats() { return bClassSeats; };
    int getEClassSeats() { return eClassSeats; };
    int getOccupiedSeats() { return occupiedSeats; };
};

/*
 * @brief booking is a struct containing information about a specific booking
 * and setter/getter functions for said booking
 */
class Booking
{
private:
    int id;
    std::string date;
    std::string time;
    std::string departure;
    std::string destination;
    std::string seatType;
    std::string firstName;
    std::string lastName;
    int row;
    int seat;
public:
    // setter functions
    void setId(int id) { this->id = id; };
    void setDate(std::string date) { this->date = date; };
    void setTime(std::string time) { this->time = time; };
    void setDeparture(std::string departure) { this->departure = departure; };
    void setDestination(std::string destination) { this->destination = destination; };
    void setSeatType(std::string seatType) { this->seatType = seatType; };
    void setFirstName(std::string firstName) { this->firstName = firstName; };
    void setLastName(std::string lastName) { this->lastName = lastName; };
    void setRow(int row) { this->row = row; };
    void setSeat(int seat) { this->seat = seat; };

    // getter functions
    int getId() { return id; };
    std::string getDate() { return date; };
    std::string getTime() { return time; };
    std::string getDeparture() { return departure; };
    std::string getDestination() { return destination; };
    std::string getSeatType() { return seatType; };
    std::string getFirstName() { return firstName; };
    std::string getLastName() { return lastName; };
    int getRow() { return row; };
    int getSeat() { return seat; };
};


void populateFlightsList(std::string file, std::vector<Flight*>* list);

void populateBookingList(std::string file, std::vector<Booking*>* list);

void createSeatMap(std::vector<Flight*>& flightList, std::vector<Booking*>& bookingList);

void checkFlightPopulation(std::vector<Flight*>& flightList, std::vector<Booking*>& bookingList, std::vector<int>& canceledList);

void clearCanceledFlights(std::vector<Flight*>& flightList, std::vector<int>& canceledList);

void createTickets(std::vector<Flight*>& flightList, std::vector<Booking*>& bookingList);