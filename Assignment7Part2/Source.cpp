#include <iostream>
#include <fstream>
#include <list>
#include <iomanip>
#include <sstream>
using namespace std;

struct Patient {
    string id;
    string name;
    string severity;
    int waitTime;
    string status; // "Waiting" or "Admitted"
};

// Function declarations
void loadPatients(list<Patient>& patients);
void savePatients(const list<Patient>& patients);
void registerPatient(list<Patient>& patients);
void displayQueue(const list<Patient>& patients);
void admitPatient(list<Patient>& patients);
void viewSummary(const list<Patient>& patients);

int main() {
    list<Patient> patients;
    loadPatients(patients);
    int choice;
    do {
        cout << "\n****** Welcome to Emergency Room Queue Manager ******\n";
        cout << "1. Register New Patient\n2. Display Patient Queue\n3. Admit Patient\n4. View ER Summary\n5. Exit\n→ ";
        cin >> choice;
        switch (choice) {
        case 1: registerPatient(patients); break;
        case 2: displayQueue(patients); break;
        case 3: admitPatient(patients); break;
        case 4: viewSummary(patients); break;
        case 5:
            savePatients(patients);
            cout << "Patient queue saved to patients.txt. Have a safe shift!" << endl;
            break;
        default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 5);
    return 0;
}

// Load patients from file
void loadPatients(list<Patient>& patients) {
    ifstream infile("patients.txt");
    if (!infile) return;

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        Patient p;
        string waitStr;
        getline(ss, p.id, ',');
        getline(ss, p.name, ',');
        getline(ss, p.severity, ',');
        getline(ss, waitStr, ',');
        getline(ss, p.status);
        p.waitTime = stoi(waitStr);
        patients.push_back(p);
    }
    infile.close();
}

// Save patients to file
void savePatients(const list<Patient>& patients) {
    ofstream outfile("patients.txt");
    for (const auto& p : patients) {
        outfile << p.id << "," << p.name << "," << p.severity << ","
            << p.waitTime << "," << p.status << endl;
    }
    outfile.close();
}

// Register new patient
void registerPatient(list<Patient>& patients) {
    Patient p;
    cout << "Enter patient ID: ";
    cin >> p.id;
    cin.ignore();
    cout << "Enter name: ";
    getline(cin, p.name);
    cout << "Enter condition severity: ";
    getline(cin, p.severity);
    cout << "Enter estimated wait time: ";
    cin >> p.waitTime;
    p.status = "Waiting";
    patients.push_back(p);
    cout << "Patient registered successfully!" << endl;
}

// Display patient queue
void displayQueue(const list<Patient>& patients) {
    cout << "\n=========== ER WAITING LIST ===========" << endl;
    cout << left << setw(6) << "ID" << setw(20) << "Name" << setw(10)
        << "Severity" << setw(10) << "Wait(min)" << "Status" << endl;
    cout << "------------------------------------------------------" << endl;
    for (const auto& p : patients) {
        cout << left << setw(6) << p.id << setw(20) << p.name << setw(10)
            << p.severity << setw(10) << p.waitTime << p.status << endl;
    }
}

// Admit first patient
void admitPatient(list<Patient>& patients) {
    for (auto& p : patients) {
        if (p.status == "Waiting") {
            p.status = "Admitted";
            cout << "Patient " << p.id << " has been admitted to the ER." << endl;
            return;
        }
    }
    cout << "No waiting patients to admit." << endl;
}

// View ER summary
void viewSummary(const list<Patient>& patients) {
    int total = 0, admitted = 0, waiting = 0, waitSum = 0;
    for (const auto& p : patients) {
        total++;
        if (p.status == "Admitted") admitted++;
        else if (p.status == "Waiting") {
            waiting++;
            waitSum += p.waitTime;
        }
    }
    float avgWait = (waiting > 0) ? static_cast<float>(waitSum) / waiting : 0.0;
    cout << "\n========== ER SUMMARY ==========" << endl;
    cout << "Total Patients: " << total << endl;
    cout << "Admitted: " << admitted << endl;
    cout << "Waiting: " << waiting << endl;
    cout << "Avg Wait Time (Waiting): " << fixed << setprecision(2) << avgWait << " minutes" << endl;
    cout << "================================" << endl;
}
