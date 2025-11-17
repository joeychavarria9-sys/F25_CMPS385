#include <iostream>
#include <string>
using namespace std;

class Task {
private:
	int id;
	string description;
	bool done;
public:
	Task() : id(0), description(""), done(false) {}
    Task(int tid, string desc) : id(tid), description(desc), done(false) {}

	int getId() const { return id; }
    string getDescription() const { return description; }
	bool isDone() const { return done; }

	void setDescription(string desc) { description = desc; }
	void toggle() { done = !done; }
	void display() const {
		cout << "ID: " << id << " | \"" << description << "\" | Status: ";
		cout << (done ? "Done" : "Pending") << endl;
	}
};

int main() {
    const int MAX_TASKS = 50;
    Task tasks[MAX_TASKS];
    int taskCount = 0;
    int nextId = 1001;
    int choice;

    cout << "*********** Welcome to To-Do Manager ***********" << endl;

    do {
        cout << "\n1 - Add task\n2 - Toggle task (by ID)\n3 - List pending tasks\n0 - Exit\nChoice: ";
        cin >> choice;
        cin.ignore(); // Clear newline from buffer

        switch (choice) {
        case 1: {
            if (taskCount >= MAX_TASKS) {
                cout << "Task limit reached!" << endl;
                break;
            }
            string desc;
            cout << "Enter description: ";
            getline(cin, desc);
            tasks[taskCount] = Task(nextId, desc);
            cout << "Task added! ID: " << nextId << endl;
            nextId++;
            taskCount++;
            break;
        }

        case 2: {
            int tid;
            cout << "Enter Task ID: ";
            cin >> tid;
            bool found = false;
            for (int i = 0; i < taskCount; ++i) {
                if (tasks[i].getId() == tid) {
                    tasks[i].toggle();
                    cout << "Toggled task " << tid << "." << endl;
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Task not found." << endl;
            }
            break;
        }

        case 3: {
            bool anyPending = false;
            for (int i = 0; i < taskCount; ++i) {
                if (!tasks[i].isDone()) {
                    tasks[i].display();
                    anyPending = true;
                }
            }
            if (!anyPending) {
                cout << "No pending tasks." << endl;
            }
            break;
        }

        case 0:
            cout << "Goodbye!" << endl;
            break;

        default:
            cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 0);

    return 0;
}