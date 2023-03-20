#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

/* works with ada.csv */

struct DataPoint {
  double timestamp;
  double altitude;
  double pressure;
};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <input-file>\n";
    return 1;
  }

  ifstream infile(argv[1]);
  if (!infile) {
    cerr << "Error: could not open file '" << argv[1] << "'\n";
    return 1;
  }

  vector<DataPoint> data;
  string line;
  getline(infile, line); // skip the header line
  while (getline(infile, line)) {
    stringstream ss(line);
    string timestamp_str, altitude_str, pressure_str;
    getline(ss, timestamp_str, ',');
    getline(ss, altitude_str, ',');
    getline(ss, pressure_str, ',');
    double timestamp = stod(timestamp_str);
    double altitude = stod(altitude_str);
    double pressure = stod(pressure_str);
    data.push_back({timestamp, altitude, pressure});
  }

  // Find apogee and liftoff timestamps
  double max_altitude = 0.0;
  double apogee_timestamp = 0.0;
  for (const auto& datapoint : data) {
    if (datapoint.altitude > max_altitude) {
      max_altitude = datapoint.altitude;
      apogee_timestamp = datapoint.timestamp;
    }
  }

  // Find the liftoff timestamp
    bool liftoff_found = false;
    int liftoff_index = 0;
    for (int i = 0; i < data.size(); ++i) {
        const auto& datapoint = data[i];
        if (datapoint.altitude > 10) {
            liftoff_found = true;
            liftoff_index = i;
            break;
        }
    }
    if (liftoff_found) {
        liftoff_index -= 22;
        if (liftoff_index >= 0) {
            double liftoff_timestamp = data[liftoff_index].timestamp;
            std::cout << "Liftoff timestamp: " << liftoff_timestamp << std::endl;
        } else {
            std::cout << "Error: not enough data points before liftoff" << std::endl;
        }
    } else {
        std::cout << "Error: liftoff not found" << std::endl;
    }

  // Output results to console
  cout << "Apogee timestamp: " << apogee_timestamp << endl;

  return 0;
}