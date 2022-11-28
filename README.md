# ticket-system

## About this software
This ticket system will parse CSV files and create flight tickets in the form of text files for each person that has booked a plane ride. Once the program has created all the tickets it will then automatically cancel all the flights that didn't have any booked seats and produce a canceled-flights.txt file containing all the flights of which got canceled. We then want to make life easier for the cabin crew so we create a seatmap which simulates the planes seating and indicates if a seat is booked or empty.

## Usage info

#### Compile using Make
Navigate to the project folder once you have cloned the project and run the following command to compile the project
```cpp
make
```
#### Compile manually
To compile this manually run the following commands
```cpp
// if you only have g++10 and later installed you can run this command
g++ tickets.cpp -o tickets -std=c++20
// if you're unsure of which version of g++ you have installed run this command
g++-10 tickets.cpp -o tickets -std=c++20
```

## Information
This is the first version of my ticket-system, the second version uses a multifile layout but is lacking some C++ features that are present in this first version. The main differences are the following

Feature     | First Version | Second Version
-------     | ------------- | -------------
Inheritance |     YES       |      NO
Flags/GetOpt|     YES       |      NO
MultiFile layout|     NO        |      YES
Seatmap     |     YES       |      YES
Canceled Flights |     YES       |      YES
Create Ticket |     YES       |      YES
C++?        |     YES       |      YES
Produce Docs|     YES       |      YES
Templates?  |     YES       |      NO
Try-Catch   |     NO        |      NO
Student Unique?|  YES       |      YES
Classes?    |     YES       |      YES
Setter/Getter functions? | NO | YES
