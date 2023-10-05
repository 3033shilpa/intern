#include <iostream>
#include<bits/stdc++.h>
using namespace std;

struct EmployeeRecord {
   string positionID;
   string positionStatus;
   string time;
   string timeOut;
    double timecardHours;
   string payCycleStartDate;
   string payCycleEndDate;
   string employeeName;
   string fileNumber;
};

// Function to parse a line and create an EmployeeRecord
EmployeeRecord parseLine(const string& line) {
    EmployeeRecord record;
   stringstream ss(line);

   getline(ss, record.positionID, ',');
   getline(ss, record.positionStatus, ',');
   getline(ss, record.time, ',');
   getline(ss, record.timeOut, ',');
    ss >> record.timecardHours;
   getline(ss, record.payCycleStartDate, ',');
   getline(ss, record.payCycleEndDate, ',');
   getline(ss, record.employeeName, ',');
   getline(ss, record.fileNumber, ',');

    return record;
}

// Function to check if two dates are consecutive
bool areDatesConsecutive(const string& date1, const string& date2) {
   std::stringstream ss1(date1);
    std::stringstream ss2(date2);

    // Define variables to store the year, month, and day
    int year1, month1, day1;
    int year2, month2, day2;

    // Parse the date strings
    ss1 >> year1; ss1.ignore(); ss1 >> month1; ss1.ignore(); ss1 >> day1;
    ss2 >> year2; ss2.ignore(); ss2 >> month2; ss2.ignore(); ss2 >> day2;

    // Compare the dates
    if (year1 == year2) {
        if (month1 == month2) {
            return std::abs(day1 - day2) == 1;
        } else if (std::abs(month1 - month2) == 1) {
            // Check for end of month
            return (day1 == 31 && day2 == 1) || (day1 == 30 && day2 == 1) || (day1 == 29 && day2 == 1 && month1 == 2);
        }
    } else if (std::abs(year1 - year2) == 1) {
        // Check for end of year
        return month1 == 12 && day1 == 31 && month2 == 1 && day2 == 1;
    }

    return false;
}


int convertToMinutes(const std::string& time) {
    int hours, minutes;
    char delimiter;
    std::istringstream stream(time);
    stream >> hours >> delimiter >> minutes;
    return hours * 60 + minutes;
}
// Function to calculate the time difference between two times
int calculateTimeDifference(const string& time1, const string& time2) {
   int minutes1 = convertToMinutes(time1);
    int minutes2 = convertToMinutes(time2);

    int difference = minutes2 - minutes1;

    if (difference < 0) {
        difference += 24 * 60; // Add a day's worth of minutes
    }

    return difference / 60;
}

int main() {
   ifstream inputFile("employee_data.txt"); // Replace with your input file name

    if (!inputFile) {
       cerr << "Error opening file." <<endl;
        return 1;
    }

   vector<EmployeeRecord> records; 

   string line;
    while (std::getline(inputFile, line)) {
        records.push_back(parseLine(line));
    }

    // a) Employees who have worked for 7 consecutive days
    for (size_t i = 0; i < records.size() - 6; ++i) {
        if (areDatesConsecutive(records[i].payCycleEndDate, records[i + 6].payCycleEndDate)) {
           cout << "Employee: " << records[i].employeeName << " Position: " << records[i].positionID <<endl;
        }
    }

    // b) Employees with less than 10 hours between shifts but greater than 1 hour
    for (size_t i = 0; i < records.size() - 1; ++i) {
        int timeDifference = calculateTimeDifference(records[i].timeOut, records[i + 1].time);
        if (timeDifference > 1 && timeDifference < 10) {
           cout << "Employee: " << records[i].employeeName << " Position: " << records[i].positionID <<endl;
        }
    }

    // c) Employees who have worked for more than 14 hours in a single shift
    for (const auto& record : records) {
        if (record.timecardHours > 14) {
           cout << "Employee: " << record.employeeName << " Position: " << record.positionID <<endl;
        }
    }

    return 0;
}
