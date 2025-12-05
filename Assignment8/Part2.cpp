#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

struct Song {
    int id;
    string title;
    string artist;
    float duration;
    Song* prev;
    Song* next;
};

Song* head = nullptr;
Song* tail = nullptr;
Song* current = nullptr;

// Function declarations
void loadPlaylist();
void savePlaylist();
void addSong();
void displayPlaylist();
void playNext();
void playPrevious();
void removeSong();

// ================== MAIN ==================
int main() {
    loadPlaylist();
    int choice;
    do {
        cout << "\n****** Welcome to Music Playlist Manager ******\n";
        cout << "1. Add Song to Playlist\n2. Display Playlist\n3. Play Next Song\n4. Play Previous Song\n5. Remove Song\n6. Save and Exit\n→ ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: addSong(); break;
        case 2: displayPlaylist(); break;
        case 3: playNext(); break;
        case 4: playPrevious(); break;
        case 5: removeSong(); break;
        case 6: savePlaylist();
            cout << "Playlist saved. See you next jam!" << endl;
            break;
        default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 6);
    return 0;
}

// ================== FUNCTIONS ==================

// Load playlist from file
void loadPlaylist() {
    ifstream fin("playlist.txt");
    if (!fin) return;
    int id; string title, artist; float duration;
    while (fin >> id) {
        fin.ignore();
        getline(fin, title, ',');
        getline(fin, artist, ',');
        fin >> duration;
        Song* newSong = new Song{ id, title, artist, duration, nullptr, nullptr };
        if (!head) {
            head = tail = current = newSong;
        }
        else {
            tail->next = newSong;
            newSong->prev = tail;
            tail = newSong;
        }
    }
    fin.close();
}

// Save playlist to file
void savePlaylist() {
    ofstream fout("playlist.txt");
    Song* temp = head;
    while (temp) {
        fout << temp->id << "," << temp->title << "," << temp->artist << "," << temp->duration << "\n";
        temp = temp->next;
    }
    fout.close();
}

// Add song at end
void addSong() {
    Song* newSong = new Song;
    cout << "Enter Song ID: "; cin >> newSong->id; cin.ignore();
    cout << "Enter Title: "; getline(cin, newSong->title);
    cout << "Enter Artist: "; getline(cin, newSong->artist);
    cout << "Enter Duration (in minutes): "; cin >> newSong->duration; cin.ignore();

    newSong->next = nullptr;
    newSong->prev = tail;

    if (!head) {
        head = tail = current = newSong;
    }
    else {
        tail->next = newSong;
        tail = newSong;
    }
    cout << "Song added to playlist!\n";
}

// Display playlist
void displayPlaylist() {
    if (!head) { cout << "Playlist is empty.\n"; return; }
    cout << "========== CURRENT PLAYLIST ==========\n";
    cout << left << setw(5) << "ID" << setw(25) << "Title" << setw(20) << "Artist" << setw(10) << "Duration\n";
    cout << "----------------------------------------------------\n";
    Song* temp = head;
    while (temp) {
        cout << left << setw(5) << temp->id
            << setw(25) << temp->title
            << setw(20) << temp->artist
            << setw(10) << temp->duration << endl;
        temp = temp->next;
    }
}

// Play next song
void playNext() {
    if (!current) { cout << "No songs in playlist.\n"; return; }
    if (!current->next) { cout << "You are at the last song.\n"; return; }
    current = current->next;
    cout << "Now playing: " << current->title << " by " << current->artist << " (" << current->duration << " min)\n";
}

// Play previous song
void playPrevious() {
    if (!current) { cout << "No songs in playlist.\n"; return; }
    if (!current->prev) { cout << "You are at the first song.\n"; return; }
    current = current->prev;
    cout << "Now playing: " << current->title << " by " << current->artist << " (" << current->duration << " min)\n";
}

// Remove song by ID
void removeSong() {
    if (!head) { cout << "Playlist is empty.\n"; return; }
    int id; cout << "Enter Song ID to remove: "; cin >> id;
    Song* temp = head;
    while (temp && temp->id != id) temp = temp->next;
    if (!temp) { cout << "Song not found.\n"; return; }

    if (temp == head) head = head->next;
    if (temp == tail) tail = tail->prev;
    if (temp->prev) temp->prev->next = temp->next;
    if (temp->next) temp->next->prev = temp->prev;

    if (current == temp) current = temp->next ? temp->next : temp->prev;

    cout << "Song '" << temp->title << "' removed from playlist.\n";
    delete temp;
}
