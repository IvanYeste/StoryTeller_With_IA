    #include <iostream>
    #include <vector>
    #include <limits>
    #include <string>
    #include <cstdlib>
    #include <fstream>
    #include <thread>
    #include <chrono>
    #include <filesystem>

    using namespace std;
    namespace fs = std::filesystem;

    // Custom singly linked list to store stories
    class List {
    private:
        class Node {
        public:
            string content;
            Node* next;
        };
        Node* head;
    public:
        List();
        ~List();
        int length();
        void insert(int pos, string x);
        void remove(int pos);
        void swap(int pos1, int pos2);
        void saveToFile(const string& filename);
        bool isEmpty();
        void print();
    };

    List::List() {
        head = nullptr;
    }

    List::~List() {
        Node* temp = head;
        while (temp != nullptr) {
            Node* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
    }

    int List::length() {
        Node* temp = head;
        int count = 0;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }

    void List::insert(int pos, string x) {
        if (pos <= length() + 1) {
            Node* newNode = new Node();
            newNode->content = x;
            if (pos == 1) {
                newNode->next = head;
                head = newNode;
            } else if (pos == length() + 1) {
                Node* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newNode;
                newNode->next = nullptr;
            } else {
                Node* temp = head;
                for (int i = 1; i <= pos - 2; i++) {
                    temp = temp->next;
                }
                Node* next = temp->next;
                temp->next = newNode;
                newNode->next = next;
            }
        }
    }

    void List::remove(int pos) {
        if (pos <= length()) {
            Node* toDelete;
            if (pos == 1) {
                toDelete = head;
                head = head->next;
            } else {
                Node* temp = head;
                for (int i = 1; i <= pos - 2; i++) {
                    temp = temp->next;
                }
                Node* nextNode = temp->next;
                temp->next = nextNode->next;
                toDelete = nextNode;
            }
            delete toDelete;
        }
    }

    void List::swap(int pos1, int pos2) {
        if (pos1 <= length() && pos2 <= length()) {
            Node* node1 = head;
            Node* node2 = head;
            for (int i = 1; i < pos1; i++) {
                node1 = node1->next;
            }
            for (int i = 1; i < pos2; i++) {
                node2 = node2->next;
            }
            string temp = node1->content;
            node1->content = node2->content;
            node2->content = temp;
        }
    }

    void List::saveToFile(const string& filename) {
        ofstream file(filename);
    
        if (!file.is_open()) {
            cerr << "Error: File doesn't found .\n";
            return;
        }
    
        Node* temp = head;
        int index = 1;
        while (temp != nullptr) {
            file << "=== Story " << index++ << " ===\n";
            file << temp->content << "\n";
            file << "------------------------\n";
            temp = temp->next;
        }
    
        file.close();
    }    

    bool List::isEmpty() {
        return head == nullptr;
    }

    void List::print() {
        Node* temp = head;
        int index = 1;
        while (temp != nullptr) {
            cout << "\nStory " << index++ << ":\n";
            cout << temp->content << "\n";
            temp = temp->next;
        }
    }

    void waitForFileCompletion(const string& filename, int checkInterval = 2, int maxWaitTime = 120) {
        int elapsedTime = 0;
        size_t lastSize = 0;
        const char spinner[] = {'|', '/', '-', '\\'};
        int spinnerIndex = 0;

        while (elapsedTime < maxWaitTime) {
            if (!fs::exists(filename)) {
                this_thread::sleep_for(chrono::seconds(checkInterval));
                elapsedTime += checkInterval;
                continue;
            }

            size_t currentSize = fs::file_size(filename);
            if (currentSize > 0 && currentSize == lastSize) {
                break; // File size hasn't changed; assume generation is done
            }

            lastSize = currentSize;

            cout << "\rGenerating story " << spinner[spinnerIndex] << flush;
            spinnerIndex = (spinnerIndex + 1) % 4;

            this_thread::sleep_for(chrono::seconds(checkInterval));
            elapsedTime += checkInterval;
        }

        cout << "\rGeneration completed!           \n";
    }

    string generateStory(const string& prompt) {
        string llamaPath = "..."; // PUT HERE YOUR llama-cli.exe COMPLETE PATH
        string modelPath = "...";  // PUT HERE YOUR MODEL COMPLETE PATH
        string outputFile = "output.txt";

        ofstream clearFile(outputFile, ios::trunc);
        clearFile.close();

        string command = llamaPath + " -m " + modelPath + " --no-mmap --simple-io -no-cnv -p \"" + prompt + "\" > " + outputFile;

        thread process_thread([&]() {
            system(command.c_str());
        });

        waitForFileCompletion(outputFile);
        system("taskkill /IM llama-cli.exe /F > nul 2>&1");

        if (process_thread.joinable()) {
            process_thread.join();
        }

        ifstream file(outputFile);
        if (!file) {
            cerr << "Error: Could not open output.txt\n";
            return "Error generating the story.";
        }

        string story;
        string line;
        int lineCount = 0;
        
        while (getline(file, line)) {
            lineCount++;
            if (lineCount <= 1) continue; // Skip first line becouse there is the prompt
            story += line + "\n";
        }

        file.close();
        
        if (story.empty()) {
            cerr << "Error: Generated story is empty.\n";
            return "Error generating the story.";
        }

        return story;
    }

    void safeStorysOnAFile( List * list)
    {
        ofstream ("stories.txt");
        
    }

    void displayStories(List* list) {
        if (list->isEmpty()) {
            cout << "\nNo stories saved yet.\n";
            return;
        }

        cout << "\nSaved stories:\n";
        list->print();
    }

    int main() {
        List* list = new List();
        int choice;
        string prompt;

        do {
            cout << "\n==== AI Story Menu ====";
            cout << "\n1. Generate a story with AI";
            cout << "\n2. Show saved stories";
            cout << "\n3. Swap two saved stories";
            cout << "\n4. Delete a story";
            cout << "\n5. Safe stories on a file";
            cout << "\n6. Exit";
            cout << "\nChoose an option: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1: {
                    cout << "\nEnter the theme of the story: ";
                    getline(cin, prompt);
                    string story = generateStory(prompt);
                    list->insert(list->length() + 1, story);
                    cout << "\nStory generated and saved!\n";
                    break;
                }
                case 2: {
                    displayStories(list);
                    break;
                }
                case 3: {
                    if (list->length() < 2) {
                        cout << "You need at least two stories to swap.\n";
                        break;
                    }
                    int pos1, pos2;
                    cout << "Enter the position of the first story: ";
                    cin >> pos1;
                    cout << "Enter the position of the second story: ";
                    cin >> pos2;
                    list->swap(pos1, pos2);
                    cout << "Stories swapped successfully.\n";
                    break;
                }
                case 4: {
                    int pos;
                    cout << "Enter the position of the story to delete: ";
                    cin >> pos;
                    if (pos >= 1 && pos <= list->length()) {
                        list->remove(pos);
                        cout << "Story deleted.\n";
                    } else {
                        cout << "Invalid position.\n";
                    }
                    break;
                }
                case 5: {
                    list->saveToFile("stories.txt");
                    cout << "Stories saved succesfully!\n";
                }
                case 6: {
                    cout << "Goodbye!\n";
                    break;
                }
                default: {
                    cout << "Invalid option. Please try again.\n";
                    break;
                }
            }
        } while (choice != 6);

        delete list;
        return 0;
    }