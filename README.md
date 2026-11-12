# Wi-Fi signal strength for indoor location (C++)

Can Wi-Fi signal strength tell you where you are inside a building? This project collects
Wi-Fi scans at five indoor locations and analyses which access points are visible at each
one and how strong and stable their signals are.

Scans were logged with a WiFly Wi-Fi module on an ARM mbed LPC1768 board, modified to take
10 scans per location and write them to a file. This repository contains the C++ program
that reads those logs and summarizes them.

## What the program does

- Parses the WiFly scan logs: one file per location, 10 scans each.
- Groups networks by physical access point. One access point broadcasts several networks
  (for example a secure, a guest and an eduroam network) whose MAC addresses differ only in
  the last byte, so the program groups on the first five bytes.
- For each access point at each location, reports how many times it was seen and the mean
  and standard deviation of received signal strength (RSS).

## Results

| Location | Networks seen | Unique access points |
|---|---|---|
| Teaching lab, room A | 44 | 21 |
| Teaching lab, room B (next door) | 42 | 15 |
| Atrium, north side, floor 1 | 58 | 30 |
| Atrium, south side, floor 1 | 57 | 24 |
| Atrium, north side, floor 2 | 62 | 28 |

- **Signal stability:** 106 of the 118 access point and location combinations (90%) had an
  RSS standard deviation of 4 dB or less over the 10 scans.
- **Telling apart locations 15 to 20 m apart:** access point `02:00:00:00:36` was seen 58
  times at -60.4 dB on the south side of the atrium, but only once, at -74.0 dB, on the
  north side.
- **Telling apart floors:** access point `02:00:00:00:39` was seen 29 times at -62.9 dB on
  floor 1, and only twice, at -81.5 dB, directly above on floor 2.
- **Neighbouring rooms:** the two teaching labs share most access points at similar
  strengths. The number of unique access points and how often each one is seen separate them
  better than signal strength alone.
- **What doesn't work:** the board's IP address was the same everywhere, so it carries no
  location information.

Signal strength is a poor way to measure range indoors because of multipath, walls and
noise. It works better as a fingerprint: survey the RSS pattern at known locations first,
then match a new scan against that database. In the original study, a scan from another
group's unknown location was correctly identified as the south side of the atrium by
comparing it against these fingerprints. That data set is not included here.

## Build and run

Requires a C++17 compiler and CMake 3.10+.

```bash
cmake -S . -B build
cmake --build build
./build/wifi_rss data/locations.txt
```

Run it from the repository root so the data paths resolve.

## Data

`data/locations.txt` lists one scan log per location. Each scan in a log looks like:

```
SCAN:Found 11
01,01,-76,08,3104,28,00,02:00:00:00:39:e0,airuc-secure
...
END:
```

The fields used are the signal strength (third field, dBm), the MAC address and the network
name. **The data is anonymized:** access point MAC addresses are replaced with stable
placeholder IDs (`02:00:00:00:NN`, keeping the last byte so networks still group by access
point), and the names of private networks and devices are replaced with `other-network-N`.

## Layout

```
src/        main.cpp (driver), WiflyInterface.cpp (log parser)
include/    WiflyInterface.h (data structures and statistics)
data/       anonymized scan logs for the five locations
```

Written in 2019 for a wireless location course in Geomatics Engineering at the University of Calgary.
