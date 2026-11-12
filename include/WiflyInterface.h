#ifndef WIFLYINTERFACE_H
#define WIFLYINTERFACE_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <vector>

using namespace std;

#define FILENOTGOOD(a)	cout << "	File " << a << " was not good. Try again...\n\n"; infile.clear(); cin.ignore(256, '\n')

#define HEADING(a) cout << "		" << a << endl << "		==========================================\n\n"

#define SHOW(a) cout << #a << ": " << (a) << endl


class WiflyInterface {

public:

	WiflyInterface();

	struct Point
	{
		int obs_num;
		int s_2;
		int signal_strength;
		int s_3;
		int s_4;
		int s_5;
		int signal_to_noise;
		string mac_address;
		string wifi_name;
		string cut_mac_address;

		void FixMacAddress() {

			cut_mac_address = mac_address.substr(0, mac_address.size() - 3);
		}

	};

	struct Scan
	{
		int num_points;
		list<Point> point_list;
		list<Point>::iterator pit;
	};

	struct Location {

		string name;

		list<Scan> scan_list;				// Data container
		list<Scan>::iterator sit;			// Container iterator

		struct rssi_data {

			double n;
			double sum_ss;
			vector<double> v;

			rssi_data() {
				n = 0;
				sum_ss = 0;
			}

			double std;
			double mean_ss;


		};

		map<string, string> audible_points;
		map<string, string>::iterator ait;

		map<string, rssi_data> mymap;
		map<string, rssi_data>::iterator mit;


		void GetAudiblePoints() {

			for (sit = scan_list.begin(); sit != scan_list.end(); sit++) {

				for (sit->pit = sit->point_list.begin(); sit->pit != sit->point_list.end(); sit->pit++) {
				
					audible_points[sit->pit->mac_address] = sit->pit->wifi_name;

				}
			}
		}

		void GetUniqueAudiblePoints() {

			for (sit = scan_list.begin(); sit != scan_list.end(); sit++) {

				for (sit->pit = sit->point_list.begin(); sit->pit != sit->point_list.end(); sit->pit++) {
					
					string hold_str;
					rssi_data hold_rssi;

					hold_str = sit->pit->cut_mac_address;
					hold_rssi.n = 1;
					hold_rssi.sum_ss = sit->pit->signal_strength;
					hold_rssi.v.push_back(sit->pit->signal_strength);
					if (mymap.find(hold_str) != mymap.end()) {

						mymap[hold_str].n += hold_rssi.n;
						mymap[hold_str].sum_ss += hold_rssi.sum_ss;
						mymap[hold_str].v.push_back(sit->pit->signal_strength);

					}
					else
						mymap[hold_str] = hold_rssi;
				}
			}
		}
		

		void CalcMeanAndStd() {

			for (mit = mymap.begin(); mit != mymap.end(); mit++) {

				mit->second.mean_ss = mit->second.sum_ss / mit->second.n;

				double sum_mis = 0;

				for (int i = 0; i < mit->second.v.size(); i++) {
					sum_mis += pow(mit->second.v[i] - mit->second.mean_ss, 2);

				}

				if (mit->second.n > 1)
					mit->second.std = sqrt(sum_mis / (mit->second.n));
				else
					mit->second.std = 0;
			}
		}
	};

	void ReadData(string filename);

	void DispData();

private:

	void ReadLocation(Location& loc);
	void ReadScan(ifstream& infile, Scan& scan);

	list<Location> loc_list;
	list<Location>::iterator lit;


};



#endif // !WIFLYINTERFACE_H

