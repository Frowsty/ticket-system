/*
 * @file dependencies.cpp
 * @author Daniel Barbis (daniel.barbis@frostys.tech)
 * @brief Contains all the function implementations of functions created in dependencies.h
 *
 * @version 1.0
 * @date 2022-10-28
 *
 * @copyright Copyright (c) 2022
 */

#include "dependencies.h"

/*
 * @brief This function will populate a vector with flight objects
 *
 * @param file The file name of which we will read data from
 * @param list The vector that should be populated with flight objects
 */
void populateFlightsList(std::string file, std::vector<Flight*>* list)
{
	std::ifstream flightData(file);
	std::string fileData;
	if (flightData.is_open())
	{
		while (getline(flightData, fileData) && !fileData.empty())
		{
			Flight* temp = new Flight;
			std::stringstream data(fileData);
			std::string tempString;
			getline(data, tempString, ',');
			temp->setId(atoi(tempString.c_str()));

			getline(data, tempString, ',');
			temp->setDeparture(tempString);

			getline(data, tempString, ',');
			temp->setDestination(tempString);

			getline(data, tempString, ',');
			temp->setDate(tempString);

			getline(data, tempString, ',');
			temp->setTime(tempString);

			getline(data, tempString, ',');
			temp->setFClassSeats(atoi(tempString.c_str()));

			getline(data, tempString, ',');
			temp->setBClassSeats(atoi(tempString.c_str()));

			getline(data, tempString, ',');
			temp->setEClassSeats(atoi(tempString.c_str()));

			list->push_back(temp);

			if (!data)
				break;
		}
	}
}

/*
 * @brief This function will create a seat map of the flights
 *
 * @param flightList A vector containing all the flights
 * @param bookingList A vector containing all the bookings
 */
void createSeatMap(std::vector<Flight*>& flightList, std::vector<Booking*>& bookingList)
{
	// remove the old seatmap file
	remove("flights-seatmap.txt");
	for (auto& flight : flightList)
	{
		int seats = flight->getFClassSeats() + flight->getBClassSeats() + flight->getEClassSeats();
		int rows = seats / 7; // In the planes at my airport we have 7 seats per row
		std::stringstream seatMap;

		int fClassRow = flight->getFClassSeats() / 7;
		int bClassRow = fClassRow + 1;
		int eClassRow = (flight->getFClassSeats() + flight->getBClassSeats()) / 7 + 1;
		bool seatMapped = false;
		seatMap << "\nFlight: " << flight->getId() << " Departure: " << flight->getDeparture() << " Destination: " << flight->getDestination() << " Date: " << flight->getDate() << " Time: " << flight->getTime() << "\n";

		seatMap << "First Class\n";
		for (int i = 1; i <= rows; i++)
		{
			if (i == bClassRow)
				seatMap << "Business Class\n";
			else if (i == eClassRow)
				seatMap << "Economy Class\n";

			for (int j = 1; j <= 7; j++)
			{
				if (j == 3 || j == 6)
					seatMap << " ";
				for (auto& booking : bookingList)
				{
					// check if the booking is on the same flight that is currently being seatMapped
					if ((flight->getTime() == booking->getTime()) && (flight->getDeparture() == booking->getDeparture()) &&
						(flight->getDestination() == booking->getDestination()) && (flight->getDate() == booking->getDate()))
					{
						if (booking->getRow() == i && booking->getSeat() == j)
						{
							seatMapped = true;
							seatMap << "[1]";
						}
					}
				}
				if (seatMapped)
					seatMapped = false;
				else
					seatMap << "[0]";
			}
			seatMap << "\n";
		}
		std::ofstream canceledFlights("flights-seatmap.txt", std::ios_base::app);
		canceledFlights << seatMap.str();
		canceledFlights.close();
	}
}

/*
 * @brief This function will check if a flight is populated and then
 * cancel the flight if we have determined that there is no seats booked
 *
 * @param flightList Reference to a vector containing flight* objects
 * @param bookingList Reference to a vector containing bookings* objects
 * @param canceledList Reference to a vector containing int (flight indexes)
 */
void checkFlightPopulation(std::vector<Flight*>& flightList, std::vector<Booking*>& bookingList, std::vector<int>& canceledList)
{
	// remove the old canceled flights data before we create the new file with new data
	remove("canceled-flights.txt");

	int index = -1;
	for (auto flight : flightList)
	{
		index++;
		if (flight->getOccupiedSeats() != 0)
			continue;

		std::ofstream canceledFlights("canceled-flights.txt", std::ios_base::app);

		std::ostringstream ss;
		ss << flight->getId() << ',' << flight->getDeparture() << ',' << flight->getDestination() << ',' << flight->getDate() << ',' << flight->getTime() << "\n";

		canceledFlights << ss.str();

		canceledFlights.close();

		canceledList.push_back(index);
	}

	clearCanceledFlights(flightList, canceledList);
}

/*
 * @brief This function will clear the flightList vector of cancelled flights
 * a flight is canceled by being empty
 *
 * @param flightList Reference to a vector containing flight* objects
 * @param canceledList Reference to a vector containing int (flight indexes)
 */
void clearCanceledFlights(std::vector<Flight*>& flightList, std::vector<int>& canceledList)
{
	for (auto i : std::ranges::views::reverse(canceledList))
	{
		//      std::cout << "Flight Nr: " << flightList[i]->id << " at time: " << flightList[i]->time<< " is canceled\n";
		flightList.erase(flightList.begin() + i);
	}
}

/*
 * @brief This function will populate a vector of bookings
 *
 * @param file The file name of which we will read data from
 * @param list The vector that should be populated with booking objects
 */
void populateBookingList(std::string file, std::vector<Booking*>* list)
{
	std::ifstream bookingData(file);
	std::string fileData;
	if (bookingData.is_open())
	{
		while (getline(bookingData, fileData) && !fileData.empty())
		{
			Booking* temp = new Booking;
			std::stringstream data(fileData);
			std::string tempString;
			getline(data, tempString, ',');
			temp->setId(atoi(tempString.c_str()));

			getline(data, tempString, ',');
			temp->setDate(tempString);

			getline(data, tempString, ',');
			temp->setTime(tempString);

			getline(data, tempString, ',');
			temp->setDeparture(tempString);

			getline(data, tempString, ',');
			temp->setDestination(tempString);

			getline(data, tempString, ',');
			temp->setSeatType(tempString);

			getline(data, tempString, ',');
			temp->setFirstName(tempString);

			getline(data, tempString, ',');
			temp->setLastName(tempString);

			list->push_back(temp);

			if (!data)
				break;
		}
	}
}

/*
 * @brief This function will create tickets for all the bookings
 *
 * @param flightList Reference to a vector containing flight* objects
 * @param bookingList Reference to a vector containing bookings* objects
 */
void createTickets(std::vector<Flight*>& flightList, std::vector<Booking*>& bookingList)
{
	srand(time(NULL));
	for (auto& flight : flightList)
	{
		int firstClassSeat = 1;
		int firstClassRow = 1;

		int businessClassSeat = 1;
		int businessClassRow = flight->getFClassSeats() / 7 + 1;

		int economyClassSeat = 1;
		int economyClassRow = (flight->getFClassSeats() + flight->getBClassSeats()) / 7 + 1;
		for (auto& booking : bookingList)
		{
			// compare as many parameters as we can to make sure we find the correct flight for the booking
			// we do this since there's no comparable ID for the booking and flight
			if ((flight->getTime() == booking->getTime()) && (flight->getDeparture() == booking->getDeparture()) &&
				(flight->getDestination() == booking->getDestination()) && (flight->getDate() == booking->getDate()))
			{
				if (booking->getSeatType() == "first")
				{
					booking->setSeat(firstClassSeat);
					booking->setRow(firstClassRow);
					firstClassSeat++;

					if (firstClassSeat > 7)
					{
						firstClassSeat = 1;
						firstClassRow++;
					}
				}
				else if (booking->getSeatType() == "business")
				{
					booking->setSeat(businessClassSeat);
					booking->setRow(businessClassRow);
					businessClassSeat++;

					if (businessClassSeat > 7)
					{
						businessClassSeat = 1;
						businessClassRow++;
					}
				}
				else if (booking->getSeatType() == "economy")
				{
					booking->setSeat(economyClassSeat);
					booking->setRow(economyClassRow);
					economyClassSeat++;

					if (economyClassSeat > 7)
					{
						economyClassSeat = 1;
						economyClassRow++;
					}
				}
				flight->addOccupiedSeat();

				std::ostringstream ss;
				ss << "ticket-" << booking->getId() << ".txt";
				std::ofstream ticketFile(ss.str());

				ss.str(""); // empty the string contained in ss (ostringstream)
				ss.clear(); // make sure we clear any possible error flags

				ss << "BOOKING: " << booking->getId() << "\nFLIGHT: " << flight->getId() << " DEPARTURE: " << flight->getDeparture() << " DESTINATION: " << flight->getDestination() << "\nPASSENGER: " << booking->getFirstName() << " " << booking->getLastName() << " CLASS: " << booking->getSeatType() << " ROW: " << booking->getRow() << " SEAT: " << booking->getSeat();
				ticketFile << ss.str();

				ticketFile.close();
			}
		}
	}
}
