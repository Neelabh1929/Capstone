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
#define ll long long
using namespace std;

// This set will store the type of the tasks already stored
set<string> type_task;

// Linked List implementation for the managing the task
// Defination of the Node for the common list, specific type list

class Node
{
    string time, type, message;
    ll imp_lvl;

    // next and prev pointers for traversing and other things
    Node *next;
    Node *prev;

    // Constructor for the node
public:
    Node(string t, string type1, string message1, ll i)
    {
        time = t; // time means deadline of the task
        type = type1;
        message = message1;
        imp_lvl = i;
        next = NULL;
        prev = NULL;
    }
    Node(string t, string type1, string message1)
    {
        time = t;
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
    friend void insert_task_common(string, string, string, ll);
    friend void createlist();
    friend int priority(Node *, Node *);
    friend void print_common_list();
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

// defination of the priority function
int priority(Node *temp, Node *temp_insert)
{
    int day1, month1, year1, day2, month2, year2;

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

    // If dates are equal then compare importance levels   ***still to update for more than 2
    if (temp->imp_lvl > temp_insert->imp_lvl)
        return 1;
    else
        return 0;

    // Add the comparision of time at particular day as well for comparision like for hour ,minutes.
}

/******************* Priority function block over ********************/

void insert_task_common(string deadline, string type_by_user, string message, ll imp_lvl_by_user)
{

    Node *temp_insert = new Node(deadline, type_by_user, message, imp_lvl_by_user);

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

    string deadline, message;
    cout << "Enter the Deadline of the task:" << endl;
    cin.ignore(); // to ignore the newline character from the buffer
    getline(cin, deadline);

    cout << "Enter the message you want with the remainder:" << endl;
    cin.ignore(); // to ignore the newline character from the buffer
    getline(cin, message);

    transform(task_type_user.begin(), task_type_user.end(), task_type_user.begin(), ::tolower);

    // To avoid duplications caused by case differences
    type_task.insert(task_type_user);

    insert_task_common(deadline, task_type_user, message, imp_lvl_user);

    /* checkig wether the the type entered by the user is Currently Present or not. If not then we will create a new head node with the particular type */

    /*

    No need of hthis section now as we will use set to stor types of task to avoid duplications


     bool type_present=0;
      for(set<string>::iterator it=type_task.begin();it!=type_task.end();it++)
      {
          if(*it==task_type_user)
          {
              type_present=1;
              break;
          }
      }
      if(type_present==0)
      {
          Node* head=new Node(task_type_user,imp_lvl_user);
      }
      else
      {
          Node* temp=new Node(deadline,task_type_user,message,imp_lvl_user);
      } */

    // Right now the work of how to get the address of the header node of  particular type is remaining. Once it is figured out , all we need to do is to traverse through the list and insert the node at the particular location as per the deadline
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
            cout << "Deadline: " << temp->time << endl;
            cout << "Message: " << temp->message << endl;
            cout << "Importance level: " << temp->imp_lvl << endl;
            temp = temp->next;
            cout << endl;
        }

        cout << "List over" << endl;
    }
    else
    {
        cout << "NO task in the list" << endl;
    }
}

int main()
{
    ll x = 1;
    while (1)
    {
        cout << endl;
        cout << "Enter 1 to insert a task." << endl;
        cout << "Enter 2 to view the whole list of task." << endl;
        cout << "Enter 3 to exit." << endl;
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
            goto end;
            break;
        }
        }
    }
end:
    x = 0;
    return 0;
}