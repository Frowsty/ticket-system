#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ranges>

struct flight
{
	int id;
	std::string departure;
	std::string destination;
	std::string date;
	std::string time;
	int fClassSeats;
	int bClassSeats;
	int eClassSeats;
	int occupiedSeats = 0;
};

struct booking
{
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
};

void populateFlightsList(std::string file, std::vector<flight*>* list)
{
	std::ifstream flightData(file);
	std::string fileData;
	if (flightData.is_open())
	{
		while (getline(flightData, fileData) && !fileData.empty())
		{
			flight* temp = new flight;
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

void populateBookingList(std::string file, std::vector<booking*>* list)
{
	std::ifstream bookingData(file);
	std::string fileData;
	if (bookingData.is_open())
	{
		while (getline(bookingData, fileData) && !fileData.empty())
		{
			booking* temp = new booking;
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

				char nameBuffer[100];
				snprintf(nameBuffer, 100, "ticket-%d.txt", booking->id);
				std::ofstream ticketFile(nameBuffer);

				// c-style formatting is the king of formatting, sadly C++ doesn't really have their own way
				// but luckily we still have access to C within C++ so we make use of some nice C features :)
				char buffer[1000];
				snprintf(buffer, 1000, "BOOKING: %d\nFLIGHT: %d DEPARTURE: %s DESTINATION: %s %s %s\nPASSENGER: %s %s\nCLASS: %s\nROW: %d SEAT: %d",
					booking->id, flight->id, flight->departure, flight->destination, flight->date, flight->time, booking->firstName, booking->lastName,
					booking->seatType, booking->row, booking->seat);

				ticketFile << buffer;

				ticketFile.close();
			}
		}
	}
}

template <typename A>
void clearCanceledFlights(std::vector<A>& flightList, std::vector<int>& canceledList)
{
	for (auto i : std::ranges::views::reverse(canceledList))
	{
		flightList.erase(flightList.begin() + i);
		std::cout << "Removed flight at index " << i << " from the list" << std::endl;
	}
}

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

		char buffer[200];
		snprintf(buffer, 200, "%d,%s,%s,%s,%s\n", flight->id, flight->departure, flight->destination, flight->date, flight->time);

		canceledFlights << buffer;

		std::cout << "Flight canceled: " << buffer << "\n";

		canceledFlights.close();

		canceledList.push_back(index);
	}

	clearCanceledFlights(flightList, canceledList);
}

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

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "No command line arguments found, please specify flights file and bookings file when running the program" << std::endl;
		return 0;
	}
	else
		std::cout << "Data gathering in progress, please wait...\n";

	std::vector<flight*> flights;
	std::vector<booking*> bookings;
	std::vector<int> canceledFlights;

	populateFlightsList(argv[1], &flights);
	populateBookingList(argv[2], &bookings);

	createTickets<flight*, booking*>(flights, bookings);

	checkFlightPopulation<flight*, booking*>(flights, bookings, canceledFlights);

	createSeatMap<flight*, booking*>(flights, bookings);

	return 0;
}