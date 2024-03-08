//tasks to be done:
/* 
1) Make the function priority(). This functions returns where the node needs to be inserted 
2) Right now the work of how to get the address of the header node of  particular type is remaining. Once it is figured out , all we need to do is to traverse through the list and insert the node at the particular location as per the deadline
*/

#include<iostream>
#include <sstream>
#include<vector>
#include<ctime>
#include<string.h>
#include<string>

#define ll long long
using namespace std;

//This vector will store the type of the tasks already stored 
vector<string>task_type;

//Linked List implementation for the managing the task
//Defination of the Node for the common list, specific type list

class Node
{
    string time,type,message;
    ll imp_lvl;

    //next and prev pointers for traversing and other things
    Node * next;
    Node * prev;

    //Constructor for the node
    public:
    Node(string t,string type1,string message1,ll i)
    {
        time=t;
        type=type1;
        message=message1;
        imp_lvl=i;
        next=NULL;
        prev=NULL;
    }
    Node(string t,string type1,string message1)
    {
        time=t;
        type=type1;
        message=message1;
        //imp_lvl=0 is considered as the most common task
        imp_lvl=0;
        next=NULL;
        prev=NULL;
    }
    Node(string type1,ll i)
    {
        time="";
        type=type1;
        message="";
        imp_lvl=i;
        next=NULL;
        prev=NULL;
    }
    Node(string type1)
    {
        type=type1;
        next=NULL;
        prev=NULL;
    }
    friend void insert_task_common(string ,string ,string , ll);
    friend void createlist();   
    friend int priority(Node*,Node*);  
};


Node* head_common=new Node("Common task list");


//defination of the priority function
int priority(Node*temp,Node*temp_insert)
{
   // comparing the eadlineslines
    if (temp->time > temp_insert->time)
        return 1;
    else if (temp->time < temp_insert->time)
        return 0;

    // deadlines are equal isliye compare importance levels
    if (temp->imp_lvl < temp_insert->imp_lvl)
        return 1;
    else
        return 0;

   
}



void insert_task_common(string deadline,string type_by_user,string message,ll imp_lvl_by_user)
{
    // Node* temp_insert=new Node(deadline,type_by_user,message,imp_lvl_by_user);
    // Node* temp=head_common;
    // while(temp->next!=NULL || priority(temp,temp_insert)==1)
    // {
    //     temp=temp->next;
    // }
    // //change the Next and prev pointers to insert the node in the list
    // temp_insert->prev=temp;
    // temp_insert->next=temp->next;
    // temp->next=temp_insert;
    // temp_insert->next->prev=temp_insert;
    Node* temp_insert = new Node(deadline, type_by_user, message, imp_lvl_by_user);
    Node* temp = head_common;
    while (temp->next != NULL && priority(temp, temp_insert) == 1)
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
    cout<<"Enter the type of task:"<<endl;
    string task_type_user;
    getline(cin,task_type_user);
    cout<<"Enter the Integral Importance level of this task:"<<endl;
    ll imp_lvl_user;
    cin>>imp_lvl_user;
    string deadline,message;
    cout<<"Enter the Deadline of the task:"<<endl;
    getline(cin,deadline);
    cout<<"Enter the message you want with the remainder:"<<endl;
    getline(cin,message);
    insert_task_common(deadline,task_type_user,message,imp_lvl_user);
    /* checkig wether the the type entered by the user is Currently Present or not. If not then we will create a new head node with the particular type */

    bool type_present=0;
    for(ll i=0;i<task_type.size();i++)
    {
        if(task_type[i]==task_type_user)
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
    }

    //Right now the work of how to get the address of the header node of  particular type is remaining. Once it is figured out , all we need to do is to traverse through the list and insert the node at the particular location as per the deadline


}
int main()
{
    return 0;
}