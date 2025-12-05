#include <iostream>
#include <string>
using namespace std;

struct TrafficLight {
    int id;
    string roadName;
    string color;
    TrafficLight* next;
};

TrafficLight* head = nullptr;
TrafficLight* current = nullptr;

// Function declarations
void addLight();
void displaySequence();
void advanceLight();
void resetSystem();

// ================== MAIN ==================
int main() {
    int choice;
    do {
        cout << "\n****** Welcome to Smart Traffic Light Controller ******\n";
        cout << "1. Add Road and Light\n2. Display Light Sequence\n3. Advance to Next Light\n4. Reset System\n5. Exit\n→ ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: addLight(); break;
        case 2: displaySequence(); break;
        case 3: advanceLight(); break;
        case 4: resetSystem(); break;
        case 5: cout << "Traffic system saved. Goodbye and drive safe!" << endl; break;
        default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 5);
    return 0;
}

// ================== FUNCTIONS ==================

// 1. Add Road and Light
void addLight() {
    TrafficLight* newLight = new TrafficLight;
    cout << "Enter Road ID: "; cin >> newLight->id; cin.ignore();
    cout << "Enter Road Name: "; getline(cin, newLight->roadName);
    cout << "Enter Initial Light Color (Green/Yellow/Red): "; getline(cin, newLight->color);

    if (!head) {
        head = current = newLight;
        newLight->next = newLight; // circular link to itself
    }
    else {
        TrafficLight* temp = head;
        while (temp->next != head) temp = temp->next;
        temp->next = newLight;
        newLight->next = head;
    }
    cout << "Traffic light added for " << newLight->roadName << ".\n";
}

// 2. Display Light Sequence
void displaySequence() {
    if (!head) { cout << "No traffic lights in system.\n"; return; }
    cout << "========== TRAFFIC LIGHT SEQUENCE ==========\n";
    cout << "ID Road Name Light\n-------------------------------\n";
    TrafficLight* temp = head;
    do {
        cout << temp->id << " " << temp->roadName << " " << temp->color << endl;
        temp = temp->next;
    } while (temp != head);
}

// 3. Advance to Next Light
void advanceLight() {
    if (!current) { cout << "No traffic lights in system.\n"; return; }

    // Reset all lights to Yellow
    TrafficLight* temp = head;
    do {
        temp->color = "Yellow";
        temp = temp->next;
    } while (temp != head);

    // Current becomes Red
    current->color = "Red";
    cout << current->roadName << " → Red\n";

    // Next becomes Green
    current->next->color = "Green";
    cout << current->next->roadName << " → Green\n";

    // Display all others as Yellow
    TrafficLight* other = current->next->next;
    while (other != current) {
        cout << other->roadName << " → Yellow\n";
        other = other->next;
    }

    // Move current pointer forward
    current = current->next;
}

// 4. Reset System
void resetSystem() {
    if (!head) { cout << "System already empty.\n"; return; }

    TrafficLight* temp = head->next;
    while (temp != head) {
        TrafficLight* nextNode = temp->next;
        delete temp;
        temp = nextNode;
    }
    delete head;
    head = current = nullptr;

    cout << "System reset. All traffic lights cleared.\n";
}
