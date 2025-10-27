#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

template <typename T1, typename T2>
struct Appointment {
    T1 id;
    string name;
    string type;
    int timeUntil;
    T2 bill;
    string status;
};

template <typename T1, typename T2>
int loadAppointments(Appointment<T1, T2> appointments[]) {
    ifstream file("appointments.txt");
    if (!file) return 0;
    int count = 0;
    while (file) {
        Appointment<T1, T2> a;
        char comma;
        file >> a.id >> comma;
        file.ignore(); getline(file, a.name, ',');
        getline(file, a.type, ',');
        file >> a.timeUntil >> comma >> a.bill >> comma;
        getline(file, a.status);
        if (file) appointments[count++] = a;
    }
    file.close();
    return count;
}

template <typename T1, typename T2>
void saveAppointments(Appointment<T1, T2> appointments[], int count) {
    ofstream file("appointments.txt");
    for (int i = 0; i < count; ++i) {
        file << appointments[i].id << "," << appointments[i].name << "," << appointments[i].type << ","
            << appointments[i].timeUntil << "," << fixed << setprecision(2) << appointments[i].bill << ","
            << appointments[i].status << endl;
    }
    file.close();
}

template <typename T1, typename T2>
void addAppointment(Appointment<T1, T2> appointments[], int& count) {
    if (count >= 100) {
        cout << "Appointment list is full.\n";
        return;
    }
    Appointment<T1, T2> a;
    cout << "Enter appointment ID: ";
    cin >> a.id;
    cout << "Enter patient name: ";
    cin.ignore();
    getline(cin, a.name);
    cout << "Enter appointment type: ";
    getline(cin, a.type);
    cout << "Enter time until appointment (hours): ";
    cin >> a.timeUntil;
    cout << "Enter estimated bill: ";
    cin >> a.bill;
    a.status = "Pending";
    appointments[count++] = a;
    cout << "Appointment added!\n";
}

template <typename T1, typename T2>
void displayAppointments(Appointment<T1, T2> appointments[], int count) {
    cout << "\n=========== APPOINTMENTS ===========\n";
    cout << "ID Name Type Time Bill Status\n";
    cout << "-------------------------------------------------\n";
    for (int i = 0; i < count; ++i) {
        cout << appointments[i].id << " " << appointments[i].name << " " << appointments[i].type << " "
            << appointments[i].timeUntil << " $" << fixed << setprecision(2) << appointments[i].bill << " "
            << appointments[i].status << endl;
    }
}

template <typename T1, typename T2>
void markCompleted(Appointment<T1, T2> appointments[], int count) {
    T1 id;
    cout << "Enter appointment ID to mark as completed: ";
    cin >> id;
    for (int i = 0; i < count; ++i) {
        if (appointments[i].id == id && appointments[i].status == "Pending") {
            appointments[i].status = "Completed";
            cout << "Appointment " << id << " marked as Completed.\n";
            return;
        }
    }
    cout << "Appointment not found or already completed.\n";
}

template <typename T1, typename T2>
void appointmentSummary(Appointment<T1, T2> appointments[], int count) {
    int completed = 0, pending = 0;
    T2 totalEarnings = 0;
    for (int i = 0; i < count; ++i) {
        if (appointments[i].status == "Completed") {
            completed++;
            totalEarnings += appointments[i].bill;
        }
        else {
            pending++;
        }
    }
    cout << "\n======== APPOINTMENT SUMMARY ========\n";
    cout << "Total Appointments: " << count << "\n";
    cout << "Completed: " << completed << "\n";
    cout << "Pending: " << pending << "\n";
    cout << "Total Earnings: $" << fixed << setprecision(2) << totalEarnings << "\n";
    cout << "=====================================\n";
}

int main() {
    Appointment<string, float> appointments[100];
    int count = loadAppointments(appointments);
    int choice;
    do {
        cout << "\n******* Welcome to Clinic Appointment Manager *******\n";
        cout << "1. Add New Appointment\n2. Display All Appointments\n3. Mark Appointment as Completed\n4. View Appointment Summary\n5. Exit\n→ ";
        cin >> choice;
        switch (choice) {
        case 1: addAppointment(appointments, count); break;
        case 2: displayAppointments(appointments, count); break;
        case 3: markCompleted(appointments, count); break;
        case 4: appointmentSummary(appointments, count); break;
        case 5: saveAppointments(appointments, count);
            cout << "Data saved to appointments.txt. Goodbye!" << endl;
            break;
        default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 5);
    return 0;
}
