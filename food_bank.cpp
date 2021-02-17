/* CS 124-01. Final Group Project - Food Bank Program
 * Team Members: Truc Phan, Nishu Sharma, Nyan Lin Tun, Erick Tung
 */

//LIBRARIES
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <iomanip>
#include <queue>

//FOR FILE
using std::ifstream;
using std::ofstream;

//structure to hold food items and their quantity
struct food{
  std::string food_name;
  int quantity=0;
};
//structure to hold donor's name and amount of donation
struct donor{
    std::string f_name;
    std::string l_name;
    double donation=0;
};
//structure to hold information of a node in a linked list
struct node{
    donor data;
    node* next=NULL;
};

//FUNCTIONS
//main option 1
void adminMode(food [], node*&);
void print_donor_list();
int print_food_log(food []);
int linear_search(food [], int, std::string);
//main option 2
void userMode(std::string [], int, food []);
int subtract_food_log(food [], int);
void ask_what_city(std::string [], int);
// main option 3
void donorMode(node*&, node*&);
void add_in_order(node*&, node*&, donor);
void print_donor(node*);
std::string change_to_upper(std::string);

//begin main
int main()
{
    std::string lines; //for getline to count the size of each text file
    int bank_size=0, log_size=0, count=0, choice=0; //hold size of each text
                                                    //file and users' choice
    char main_menu_option;
    ifstream food_bankfile, food_logfile; //declare file name
    //open the file of food banks
    food_bankfile.open("food_banks.txt");
    //check for failure to open file
    if(food_bankfile.fail()) {
        std::cout << "Error opening file: food_banks.txt\n";
        return -1;
    }
    //Now the file is open, read from the file line by line to get how many lines are in file
    while(!food_bankfile.eof())
    {
        getline(food_bankfile, lines);
        bank_size ++; //increment size of file by 1
    }
    food_bankfile.close();
    
    //create a dynamically allocated array called food_banks_array
    std::string* food_banks_array = new std::string[bank_size];
    
    //now read from food_banks file and store the food banks in array
    food_bankfile.open("food_banks.txt");
    //check for failure to open file
    if(food_bankfile.fail()) {
        std::cout << "Error opening file: food_banks.txt\n";
        return -1;
    }
    while (getline(food_bankfile, lines))
    {
        //Store the word in array
        food_banks_array[count] = lines;
        //Increment count so next word gets stored in next array spot
        count++;
    }
    food_bankfile.close();
    
    //open the file of food log
    food_logfile.open("food_log.txt");
    //check for failure to open file
    if(food_logfile.fail()) {
        std::cout << "Error opening file: food_log.txt\n";
        return -1;
    }
    //Now the file is open, read from the file line by line to get how many lines are in file
    while(!food_logfile.eof())
    {
        getline(food_logfile, lines);
        log_size ++; //increment size of file by 1
    }
    food_logfile.close();
    
    //create a dynamically allocated array called food_log_array
    food* food_log_array = new food[log_size];
    
    //create a linked list for donor mode
    node* hp=NULL, *tp=NULL;
    
    //begin main menu using a do-while loop
    do
    {
        //ask user for their choice
        std::cout << "Welcome! Please select a mode:\n"
        << "1. Admin Mode\n" << "2. User Mode\n" << "3. Donor Mode\n";
        std::cin >> choice;
        //DATA VALIDATION
        while (std::cin.fail() || (choice!=1 && choice!=2 && choice!=3))
        {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            //Tell user to pick a valid choice
            std::cout << "Invalid input. Please enter again:\n";
            std::cin >> choice;
        }
        //ADMIN MODE
        if (choice==1) {adminMode(food_log_array, hp);}
        //USER MODE
        else if (choice==2) {userMode(food_banks_array, bank_size, food_log_array);}
        //DONOR MODE
        else if (choice==3) {donorMode(hp, tp);}
        //ask user if they wanna go back to main prompt
        std::cout << "Do you want to go back to main menu? [Y or N] " << std::endl;
        std::cin >> main_menu_option;
        //uppercase user choice
        main_menu_option = toupper(main_menu_option);
        //DATA VALIDATION
        while (main_menu_option != 'Y' && main_menu_option != 'N')
        {
            //tell user to only pick given choice
            std::cout << "Invalid input. Please enter again:\n";
            std::cout << "Do you want to go back to main menu? [Y or N] " << std::endl;
            std::cin >> main_menu_option;
        }
        //say goodbye to user
        if (main_menu_option == 'N')
        {std::cout << "Thank you. Have a good one!" << std::endl;}
    } while (main_menu_option != 'N');
    
    return 0;
    //delete dynamically allocated arrays when program is finished
    delete food_banks_array, food_log_array;
} //end main

/* ADMIN MODE: Allow user to open food log, update food log, and open donor list
 * @param - food a[]: array holding all the food items and their quantity,
 *          node*& h: head pointer of the linked list of donors
 * @return - none
 */
void adminMode(food a[], node*& h)
{
    int c1=0, c2=0, size=0; //hold user's choices and size of food_log_array
    std::string pw; //hold admin's password
    ofstream food_outfile; //for printing updated data into text file
    
    //allow user to go back to main menu if they are not the admin
    std::cout << "Hello! You are in Admin Mode. Please press 1"
    " to enter your password, or 2 to return to main menu.\n";
    std::cin >> c1;
    //check for invalid input;
    while (std::cin.fail() || (c1!=1 && c1!=2)) {
        std::cin.clear();
        std::cin.ignore(1000,'\n');
        std::cout << "Invalid input. Please enter again:\n";
        std::cin >> c1;
    }
    //enter admin mode
    if (c1==1) {
        std::cout << "Please enter password:\n";
        std::cin >> pw;
        //check for wrong password
        while (pw!="0000#") {
            std::cout << "Wrong password. Please enter again:\n";
            std::cin >> pw;
        }
        bool check=true;
        while (check) {
            //printing options for admin
            std::cout << "Admin Mode. Please choose one option:\n"
            << "1. Open food log.\n" << "2. Update food quantity.\n" <<
            "3. Open list of donors.\n" << "4. Quit Admin Mode.\n";
            std::cin >> c2;
            //check for invalid input
            while (std::cin.fail() || (c2!=1 && c2!=2 && c2!=3 && c2!=4)) {
                std::cin.clear();
                std::cin.ignore(1000,'\n');
                std::cout << "Invalid input. Please enter again:\n";
                std::cin >> c2;
            }
            //see list of food log
            if (c2==1) {
                std::cout << std::endl;
                size = print_food_log(a); // call print_food_log function
                check=true; //return to admin menu
            }
            //update food items
            else if (c2==2) {
                std::string update; //hold food item's name
                int pos=0; //hold position of the food item to be updated in food_log_array
                
                //print menu for admin to review, also get the size of food_log_array
                std::cout << "\nReview menu:\n";
                size = print_food_log(a);
                //ask the admin to type in the name of the food to be updated
                std::cout << "\nWhich item do you want to update?\n";
                std::cin >> update;
                //find the position of the food item in food_log_array using linear search
                pos = linear_search(a, size, update);
            
                //if the string input is invalid (not in food_log_array)
                while (pos==-1) {
                    std::cout << "Item not found\n";
                    std::cout << "Please enter again:\n";
                    std::cin >> update;
                    pos = linear_search(a, size, update);
                }
                //ask the admin to enter new quantity of the food item
                std::cout << "\nPlease enter new value:\n";
                std::cin >> a[pos].quantity;
            
                //open food_log.txt to outfile
                food_outfile.open("food_log.txt");
                //print updated list for admin
                std::cout << "\nNew list is:\n";
                for (int i=0; i<size; i++) {
                    //print on console
                    std::cout << a[i].food_name << " " << a[i].quantity << std::endl;
                    //update food_log.txt
                    food_outfile << a[i].food_name << " " << a[i].quantity << std::endl;
                }
                check=true; //return to admin menu
            }
            //call print donor function
            else if (c2==3) {
                std::cout << std::endl;
                print_donor(h); //call print_donor function
                check=true; //return to admin menu
            }
            else {check=false;} //return to main menu
        }
    }
    else {return;}
}
/* Open and print every line of the food log text file
 * @param - food a[]: array holding all the food items and their quantity
 * @return - int i: size of the food log text file
 */
int print_food_log(food a[])
{
    ifstream food_infile; //for reading the text file
    int i=0; //counter of lines/items in the text file
    
    food_infile.open("food_log.txt");
    //test for files that can't be opened
    if(food_infile.fail()) {
        std::cout << "Error opening food_log.txt\n";
    }
    //input data line by line using a while loop
    else {
        while (food_infile >> a[i].food_name >> a[i].quantity) {
            i++; //increment counter
        }
        food_infile.close(); //close file
    }
    //traverse through the file and print all food items
    for (int j=0; j<i; j++) {
       std::cout << a[j].food_name << " " << a[j].quantity << std::endl;
    }
    return i; //return counter/number of food items
}
/* Linear search - traverse through an array and find the matching data
 * @param - food a[]: array holding all the food items and their quantity,
 *          int size: size of array a,
 *          std::string value: data to be found in array a
 * @return - int i: the index of matching data in array a
 */
int linear_search(food a[], int size, std::string value)
{
    for (int i=0; i<size; i++) {
        //if found, return index
        if (a[i].food_name == value) {return i;}
    }
    return -1; //otherwise, return -1
}

/* USER MODE: Allow users to choose food based on categories, within limited amount
 * @param - std::string food_bank_array[]: array holding all the food banks' information
 *                                         (name, address, phone #),
 *          int size: size of array food_bank_array,
            food a[]: array holding all the food items and their quantity
 * @return - none
 */
void userMode(std::string food_bank_array[], int size, food a[])
{
    int name=0, num=0; //hold food item and their quantity to be taken
    int choice=2, len=0; //hold user's choice and size of food_log_array
    ofstream food_outfile; //for updating food_log_array
    std::string line; //to create user's choice by priority
    std::priority_queue<std::string> s; //hold all user's food choices
    
    while(choice==2) {
        //call ask_what_city function
        ask_what_city(food_bank_array, size);
        
        //inform user how many items in each category they can choose
        std::cout << std::endl;
        std::cout <<"Hello! You can pick:\n"<<"1 kind of Meat\n"<<"2 kinds of Fruit\n"
        <<"2 kinds of Vegetables\n"<<"2 kinds of Grain\n"<<"1 kind of Dairy\n"
        <<"1 kind of Snack\n";
        //print menu for user to review
        std::cout << "\nHere is the Menu:\n";
        len=print_food_log(a); //get the size of food_log_array
    
        //the following lines allow user to choose food items based on 6 categories
        //the algorithm is similar for each item so we are going to explain the first
        //one only
        
        //1. Meat
        std::cout<<"\nPlease choose your meat (by pounds)\n";
        for (int x=0;x<=3; x++)
        {std::cout<<x+1<<"."<<a[x].food_name<<" "<<a[x].quantity<<std::endl;}
        std::cin>>name;
        //check for invalid input
        while (std::cin.fail() || (name!=1 && name!=2 && name!=3 && name!=4))
        {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Invalid input. Please enter again:\n";
            std::cin >> name;
        }
        name--; //update the user's input to the "correct" index in food_log_array
        num=subtract_food_log(a,name); //get the amount the user wants
        a[name].quantity-=num; //update food quantity in food_log_array
        //create a string of user's food choice and its amount to be sorted by priority
        line = std::to_string(num) + " - " + a[name].food_name;
        s.push(line); //push the string onto the priority queue

        //2. Fruit
        std::cout<< "\nPlease choose your first fruit (by units)\n";
        for (int x=4;x<=7; x++)
        {std::cout<<x-3<<"."<<a[x].food_name<<" "<<a[x].quantity<<std::endl;}
        std::cin>>name;
        while (std::cin.fail() || (name!=1 && name!=2 && name!=3 && name!=4))
        {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Invalid input. Please enter again:\n";
            std::cin >> name;
        }
        name+=3;
        num=subtract_food_log(a,name);
        a[name].quantity-=num;
        line = std::to_string(num) + " - " + a[name].food_name;
        s.push(line);
    
        std::cout<<"\nPlease choose your second fruit (by units)\n";
        for (int x=4;x<=7; x++)
        {std::cout<<x-3<<"."<<a[x].food_name<<" "<<a[x].quantity<<std::endl;}
        std::cin>>name;
        while (std::cin.fail() || (name!=1 && name!=2 && name!=3 && name!=4))
        {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Invalid input. Please enter again:\n";
            std::cin >> name;
        }
        name+=3;
        num=subtract_food_log(a,name);
        a[name].quantity-=num;
        line = std::to_string(num) + " - " + a[name].food_name;
        s.push(line);
    
        //3. Vegetables
        std::cout<< "\nPlease choose your first vegetable (by pounds)\n";
        for (int x=8;x<=11; x++)
        {std::cout<<x-7<<"."<<a[x].food_name<<" "<<a[x].quantity<<std::endl;}
        std::cin>>name;
        while (std::cin.fail() || (name!=1 && name!=2 && name!=3 && name!=4))
        {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Invalid input. Please enter again:\n";
            std::cin >> name;
        }
        name+=7;
        num=subtract_food_log(a,name);
        a[name].quantity-=num;
        line = std::to_string(num) + " - " + a[name].food_name;
        s.push(line);
       
        std::cout<<"\nPlease choose your second vegetable (by pounds)\n";
        for (int x=8;x<=11; x++)
        {std::cout<<x-7<<"."<<a[x].food_name<<" "<<a[x].quantity<<std::endl;}
        std::cin>>name;
        while (std::cin.fail() || (name!=1 && name!=2 && name!=3 && name!=4))
        {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Invalid input. Please enter again:\n";
            std::cin >> name;
        }
        name+=7;
        num=subtract_food_log(a,name);
        a[name].quantity-=num;
        line = std::to_string(num) + " - " + a[name].food_name;
        s.push(line);
        
        //4. Grain
        std::cout<< "\nPlease choose your first Grain (by pounds)\n";
        for (int x=12;x<=15; x++ )
        {std::cout<<x-11<<"."<<a[x].food_name<<" "<<a[x].quantity<<std::endl;}
        std::cin >> name;
        while (std::cin.fail() || (name!=1 && name!=2 && name!=3 && name!=4))
        {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Invalid input. Please enter again:\n";
            std::cin >> name;
        }
        name+=11;
        num=subtract_food_log(a,name);
        a[name].quantity-=num;
        line = std::to_string(num) + " - " + a[name].food_name;
        s.push(line);
               
        std::cout<<"\nPlease choose your second Grain (by pounds)\n";
        for (int x=12;x<=15; x++)
        {std::cout<<x-11<<"."<<a[x].food_name<<" "<<a[x].quantity<<std::endl;}
        std::cin >> name;
        while (std::cin.fail() || (name!=1 && name!=2 && name!=3 && name!=4))
        {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Invalid input. Please enter again:\n";
            std::cin >> name;
        }
        name+=11;
        num=subtract_food_log(a,name);
        a[name].quantity-=num;
        line = std::to_string(num) + " - " + a[name].food_name;
        s.push(line);
    
        //5. Dairy
        std::cout<< "\nPlease choose your Dairy (by units)\n";
        for (int x=16;x<=19; x++ )
        {std::cout<<x-15<<"."<<a[x].food_name<<" "<<a[x].quantity<<std::endl;}
        std::cin >> name;
        while (std::cin.fail() || (name!=1 && name!=2 && name!=3 && name!=4))
        {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Invalid input. Please enter again:\n";
            std::cin >> name;
        }
        name+=15;
        num=subtract_food_log(a,name);
        a[name].quantity-=num;
        line = std::to_string(num) + " - " + a[name].food_name;
        s.push(line);
    
        //6. Snack
        std::cout<< "\nPlease choose your snack (by units)"<<std::endl;
        for (int x=20;x<=23; x++ )
        {std::cout<<x-19<<"."<<a[x].food_name<<" "<<a[x].quantity<<std::endl;}
        std::cin >> name;
        while (std::cin.fail() || (name!=1 && name!=2 && name!=3 && name!=4))
        {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Invalid input. Please enter again:\n";
            std::cin >> name;
        }
        name+=19;
        num=subtract_food_log(a,name);
        a[name].quantity-=num;
        line = std::to_string(num) + " - " + a[name].food_name;
        s.push(line);
        
        //open food_log.txt to update food quantities
        food_outfile.open("food_log.txt");
        for (int i=0; i<len; i++) {food_outfile<<a[i].food_name<<" "<<a[i].quantity<<std::endl;}
        
        //allow user to review what they have chosen by popping out the priority queue
        //sorted by largest amount to smallest amount of each food item
        std::cout << "Here is what you chose:\n";
        while(s.size()>0) {
            std::cout<<s.top()<<std::endl;
            s.pop();
        }
        
        //allow user to choose again if they are not happy with what they chose
        std::cout<<"Press 1 to confirm your choice, press 2 to start over:\n";
        std::cin>>choice;
        while (std::cin.fail() || choice!=1 && choice!=2) {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout<<"Inavlid input. Please enter again:\n";
            std::cin>>choice;
        }
    }
}
/* Update food items' quantity after the users choose their options
 * @param - food a[]: array holding all the food items and their quantity,
            int x: index of the chosen food in array a
 * @return - int y: the amount of each food item the users choose
 */
int subtract_food_log(food a[], int x)
{
    int y; //quantity of food the user wants
    
    std::cout<<"Please enter the amount of "<<a[x].food_name
    <<" you want to take (Limit 5 for each category)"<<std::endl;
    std::cin>>y;
    //check for invalid input and input exceeding allowed amount
    while (std::cin.fail() || y>5 || y<=0) {
        std::cin.clear();
        std::cin.ignore(1000,'\n');
        std::cout << "Invalid input. Please enter again:\n";
        std::cin >> y;
    }
    //check if user wants more than what is left in stock and ask them to enter again
    while(y>a[x].quantity) {
        std::cout<<"Sorry! We currently do not have enough.\n"
        <<"We only have "<<a[x].quantity<<" left. Please enter the amount again.\n";
        std::cin>>y;
    }
    return y; //return quantity
}
/* Find the food banks closest to the user's address
 * @param - std::string food_bank_array[]: array holding all the food banks' information
 *                                         (name, address, phone #),
 *          int size: size of food_bank_array
 * @return - none
 */
void ask_what_city(std::string food_bank_array[], int size)
{
    int choice;
    //ask what city user wants address of
    std::cout << "What city do you live in?\n"
    << "1. Fremont\n" << "2. Hayward\n" << "3. Newark\n";
    std::cin >> choice;
    //create map to hold cities
    std::map<int,std::string> city;
    //give value
    city[1] = "Fremont";
    city[2] = "Hayward";
    city[3] = "Newark";
    //DATA VALIDATION
    while (std::cin.fail() || (choice!=1 && choice!=2 && choice!=3))
    {
        std::cin.clear();
        std::cin.ignore(1000,'\n');
        //Tell user to pick a valid choice
        std::cout << "Invalid input. Please enter again:\n";
        std::cin >> choice;
    }
    std::cout << "The food banks in your city are\n";
    std::cout << std::endl;
    //go through array and only print the info with the city user lives in
    for (int i = 0; i < size; i++)
    {
        int found = food_bank_array[i].find(city[choice]);
        if(found!=std::string::npos)
        {
            //city is found, print the info
            std::cout << food_bank_array[i] << std::endl;
        }
    }
}
/* DONOR MODE: Allow user to type in first and last name, along with donation
 * @param - node*& h: head pointer of the linked list of donors,
 *          node*& t: tail pointer of the linked list of donors
 * @return - none
 */
void donorMode(node*& h, node*& t)
{
    donor person; //hold donor's first, last names and donation
    ofstream outfile; //for updating the donors.txt
    
    //create a file to store the list of donor names with donations
    //appending rather than rewriting
    outfile.open("donors.txt", ofstream::app);
    
    //Enter donor name
    std::cout<<"Please enter your first name: ";
    std::cin.ignore();
    getline(std::cin, person.f_name);
    //change to upppercase for accurate comparison
    person.f_name = change_to_upper(person.f_name);
    std::cout<<"Please enter your last name: ";
    getline(std::cin, person.l_name);
    //change to upppercase for accurate comparison
    person.l_name = change_to_upper(person.l_name);
    
    //Enter amount of donation
    std::cout<<"How much would you like to donate?\n";
    std::cin>>person.donation;
    //call add_in_order for linked list
    add_in_order(h, t, person);
    
    std::cout<<"Thank you "<<person.f_name<<" "<<person.l_name<<" for the donation!\n";

    //outfile the data to the text file
    outfile<<person.f_name<<" "<<person.l_name<<
    std::right<<std::setw(20)<<"$"<<person.donation<<"\n";
    outfile.close();
}
/* Add new node into linked list so that data is sorted at the same time
 * Function is inspired by Jay's add_in_order function posted on Piazza
 * @param - node*& h: head pointer of the linked list of donors,
 *          node*& t: tail pointer of the linked list of donors,
 *          donor d: data of type donor to be added into the linked list
 * @return - none
 */
void add_in_order(node*& h, node*& t, donor d)
{
    node* newnode = new node; //create new node to linked list
    newnode->data = d; //set parsed data equal to new node
    node* prev; //previous pointer
    node* curr = h; //current pointer is head
    //if list is empty
    if(h == NULL)
    {
        h = newnode;
        t = newnode;
        return;
    }
    //if data of new node is greater than data of last node, add at end
    if(newnode->data.l_name > t->data.l_name)
    {
        t->next = newnode;
        t = newnode;
        return;
    }
    //move current and previous pointers
    while (newnode->data.l_name > curr->data.l_name && curr!= NULL)
    {
        prev = curr;
        curr = curr->next;
    }
    //if data of new node is the smallest, add at start
    if(curr == h)
    {
        newnode->next = h;
        h = newnode;
    }
    //if data of new node is in the middle
    else
    {
        prev->next = newnode;
        newnode->next = curr;
    }
}
/* Traverse through the linked list and print all donors
 * @param - node* head: head pointer of the linked list of donors
 * @return - none
 */
void print_donor(node* head)
{
    node* temp = head; //create a temporary node to traverse through the list
    double total=0; //store total amount of donation
    
    std::cout<<"List of Donors Sorted by Last Names:\n\n";
    std::cout<<std::setw(11)<<"First name"<<std::setw(20)<<
    "Last name"<<std::right<<std::setw(24)<<"Amount\n";
    for(temp; temp!=NULL; temp=temp->next)
    {
        total+=temp->data.donation; //calculate total donation
        
        std::cout<<std::right<<std::setw(10)<<temp->data.f_name<<
        std::right<<std::setw(20)<<temp->data.l_name<<std::right<<
        std::setw(20)<<"$"<<temp->data.donation<<std::endl;
    }
    //print total amount of donation at the end
    std::cout<<"\nTOTAL:"<<std::setw(44)<<"$"<<total<<std::endl<<std::endl;
}
/* Change all characters in a string to uppercase for accurate comparison
 * @param - std::string w: a string to be altered
 * @return - std::string w: the string after being changed to all uppercase
 */
std::string change_to_upper(std::string w)
{
    for (int a=0; a<w.size(); a++)
    {
        w[a] = std::toupper(w[a]);
    }
    return w;
}




