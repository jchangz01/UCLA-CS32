#include "CarMap.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
	CarMap Map1;
	//defining 3 potential car license plates
	string license1 = "X129rf"; 
	string license2 = "iojre9";
	string license3 = "xFaraz";

	//add 3 cars to map
	Map1.addCar(license1);
	Map1.addCar(license2);
	Map1.addCar(license3);
	//add 3290 miles driven to car 3
	Map1.drive(license3, 69);
	Map1.drive(license3, 3221);

	//output data for map 1
	cout << "There are currently " << Map1.fleetSize() << " cars in map 1:" << endl;
	Map1.print();
	cout << "--------" << endl;
	
	/////////////////////////////////////

	CarMap Map2;
	//defining 4 potential car license plates
	string license4 = "3atmi@$$";
	string license5 = "mlg*fwww";
	string license6 = "hfaisnvs";
	string license7 = "fwefopew";

	//adds 3 cars to map (attempts to add car 4 twice)
	Map2.addCar(license4);
	if (!Map2.addCar(license4))
		cout << "That car is already on map 2!" << endl;
	Map2.addCar(license5);
	Map2.addCar(license6);
	//add mileage to car 4 and 6 (attempts to add mileage to an invalide car and add negative mileage)
	if (!Map2.drive(license4, -10))
		cout << "A car can't drive a negative amount";
	if (!Map2.drive(license7, 2190))
		cout << license7 << " is not a registered car!" << endl;
	Map2.drive(license4, 84224);
	Map2.drive(license6, 12309);

	//output data for map 2
	cout << "There are currently " << Map2.fleetSize() << " cars in map 2:" << endl;
	Map2.print();
	cout << endl;

	//display some other statistics for map 1 and 2
	cout << "The total miles driven for Map 1 is " << Map1.miles(license3) << " miles." << endl;
	cout << "The total miles driven for Map 2 is " << Map2.miles(license4) + Map2.miles(license6) << " miles." << endl;
} 