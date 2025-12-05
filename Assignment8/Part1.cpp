#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

struct Patient {
    string id;
    string name;
    string condition;
    int severity;
    Patient* next;
};

Patient* head = nullptr;
int totalServed = 0;

// Function declarations
void loadPatients();
void savePatients();
void addPatient();
void displayQueue();
void servePatient();
void showSummary();

// ================== MAIN ==================
int main() {
    loadPatients();
    int choice;
    do {
        cout << "\n****** Welcome to ER Patient Queue Manager ******\n";
        cout << "1. Add New Patient\n2. Display Patient Queue\n3. Serve Next Patient\n4. View ER Summary\n5. Exit\n? ";
        cin >> choice;
        cin.ignore(); // clear newline from input buffer

        switch (choice) {
        case 1: addPatient(); break;
        case 2: displayQueue(); break;
        case 3: servePatient(); break;
        case 4: showSummary(); break;
        case 5: savePatients();
            cout << "Patient queue saved to patients.txt. Stay safe!" << endl;
            break;
        default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}

// ================== FUNCTIONS ==================

// Load patients from file into linked list
void loadPatients() {
    ifstream fin("patients.txt");
    if (!fin) return; // no file yet

    string id, name, condition;
    int severity;
    while (getline(fin, id, ',') &&
        getline(fin, name, ',') &&
        getline(fin, condition, ',') &&
        fin >> severity) {
        fin.ignore(); // consume newline
        Patient* newPatient = new Patient{ id, name, condition, severity, nullptr };
        if (!head) head = newPatient;
        else {
            Patient* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newPatient;
        }
    }
    fin.close();
}

// Save patients still in queue to file
void savePatients() {
    ofstream fout("patients.txt");
    Patient* temp = head;
    while (temp) {
        fout << temp->id << "," << temp->name << "," << temp->condition << "," << temp->severity << "\n";
        temp = temp->next;
    }
    fout.close();
}

// Add new patient to end of queue
void addPatient() {
    Patient* newPatient = new Patient;
    cout << "Enter Patient ID: ";
    getline(cin, newPatient->id);
    cout << "Enter patient name: ";
    getline(cin, newPatient->name);
    cout << "Enter condition: ";
    getline(cin, newPatient->condition);
    cout << "Enter severity (1-5): ";
    cin >> newPatient->severity;
    cin.ignore();

    newPatient->next = nullptr;

    if (!head) head = newPatient;
    else {
        Patient* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newPatient;
    }

    cout << "Patient added to the queue.\n";
}

// Display all patients in queue
void displayQueue() {
    if (!head) {
        cout << "No patients in queue.\n";
        return;
    }
    cout << "=========== CURRENT QUEUE ===========\n";
    cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(20) << "Condition" << setw(10) << "Severity" << endl;
    cout << "-----------------------------------------\n";

    Patient* temp = head;
    while (temp) {
        cout << left << setw(10) << temp->id
            << setw(20) << temp->name
            << setw(20) << temp->condition
            << setw(10) << temp->severity << endl;
        temp = temp->next;
    }
}

// Serve next patient (remove from head)
void servePatient() {
    if (!head) {
        cout << "No patients in queue.\n";
        return;
    }
    Patient* temp = head;
    cout << "Serving Patient: " << temp->id << " - " << temp->name << endl;
    head = head->next;
    delete temp;
    totalServed++;
}

// Show ER summary
void showSummary() {
    int count = 0;
    int severitySum = 0;
    Patient* temp = head;
    while (temp) {
        count++;
        severitySum += temp->severity;
        temp = temp->next;
    }
    double avgSeverity = (count > 0) ? (double)severitySum / count : 0.0;

    cout << "========== ER SUMMARY ==========\n";
    cout << "Total Patients Served: " << totalServed << endl;
    cout << "Patients in Queue: " << count << endl;
    cout << "Average Severity of Waiting Patients: " << fixed << setprecision(2) << avgSeverity << endl;
    cout << "================================\n";
}
