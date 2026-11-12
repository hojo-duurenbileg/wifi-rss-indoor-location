#include "WiflyInterface.h"


WiflyInterface::WiflyInterface() {

	cout << "\n\nStarting WiflyInterface Program!!!\n\n";

	time_t rawtime;

	time(&rawtime);

	printf("Program started at local time: %s\n\n", ctime(&rawtime));
}

void WiflyInterface::ReadData(string filename) {

	ifstream infile;

	infile.open(filename, fstream::in);
	if (!infile.good()) {
		FILENOTGOOD(filename);
	}

	while (!infile.eof()) {

		Location hold_location;

		if (infile >> hold_location.name) {

			ReadLocation(hold_location);

			loc_list.push_back(hold_location);

		}
	}

	for (lit = loc_list.begin(); lit != loc_list.end(); lit++) {

		lit->GetAudiblePoints();
		lit->GetUniqueAudiblePoints();
		lit->CalcMeanAndStd();
	}


	infile.close();

}

void WiflyInterface::ReadLocation(Location& loc) {

	ifstream infile;

	infile.open(loc.name, fstream::in);
	if (!infile.good()) {
		FILENOTGOOD(loc.name);
	}

	string hold_line;


	while (!infile.eof()) {

		if (getline(infile, hold_line, '\n')) {

			if (hold_line.substr(0, 4) == "SCAN") {

				Scan hold_scan;

				istringstream iss(hold_line);
				iss.ignore(256, ' ');

				iss >> hold_scan.num_points;

				ReadScan(infile, hold_scan);

				loc.scan_list.push_back(hold_scan);
			}
		}
	}

	infile.close();

}

void WiflyInterface::ReadScan(ifstream& infile, Scan& scan) {

	string hold_line;

	while (1) {

		if (getline(infile, hold_line, '\n')) {

			if (hold_line.substr(0, 3) == "END")
				break;

			Point hold_point;
			char c;

			istringstream iss(hold_line);

			iss >> hold_point.obs_num;
			iss.ignore(256, ',');
			iss >> hold_point.s_2;
			iss.ignore(256, ',');
			iss >> hold_point.signal_strength;
			iss.ignore(256, ',');
			iss >> c >> hold_point.s_3;
			iss.ignore(256, ',');
			iss >> hold_point.s_4;
			iss.ignore(256, ',');
			iss >> hold_point.s_5;
			iss.ignore(256, ',');
			iss >> hold_point.signal_to_noise;
			iss.ignore(256, ',');
			getline(iss, hold_point.mac_address, ',');
			getline(iss, hold_point.wifi_name, '\n');

			hold_point.FixMacAddress();

			scan.point_list.push_back(hold_point);

		}
	}
}

void WiflyInterface::DispData() {

	HEADING("INPUTS");

	HEADING("NUMBER OF AUDIBLE/VISIBLE POINTS AT EACH LOCATION");


	for (lit = loc_list.begin(); lit != loc_list.end(); lit++) {
		cout << endl;
		HEADING(lit->name);

		cout << "		Number of audible points was " << lit->audible_points.size() << endl << endl;

	}
	

	HEADING("NUMBER OF UNIQUE VISIBLE POINTS AT EACH LOCATION");
	
	
	for (lit = loc_list.begin(); lit != loc_list.end(); lit++) {
		cout << endl;
		HEADING(lit->name);
	
		cout << "		Number of unique points:	" << lit->mymap.size() << endl << endl;
		cout << "		Mac-Address		NumObs	MeanSS	STD\n\n";
		for (lit->mit = lit->mymap.begin(); lit->mit != lit->mymap.end(); lit->mit++) {

			cout << fixed << "		" << lit->mit->first << "		" << setprecision(0) << lit->mit->second.n << "	" << setprecision(1) << lit->mit->second.mean_ss << "	+/-" << lit->mit->second.std << endl;
		}	

	}

}

//void WiflyInterface::DispData() {
//
//	HEADING("INPUTS");
//
//	HEADING("NUMBER OF VISIBLE POINTS AT EACH LOCATION");
//
//	//HEADING("NUMBER OF UNIQUE VISIBLE POINTS AT EACH LOCATION");
//
//
//	for (lit = loc_list.begin(); lit != loc_list.end(); lit++) {
//		cout << endl;
//		HEADING(lit->name);
//
//		//cout << "		Number of unique points was " << lit->mymap.size() << endl << endl << "		Mac-Address\n\n";
//		//for (lit->mit = lit->mymap.begin(); lit->mit != lit->mymap.end(); lit->mit++) {
//
//		//	cout << "		" << lit->mit->first << " average signal strength for " << lit->mit->second.n << " values: " << lit->mit->second.mean_ss << ", standard deviation: " << lit->mit->second.std << endl;
//		//}
//
//		int i = 0;
//		for (lit->sit = lit->scan_list.begin(); lit->sit != lit->scan_list.end(); lit->sit++) {
//
//
//
//			//i++;
//			//cout<< "		Observation number " << i << " had " << lit->sit->num_points << " visible points\n";
//
//			for (lit->sit->pit = lit->sit->point_list.begin(); lit->sit->pit != lit->sit->point_list.end(); lit->sit->pit++) {
//
//				//cout << lit->sit->pit->obs_num << ',' << lit->sit->pit->s_2 << ',' << lit->sit->pit->signal_strength << ',' << lit->sit->pit->s_3;
//				//cout << ',' << lit->sit->pit->s_4 << ',' << lit->sit->pit->s_5 << ',' << lit->sit->pit->signal_to_noise << ',' << lit->sit->pit->mac_address;
//				//cout << ',' << lit->sit->pit->wifi_name << endl;
//
//			}
//		}
//	}
//}