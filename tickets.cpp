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


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ranges>
#ifdef _WIN32
#include <GetOpt.h>
#else
#include <unistd.h>
#endif

/*
 * @brief Base is a class that contains information that is used in both Flight and Booking thus is used as a base  * class to inherit from
 */
class Base
{
public:
    int id;
    std::string departure;
    std::string destination;
    std::string date;
    std::string time;
};

/*
 * @brief flight is a class containing information about a specific flight
 */
class Flight : public Base
{
public:
	int fClassSeats;
	int bClassSeats;
	int eClassSeats;
	int occupiedSeats = 0;
};

/*
 * @brief booking is a class containing information about a specific booking
 */
class Booking : public Base
{
public:
	std::string seatType;
	std::string firstName;
	std::string lastName;
	int row;
	int seat;
};

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
			temp->id = atoi(tempString.c_str());

			getline(data, temp->departure, ',');
			getline(data, temp->destination, ',');
			getline(data, temp->date, ',');
			getline(data, temp->time, ',');

			getline(data, tempString, ',');
			temp->fClassSeats = atoi(tempString.c_str());

			getline(data, tempString, ',');
			temp->bClassSeats = atoi(tempString.c_str());

			getline(data, tempString, ',');
			temp->eClassSeats = atoi(tempString.c_str());

			list->push_back(temp);

			if (!data)
				break;
		}
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
			temp->id = atoi(tempString.c_str());

			getline(data, temp->date, ',');
			getline(data, temp->time, ',');

			getline(data, temp->departure, ',');
			getline(data, temp->destination, ',');

			getline(data, temp->seatType, ',');
			getline(data, temp->firstName, ',');
			getline(data, temp->lastName, ',');
			list->push_back(temp);

			if (!data)
				break;
		}
	}
}

/*
 * @brief This function will create tickets for all the bookings
 *
 * @param flightList Reference to a vector containing Flight* objects
 * @param bookingList Reference to a vector containing Bookings* objects
 */
template <typename A, typename B>
void createTickets(std::vector<A>& flightList, std::vector<B>& bookingList)
{
	srand(time(NULL));
	for (auto& flight : flightList)
	{
		int firstClassSeat = 1;
		int firstClassRow = 1;

		int businessClassSeat = 1;
		int businessClassRow = flight->fClassSeats / 7 + 1;

		int economyClassSeat = 1;
		int economyClassRow = (flight->fClassSeats + flight->bClassSeats) / 7 + 1;
		for (auto& booking : bookingList)
		{
			// compare as many parameters as we can to make sure we find the correct flight for the booking
			// we do this since there's no comparable ID for the booking and flight
			if ((flight->time == booking->time) && (flight->departure == booking->departure) &&
				(flight->destination == booking->destination) && (flight->date == booking->date))
			{
				if (booking->seatType == "first")
				{
					booking->seat = firstClassSeat;
					booking->row = firstClassRow;
					firstClassSeat++;

					if (firstClassSeat > 7)
					{
						firstClassSeat = 1;
						firstClassRow++;
					}
				}
				else if (booking->seatType == "business")
				{
					booking->seat = businessClassSeat;
					booking->row = businessClassRow;
					businessClassSeat++;

					if (businessClassSeat > 7)
					{
						businessClassSeat = 1;
						businessClassRow++;
					}
				}
				else if (booking->seatType == "economy")
				{
					booking->seat = economyClassSeat;
					booking->row = economyClassRow;
					economyClassSeat++;

					if (economyClassSeat > 7)
					{
						economyClassSeat = 1;
						economyClassRow++;
					}
				}
				flight->occupiedSeats++;

                std::ostringstream ss;
                ss << "ticket-" << booking->id << ".txt";
				std::ofstream ticketFile(ss.str());

                ss.str(""); // empty the string contained in ss (ostringstream)
                ss.clear(); // make sure we clear any possible error flags

                ss << "BOOKING: " << booking->id << "\nFLIGHT: " << flight->id << " DEPARTURE: " << flight->departure << " DESTINATION: " << flight->destination << "\nPASSENGER: " << booking->firstName << " " << booking->lastName << " CLASS: " << booking->seatType << " ROW: " << booking->row << " SEAT: " << booking->seat;
				ticketFile << ss.str();

				ticketFile.close();
			}
		}
	}
}

/*
 * @brief This function will clear the flightList vector of cancelled flights
 * a flight is canceled by being empty
 *
 * @param flightList Reference to a vector containing flight* objects
 * @param canceledList Reference to a vector containing int (flight indexes)
 */
template <typename A>
void clearCanceledFlights(std::vector<A>& flightList, std::vector<int>& canceledList)
{
	for (auto i : std::ranges::views::reverse(canceledList))
    {
//      std::cout << "Flight Nr: " << flightList[i]->id << " at time: " << flightList[i]->time<< " is canceled\n";
        flightList.erase(flightList.begin() + i);
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
template <typename A, typename B>
void checkFlightPopulation(std::vector<A>& flightList, std::vector<B>& bookingList, std::vector<int>& canceledList)
{
	// remove the old canceled flights data before we create the new file with new data
	remove("canceled-flights.txt");

	int index = -1;
	for (auto flight : flightList)
	{
		index++;
		if (flight->occupiedSeats != 0)
			continue;

		std::ofstream canceledFlights("canceled-flights.txt", std::ios_base::app);

        std::ostringstream ss;
        ss << flight->id << ',' << flight->departure << ',' << flight->destination << ',' << flight->date << ',' << flight->time << "\n";

		canceledFlights << ss.str();

		canceledFlights.close();

		canceledList.push_back(index);
	}

	clearCanceledFlights(flightList, canceledList);
}

/*
 * @brief This function will create a seat map of the flights
 *
 * @param flightList A vector containing all the flights
 * @param bookingList A vector containing all the bookings
 */
template <typename A, typename B>
void createSeatMap(std::vector<A>& flightList, std::vector<B>& bookingList)
{
	// remove the old seatmap file
	remove("flights-seatmap.txt");
	for (auto& flight : flightList)
	{
		int seats = flight->fClassSeats + flight->bClassSeats + flight->eClassSeats;
		int rows = seats / 7; // In the planes at my airport we have 7 seats per row
		std::stringstream seatMap;

		int fClassRow = flight->fClassSeats / 7;
		int bClassRow = fClassRow + 1;
		int eClassRow = (flight->fClassSeats + flight->bClassSeats) / 7 + 1;
		bool seatMapped = false;
		seatMap << "\nFlight: " << flight->id << " Departure: " << flight->departure << " Destination: " << flight->destination << " Date: " << flight->date << " Time: " << flight->time << "\n";

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
					if ((flight->time == booking->time) && (flight->departure == booking->departure) &&
						(flight->destination == booking->destination) && (flight->date == booking->date))
					{
						if (booking->row == i && booking->seat == j)
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
 * Main entry point
 * 
 * @param[in] argc Number of command line arguments
 * @param[in] argv Array of command line arguments
 *
 * @param[out] returns 0 on successful execution
 */
int main(int argc, char** argv)
{
	char* flightFile = NULL;
	char* bookingFile = NULL;
	int c;

	while ((c = getopt(argc, argv, "f:b:")) != -1)
	{
		switch (c)
		{
		case 'f':
			flightFile = optarg;
			break;
		case 'b':
			bookingFile = optarg;
			break;
		case '?':
			std::cout << "Usage: " << argv[0] << " -f <flights_file.csv> -b <bookings_file.csv>" << std::endl;
			exit(-1);
			break;
		default:
			std::cout << "Usage: " << argv[0] << " -f <flights_file.csv> -b <bookings_file.csv>" << std::endl;
			exit(-1);
			break;
		}
	}

	if (!flightFile || !bookingFile)
	{
		std::cout << "Failed to load files from command arguments, exiting program\n";
		std::cout << "Usage: " << argv[0] << " -f <flights_file.csv> -b <bookings_file.csv>" << std::endl;
		exit(-1);
	}

	std::vector<Flight*> flights;
	std::vector<Booking*> bookings;
	std::vector<int> canceledFlights;

	populateFlightsList(flightFile, &flights);
	populateBookingList(bookingFile, &bookings);

	createTickets<Flight*, Booking*>(flights, bookings);

	checkFlightPopulation<Flight*, Booking*>(flights, bookings, canceledFlights);

	createSeatMap<Flight*, Booking*>(flights, bookings);

	std::cout << "Operations completed!" << std::endl;

	return 0;
}
