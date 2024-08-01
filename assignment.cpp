#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <string>
#include <tuple>

using namespace std;




string getCurrentTimestamp() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

class GroupMember {
protected:
    string fullName, idNumber, lastAccess, emailAddress, phoneNumber, birthDate;

public:
    string getName() const { return fullName; }
    string getBirthDate() const { return birthDate; }
    string getLastAccess() const { return lastAccess; }

    void updateLastAccess(const string& access) { lastAccess = access; }
    void updatePhoneNumber(const string& phone) { phoneNumber = phone; }
    void updateBirthDate(const string& date) { birthDate = date; }
};

class Task {
private:
    string title, description, status, feedback, dueDate, submissionTime;

public:
    Task() {}
    Task(string t, string d, string s = "Not Started", string f = "", string due = "")
        : title(t), description(d), status(s), feedback(f), dueDate(due) {}

    string getStatus() const { return status; }
    string getFeedback() const { return feedback; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    string getSubmissionTime() const { return submissionTime; }

    void updateStatusAndFeedback(string s, string f) {
        status = s;
        feedback = f;
    }

    vector<string> getDetails() const {
        return {title, description, status, feedback, dueDate, submissionTime};
    }

    void displaySummary() const {
        cout << "Task: " << title << "\nStatus: " << status << endl;
    }

    void displayFull() const {
        cout << "Task: " << title << "\nDescription: " << description << "\nDue: " << dueDate << endl;
    }
};

vector<Task> globalTaskList;

class Trainee : public GroupMember {
private:
    vector<Task> personalTaskList;
    int secretCode;

public:
    Trainee() {}
    Trainee(string name, string id, string email, int code) {
        fullName = name;
        idNumber = id;
        emailAddress = email;
        secretCode = code;
    }

    int getSecretCode() const { return secretCode; }

    void showInfo() const {
        cout << "Name: " << fullName << "\nID: " << idNumber << "\nEmail: " << emailAddress
             << "\nPhone: " << phoneNumber << "\nBirth Date: " << birthDate
             << "\nLast Access: " << lastAccess << endl;
    }

    vector<string> getInfo() const {
        return {fullName, idNumber, emailAddress, phoneNumber, lastAccess, birthDate};
    }

    vector<Task> getTasks() const { return personalTaskList; }

    void addTask(string title, string content, string deadline, string added) {
        Task newTask(title, content, "Not Started", "", deadline);
        personalTaskList.push_back(newTask);
    }
};

class Mentor : public GroupMember {
private:
    vector<tuple<string, string, string>> reviewRequests;
    int secretCode;

public:
    Mentor() {}
    Mentor(string name, string id, string email, int code) {
        fullName = name;
        idNumber = id;
        emailAddress = email;
        secretCode = code;
    }

    int getSecretCode() const { return secretCode; }

    void showInfo() const {
        cout << "Name: " << fullName << "\nID: " << idNumber << "\nEmail: " << emailAddress
             << "\nPhone: " << phoneNumber << "\nBirth Date: " << birthDate
             << "\nLast Access: " << lastAccess << endl;
    }

    void addReviewRequest(string n, string s, string p) {
        reviewRequests.push_back({n, s, p});
    }
};

map<string, Trainee> traineeRecords;
map<string, Mentor> mentorRecords;

void mainMenu() {
    while (true) {
        cout << "=================================================\n";
        cout << "Welcome to Group Task Management System\n\n";
        cout << "1. Existing Member\n";
        cout << "2. New Member Registration\n";
        cout << "3. Exit\n\n";
        cout << "Select an option (1/2/3): ";
        cout << "=================================================\n";

        int choice;
        cin >> choice;

        if (choice == 1) {
            while (true) {
                cout << "=================================================\n";
                cout << "Login as:\n\n";
                cout << "1.Student\n";
                cout << "2. Mentor\n";
                cout << "3. Back\n";
                cout << "4. Exit\n";
                cout << "Current time: " << getCurrentTimestamp() << endl;
                cout << "=================================================\n";

                int loginChoice;
                cout << "Select: ";
                cin >> loginChoice;

                if (loginChoice == 3) break;
                if (loginChoice == 4) exit(0);

                string id;
                int pin;

                cout << "Enter ID: ";
                cin >> id;
                cout << "Enter PIN: ";
                cin >> pin;

                if (loginChoice == 1) {
                    if (traineeRecords.count(id) && traineeRecords[id].getSecretCode() == pin) {
                        traineeRecords[id].updateLastAccess(getCurrentTimestamp());
                        while (true) {
                            cout << "=================================================\n";
                            cout << "Welcome " << traineeRecords[id].getName() << endl << endl;
                            cout << "1. View Personal Info\n";
                            cout << "2. View Task List\n";
                            cout << "3. View Task Details\n";
                            cout << "4. View Access History\n";
                            cout << "5. Submit Task\n";
                            cout << "6. Logout\n";
                            cout << "=================================================\n";

                            int traineeChoice;
                            cout << "Select: ";
                            cin >> traineeChoice;

                            vector<Task> tasks = traineeRecords[id].getTasks();

                            switch (traineeChoice) {
                                case 1:
                                    traineeRecords[id].showInfo();
                                    break;
                                case 2:
                                    for (const auto& task : tasks) {
                                        task.displaySummary();
                                    }
                                    break;
                                case 3:
                                    for (const auto& task : tasks) {
                                        task.displayFull();
                                    }
                                    break;
                                case 4:
                                    cout << "Last access: " << traineeRecords[id].getLastAccess() << endl;
                                    break;
                                case 5:
                                    {
                                        string title, desc, deadline;
                                        cout << "Task title: ";
                                        cin.ignore();
                                        getline(cin, title);
                                        cout << "Description: ";
                                        getline(cin, desc);
                                        cout << "Deadline: ";
                                        getline(cin, deadline);
                                        traineeRecords[id].addTask(title, desc, deadline, getCurrentTimestamp());
                                    }
                                    break;
                                case 6:
                                    goto traineeLogout;
                                default:
                                    cout << "Invalid option.\n";
                            }
                        }
                        traineeLogout:;
                    } else {
                        cout << "Invalid credentials.\n";
                    }
                } else if (loginChoice == 2) {
                    if (mentorRecords.count(id) && mentorRecords[id].getSecretCode() == pin) {
                        mentorRecords[id].updateLastAccess(getCurrentTimestamp());
                        while (true) {
                            cout << "=================================================\n";
                            cout << "Welcome " << mentorRecords[id].getName() << endl << endl;
                            cout << "1. View Personal Info\n";
                            cout << "2. View Access History\n";
                            cout << "3. View All Tasks\n";
                            cout << "4. Add New Task\n";
                            cout << "5. Back\n";
                            cout << "6. Exit\n";
                            cout << "=================================================\n";

                            int mentorChoice;
                            cout << "Select: ";
                            cin >> mentorChoice;

                            switch (mentorChoice) {
                                case 1:
                                    mentorRecords[id].showInfo();
                                    break;
                                case 2:
                                    cout << "Last access: " << mentorRecords[id].getLastAccess() << endl;
                                    break;
                                case 3:
                                    for (const auto& task : globalTaskList) {
                                        task.displaySummary();
                                    }
                                    break;
                                case 4:
                                    {
                                        string title, desc, deadline;
                                        cout << "Task title: ";
                                        cin.ignore();
                                        getline(cin, title);
                                        cout << "Description: ";
                                        getline(cin, desc);
                                        cout << "Deadline: ";
                                        getline(cin, deadline);
                                        Task newTask(title, desc, "Not Started", "", deadline);
                                        globalTaskList.push_back(newTask);
                                        for (auto& trainee : traineeRecords) {
                                            trainee.second.addTask(title, desc, deadline, getCurrentTimestamp());
                                        }
                                        cout << "Task added successfully.\n";
                                    }
                                    break;
                                case 5:
                                    goto mentorLogout;
                                case 6:
                                    exit(0);
                                default:
                                    cout << "Invalid option.\n";
                            }
                        }
                        mentorLogout:;
                    } else {
                        cout << "Invalid credentials.\n";
                    }
                }
            }
        } else if (choice == 2) {
            string name, id, email, phone, dob;
            int pin;

            while (true) {
                cout << "=================================================\n";
                cout << "New Member Registration\n\n";
                cout << "Register as:\n";
                cout << "1. Student\n";
                cout << "2. Mentor\n";
                cout << "3. Back\n";
                cout << "4. Exit\n";
                cout << "Current time: " << getCurrentTimestamp() << endl;
                cout << "=================================================\n";

                int regChoice;
                cout << "Select: ";
                cin >> regChoice;

                if (regChoice == 3) break;
                if (regChoice == 4) exit(0);

                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter ID: ";
                getline(cin, id);
                cout << "Enter Email: ";
                getline(cin, email);
                cout << "Enter Phone: ";
                getline(cin, phone);
                cout << "Enter Date of Birth: ";
                getline(cin, dob);
                cout << "Enter PIN: ";
                cin >> pin;

                if (regChoice == 1) {
                    Trainee newTrainee(name, id, email, pin);
                    newTrainee.updatePhoneNumber(phone);
                    newTrainee.updateBirthDate(dob);
                    traineeRecords[id] = newTrainee;
                    cout << "Trainee registration successful.\n";
                } else if (regChoice == 2) {
                    Mentor newMentor(name, id, email, pin);
                    newMentor.updatePhoneNumber(phone);
                    newMentor.updateBirthDate(dob);
                    mentorRecords[id] = newMentor;
                    cout << "Mentor registration successful.\n";
                }
            }
        } else if (choice == 3) {
            exit(0);
        }
    }
}

int main() {
   
    mainMenu();
    return 0;
}