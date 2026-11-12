// Wi-Fi RSS fingerprinting: example driver.
//
// Reads a list of location files (one Wi-Fi scan log per location), groups the
// observations by access point, and reports the number of visible access points
// and the mean and standard deviation of signal strength for each one.
//
// Author: Khongorzul Duurenbileg, October 2019

#include <iostream>
#include "WiflyInterface.h"

using namespace std;

int main(int argc, char* argv[])
{
	const string location_list = argc > 1 ? argv[1] : "data/locations.txt";

	WiflyInterface Lab2;

	Lab2.ReadData(location_list);
	Lab2.DispData();
	return 0;
}
