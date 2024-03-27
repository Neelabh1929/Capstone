// tasks to be done:
/*
1) Modify the priorityfunction to compare hours and minutes and seconds as well
2) Right now the work of how to get the address of the header node of  particular type is remaining. Once it is figured out , all we need to do is to traverse through the list and insert the node at the particular location as per the deadline
*/

#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <ctime>
#include <string.h>
#include <ctime>
#include <fstream>
#define ll long long
using namespace std;

// This set will store the type of the tasks already stored
set<string> type_task;

// Linked List implementation for the managing the task
// Defination of the Node for the common list, specific type list

class Node
{
    string time, time1, type, message;
    ll imp_lvl;

    // next and prev pointers for traversing and other things
    Node *next;
    Node *prev;

    // Constructor for the node
public:
    Node(string t, string t1, string type1, string message1, ll i)
    {
        time = t; // time means deadline of the task
        time1 = t1;
        type = type1;
        message = message1;
        imp_lvl = i;
        next = NULL;
        prev = NULL;
    }

    Node(string t, string t1, string type1, string message1)
    {
        time = t;
        time1 = t1;
        type = type1;
        message = message1;
        // imp_lvl=0 is considered as the most common task
        imp_lvl = 0;
        next = NULL;
        prev = NULL;
    }
    Node(string type1, string message1)
    {

        type = type1;
        message = message1;
        // imp_lvl=0 is considered as the most common task
        imp_lvl = 0;
        next = NULL;
        prev = NULL;
    }

    Node(string type1, ll i)
    {
        time = "";
        type = type1;
        message = "";
        imp_lvl = i;
        next = NULL;
        prev = NULL;
    }
    Node(string type1)
    {
        type = type1;
        next = NULL;
        prev = NULL;
    }
    friend void insert_task_common(string, string, string, string, ll);
    friend void createlist();
    friend int priority(Node *, Node *);
    friend void print_common_list();
    friend void writeDataToFile();
    friend void eraseFileContents(const string &);
    friend bool comparetime12hr(Node *current, ll chour, ll cminute, ll csecond, ll cdate, ll cmonth, ll cyear);
    friend void reminder12hr();
};

Node *head_common = new Node("Common task list");

/***************************priority function block*************************************/
ll stringToInt(const string &str)
{
    ll value = 0;
    for (size_t i = 0; i < str.length(); ++i)
    {
        char c = str[i];

        if (c >= '0' && c <= '9')
        {
            value = value * 10 + (c - '0');
        }
        else
        {
            // Handle unexpected character
            // For example, you might print an error message
            cout << "Unexpected character encountered: " << c << endl;
        }
    }
    return value;
}

void extractDateComponents(const string &date, int &day, int &month, int &year)
{
    day = stringToInt(date.substr(0, 2));
    month = stringToInt(date.substr(2, 2));
    year = stringToInt(date.substr(4, 2));
    // used substr as it works on only two arguments takes the initial position from where to start and the number of steps thus easy to extract
}
void extractTimeComponents(const string &time, int &hours, int &minutes, int &seconds)
{
    hours = stringToInt(time.substr(0, 2));
    minutes = stringToInt(time.substr(2, 2));
    seconds = stringToInt(time.substr(4, 2));
}

// defination of the priority function
int priority(Node *temp, Node *temp_insert)
{
    int day1, month1, year1;
    int day2, month2, year2;

    // date components from the current node temp
    extractDateComponents(temp->time, day1, month1, year1);
    // date components from the node tempinsert
    extractDateComponents(temp_insert->time, day2, month2, year2);

    // Comparing year
    if (year1 < year2)
        return 1;
    else if (year1 > year2)
        return 0;

    // Compareng month
    if (month1 < month2)
        return 1;
    else if (month1 > month2)
        return 0;

    // Comparing day
    if (day1 < day2)
        return 1;
    else if (day1 > day2)
        return 0;
        int hours1, minutes1, seconds1, hours2, minutes2, seconds2;
        extractTimeComponents(temp->time1, hours1, minutes1, seconds1);
        extractTimeComponents(temp_insert->time1, hours2, minutes2, seconds2);

        // Compare hours, then minutes, then seconds
        // Comparing year
        if (hours1 < hours2)
            return 1;
        else if (hours1 > hours2)
            return 0;

        // Compareng month
        if (minutes1 < minutes2)
            return 1;
        else if (minutes1 > minutes2)
            return 0;

        // Comparing day
        if (seconds1 < seconds2)
            return 1;
        else if (seconds1 > seconds2)
            return 0;
            // If dates are equal then compare importance levels   ***still to update for more than 2
            if (temp->imp_lvl > temp_insert->imp_lvl)
                return 1;
            else
                return 0;

            // Add the comparision of time at particular day as well for comparision like for hour ,minutes.
    return 0;
}

/******************* Priority function block over ********************/

void insert_task_common(string deadlinedate, string deadlinetime, string type_by_user, string message, ll imp_lvl_by_user)
{

    Node *temp_insert = new Node(deadlinedate, deadlinetime, type_by_user, message, imp_lvl_by_user);

    Node *temp = head_common;

    while (temp->next != NULL && priority(temp->next, temp_insert))
    {
        temp = temp->next;
    }

    // Change the Next and prev pointers to insert the node in the list
    temp_insert->prev = temp;
    temp_insert->next = temp->next;

    if (temp->next != NULL)
        temp->next->prev = temp_insert;
    temp->next = temp_insert;
}

void insert_task()
{
    cout << "Enter the type of task:" << endl;
    string task_type_user;
    cin.ignore(); // to ignore the newline character from the buffer
    getline(cin, task_type_user);

    cout << "Enter the Integral Importance level of this task:" << endl;
    ll imp_lvl_user;
    cin >> imp_lvl_user;

    string deadlinedate, deadlinetime, message;
    cout << "Enter the Deadline date of the task:" << endl;
    cin.ignore(); // to ignore the newline character from the buffer
    getline(cin, deadlinedate);
    cout << "Enter the Deadline time of the task:" << endl;
    cin.ignore(); // to ignore the newline character from the buffer
    getline(cin, deadlinetime);

    cout << "Enter the message you want with the remainder:" << endl;
    getline(cin, message);

    transform(task_type_user.begin(), task_type_user.end(), task_type_user.begin(), ::tolower);

    // To avoid duplications caused by case differences
    type_task.insert(task_type_user);

    insert_task_common(deadlinedate, deadlinetime, task_type_user, message, imp_lvl_user);
}

void print_common_list()
{
    if (head_common->next != NULL)
    {
        Node *temp = head_common->next;
        cout << endl;
        while (temp != NULL)
        {
            cout << "Type: " << temp->type << endl;
            cout << "Deadline Date: " << temp->time << endl;
            cout << "Deadline time: " << temp->time1 << endl;

            cout << "Message: " << temp->message << endl;
            cout << "Importance level: " << temp->imp_lvl << endl;
            temp = temp->next;
            cout << endl;
        }

        cout << " ******* List over *******" << endl;
    }
    else
    {
        cout << "NO task in the list" << endl;
    }
}

// function to write data to the file
void writeDataToFile()
{
    ofstream file("task_data.txt"); // Open the file named "task_data.txt" for writing
    if (file.is_open())
    {
        Node *temp = head_common->next;
        while (temp != NULL)
        {
            // Replace occurrences of "$#" with "@@" to avoid conflicts
            string deadlinedate = temp->time;
            string deadlinetime = temp->time1;

            string type = temp->type;
            string message = temp->message;
            ll imp_lvl = temp->imp_lvl;
            size_t pos;
            while ((pos = deadlinedate.find("$#")) != string::npos)
            {
                deadlinedate.replace(pos, 2, "@@");
            }
            while ((pos = deadlinetime.find("$#")) != string::npos)
            {
                deadlinetime.replace(pos, 2, "@@");
            }

            while ((pos = type.find("$#")) != string::npos)
            {
                type.replace(pos, 2, "@@");
            }
            while ((pos = message.find("$#")) != string::npos)
            {
                message.replace(pos, 2, "@@");
            }
            // Write task data to the file
            file << deadlinedate << "$#" << deadlinetime << "$#" << type << "$#" << message << "$#" << imp_lvl << '\n';
            temp = temp->next;
        }
        file.close(); // Close the file after writing
    }
    /* else
    {
        cout << "Unable to open file for writing." << endl;
    } */
}

// function to retrieve data from file
void loadDataFromFile()
{
    ifstream file("task_data.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            // Replace occurrences of "$#" with a different delimiter '|'
            size_t pos;
            while ((pos = line.find("$#")) != string::npos)
            {
                line.replace(pos, 2, "|"); // Replace "$#" with '|'
            }
            // Tokenize the modified line using '|' as the delimiter
            stringstream ss(line);
            string deadlinedate, deadlinetime, type, message, imp_lvl_str;
            if (getline(ss, deadlinedate, '|') &&
                getline(ss, deadlinetime, '|') &&
                getline(ss, type, '|') &&
                getline(ss, message, '|') &&
                getline(ss, imp_lvl_str, '|'))
            {
                ll imp_lvl = stoll(imp_lvl_str);                                        // Convert importance level string to long long
                insert_task_common(deadlinedate, deadlinetime, type, message, imp_lvl); // Insert task into the linked list
            }
            else
            {
                cout << "Invalid line in data file: " << line << endl;
            }
        }
        file.close();
    }
    /*  else
     {
         cout << "No existing data file found. Starting with an empty task list." << endl;
     } */
}

// funcction to erase all the tasks from the file

void eraseFileContents(const string &filename)
{
    ofstream file(filename, ofstream::out | ofstream::trunc);
    if (file.is_open())
    {
        file.close();
        head_common->next = NULL;
        cout << "File contents erased successfully." << endl;
    }
    else
    {
        cerr << "Unable to open file for erasing." << endl;
    }
}

bool comparetime12hr(Node *current, ll chour, ll cminute, ll csecond, ll cdate, ll cmonth, ll cyear)
{
    int day, month, year, hour, second, minute;
    extractDateComponents(current->time, day, month, year);
    extractTimeComponents(current->time1, hour, minute, second);
    // Comparing year
    if (cyear < year)
        return 1;
    else if (cyear > year)
        return 0;

    // Compareng month
    if (cmonth < month)
        return 1;
    else if (cmonth > month)
        return 0;

    // Comparing day
    if (cdate < day)
        return 1;
    else if (cdate > day)
        return 0;

    if (cdate == day && cmonth == month && cyear == year)
    {
        if (chour < hour)
            return 1;
        else if (chour > hour)
            return 0;

        // Compareng month
        if (cminute < minute)
            return 1;
        else if (cminute > minute)
            return 0;

        // Comparing day
        if (csecond <= second)
            return 1;
        else if (csecond > second)
            return 0;
    }
    return 0;
}
/* void reminder12hr()
{
    time_t t = time(0);
    tm *ti = localtime(&t);
    ll chour = ti->tm_hour + 12; // chour=current hour. Adding 12 to rempove the tasks which are going to be displayed as reminder
    ll cminute = ti->tm_min;
    ll csecond = ti->tm_sec;
    ll cdate = ti->tm_mday;
    ll cmonth = ti->tm_mon;
    ll cyear = ti->tm_year;
    Node *temp = head_common;
    while (temp->next != NULL && !comparetime12hr(temp, chour, cminute, csecond, cdate, cmonth, cyear))
    {
        cout << "Type: " << temp->type << endl;
        cout << "Deadline Date: " << temp->time << endl;
        cout << "Deadline time: " << temp->time1 << endl;
        cout << "Message: " << temp->message << endl;
        cout << "Importance level: " << temp->imp_lvl << endl;
        temp = temp->next;
        cout << endl;
        temp = temp->next;
    }
} */

/* void reminder12hr()
{
    time_t currentTime = time(0);
    tm *currentDateTime = localtime(&currentTime);

    // Calculate current time in seconds
    long currentSeconds = currentDateTime->tm_hour * 3600 + currentDateTime->tm_min * 60 + currentDateTime->tm_sec;
    Node *temp = head_common->next;
    while (temp != NULL)
    {
        tm deadlineTime = {};
        sscanf(temp->time.c_str(), "%02d%02d%02d", &deadlineTime.tm_mday, &deadlineTime.tm_mon, &deadlineTime.tm_year);
        sscanf(temp->time1.c_str(), "%02d%02d%02d", &deadlineTime.tm_hour, &deadlineTime.tm_min, &deadlineTime.tm_sec);
        deadlineTime.tm_mon -= 1;
        time_t deadline = mktime(&deadlineTime);
        long timeDifference = difftime(deadline, currentTime);
        if (timeDifference > 0 && timeDifference < 43200)
        {
            cout << "Type: " << temp->type << endl;
            cout << "Deadline Date: " << temp->time << endl;
            cout << "Deadline Time: " << temp->time1 << endl;
            cout << "Message: " << temp->message << endl;
            cout << "Importance Level: " << temp->imp_lvl << endl;
            cout << endl;
        }
        temp = temp->next;
    }
} */

void reminder12hr()
{
    time_t currentTime = time(0);
    tm *currentDateTime = localtime(&currentTime);
    currentDateTime->tm_hour += 12;
    time_t adjustedTime = mktime(currentDateTime);
    Node *temp = head_common->next;
    while (temp != nullptr)
    {
        int day, month, year, hour, minute, second;
        sscanf(temp->time.c_str(), "%2d%2d%2d", &day, &month, &year);
        sscanf(temp->time1.c_str(), "%2d%2d%2d", &hour, &minute, &second);

        month -= 1;// because the ctime gives month from 0-11
        tm deadlineTime = {0};
        deadlineTime.tm_mday = day;
        deadlineTime.tm_mon = month;
        deadlineTime.tm_year = year + 100;
        deadlineTime.tm_hour = hour;
        deadlineTime.tm_min = minute;
        deadlineTime.tm_sec = second;
        time_t deadline = mktime(&deadlineTime);
        if (deadline < adjustedTime)
        {
            deadlineTime = *localtime(&deadline);
            deadlineTime.tm_mday += 1; 
            deadline = mktime(&deadlineTime);
        }
        if (deadline < adjustedTime)
        {
            cout << "Type: " << temp->type << endl;
            cout << "Deadline Date: " << temp->time << endl;
            cout << "Deadline Time: " << temp->time1 << endl;
            cout << "Message: " << temp->message << endl;
            cout << "Importance Level: " << temp->imp_lvl << endl;
            cout << endl;
        }
        else
        {
            break;
        }
        temp = temp->next;
    }
}


int main()
{
    loadDataFromFile();
    reminder12hr();
    ll x = 1;
    while (1)
    {
        cout << endl;
        cout << "Enter 1 to insert a task." << endl;
        cout << "Enter 2 to view the whole list of task." << endl;
        cout << "Enter 3 to remove all the previous tasks" << endl;
        cout << "Enter 4 to exit." << endl;
        ll y;
        cin >> y;
        switch (y)
        {
        case 1:
        {
            insert_task();
            cout << endl;
            break;
        }
        case 2:
        {
            print_common_list();
            break;
        }
        case 3:
        {
            eraseFileContents("task_data.txt");
            break;
        }
        case 4:
        {
            writeDataToFile();
            goto end;
            break;
        }
        default:
            cout << "Enter valid option." << endl;
        }
    }
end:
    x = 0;
    return 0;
}