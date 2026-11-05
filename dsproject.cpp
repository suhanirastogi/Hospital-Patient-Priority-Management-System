/*
    PROJECT: HOSPITAL PATIENT PRIORITY MANAGEMENT SYSTEM

    PRIORITY RULE:
    1 = Normal
    2 = Low emergency
    3 = Medium emergency
    4 = High emergency
    5 = Critical emergency

    A larger priority number means greater urgency.
*/
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <list>
#include <iomanip>
#include <limits>
#include <set>

using namespace std;

class Patient
{
public:
    int id;
    string name;
    int age;
    string gender;
    string disease;
    int priority;
    int arrivalNumber;
    string status;

    Patient()
    {
        id = 0;
        name = "";
        age = 0;
        gender = "";
        disease = "";
        priority = 1;
        arrivalNumber = 0;
        status = "Waiting";
    }

    Patient(int patientId,
            string patientName,
            int patientAge,
            string patientGender,
            string patientDisease,
            int patientPriority,
            int arrival)
    {
        id = patientId;
        name = patientName;
        age = patientAge;
        gender = patientGender;
        disease = patientDisease;
        priority = patientPriority;
        arrivalNumber = arrival;
        status = "Waiting";
    }
};

/*
    The patient with:
    1. Higher priority number is selected first.
    2. If priority is the same, earlier arrival is selected first.
*/

struct EmergencyComparator
{
    bool operator()(const Patient &a, const Patient &b) const
    {
        if (a.priority == b.priority)
        {
            return a.arrivalNumber > b.arrivalNumber;
        }

        return a.priority < b.priority;
    }
};

class HospitalSystem
{
private:

    // Linked list stores every registered patient record.
    list<Patient> patientRecords;

    // Hash table maps Patient ID to the patient's linked-list iterator.
    unordered_map<int, list<Patient>::iterator> patientTable;

    // Emergency patients are managed by priority.
    priority_queue<Patient,
                   vector<Patient>,
                   EmergencyComparator> emergencyQueue;

    // Normal patients are managed in First-In-First-Out order.
    queue<int> normalQueue;

    int nextArrivalNumber;

public:

    HospitalSystem()
    {
        nextArrivalNumber = 1;
    }



    int readInteger(string message, int minimum, int maximum)
    {
        int value;

        while (true)
        {
            cout << message;

            if (cin >> value)
            {
                if (value >= minimum && value <= maximum)
                {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return value;
                }
            }

            cout << "Invalid input! Enter a value between "
                 << minimum << " and " << maximum << ".\n";

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    string readText(string message)
    {
        string value;

        while (true)
        {
            cout << message;
            getline(cin, value);

            if (!value.empty())
            {
                return value;
            }

            cout << "Input cannot be empty. Please try again.\n";
        }
    }

    string getPriorityName(int priority)
    {
        switch (priority)
        {
            case 1:
                return "Normal";

            case 2:
                return "Low Emergency";

            case 3:
                return "Medium Emergency";

            case 4:
                return "High Emergency";

            case 5:
                return "Critical Emergency";

            default:
                return "Unknown";
        }
    }

    void printLine()
    {
        cout << "----------------------------------------------------------\n";
    }

    void printHeading(string heading)
    {
        printLine();
        cout << "                 " << heading << "\n";
        printLine();
    }

    void displayPatient(Patient p)
    {
        cout << left
             << setw(10) << p.id
             << setw(20) << p.name
             << setw(8) << p.age
             << setw(12) << p.priority
             << setw(18) << p.status
             << "\n";
    }

    void displayTableHeading()
    {
        cout << left
             << setw(10) << "ID"
             << setw(20) << "Name"
             << setw(8) << "Age"
             << setw(12) << "Priority"
             << setw(18) << "Status"
             << "\n";

        printLine();
    }


    void addPatient()
    {
        printHeading("REGISTER NEW PATIENT");

        int id = readInteger("Enter Patient ID: ", 1, 1000000000);

        // Check duplicate ID using hashing.
        if (patientTable.find(id) != patientTable.end())
        {
            cout << "\nA patient with this ID already exists.\n";
            return;
        }

        string name = readText("Enter Patient Name: ");

        int age = readInteger("Enter Age: ", 1, 120);

        string gender = readText("Enter Gender: ");

        string disease = readText("Enter Disease / Problem: ");

        cout << "\nSelect Patient Priority:\n";
        cout << "1. Normal\n";
        cout << "2. Low Emergency\n";
        cout << "3. Medium Emergency\n";
        cout << "4. High Emergency\n";
        cout << "5. Critical Emergency\n";

        int priority = readInteger("Enter Priority (1-5): ", 1, 5);

        Patient newPatient(
            id,
            name,
            age,
            gender,
            disease,
            priority,
            nextArrivalNumber
        );

        // Insert the patient into the linked list.
        patientRecords.push_back(newPatient);

        // Get the iterator of the newly inserted patient.
        list<Patient>::iterator patientIterator;
        patientIterator = patientRecords.end();
        --patientIterator;

        // Store Patient ID and iterator in the hash table.
        patientTable[id] = patientIterator;

        // Add the patient to the correct waiting structure.
        if (priority == 1)
        {
            normalQueue.push(id);
            cout << "\nPatient added to the normal waiting queue.\n";
        }
        else
        {
            emergencyQueue.push(newPatient);
            cout << "\nEmergency patient added to the priority queue.\n";
        }

        nextArrivalNumber++;

        cout << "\nPatient registered successfully!\n";
        cout << "Patient ID: " << id << "\n";
        cout << "Assigned Priority: " << getPriorityName(priority) << "\n";
    }



    void searchPatient()
    {
        printHeading("SEARCH PATIENT");

        int id = readInteger("Enter Patient ID to search: ", 1, 1000000000);

        // Hashing provides direct lookup by Patient ID.
        auto found = patientTable.find(id);

        if (found == patientTable.end())
        {
            cout << "\nPatient record not found.\n";
            return;
        }

        Patient &p = *(found->second);

        cout << "\nPatient Record Found!\n";
        printLine();

        cout << "Patient ID       : " << p.id << "\n";
        cout << "Name             : " << p.name << "\n";
        cout << "Age              : " << p.age << "\n";
        cout << "Gender           : " << p.gender << "\n";
        cout << "Disease          : " << p.disease << "\n";
        cout << "Priority Number  : " << p.priority << "\n";
        cout << "Priority Type    : " << getPriorityName(p.priority) << "\n";
        cout << "Arrival Number   : " << p.arrivalNumber << "\n";
        cout << "Current Status   : " << p.status << "\n";

        printLine();
    }



    void callNextPatient()
    {
        printHeading("CALL NEXT PATIENT");

        /*
            Emergency patients always get preference.
            The priority queue automatically selects the most urgent
            emergency patient.

            If no emergency patient is waiting, the normal queue
            follows FIFO order.
        */

        if (!emergencyQueue.empty())
        {
            Patient nextPatient = emergencyQueue.top();
            emergencyQueue.pop();

            // Ignore an old queue entry if the patient is no longer waiting.
            auto found = patientTable.find(nextPatient.id);

            if (found != patientTable.end() &&
                found->second->status == "Waiting")
            {
                found->second->status = "Treated";

                cout << "\nCalling emergency patient:\n";
                printLine();
                cout << "Patient ID : " << found->second->id << "\n";
                cout << "Name       : " << found->second->name << "\n";
                cout << "Priority   : "
                     << getPriorityName(found->second->priority) << "\n";
                cout << "Disease    : " << found->second->disease << "\n";
                printLine();

                cout << "Patient marked as treated.\n";
                return;
            }

            // Continue searching if an outdated entry was encountered.
            callNextPatient();
            return;
        }

        // No emergency patient is available; check normal queue.
        while (!normalQueue.empty())
        {
            int id = normalQueue.front();
            normalQueue.pop();

            auto found = patientTable.find(id);

            if (found != patientTable.end() &&
                found->second->status == "Waiting")
            {
                found->second->status = "Treated";

                cout << "\nCalling normal patient:\n";
                printLine();
                cout << "Patient ID : " << found->second->id << "\n";
                cout << "Name       : " << found->second->name << "\n";
                cout << "Priority   : Normal\n";
                cout << "Disease    : " << found->second->disease << "\n";
                printLine();

                cout << "Patient marked as treated.\n";
                return;
            }
        }

        cout << "\nNo patients are currently waiting.\n";
    }

    // ---------------------------------------------------------
    // DISPLAY WAITING LIST
    // ---------------------------------------------------------

    void displayWaitingList()
    {
        printHeading("CURRENT WAITING LIST");

        bool foundAny = false;
        set<int> displayedIds;

        cout << "\nEMERGENCY PATIENTS\n";
        printLine();

        cout << left
             << setw(10) << "ID"
             << setw(20) << "Name"
             << setw(8) << "Age"
             << setw(12) << "Priority"
             << setw(18) << "Status"
             << "\n";

        printLine();

        // Copy the priority queue so the original queue is not changed.
        priority_queue<Patient,
                       vector<Patient>,
                       EmergencyComparator> emergencyCopy = emergencyQueue;

        while (!emergencyCopy.empty())
        {
            Patient p = emergencyCopy.top();
            emergencyCopy.pop();

            auto found = patientTable.find(p.id);

            if (found != patientTable.end() &&
                found->second->status == "Waiting" &&
                displayedIds.find(p.id) == displayedIds.end())
            {
                displayPatient(*(found->second));
                displayedIds.insert(p.id);
                foundAny = true;
            }
        }

        cout << "\nNORMAL PATIENTS\n";
        printLine();
        displayTableHeading();

        // Copy the normal queue to preserve its original order.
        queue<int> normalCopy = normalQueue;

        while (!normalCopy.empty())
        {
            int id = normalCopy.front();
            normalCopy.pop();

            auto found = patientTable.find(id);

            if (found != patientTable.end() &&
                found->second->status == "Waiting" &&
                displayedIds.find(id) == displayedIds.end())
            {
                displayPatient(*(found->second));
                displayedIds.insert(id);
                foundAny = true;
            }
        }

        if (!foundAny)
        {
            cout << "\nNo patients are waiting at the moment.\n";
        }

        printLine();
    }

    // ---------------------------------------------------------
    // DISPLAY ALL PATIENT RECORDS
    // ---------------------------------------------------------

    void displayAllRecords()
    {
        printHeading("ALL PATIENT RECORDS");

        if (patientRecords.empty())
        {
            cout << "\nNo patient records available.\n";
            return;
        }

        displayTableHeading();

        // Traverse the linked list.
        for (auto it = patientRecords.begin();
             it != patientRecords.end();
             ++it)
        {
            displayPatient(*it);
        }

        printLine();
        cout << "Total registered records: "
             << patientRecords.size() << "\n";
    }

    void removePatient()
    {
        printHeading("CANCEL PATIENT");

        int id = readInteger("Enter Patient ID to cancel: ", 1, 1000000000);

        auto found = patientTable.find(id);

        if (found == patientTable.end())
        {
            cout << "\nPatient record not found.\n";
            return;
        }

        Patient &p = *(found->second);

        if (p.status != "Waiting")
        {
            cout << "\nThis patient is already marked as "
                 << p.status << ".\n";
            return;
        }

        cout << "\nPatient Name: " << p.name << "\n";

        string confirmation = readText(
            "Are you sure you want to cancel this patient? (yes/no): "
        );

        if (confirmation == "yes" || confirmation == "YES" ||
            confirmation == "Yes")
        {
            /*
                The patient is marked as Cancelled.
                The old ID may still exist in a queue, but it will be
                ignored when the waiting list or call-next operation
                checks the patient's status.
            */

            p.status = "Cancelled";

            cout << "\nPatient cancelled successfully.\n";
        }
        else
        {
            cout << "\nCancellation stopped.\n";
        }
    }

    void checkPatientStatus()
    {
        printHeading("CHECK PATIENT STATUS");

        int id = readInteger("Enter Patient ID: ", 1, 1000000000);

        auto found = patientTable.find(id);

        if (found == patientTable.end())
        {
            cout << "\nNo record found for this Patient ID.\n";
            return;
        }

        Patient &p = *(found->second);

        cout << "\nPatient Name : " << p.name << "\n";
        cout << "Patient ID   : " << p.id << "\n";
        cout << "Status       : " << p.status << "\n";

        if (p.status == "Waiting")
        {
            cout << "Priority     : "
                 << getPriorityName(p.priority) << "\n";
        }
    }



    void displayStatistics()
    {
        printHeading("HOSPITAL STATISTICS");

        int waiting = 0;
        int treated = 0;
        int cancelled = 0;
        int emergency = 0;
        int normal = 0;

        // Traverse the linked list and count patient categories.
        for (auto it = patientRecords.begin();
             it != patientRecords.end();
             ++it)
        {
            if (it->status == "Waiting")
            {
                waiting++;

                if (it->priority == 1)
                {
                    normal++;
                }
                else
                {
                    emergency++;
                }
            }
            else if (it->status == "Treated")
            {
                treated++;
            }
            else if (it->status == "Cancelled")
            {
                cancelled++;
            }
        }

        cout << "\nTotal Patient Records : "
             << patientRecords.size() << "\n";

        cout << "Currently Waiting     : " << waiting << "\n";
        cout << "Emergency Waiting     : " << emergency << "\n";
        cout << "Normal Waiting        : " << normal << "\n";
        cout << "Patients Treated      : " << treated << "\n";
        cout << "Patients Cancelled    : " << cancelled << "\n";

        printLine();
    }

    // ---------------------------------------------------------
    // DISPLAY MENU
    // ---------------------------------------------------------

    void displayMenu()
    {
        cout << "\n\n";
        printLine();
        cout << "       HOSPITAL PATIENT PRIORITY MANAGEMENT SYSTEM\n";
        printLine();

        cout << "1.  Add New Patient\n";
        cout << "2.  Call Next Patient\n";
        cout << "3.  Search Patient by ID\n";
        cout << "4.  Display Waiting List\n";
        cout << "5.  Display All Patient Records\n";
        cout << "6.  Cancel / Remove Patient\n";
        cout << "7.  Check Patient Status\n";
        cout << "8.  Display Hospital Statistics\n";
        cout << "9.  Exit\n";

        printLine();
    }


    void run()
    {
        int choice;

        cout << "\n";
        printHeading("WELCOME TO THE HOSPITAL MANAGEMENT SYSTEM");

        do
        {
            displayMenu();

            choice = readInteger("Enter your choice (1-9): ", 1, 9);

            switch (choice)
            {
                case 1:
                    addPatient();
                    break;

                case 2:
                    callNextPatient();
                    break;

                case 3:
                    searchPatient();
                    break;

                case 4:
                    displayWaitingList();
                    break;

                case 5:
                    displayAllRecords();
                    break;

                case 6:
                    removePatient();
                    break;

                case 7:
                    checkPatientStatus();
                    break;

                case 8:
                    displayStatistics();
                    break;

                case 9:
                    cout << "\nThank you for using the system!\n";
                    cout << "Exiting the application...\n";
                    break;

                default:
                    cout << "\nInvalid choice. Please try again.\n";
            }

        } while (choice != 9);
    }
};


int main()
{
    HospitalSystem hospital;

    hospital.run();

    return 0;
}
