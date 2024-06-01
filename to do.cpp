#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>

// Structure to represent a task
struct Task {
    int id;
    std::string description;
    bool completed;
};

// Class to handle the to-do list application
class ToDoList {
private:
    std::vector<Task> tasks;
    int nextId;

    void saveTasksToFile() {
        std::ofstream outFile("tasks.txt");
        if (!outFile) {
            std::cerr << "Error opening file for writing.\n";
            return;
        }
        for (const auto& task : tasks) {
            outFile << task.id << ";" << task.description << ";" << task.completed << "\n";
        }
        outFile.close();
    }

    void loadTasksFromFile() {
        std::ifstream inFile("tasks.txt");
        if (!inFile) {
            return;  // If the file does not exist, just return
        }
        std::string line;
        while (std::getline(inFile, line)) {
            std::istringstream iss(line);
            std::string token;
            Task task;
            std::getline(iss, token, ';');
            task.id = std::stoi(token);
            std::getline(iss, token, ';');
            task.description = token;
            std::getline(iss, token, ';');
            task.completed = (token == "1");
            tasks.push_back(task);
            if (task.id >= nextId) {
                nextId = task.id + 1;
            }
        }
        inFile.close();
    }

public:
    ToDoList() : nextId(1) {
        loadTasksFromFile();
    }

    void addTask(const std::string& description) {
        Task newTask = { nextId++, description, false };
        tasks.push_back(newTask);
        saveTasksToFile();
    }

    void viewTasks() const {
        std::cout << "ID\tDescription\t\tCompleted\n";
        for (const auto& task : tasks) {
            std::cout << task.id << "\t" << task.description << "\t\t" << (task.completed ? "Yes" : "No") << "\n";
        }
    }

    void deleteTask(int id) {
        auto it = std::remove_if(tasks.begin(), tasks.end(),
            [id](const Task& task) { return task.id == id; });
        if (it != tasks.end()) {
            tasks.erase(it, tasks.end());
            saveTasksToFile();
        } else {
            std::cerr << "Task ID not found.\n";
        }
    }

    void markTaskCompleted(int id) {
        for (auto& task : tasks) {
            if (task.id == id) {
                task.completed = true;
                saveTasksToFile();
                return;
            }
        }
        std::cerr << "Task ID not found.\n";
    }
};

void displayMenu() {
    std::cout << "To-Do List Application\n";
    std::cout << "1. Add Task\n";
    std::cout << "2. View Tasks\n";
    std::cout << "3. Delete Task\n";
    std::cout << "4. Mark Task Completed\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    ToDoList toDoList;
    int choice;
    do {
        displayMenu();
        std::cin >> choice;

        // Validate input
        while (std::cin.fail() || choice < 1 || choice > 5) {
            std::cin.clear(); // clear input buffer to restore cin to a usable state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore last input
            std::cout << "Invalid input. Please enter a number between 1 and 5: ";
            std::cin >> choice;
        }

        switch (choice) {
            case 1: {
                std::cout << "Enter task description: ";
                std::cin.ignore(); // clear newline character from the buffer
                std::string description;
                std::getline(std::cin, description);
                toDoList.addTask(description);
                break;
            }
            case 2: {
                toDoList.viewTasks();
                break;
            }
            case 3: {
                std::cout << "Enter task ID to delete: ";
                int id;
                std::cin >> id;
                toDoList.deleteTask(id);
                break;
            }
            case 4: {
                std::cout << "Enter task ID to mark as completed: ";
                int id;
                std::cin >> id;
                toDoList.markTaskCompleted(id);
                break;
            }
            case 5: {
                std::cout << "Exiting application...\n";
                break;
            }
        }
        std::cout << "\n";
    } while (choice != 5);

    return 0;
}
