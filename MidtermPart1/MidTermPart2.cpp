#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

const int MAX = 100;

struct Taxi {
    int id;
    string driver;
    string model;
    string plate;
};

class Queue {
private:
    Taxi taxis[MAX];
    int front, rear, count;

public:
    Queue() {
        front = 0;
        rear = -1;
        count = 0;
    }
    bool isFull() { return count == MAX; }
    bool isEmpty() { return count == 0; }

    void enqueue(Taxi t) {
        if (isFull()) {
            cout << "Queue is full. No more taxis can be added at this time.\n";
            return;
        }
        rear = (rear + 1) % MAX;
        taxis[rear] = t;
        count++;
        cout << "Taxi added to queue!\n";
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "No taxis available for dispatch.\n";
            return;
        }
        Taxi t = taxis[front];
        front = (front + 1) % MAX;
        count--;
        cout << "Taxi dispatched:\n";
        cout << "ID: " << t.id << ", Driver: " << t.driver
            << ", Car: " << t.model << ", Plate: " << t.plate << endl;
    }

    void viewQueue() {
        if (isEmpty()) {
            cout << "No taxis in the queue.\n";
            return;
        }
        cout << "========= WAITING TAXIS (Front to Rear) =========\n";
        cout << left << setw(6) << "ID" << setw(20) << "Driver"
            << setw(15) << "Model" << "Plate\n";
        cout << "---------------------------------------------\n";
        for (int i = 0; i < count; i++) {
            int index = (front + i) % MAX;
            cout << left << setw(6) << taxis[index].id
                << setw(20) << taxis[index].driver
                << setw(15) << taxis[index].model
                << taxis[index].plate << endl;
        }
    }

    void queueSummary() {
        if (isEmpty()) {
            cout << "========== QUEUE SUMMARY ==========\n";
            cout << "Total Taxis Waiting: 0\n";
            cout << "===================================\n";
            return;
        }
        cout << "========== QUEUE SUMMARY ==========\n";
        cout << "Total Taxis Waiting: " << count << endl;
        cout << "Front Taxi: ID " << taxis[front].id << " – " << taxis[front].driver << endl;
        cout << "Rear Taxi: ID " << taxis[rear].id << " – " << taxis[rear].driver << endl;
        cout << "===================================\n";
    }

    void saveToFile() {
        ofstream out("taxis.txt");
        for (int i = 0; i < count; i++) {
            int index = (front + i) % MAX;
            out << taxis[index].id << "," << taxis[index].driver << ","
                << taxis[index].model << "," << taxis[index].plate << endl;
        }
        out.close();
        cout << "Taxi queue saved to taxis.txt. Have a safe dispatch!\n";
    }
};

int main() {
    Queue airportQueue;
    int choice;

    cout << "****** Welcome to Airport Taxi Queue System ******\n";

    do {
        cout << "\nPlease choose one of the following operations:\n";
        cout << "1. Add Taxi to Queue\n";
        cout << "2. View Waiting Taxis\n";
        cout << "3. Dispatch Next Taxi\n";
        cout << "4. View Queue Summary\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            Taxi t;
            cout << "Enter Taxi ID: ";
            cin >> t.id;
            cin.ignore();
            cout << "Enter Driver Name: ";
            getline(cin, t.driver);
            cout << "Enter Car Model: ";
            getline(cin, t.model);
            cout << "Enter License Plate: ";
            getline(cin, t.plate);
            airportQueue.enqueue(t);
        }
        else if (choice == 2) {
            airportQueue.viewQueue();
        }
        else if (choice == 3) {
            airportQueue.dequeue();
        }
        else if (choice == 4) {
            airportQueue.queueSummary();
        }
        else if (choice == 5) {
            airportQueue.saveToFile();
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 5);

    return 0;
}