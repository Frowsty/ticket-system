target:
	echo "Compiling Target"
	g++-10 tickets.cpp -o tickets -std=c++20
	./tickets -f flights.csv -b bookings.csv

gen-docs:
	echo "Generating Documentation..."
	doxygen tickets.cpp

run:
	./tickets -f flights.csv -b bookings.csv
