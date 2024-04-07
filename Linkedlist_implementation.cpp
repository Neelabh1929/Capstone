#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <ctime>
#include <string.h>
#include <ctime>
#include <fstream>
#define ll long long
#define colorend "\033[0m"
using namespace std;

//for text coloring
char normal[] = {0x1b, '[', '0', ';', '3', '9', 'm', 0};
char black[] = {0x1b, '[', '0', ';', '3', '0', 'm', 0};
char red[] = {0x1b, '[', '0', ';', '3', '1', 'm', 0};
char green[] = {0x1b, '[', '0', ';', '3', '2', 'm', 0};
char yellow[] = {0x1b, '[', '0', ';', '3', '3', 'm', 0};
char blue[] = {0x1b, '[', '0', ';', '3', '4', 'm', 0};
char Upurple[] = {0x1b, '[', '4', ';', '3', '5', 'm', 0};
char cyan[] = {0x1b, '[', '0', ';', '3', '6', 'm', 0};
char lgray[] = {0x1b, '[', '0', ';', '3', '7', 'm', 0};
char dgray[] = {0x1b, '[', '0', ';', '3', '8', 'm', 0};
char underlinedgray[] = {0x1b, '[', '4', ';', '3', '8', 'm', 0};
char Bred[] = {0x1b, '[', '1', ';', '3', '1', 'm', 0};

// This set will store the type of the tasks already stored
set<string> type_task;

class Node_daily
{
    string type, message, deadline;
    ll imp_lvl;
    Node_daily *next;
    Node_daily *prev;

public:
    Node_daily(string s)
    {
        type = s;
        message = "";
        imp_lvl = 0;
        deadline = "";
        next = NULL;
        prev = NULL;
    }
    Node_daily(string s, string m, ll a, string deadlinetime)
    {
        type = s;
        message = m;
        imp_lvl = a;
        deadline = deadlinetime;
        next = NULL;
        prev = NULL;
    }
    friend bool compare_time_daily(string);
    friend void insert_daily_task();
    friend void display_daily();
    friend void display_incomplete_dailytask();
    friend void delete_daily_task();
    friend void write_daily_task(string);
    friend void load_daily_task_file(string);
    friend void erase_all_daily_task(string);
    friend void insert_daily(string deadlinetime, string type, string message, ll imp_lvl);
};

Node_daily *head_daily = new Node_daily("Daily Task");
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
        time1 = t1;//time1 means deadline time in hour,minute,second of the task
        type = type1;
        message = message1;
        imp_lvl = i;
        next = NULL;
        prev = NULL;
    }

    Node(string t, string t1, string type1, string message1)
    {
        time = t;   // this is to store the deadline date
        time1 = t1; // this is to store the deadline time
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

    // All the friend functions of common Node clss
    friend void insert_task_common(string, string, string, string, ll);
    friend void createlist();
    friend int priority(Node *, Node *);
    friend void print_common_list(Node *head_common);
    friend void writeDataToFile(string);
    friend void eraseFileContents(const string &);
    friend bool cmoparet2ime(Node *current, ll chour, ll cminute, ll csecond, ll cdate, ll cmonth, ll cyear);
    friend void reminder_x_hr(ll);
    friend void remove_tasks(string);
    friend void alaram(Node *, string);
    friend bool is_list_empty();
    friend void reschedule(string);
    friend void edit_task(string);
    friend void daily_task();
    friend void insert_task_daily();
};

Node *head_common = new Node("Common task list");

/***************************priority function block*************************************/
ll string_to_int(const string &str)
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
            //to handle unexpected character, print an error messaeg
            cout <<red<< "Unexpected character encountered: "<<colorend << c << endl;
        }
    }
    return value;
}

void extract_datecomponents(const string &date, int &day, int &month, int &year)
{
    day = string_to_int(date.substr(0, 2));
    month = string_to_int(date.substr(2, 2));
    year = string_to_int(date.substr(4, 4));
    // used substr as it works on only two arguments takes the initial position from where to start and the number of steps thus easy to extract
}

void extract_time_components(const string &time, int &hours, int &minutes, int &seconds)
{
    hours = string_to_int(time.substr(0, 2));
    minutes = string_to_int(time.substr(2, 2));
    seconds = string_to_int(time.substr(4, 2));
}

bool correct_date(string deadlinedate) 
{
    int year,month,date;
    extract_datecomponents(deadlinedate,date,month,year);
    if (year <= 0)
    return false;
    if (month < 1 || month > 12)
    return false;
    switch (month) 
    {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return (date >= 1 && date <= 31);
        case 4: case 6: case 9: case 11:
            return (date >= 1 && date <= 30);
        case 2:
            if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) 
            {
                return (date >= 1 && date <= 29);
            } 
            else 
            {
                return (date >= 1 && date <= 28);
            }
    }
    return true;
}

bool correct_time(string deadlinetime)
{
    int hour,minute,second;
    extract_time_components(deadlinetime,hour,minute,second);
    if(hour>=24 || minute>=60 || second>=60 || hour<0 || minute<0 || second<0)
    return false;
    return true;
}

// defination of the priority function
int priority(Node *temp, Node *temp_insert)
{
    int day1, month1, year1;
    int day2, month2, year2;

    // date components from the current node temp
    extract_datecomponents(temp->time, day1, month1, year1);
    // date components from the node tempinsert
    extract_datecomponents(temp_insert->time, day2, month2, year2);

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
    extract_time_components(temp->time1, hours1, minutes1, seconds1);
    extract_time_components(temp_insert->time1, hours2, minutes2, seconds2);

    // Compare hours, then minutes, then seconds
    //comparing hours
    if (hours1 < hours2)
        return 1;
    else if (hours1 > hours2)
        return 0;

    // comparing minutes
    if (minutes1 < minutes2)
        return 1;
    else if (minutes1 > minutes2)
        return 0;

    // comparing day
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

bool compare_time_ctime_user_time(string user_date, string user_time)
// This function to check valid deadline input from user
{
    int year, month, date, hour, minute, seconds;
    extract_datecomponents(user_date, date, month, year);
    extract_time_components(user_time, hour, minute, seconds);
    time_t currentTime = time(0);
    tm *ct = localtime(&currentTime); // ct= stores currenttime

    ll cyear = ct->tm_year + 1900, cmonth = ct->tm_mon + 1, cdate = ct->tm_mday, chour = ct->tm_hour, cminute = ct->tm_min, csecond = ct->tm_sec;
    //Need to 1900 cause ct->tm_year returns year from 1900 , so for 2024 it returns 124 only. And in month indexing starts from 0 so need to add 1 for our conviniency

    // Compare year
    if (year < cyear)
        return false;
    else if (year > cyear)
        return true;

    // Compare month
    if (month < cmonth)
        return false;
    else if (month > cmonth)
        return true;

    // Compare day
    if (date < cdate)
        return false;
    else if (date > cdate)
        return true;

    // Compare hours
    if (hour < chour)
        return false;
    else if (hour > chour)
        return true;

    // Compare minutes
    if (minute < cminute)
        return false;
    else if (minute > cminute)
        return true;

    // Compare seconds
    if (seconds < csecond)
        return false;

    // Default: If all components are equal, return true
    return true;
}

void insert_task()
{
    cout<<cyan<<endl;
    cout << "Enter the type of task:"<<colorend << endl;
    string task_type_user;
    cin.ignore();
    getline(cin, task_type_user);

    cout <<cyan<< "Enter the Integral Importance level of this task:" <<colorend<< endl;
    ll imp_lvl_user;
    cin >> imp_lvl_user;
   /*  while((float)(imp_lvl_user%1)!=0.000000 || imp_lvl_user<0)
    {
        cout<<red<<"Please enter positive integer as importance level."<<colorend<<endl;
        cin>>imp_lvl_user;
    } */
    string deadlinedate, deadlinetime, message;

again: // Brings here when deadline entered is already expired

    cout<<cyan << "Enter the Deadline date of the task"<<yellow<<"(FORMAT = DDMMYYYY):"<<colorend << endl;

    getline(cin, deadlinedate);
    int month,year,date,hour,minute,seconds;
    // This passage of code is for taking valid input of deadline DATE
    if (deadlinedate.size() != 8 || !correct_date(deadlinedate))
    {
        cout <<red<< "Please Enter valid date in correct format." << colorend<<endl;
    }
    while (deadlinedate.size() != 8 || !correct_date(deadlinedate))
    {
        getline(cin, deadlinedate);
        if (deadlinedate.size() != 8 || !correct_date(deadlinedate)) 
        {
            cout<<red << "Please Enter valid date in correct format." <<colorend<< endl;
        }
    }

    cout<<cyan << "Enter the Deadline time of the task"<<yellow<<"(FORMAT = HHMMSS):"<<colorend << endl;
    getline(cin, deadlinetime);
    // This passage of code is for taking valid input of deadline DATE
    if (deadlinetime.size() != 6 || !correct_time(deadlinetime))
    {
        cout<<red << "Please Enter valid time in correct format"<<yellow<<"(HHMMSS):"<<colorend << endl;
    }
    while (deadlinetime.size() != 6 || !correct_time(deadlinetime))
    {
        getline(cin, deadlinetime);
        if (deadlinetime.size() != 6 || !correct_time(deadlinetime))
        {
            cout<<red << "Please Enter valid time in correct format"<<yellow<<"(HHMMSS):"<<colorend << endl;
        }
    }

    if (!compare_time_ctime_user_time(deadlinedate, deadlinetime)) 
    // if deadline is already expired
    {
        cout <<red<< "Deadline already expired. Please Enter correct deadline." <<colorend<<endl;
        goto again;
    }
    cout <<cyan<< "Enter the message you want with the remainder:"<<colorend << endl;
    getline(cin, message);

    transform(task_type_user.begin(), task_type_user.end(), task_type_user.begin(), ::tolower);

    // To avoid duplications caused by case differences
    type_task.insert(task_type_user);

    insert_task_common(deadlinedate, deadlinetime, task_type_user, message, imp_lvl_user);

    cout<<green<<"Task Added successfully."<<colorend<<endl;
}

void print_common_list(Node *head)
{
    ll cnt = 1;
    cout<<yellow<<endl<<"Common Task List:"<<colorend<<endl;
    if (head->next != NULL)
    {
        Node *temp = head->next;
        cout << endl;
        while (temp != NULL)
        {
            cout << Upurple<<endl<< "TASK:"<<colorend<<" " << cnt++<< endl;
            cout <<cyan<< "Type: "<<colorend << colorend<<temp->type << endl;
            cout <<cyan<< "Deadline Date: " << colorend<<temp->time << endl;
            cout <<cyan<< "Deadline time: " << colorend<<temp->time1 << endl;

            cout <<cyan<<"Message: " << colorend<<temp->message << endl;
            cout << cyan<<"Importance level: " <<colorend<< temp->imp_lvl << endl;
            temp = temp->next;
            cout << endl;
        }

        cout <<Bred<< "******* List Over *******"<<colorend << endl;
        //Bred==bold red
    }
    else
    {
        cout<< yellow<< "NO task in the list"<<colorend << endl;
    }
}

// function to write data to the file
void writeDataToFile(string username)
{
    string fle = username + "task_data";
    ofstream file(fle); // Open the file named "task_data.txt" for writing
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
            file << deadlinedate << "$#" << deadlinetime << "$#" << type << "$#" << message << "$#" << imp_lvl << endl;
            temp = temp->next;
        }
        file.close();
    }
}

// function to retrieve data from file

void loadDataFromFile(string username)
{
    string fle = username + "task_data";
    ifstream file(fle);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            size_t pos;
            while ((pos = line.find("$#")) != string::npos)
            {
                line.replace(pos, 2, "|");
            }
            stringstream ss(line);
            string deadlinedate, deadlinetime, type, message, imp_lvl_str;
            if (getline(ss, deadlinedate, '|') &&
                getline(ss, deadlinetime, '|') &&
                getline(ss, type, '|') &&
                getline(ss, message, '|') &&
                getline(ss, imp_lvl_str, '|'))
            {
                ll imp_lvl=stoll(imp_lvl_str);                                        
                // convert importance level string to long long
                insert_task_common(deadlinedate, deadlinetime, type, message, imp_lvl); // Insert task into the linked list
            }
            else
            {
                cout<<red << "Invalid line in data file: " << line <<colorend<< endl;
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
        cout <<green<< "All Common Tasks are erased."<<colorend << endl;
    }
    else
    {
        cerr<<red << "Unable to open file for erasing."<<colorend<< endl;
    }
}

bool cmoparet2ime(Node *current, ll chour, ll cminute, ll csecond, ll cdate, ll cmonth, ll cyear)
{
    int taskYear, taskMonth, taskDate, taskHour, taskMinute, taskSecond;
    extract_datecomponents(current->time, taskDate, taskMonth, taskYear);
    extract_time_components(current->time1, taskHour, taskMinute, taskSecond);

    /* cout << taskYear << " " << taskMonth << " " << taskDate << " " << taskHour << " " << taskMinute << " " << taskSecond << endl; */ 
    // for testing

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

bool checkdate(ll month, ll *date, ll year)
{
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 | month == 10 || month == 12)
    {
        if (*date > 31)
        {
            *date = 1;
            return 1;
        }
        return 0;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (*date > 30)
        {
            *date = 1;
            return 1;
        }
        return 0;
    }
    else
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        {
            if (*date > 28)
            {
                *date = 1;
                return 1;
            }
            return 0;
        }
        else
        {
            if (*date > 29)
            {
                *date = 1;
                return 1;
            }
            return 0;
        }
    }
}

bool is_list_empty()
// This function is to check wether the list is empty or not
{
    if (head_common->next == NULL)
    {
        return 1;
    }
    return 0;
}

void reminder_x_hr(ll rhour)
{
    time_t currentTime = time(0);
    tm *ct = localtime(&currentTime); // ct= stores currenttime

    ll year = ct->tm_year + 1900, month = ct->tm_mon + 1, date = ct->tm_mday, hour = ct->tm_hour, minute = ct->tm_min, second = ct->tm_sec;

    /* cout << year << " " << month << " " << date << " " << hour << " " << minute << " " << second << endl; */
    // For testing
    hour += rhour;
    ll excess_hour = 0;
    if (hour >= 24)
    {
        excess_hour = hour - 24;
        hour -= 24;
    }
    if (excess_hour)
        date += 1;
    ll excess_month = 0;
    if (checkdate(month, &date, year))
    {
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
    while (temp->next != NULL && !cmoparet2ime(temp->next, hour, minute, second, date, month, year))
    {
        cout << Upurple<<endl<< "Reminder for-->" << endl;
        temp = temp->next;
        cout <<cyan <<"Type: " <<colorend<< temp->type << endl;
        cout << cyan<<"Deadline Date: " << temp->time << endl;
        cout <<cyan <<"Deadline Time: "<<colorend << temp->time1 << endl;
        cout <<cyan <<"Message: "<<colorend << temp->message << endl;
        cout << cyan <<"Importance Level: "<<colorend << temp->imp_lvl << endl;
        cout << endl;
    }
}

void remove_tasks(string username)
{
    Node *temp = head_common;
    if (is_list_empty())
    {
        cout <<red<< "There is no task in the list." <<colorend<< endl;
        return;
    }
    cout <<cyan<<endl<< "Enter the task number to be removed."<<colorend << endl;
    ll k;
    cin >> k;
    ll cnt = 0;
    while (temp->next != NULL && cnt < k)
    {
        temp = temp->next;
        ++cnt;
    }
    while (cnt < k || k == 0 || cnt<0)
    {
        cout <<red<< "Enter a valid task number to be removed."<<colorend << endl;
        cin >> k;
    }
    Node *p = temp->prev;
    p->next = temp->next;
    if (temp->next != NULL)
        temp->next->prev = p;
    delete temp;
    cout<<green<< "Task removed."<<colorend << endl;
    writeDataToFile(username);
}

void alaram(Node *head, string username)
{
    time_t currentTime = time(0);
    tm *ct = localtime(&currentTime); // ct= stores currenttime

    ll cnt = 0;

    ll year = ct->tm_year + 1900, month = ct->tm_mon + 1, date = ct->tm_mday, hour = ct->tm_hour, minute = ct->tm_min, second = ct->tm_sec;
    /* cout << year << " " << month << " " << date << " " << hour << " " << minute << " " << second << endl; */ // For testing
    hour += 0;
    ll excess_hour = 0;
    if (hour >= 24)
    {
        excess_hour = hour - 24;
        hour -= 24;
    }
    if (excess_hour)
        date += 1;
    ll excess_month = 0;
    if (checkdate(month, &date, year))
    {
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

    Node *temp = head;
    if (head->next == NULL)
        return;
    temp = temp->next;
    while (temp != NULL && !cmoparet2ime(temp, hour, minute, second, date, month, year))
    {
        cnt = 1; // This condition is very important as it updates the next of the head_common
        cout << Upurple <<endl<< "ALARAM FOR:-->" <<colorend<< endl;
        cout<<cyan << "Type: "<<colorend << temp->type << endl;
        cout<<cyan << "Deadline Date: "<<colorend << temp->time << endl;
        cout <<cyan<< "Deadline Time: " <<colorend<< temp->time1 << endl;
        cout <<cyan<< "Message: " << colorend<<temp->message << endl;
        cout <<cyan<< "Importance Level: " << colorend<<temp->imp_lvl << endl;
        cout << endl;
        temp = temp->next;
    }
    if (cnt == 1)
    {
        head->next = temp;
        cout << green<< "completed tasks are removed"<<colorend << endl;
    }
    writeDataToFile(username);
}

void reschedule(string username)
{
    Node *temp = head_common;
    if (is_list_empty())
    {
        cout <<red<< "There is no task int the list."<<colorend << endl;
        return;
    }
    cout << Upurple<<"Enter the task number to be rescheduled"<<colorend << endl;
    ll k;
    cin >> k;
    ll cnt = 0;
    while (temp->next != NULL && cnt < k)
    {
        temp = temp->next;
        ++cnt;
    }
    while (cnt < k || k == 0 || cnt<0)
    {
        cout <<red<< "Enter a valid task number to be rescheduled"<<colorend << endl;
        cin >> k;
    }
    string deadlinedate, deadlinetime, message;

again: // Brings here when deadline entered is already expired

    cout <<cyan<< "Enter the Deadline date of the task"<<yellow<<"(FORMAT = DDMMYYYY):"<<colorend << endl;
    getline(cin, deadlinedate);
    int month,year,date,hour,minute,seconds;
    // This passage of code is for taking valid input of deadline DATE
    if (deadlinedate.size() != 8 || !correct_date(deadlinedate))
    {
        cout << red<<"Please Enter valid date in correct format."<<colorend << endl;
    }
    while (deadlinedate.size() != 8 || !correct_date(deadlinedate))
    {
        getline(cin, deadlinedate);
        if (deadlinedate.size() != 8 || !correct_date(deadlinedate))
        {
            cout <<red<< "Please Enter valid date in correct format."<<colorend << endl;
        }
    }

    cout << cyan<<"Enter the Deadline time of the task"<<yellow<<"(FORMAT = HHMMSS):"<<colorend << endl;
    getline(cin, deadlinetime);
    // This passage of code is for taking valid input of deadline DATE
    if (deadlinetime.size() != 6 || !correct_time(deadlinetime))
    {
        cout <<red<< "Please Enter valid time in correct format."<<colorend << endl;
    }
    while (deadlinetime.size() != 6 || !correct_time(deadlinetime))
    {
        getline(cin, deadlinetime);
        if (deadlinetime.size() != 6 || !correct_time(deadlinetime))
        {
            cout << red<<"Please Enter valid time in correct format." <<colorend<< endl;
        }
    }

    if (!compare_time_ctime_user_time(deadlinedate, deadlinetime)) // if deadline is already expired
    {
        cout<<red << "Deadline already expired. Please Enter correct deadline." << endl;
        goto again;
    }

    temp->time = deadlinedate;
    temp->time1 = deadlinetime;
    writeDataToFile(username);
    cout<<green<<"Task Rescheduled"<<colorend<<endl;
}

void edit_task(string username)
{
    cout <<cyan<<endl<<"Enter the task number which you want to edit"<<colorend << endl;
    ll k;
    Node *temp = head_common;
    if (is_list_empty())
    {
        cout << red<<"There is no task in the list." <<colorend<< endl;
        return;
    }
    cin >> k;
    ll cnt = 0;
    while (temp->next != NULL && cnt < k)
    {
        temp = temp->next;
        ++cnt;
    }
    while (cnt < k || k == 0 || cnt<0)
    {
        cout <<red<<"Enter a valid task number(Either you entered 0 or there are less task then your number)"<<colorend << endl;
        cin >> k;
    }
    ll counter;
    cout <<endl<< "Enter 1 to edit task type." << endl;
    cout << "Enter 2 to edit task importance level." << endl;
    cout << "Enter 3 to edit task message." << endl;
    cin >> counter; 
    switch (counter)
    {
    case 1:
    {
        cout <<cyan<<endl<< "Enter the new type of task:"<<colorend << endl;
        string task_type_user;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // igonres newline char
        getline(cin, task_type_user);
        temp->type = task_type_user;
        type_task.insert(task_type_user);
        writeDataToFile(username);
        break;
    }
    case 2:
    {
        cout << cyan<<endl<<"Enter the new importance level:"<<colorend << endl;
        ll task_imp_lvl;
        cin >> task_imp_lvl;
        temp->imp_lvl = task_imp_lvl;
        writeDataToFile(username);
        break;
    }
    case 3:
    {
        cout <<cyan<<endl<< "Enter the new message:"<<colorend << endl;
        string task_message;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // igonres newline char
        getline(cin, task_message);
        temp->message = task_message;
        writeDataToFile(username);
        break;
    }
    }
    cout<<green<<"Task Edited"<<colorend<<endl;
}

// Daily task block

bool compare_time_daily(string deadline)
{
    int hour, minute, seconds;
    extract_time_components(deadline, hour, minute, seconds);
    time_t currentTime = time(0);
    tm *ct = localtime(&currentTime); // ct= stores currenttime

    ll chour = ct->tm_hour, cminute = ct->tm_min, csecond = ct->tm_sec;
    // Compare hours
    if (hour < chour)
        return false;
    else if (hour > chour)
        return true;

    // Compare minutes
    if (minute < cminute)
        return false;
    else if (minute > cminute)
        return true;

    // Compare seconds
    if (seconds < csecond)
        return false;

    // Default: If all components are equal, return true
    return true;
}


void insert_daily(string deadlinetime, string task_type_user, string message, ll imp_lvl_user)
{
    Node_daily *temp = head_daily;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    Node_daily *insert = new Node_daily(task_type_user, message, imp_lvl_user, deadlinetime);
    if (head_daily->next == NULL)
    {
        head_daily->next = insert;
        insert->prev = head_daily;
    }
    else
    {
        insert->prev = temp;
        temp->next = insert;
    }
}

void insert_daily_task()
{
    cout<<cyan << "Enter the type of daily task:"<<colorend << endl;
    string task_type_user;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // igonres newline char
    getline(cin, task_type_user);
    cout<<cyan << "Enter the Integral Importance level of this task:"<<colorend << endl;
    ll imp_lvl_user;
    cin >> imp_lvl_user;
    string deadlinetime, message;

    cout <<cyan<< "Enter the time of the task(FORMAT = HHMMSS):" <<colorend<< endl;
    cin.ignore();
    getline(cin, deadlinetime);
    // This passage of code is for taking valid input of deadline DATE
    if (deadlinetime.size() != 6 || !correct_time(deadlinetime))
    {
        cout <<red<< "Please Enter valid time in correct format"<<colorend << endl;
    }
    while (deadlinetime.size() != 6 || !correct_time(deadlinetime))
    {
        getline(cin, deadlinetime);
        if (deadlinetime.size() != 6 || !correct_time(deadlinetime))
        {
            cout<<red << "Please Enter valid time in correct format"<<colorend << endl;
        }
    }

    cout<<cyan << "Enter the message you want with the remainder:"<<colorend << endl;
    getline(cin, message);

    transform(task_type_user.begin(), task_type_user.end(), task_type_user.begin(), ::tolower);

    insert_daily(deadlinetime, task_type_user, message, imp_lvl_user);
    cout<<green<<"Daily Task added."<<endl;
}

void display_daily()
{
    cout<<yellow<<endl<<"Daily-Task List:"<<endl;
    Node_daily *temp = head_daily;
    ll cnt = 1;
    if (temp->next == NULL)
    {
        cout <<red<<endl <<"No task in daily task list."<<colorend << endl;
        return;
    }
    while (temp->next != NULL)
    {
        temp = temp->next;
        cout << Upurple<<endl<< "TASK:" <<colorend<<" "<< cnt++ << endl;
        cout << cyan<<"Type: "<<colorend << temp->type << endl;
        cout << cyan<<"Impotance level: " <<colorend<< temp->imp_lvl << endl;
        cout << cyan<<"Time:"<<colorend << temp->deadline << endl;
        cout << cyan<<"Message:"<<colorend << temp->message << endl;
    }
}

void display_incomplete_dailytask()
{
    cout << endl;
    cout << yellow << "List of Incomplete Daily tasks:"<<colorend << endl;
    Node_daily *temp = head_daily;
    ll cnt = 1, k = 0;
    while (temp->next != NULL && !compare_time_daily(temp->next->deadline))
    {
        temp = temp->next;
    }
    while (temp->next != NULL)
    {
        k = 1;
        temp = temp->next;
        cout << Upurple << "TASK: " <<colorend<< cnt++ << endl;
        cout <<cyan<< "Type: "<<colorend << temp->type << endl;
        cout <<cyan<< "Impotance level: "<<colorend << temp->imp_lvl << endl;
        cout << cyan<<"Time:"<<colorend << temp->deadline << endl;
        cout << cyan<<"Message: "<<colorend << temp->message << endl;
    }
    if (k == 0)
    {
        cout <<red<< "No Incomplete daily task"<<colorend << endl;
    }
}

void delete_daily_task()
{
    Node_daily *temp = head_daily;
    if (temp->next == NULL)
    {
        cout <<red<< "There is no task in the Daily task list."<<colorend << endl;
        return;
    }
    cout <<cyan<<endl<< "Enter the task number to be removed"<<colorend << endl;
    ll k;
    cin >> k;
    ll cnt = 0;
    while (temp->next != NULL && cnt < k)
    {
        temp = temp->next;
        ++cnt;
    }
    while (cnt < k || k == 0 || cnt<0)
    {
        cout <<red<< "Enter a valid task number to be removed"<<colorend << endl;
        cin >> k;
    }
    Node_daily *p = temp->prev;
    p->next = temp->next;
    if (temp->next != NULL)
        temp->next->prev = p;
    delete temp;
    cout << green<<"Task removed."<<colorend << endl;
}

void write_daily_task(string username)
{
    string fle = username + "dailytask_data.txt";
    ofstream file(fle); // Open the file named "dailytask_data.txt" for writing
    if (file.is_open())
    {
        if (head_daily->next == NULL)
        {
            return;
        }
        Node_daily *temp = head_daily->next;
        while (temp != NULL)
        {
            string deadlinetime = temp->deadline;
            string type = temp->type;
            string message = temp->message;
            ll imp_lvl = temp->imp_lvl;
            size_t pos;
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
            file << deadlinetime << "$#" << type << "$#" << message << "$#" << imp_lvl << '\n';
            temp = temp->next;
        }
        file.close();
    }
}

void load_daily_task_file(string username)
{
    string fle = username + "dailytask_data.txt";
    ifstream file(fle);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            size_t pos;
            while ((pos = line.find("$#")) != string::npos)
            {
                line.replace(pos, 2, "|"); 
            }
            stringstream ss(line);
            string deadlinedate, deadlinetime, type, message, imp_lvl_str;
            if (getline(ss, deadlinetime, '|') &&
                getline(ss, type, '|') &&
                getline(ss, message, '|') &&
                getline(ss, imp_lvl_str, '|'))
            {
                ll imp_lvl = stoll(imp_lvl_str);                    
                insert_daily(deadlinetime, type, message, imp_lvl);
            }
        }
        file.close();
    }
}
void erase_all_daily_task(string username)
{
    string fle = username + "dailytask_data.txt";
    ofstream file(fle, ofstream::out | ofstream::trunc);
    if (file.is_open())
    {
        file.close();
        head_daily->next = NULL;
        cout <<green<<"All Daily tasks removed"<<colorend<< endl;
    }
}

// Daiy task block ended

void loaduname(set<string> &unames)
{
    ifstream infile("usernames.txt");
    if (!infile.good())
    {
        // If file doesn't exist, create an empty one
        ofstream createFile("usernames.txt");
        createFile.close();
        return;
    }
    string username;
    while (infile >> username)
    {
        unames.insert(username);
    }
    infile.close();
}

void writeuname(const set<string> &unames)
{
    ofstream outfile("usernames.txt");
    for (set<string>::iterator i = unames.begin(); i != unames.end(); i++)
    {
        outfile << *i << endl;
    }
    outfile.close();
}

void writeuser_pass(const vector<pair<string, string> > &user_pass)
{
    ofstream outfile("user_pass.txt", ios::app); // Open file in append mode
    if (outfile.is_open())
    {
        for (ll i=0;i<user_pass.size();i++)
        {
            outfile << user_pass[i].first << "$#" << user_pass[i].second << endl;
        }
        outfile.close();
    }
}

void readuser_pass(vector<pair<string, string> >&user_pass)
{
    ifstream infile("user_pass.txt");
    if (infile.is_open())
    {
        string line;
        while (getline(infile, line))
        {
            size_t pos = line.find("$#");
            if (pos != string::npos)
            {
                string username = line.substr(0, pos);
                string password = line.substr(pos + 2); // Skipping $# separator
                user_pass.push_back(make_pair(username, password));
            }
        }
        infile.close();
    }
}

//*************** Main function ****************

int main()
{
    set<string> unames; // stores all the pre-existing usernames
    loaduname(unames);
    ll cnt = unames.size(); // This cnt is important to check that wether username is present or not
    string username, password;
    vector<pair<string, string> > user_pass;
    readuser_pass(user_pass);
    cout<<Upurple<<endl<<"Welcome to Task-Manager:"<<colorend<<endl;
    cout<<endl;//For better visualization of output in console
    cout<<dgray;
    cout << "Enter 1 for Signing up for the to-do list." << endl;
    cout << "Enter 2 to Login to your ID ." << endl;
    cout << "Enter 3 to Exit." <<endl;
    cout<<endl;
    int user_login;
    cin >> user_login;
    ll x = 1;
    if (user_login == 1)
    {
        cout<<endl;
        cout<<Upurple << "Enter the username ("<<underlinedgray<<"CASE SENSITIVE and WITHOUT SPACE"<<colorend<<") : "<<colorend << endl;
        cout<<dgray;
        cin >> username;
        unames.insert(username);
        while (cnt == unames.size())
        {
            cout<<red;
            cout << "This username is not available. Enter any other username." << endl;
            cout<<colorend<<endl;
            cin >> username;
            unames.insert(username);
        }
        cout<<endl;
        cout<<dgray<< "Enter yor 8 character password (Any characters)" << endl;
        {
            cin >> password;
            while (password.size() != 8)
            {
                cout<<red << "Entered Password did not qualify required qualities. Please Enter Properly." <<colorend<< endl;
                cout<<endl;
                cin >> password;
            }
        }
        user_pass.push_back(make_pair(username, password));
        writeuser_pass(user_pass);
    }
    else if(user_login==2)
    {
    useragain:
    cout<<endl;
        cout <<dgray<< "Enter the username (CASE SENSITIVE and WITHOUT SPACE):" << endl;
        cin >> username;
        int condition = 0;
        for (set<string>::iterator i = unames.begin(); i != unames.end(); i++)
        {
            if (*i == username)
            {
                condition = 1;
                break;
            }
        }
        if (!condition)
        {
            cout<<red << "Username does not exist .Please Enter valid Username." << endl;
            goto useragain;
        }
        else
        {
            cout<<dgray <<endl<< "Enter password." << endl;
        passagain:
            cin >> password;
            ll i = 0;
            while (i < user_pass.size() && user_pass[i].first != username)
            {
                ++i;
            }
            while (i < user_pass.size() && user_pass[i].second != password)
            {
                cout<<red << "Wrong password, Enter again."<<colorend << endl;
                goto passagain;
            }
        }
    }
    else
    {
        goto end;
    }
    writeuname(unames);
    if (user_login == 1)
    {
        cout <<green<<endl<<"Hey! "<< username << " , Welcome to TASK MANAGER... "<<colorend << endl;
        goto skip;
    }
    load_daily_task_file(username);
    display_incomplete_dailytask();
skip:
    loadDataFromFile(username);
    alaram(head_common, username); // function to remove tasks whose deadline is over
    reminder_x_hr(12);
    while (1)
    {
        cout <<dgray<< endl;
        cout<<yellow<<"MAIN MENU:"<<colorend<<endl;
        cout<<"Enter 1 to perform operations on daily tasks."<<endl;
        cout<<"Enter 2 to perform operations on common(Non-daily) tasks."<<endl;
        cout<<"Enter 3 to Exit"<<endl;
        cout<<endl;
        ll y;
        cin>>y;
        switch(y)
        {
            case 1:
            {
                while(1)
                {
                cout<<yellow<<endl<<"Daily-Task Menu:"<<colorend<<endl;
                cout<<dgray<<endl<<"Enter 1 to Insert a Daily task."<<endl;
                cout<<"Enter 2 to delete a Daily task."<<endl;
                cout<<"Enter 3 to delete all Daily tasks."<<endl;
                cout<<"Enter 4 to display all Daily tasks"<<endl;
                cout<<"Enter 5 to return to Main Menu."<<endl;
                cout<<endl;
                ll daily_counter;
                cin>>daily_counter;
                switch(daily_counter)
                {
                    case 1:
                    {
                        insert_daily_task();
                        break;  
                    }
                    case 2:
                    {
                        delete_daily_task();
                        //write_daily_task(username);
                        break;
                    }
                    case 3:
                    {
                        erase_all_daily_task(username);
                        break;
                    }
                    case 4:
                    {
                        display_daily();
                        break;
                    }
                    case 5:
                    {
                        goto task_switch_end;
                    }
                }
                }
            }
            case 2:
            {
                while(1)
                {
                cout<<yellow<<endl<<"Common-Task Menu:"<<colorend<<endl;
                cout<<dgray<<endl<<"Enter 1 to Insert a Task."<<endl;
                cout<<"Enter 2 to delete a Task."<<endl;
                cout<<"Enter 3 to delete all Common Tasks."<<endl;
                cout<<"Enter 4 to View all the Common Tasks."<<endl;
                cout<<"Enter 5 to Make some Edits in the Tasks."<<endl;
                cout<<"Enter 6 to return to Main Menu."<<endl;
                cout<<endl;
                ll common_counter;
                cin>>common_counter;
                switch(common_counter)
                {
                    case 1:
                    {
                        insert_task();
                        cout<<endl;
                        break;
                    }
                    case 2:
                    {
                        remove_tasks(username);
                        break;
                    }
                    case 3:
                    {
                        string fle = username + "task_data";
                        eraseFileContents(fle);
                        break;
                    }
                    case 4:
                    {
                        print_common_list(head_common);
                        break;
                    }
                    case 5:
                    {
                        while(1)
                        {
                        cout<<yellow<<"Task Editing Menu:"<<colorend<<endl;
                        cout<<dgray<<endl;
                        cout<<"Enter 1 to Reschedule a Task."<<endl;
                        cout<<"Enter 2 to Edit type, message, Importance level of a Task."<<endl;
                        cout<<"Enter 3 to return to Common Task Menu."<<endl;
                        cout<<endl;
                        ll edit_counter;
                        cin>>edit_counter;
                        switch(edit_counter)
                        {
                            case 1:
                            {
                                reschedule(username);
                                break;
                            }
                            case 2:
                            {
                                edit_task(username);
                                break;
                            }
                            case 3:
                            {
                                goto common_end;
                            }
                            default:
                            cout<<red<<endl << "Enter valid option."<<colorend << endl; 
                        }
                        }
                        common_end:
                        break;
                    }
                    case 6:
                    {
                        goto task_switch_end;
                        ll temporary_var1=0;//to avoid label to be the last line of the block
                    }
                    default:
                    cout<<red<<endl << "Enter valid option."<<colorend << endl;
                }
                }
            }
            case 3:
            {
                writeDataToFile(username);
                write_daily_task(username);
                cout<<yellow<<endl<<"Have a Nice Day !"<<colorend<<endl;
                goto end;
            }
            default:
            cout<<red<<endl << "Enter valid option."<<colorend << endl;
        }
        task_switch_end:
        ll temporary_var2=0;//to avoid label to be the last line of the block
    }
end:
    x = 0;
    return 0;
}