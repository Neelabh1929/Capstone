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

char yellow[]={0x1b,'[','0',';','3', '3', 'm',0};//for text coloring
char red[]={0x1b,'[','0',';','3','1','m',0};//for text coloring

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
    friend void remove_tasks();
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
    year = stringToInt(date.substr(4, 4));
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
    ll cnt = 1;
    if (head_common->next != NULL)
    {
        Node *temp = head_common->next;
        cout << endl;
        while (temp != NULL)
        {
            cout <<yellow<< "TASK: " << cnt++ << endl;
            cout << "\033[0m"; 
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
            file << deadlinedate << "$#" << deadlinetime << "$#" << type << "$#" << message << "$#" << imp_lvl << '\n';
            temp = temp->next;
        }
        file.close();
    }
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
    int taskYear, taskMonth, taskDate, taskHour, taskMinute, taskSecond;
    extractDateComponents(current->time, taskDate, taskMonth, taskYear);
    extractTimeComponents(current->time1, taskHour, taskMinute, taskSecond);

    /* cout << taskYear << " " << taskMonth << " " << taskDate << " " << taskHour << " " << taskMinute << " " << taskSecond << endl; */ //For testing

    // Compare year
    if (taskYear < cyear)
        return false;
    else if (taskYear > cyear)
        return true;

    // Compare month
    if (taskMonth < cmonth)
        return false;
    else if (taskMonth > cmonth)
        return true;

    // Compare day
    if (taskDate < cdate)
        return false;
    else if (taskDate > cdate)
        return true;

    // Compare hours
    if (taskHour < chour)
        return false;
    else if (taskHour > chour)
        return true;

    // Compare minutes
    if (taskMinute < cminute)
        return false;
    else if (taskMinute > cminute)
        return true;

    // Compare seconds
    if (taskSecond < csecond)
        return false;

    // Default: If all components are equal, return true
    return true;
}


void reminder12hr()
{
    time_t currentTime = time(0);
    tm *ct = localtime(&currentTime); // ct= stores currenttime

    ll year = ct->tm_year + 1900, month = ct->tm_mon + 1, date = ct->tm_mday, hour = ct->tm_hour, minute = ct->tm_min, second = ct->tm_sec;
    /* cout << year << " " << month << " " << date << " " << hour << " " << minute << " " << second << endl; */ // For testing
    hour += 12;
    ll excess_hour = 0;
    if (hour >= 24)
    {
        excess_hour = hour - 24;
        hour -= 24;
    }
    if (excess_hour)
        date += 1;
    ll excess_month = 0;
    if (date > 31)
    {
        date -= 31;
        excess_month = 1;
    }
    if (excess_month)
        month += 1;
    ll excess_year = 0;
    if (month > 12)
    {
        month -= 12;
        excess_year = 1;
    }
    if (excess_year)
        year++;
   /*  cout << year << " " << month << " " << date << " " << hour << " " << minute << " " << second << endl; */
    Node *temp = head_common;
    while (temp->next != NULL && !comparetime12hr(temp->next, hour, minute, second, date, month, year))
    {
        cout<<red<<"Reminder for-->"<<endl;
        cout << "\033[0m"; 
        temp = temp->next;
        cout << "Type: " << temp->type << endl;
        cout << "Deadline Date: " << temp->time << endl;
        cout << "Deadline Time: " << temp->time1 << endl;
        cout << "Message: " << temp->message << endl;
        cout << "Importance Level: " << temp->imp_lvl << endl;
        cout << endl;
    }
}

void remove_tasks()
{
    Node *temp = head_common;
    cout << "Enter the task number to be removed" << endl;
    ll k;
    cin >> k;
    ll cnt = 0;
    while (temp->next != NULL && cnt < k)
    {
        temp = temp->next;
        ++cnt;
    }
    if (cnt < k || k == 0)
    {
        cout << "Enter a valid task number to be removed" << endl;
    }
    else
    {
        Node *p = temp->prev;
        p->next = temp->next;
        if (temp->next != nullptr)
            temp->next->prev = p;
        delete temp;
        cout << "Task removed ." << endl;
        writeDataToFile();
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
        cout << "Enter 4 to remove a task" << endl;
        cout << "Enter 5 to exit." << endl;
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
            remove_tasks();
            break;
        }
        case 5:
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