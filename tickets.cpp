/*
 * @file tickets.cpp
 * @author Daniel Barbis (daniel.barbis@frostys.tech)
 * @brief Takes flight and ticket information to then create tickets matching the correct flight
 * and creates both a seatmap for the personell and a canceled-flights list
 *
 * @version 1.0
 * @date 2022-10-28
 *
 * @copyright Copyright (c) 2022
 */

#include "dependencies.h"

 /*
  * Main entry point
  *
  * @param[in] argc Number of command line arguments
  * @param[in] argv Array of command line arguments
  *
  * @param[out] returns 0 on successful execution
  */
int main(int argc, char** argv)
{
	std::string flightFile = "flights.csv";
	std::string bookingFile = "bookings.csv";

	std::vector<Flight*> flights;
	std::vector<Booking*> bookings;
	std::vector<int> canceledFlights;

	populateFlightsList(flightFile, &flights);
	populateBookingList(bookingFile, &bookings);

	createTickets(flights, bookings);

	checkFlightPopulation(flights, bookings, canceledFlights);

	createSeatMap(flights, bookings);

	std::cout << "Operations completed!" << std::endl;

	return 0;
}
