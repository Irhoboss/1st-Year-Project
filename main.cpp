#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <map>
using namespace std;

int task_to_do , concert_to_attend, section_to_avail, mode_of_payment, again, back;
void Phase2(int task), Phase();

struct TicketNode {
    string name;
    int age;
    string contact_number;
    string section;
    string concert;
    int seat_number;
    float price;
    string payment_method;
    string transaction_id;
    string serial_number;

    TicketNode* next;
};

struct seat{
    bool is_taken = false;
};

struct section{
    bool is_full = false;
    string section_name;
    vector <seat> section_for_seat; 
    int price;
};

struct concert{
    string concert_name;
    string date;
    bool is_done = false;
    vector <section> concert_for_section;
    TicketNode* front = nullptr;
    TicketNode* rear = nullptr;
};

vector <concert> all_queues;

class concert_variable {
public:
    vector <string> dateforconcert = {
        "May 3, 2025 07:30 PM",
        "May 17, 2025 08:00 PM",
        "May 24, 2025 06:45 PM",
        "June 1, 2025 07:00 PM",
        "June 8, 2025 08:15 PM",
        "June 15, 2025 07:30 PM",
        "June 22, 2025 09:00 PM",
        "July 5, 2025 06:30 PM",
        "July 12, 2025 08:00 PM",
        "July 19, 2025 07:00 PM"
    };

    vector<string> ModeofPayment { "GCASH", "PAYMAYA" };

    vector <string> choices ={"Yes", "No"};

    vector<string> TicketChoices { "Buy Ticket", "Ticket Sold", "Cancel Ticket", "Exit" };

    vector<string> concert = {
        "SB19", "BINI", "ALAMAT", "BGYO", "G22", "VXON", "KAIA", "1ST.ONE", "DIONE", "Press Hit Play"
    };

    vector<string> section = {
        "VIP STANDING", "PATRON A9", "PATRON B7", "LOWER BOX5", "UPPER BOX2", "GEN AD"
    };

    vector <vector<int>> seats_per_section = {
        {2, 2, 2, 2, 2, 2}, {1200, 800, 1500, 2000, 2500, 6000},
        {1000, 700, 1200, 1800, 2200, 5000}, {1800, 1500, 2200, 3000, 3500, 9000},
        {1300, 1000, 1700, 2400, 2800, 7500}, {1400, 1100, 1900, 2300, 2900, 7800},
        {1100, 800, 1400, 2000, 2500, 6500}, {900, 600, 1000, 1500, 1800, 4500},
        {1200, 950, 1400, 1800, 2200, 6200}, {1500, 1200, 1900, 2300, 2800, 7600}
    };

    vector<vector<int>> price_per_section = {
        {1500, 1200, 1800, 2500, 3000, 8000},
        {1200, 800, 1500, 2000, 2500, 6000},
        {1000, 700, 1200, 1800, 2200, 5000},
        {1800, 1500, 2200, 3000, 3500, 9000},
        {1300, 1000, 1700, 2400, 2800, 7500},
        {1400, 1100, 1900, 2300, 2900, 7800},
        {1100, 800, 1400, 2000, 2500, 6500},
        {900, 600, 1000, 1500, 1800, 4500},
        {1200, 950, 1400, 1800, 2200, 6200},
        {1500, 1200, 1900, 2300, 2800, 7600}
    };
};

void organizecss(concert_variable& v ){
    for (int i = 0; i < (int)v.concert.size(); i++ ){
        concert c;
        c.concert_name = v.concert[i];
        c.date = v.dateforconcert[i];
        
        for (int a = 0; a < (int)v.section.size(); a++){
            section s;
            s.section_name = v.section[a];
            s.price = v.price_per_section[i][a];
            
            for (int j = 0; j < v.seats_per_section[i][a]; j++){
                s.section_for_seat.push_back(seat());
            }
            
            c.concert_for_section.push_back(s);
        }
        all_queues.push_back(c);
    }
}

bool check_age(int age){
    return age >= 18;
}

bool check_contact(const string& number) {
    if (number.length() != 11 ) return false;
    if (number.substr(0, 2) != "09") return false;
    for (char c : number){
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isConcertDateValid(const string& concert_date) {
    struct tm tm = {};
    istringstream ss(concert_date);
    ss >> get_time(&tm, "%b %d, %Y  %I:%M  %p");
    if (ss.fail()) {
        return false;
    }
    time_t concert_time = mktime(&tm);
    time_t current_time = time(0); 
    return difftime(concert_time, current_time) > 0;
}

string generate_serial(const string& concert_name) {
    static map<string, int> generateSerial;
    time_t m  = time(0);
    tm* current_time = localtime(&m);
    string ticketserial = concert_name ;
    generateSerial[ticketserial]++;
    stringstream ss;
    ss << put_time(current_time, "%Y%m%d")  << "-"<< setw(6) << setfill('0')  << generateSerial[ticketserial] ;
    ticketserial += ss.str();
    return ticketserial;
}

string generate_transaction_id(const string& concert_name, const string& section_name) {
    static map<string, int> ticketCountMap;  
    string ticketid = concert_name + "-" + section_name;
    ticketCountMap[ticketid]++;
    stringstream ss;
    ss << setw(6) << setfill('0') << ticketCountMap[ticketid];
    ticketid += ss.str();
    return ticketid;
}

void enqueue(concert& con, TicketNode* ticket, int section_index) {
    concert_variable v;
    section& sec = con.concert_for_section[section_index];
    int queue_count = 0;
    TicketNode* curr = con.front;
    while (curr) {
        if (curr->section == sec.section_name)
            queue_count++;
        curr = curr->next;
    }
    if (queue_count >= (int)sec.section_for_seat.size()) {
        sec.is_full = true;
        cout << "\n\t\t\t\033[31mThis section is already FULL (queue).\033[0m\n";
        delete ticket;
        for (int i = 0; i < (int)v.choices.size(); i++){
            cout <<"\n\t\t\t" << "[" << i + 1 << "] " << v.choices[i]<< "\n";
        }
        cout << "\n\t\t\t" << "Bot: Would you like to go back : ";
        cin >> back;
        if (back == 1 ){
            Phase();
        } else {
            cout << "\n\n\n\t\t\t\t\033[32mBot: Thank you !\033[0m";
        }
        return;
    }
    if (con.front == nullptr) {
        con.front = con.rear = ticket;
        cout << "\n\n\n\t\t\t\t\033[32mBot: You've been succesfully book a ticket!\033[0m";
        cout << "\n\t\t\tTICKET INFORMATION\n";
    } else {
        con.rear->next = ticket;
        con.rear = ticket;
        cout << "\n\n\n\t\t\t\t\033[32mBot: You've been succesfully book a ticket!\033[0m";
        cout << "\n\t\t\tTICKET INFORMATION\n";
    }
    ticket->next = nullptr;
}

void Cancel_Ticket() {
    concert_variable v;
    string serial_input;
    cout << "\n\n\n\t\t\tEnter the serial number of the ticket to cancel: ";
    cin.ignore();
    getline(cin, serial_input);

    bool found = false;

    
    for (concert& c : all_queues) {
        TicketNode* curr = c.front;
        TicketNode* prev = nullptr;

        while (curr != nullptr) {
            if (curr->serial_number == serial_input) {
                
                found = true;
                
                int sec_idx = -1; 
                for (int i = 0; i < (int)c.concert_for_section.size(); i++) {
                    if (c.concert_for_section[i].section_name == curr->section) {
                        sec_idx = i;
                        break;
                    }
                }
                if (sec_idx != -1) {
                    section& sec = c.concert_for_section[sec_idx];
                    
                    int seat_idx = curr->seat_number - 1;
                    if (seat_idx >=0 && seat_idx < (int)sec.section_for_seat.size()) {
                        sec.section_for_seat[seat_idx].is_taken = false;
                        sec.is_full = false; 
                    }
                }

                if (prev == nullptr) {
                    c.front = curr->next;
                } else {
                    prev->next = curr->next;
                }
                if (curr == c.rear) {
                    c.rear = prev;
                }

                delete curr;
                cout << "\n\t\t\t\033[32mTicket cancelled successfully!\033[0m\n";
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    if (!found) {
        cout << "\n\t\t\t\033[31mTicket with serial number " << serial_input << " not found.\033[0m\n";
    }
}

void Ticket_Buy(){
    concert_variable v;
    TicketNode* newTicketNode = new TicketNode;

    cout << "\n\n\n\t\t\t\tTICKET TO AVAIL\n";
    for (int i = 0; i < (int)v.concert.size(); i++) {
        bool done = !isConcertDateValid(v.dateforconcert[i]);
        const concert& c = all_queues[i];
        int total_available = 0;
        for (const section& sec : c.concert_for_section) {
            for (const seat& s : sec.section_for_seat) {
                if (!s.is_taken)
                    total_available++;
            }
        }
        cout << "\n\t\t\t" << "[" << i + 1 << "] " << v.concert[i] << " (" << v.dateforconcert[i] << ")";
        if (done)
            cout << " (Finished)";
        else if (total_available == 0)
            cout << " (Sold Out)";
        else
            cout << " (Available Seats)";
    }

    do {
        cout << "\n\t\t\tBot: what concert you want to attend ?  ";
        cin >> concert_to_attend;

        if (cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n\t\t\t\033[31mInvalid input. Please enter a number.\033[0m\n"; 
            continue;
        }
        if (concert_to_attend < 1 || concert_to_attend > (int)v.concert.size()) {
            cout << "\t\t\t\033[31mInvalid selection, please choose a valid concert.\033[0m\n";
            continue;
        }

        int idx = concert_to_attend - 1;
        bool done = !isConcertDateValid(v.dateforconcert[idx]);
        const concert& c = all_queues[idx];
        int total_available = 0;
        for (const section& sec : c.concert_for_section) {
            for (const seat& s : sec.section_for_seat) {
                if (!s.is_taken)
                    total_available++;
            }
        }
        if (done) {
            cout << "\t\t\t\033[31mSorry, this concert has already passed and cannot be selected.\033[0m\n";
            continue;
        }
        if (total_available == 0) {
            cout << "\t\t\t\033[31mSorry, this concert is sold out and cannot be selected.\033[0m\n";
            continue;
        }
        break;
    } while (true);

    newTicketNode->concert = v.concert[concert_to_attend - 1];
    concert& selectcon = all_queues[concert_to_attend - 1];

    cout << "\n\n\n\t\t\t\tSections Available:\n";
    for (int i = 0; i < (int)v.section.size(); i++) {
        const section& sec = selectcon.concert_for_section[i];
        int available_seats = 0;
        for (const seat& s : sec.section_for_seat)
            if (!s.is_taken)
                available_seats++;
        cout << "\n\t\t\t" << "[" << i + 1 << "] " << v.section[i] << " (Available Seats: " << available_seats << ")";
    }

    do {
        cout << "\n\t\t\tBot: What section you want to avail ?  ";
        cin >> section_to_avail;

        if (cin.fail()){
            cin.clear();
            cin.ignore(10000, '\n');
            cout <<"\n\t\t\t\033[31mInvalid input. Please enter a number.\033[0m\n"; 
            continue;
        }
        if (section_to_avail < 1 || section_to_avail > (int)v.section.size()) {
            cout << "\t\t\t\033[31mInvalid section selection, please choose again.\033[0m\n";
            continue;
        }
        const section& sec = selectcon.concert_for_section[section_to_avail - 1];
        int available_seats = 0;
        for (const seat& s : sec.section_for_seat) {
            if (!s.is_taken)
                available_seats++;
        }
        if (available_seats == 0) {
            cout << "\t\t\t\033[31mSelected section is full, please choose another section.\033[0m\n";
            continue;
        }
        break;
    } while (true);

    cout << "\n\n\n\t\t\t\tFILL THE INFORMATION FOR YOUR TICKET\n";
    cout << "\n\t\t\tEnter name: ";
    cin.ignore();
    getline(cin, newTicketNode->name);

    do {
        cout << "\n\t\t\tEnter age: ";
        cin >> newTicketNode->age;

        if(cin.fail()){
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n\t\t\t\033[31mInvalid input. Please enter a number.\033[0m\n"; 
            continue;
        }

        if (!check_age(newTicketNode->age))
            cout << "\n\t\t\t\033[31mMust be 18 or above!\033[0m\n";
    } while (!check_age(newTicketNode->age));

    cin.ignore();
    do {
        cout << "\n\t\t\tEnter contact number: ";
        getline(cin, newTicketNode->contact_number);
        if (!check_contact(newTicketNode->contact_number))
            cout << "\n\t\t\t\033[31mInvalid contact number!\033[0m\n";
    } while (!check_contact(newTicketNode->contact_number));

    concert& select_con = all_queues[concert_to_attend - 1];
    section& select_sec = select_con.concert_for_section[section_to_avail - 1];

    int assigned_seat = -1;
    for (int i = 0; i < (int)select_sec.section_for_seat.size(); i++) {
        if (!select_sec.section_for_seat[i].is_taken) {
            assigned_seat = i + 1;
            select_sec.section_for_seat[i].is_taken = true;
            break;
        }
    }

    if (assigned_seat == -1) {
        cout << "\n\t\t\t\033[31mSorry, section is full!\033[0m\n";
        delete newTicketNode;
        for (int i = 0; i < (int)v.choices.size(); i++) {
            cout << "\n\t\t\t" << "[" << i + 1 << "] " << v.choices[i] << "\n";
        }
        do{
            cout << "\n\t\t\tBot: Would you like to go back : ";
            cin >> back;

            if(cin.fail()){
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\n\t\t\t\033[31mInvalid input. Please enter a number.\033[0m\n"; 
                continue;
            } 
            if (back < 1 || back >(int) v.choices.size()) {
                cout << "\n\t\t\t\033[31mInvalid input. Please enter a number.\033[0m\n"; 
                continue;
            } else if(back == 1){
                Phase();
                break;
            } else if(back == 2){
                cout << "\n\n\n\t\t\t\t\033[32mBot: Thank you !\033[0m";
                break;
            }
        } while(true);

        return; 
    }

    
    cout << "\n\n\n\t\t\t\tPAYMENT PROCESS: \n";
    for (int i = 0; i < (int)v.ModeofPayment.size(); i++) {
        cout << "\n\t\t\t"
             << "[" << i + 1 << "] " << v.ModeofPayment[i] << "\n";
    }
    do {
        cout << "\n\t\t\tBot: Which payment method would you like to use? ";
        cin >> mode_of_payment;

        if(cin.fail()){
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n\t\t\t\033[31mInvalid input. Please enter a number.\033[0m\n"; 
            continue;
        } 

        if (mode_of_payment < 1 || mode_of_payment > v.ModeofPayment.size() ){
            cout << "\n\t\t\t\033[31mInvalid input. Please enter a number.\033[0m\n"; 
            continue;
        }
        break;
    } while (true);

    newTicketNode->payment_method = v.ModeofPayment[mode_of_payment - 1];
    newTicketNode->section = select_sec.section_name;
    newTicketNode->price = select_sec.price;
    newTicketNode->seat_number = assigned_seat;
    newTicketNode->serial_number = generate_serial(newTicketNode->concert);
    newTicketNode->transaction_id = generate_transaction_id(newTicketNode->concert, newTicketNode->section);

    enqueue(select_con, newTicketNode, section_to_avail - 1);

    cout << "\n\t\t\tName: " << newTicketNode->name << "\n";
    cout << "\n\t\t\tConcert: " << newTicketNode->concert << "\n";
    cout << "\n\t\t\tSection: " << newTicketNode->section << "\n";
    cout << "\n\t\t\tSeat No.: " << newTicketNode->seat_number << "\n";
    cout << "\n\t\t\tMode of Payment: " << newTicketNode->payment_method << "\n";
    cout << "\n\t\t\tPrice: ₱" <<  fixed << setprecision(2) << newTicketNode->price  << "\n";
    cout << "\n\t\t\tSerial: " << newTicketNode->serial_number << "\n";
    cout << "\n\t\t\tTransaction ID: " << newTicketNode->transaction_id << "\n";
}


void ShowTicketsSold(const concert_variable& v) {
    for (int i = 0; i < (int)all_queues.size(); i++) {
        concert& c = all_queues[i];
        cout << "\n\t\t\tConcert: " << c.concert_name << "\n";
        vector<int> Section_sales((int)v.section.size(), 0);
        TicketNode* curr = c.front;
        while (curr != nullptr) {
            for (int j = 0; j < (int)v.section.size(); j++) {
                if (curr->section == v.section[j]) {
                    Section_sales[j]++;
                    break;
                }
            }
            curr = curr->next;
        }
        for (int k = 0; k < (int)v.section.size(); k++) {
            cout << "\t\t\t  Section: " << v.section[k];
            cout << " | Tickets Sold: " << Section_sales[k] << "\n";
        }
    }
}

void Phase() {
    concert_variable v;
    
    cout << "\n\n\n\t\t\t\t\033[34mConcert List Status\033[0m\n";
    for (int i = 0; i < (int)all_queues.size(); i++) {
        const concert& c = all_queues[i];
        bool done = !isConcertDateValid(c.date);

        int total_available = 0;
        vector<int> available_per_section;
        for (const section& sec : c.concert_for_section) {
            int section_available = 0;
            for (const seat& s : sec.section_for_seat) {
                if (!s.is_taken) section_available++;
            }
            available_per_section.push_back(section_available);
            total_available += section_available;
        }
        cout << "\033[32m" << "\t\t\t" << c.concert_name << "\033[0m" << "\n\t\t\t(" << c.date << ")"  ;
        if (done) {
            cout << "\n\t\t\t(Finished) \n";
        } else {
            cout << "\n\t\t\t(Upcoming)\n";
        }

        if (done) {
            cout << "\n\t\t\t - No ticket sales";
        } else if (total_available == 0) {
            cout << "\n\t\t\t - SOLD OUT";
        } else {
            cout << "\n\t\t\t\tSeats Available by Section:"<<"\n";
            for (size_t s = 0; s < available_per_section.size(); s++) {
                cout << "\033[32m" << "\t\t\t\t"<< "Price ₱ : \033[0m"  << c.concert_for_section[s].price ;
                cout << "\n\t\t\t\t  " << c.concert_for_section[s].section_name << ": " << available_per_section[s] << "\n";
                
            }
        }
        cout << endl;
    }

    cout << "\n\n\n\n\t\t\t\t\033[32mTICKETHYPE MENU\n\033[0m";
    for (int i = 0; i < (int)v.TicketChoices.size(); i++) {
        cout << "\n\t\t\t" << "[" << i + 1 << "] " << v.TicketChoices[i] << "\n";
    }


    while (true) {
        cout << "\n\n\n\n\t\t\tBot: Please enter the task you would like the bot to perform: ";
        cin >> task_to_do;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n'); 
            cout << "\n\t\t\t\033[31mInvalid input. Please enter a number.\033[0m\n";
            continue; 
        }

        if (task_to_do < 1 || task_to_do > 4) {
            cout << "\n\t\t\t\033[31mInvalid choice. Please try again.\033[0m\n";
            continue; 
        }


        break;
    }

    Phase2(task_to_do);
}

void Phase2(int task){
    concert_variable v;

    if (task == 1 ){
        Ticket_Buy();
        for (int i = 0; i < (int)v.choices.size(); i++){
            cout << "\n\t\t\t" << "[" << i + 1 << "] " << v.choices[i]<< "\n";
        }
        cout << "\n\t\t\tBot: Would you like to go back : ";
        cin >> back;
        if (back == 1 ){
            Phase();
        } else {
            cout << "\n\n\n\t\t\t\t\033[32mBot: Thank you !\033[0m";
        }
    } else if (task == 2){
        ShowTicketsSold(v);
        for (int i = 0; i < (int)v.choices.size(); i++){
            cout << "\n\t\t\t" << "[" << i + 1 << "] " << v.choices[i]<< "\n";
        }
        cout << "\n\t\t\tBot: Would you like to go back : ";
        cin >> back;
        if (back == 1 ){
            Phase();
        } else {
            cout << "\t\t\t\033[31mBot: Thank you\033[0m";
        }
    } else if (task == 3){
        Cancel_Ticket();
        for (int i = 0; i < (int)v.choices.size(); i++){
            cout << "\n\t\t\t" << "[" << i + 1 << "] " << v.choices[i]<< "\n";
        }
        cout << "\n\t\t\tBot: Would you like to go back : ";
        cin >> back;
        if (back == 1 ){
            Phase();
        } else {
            cout << "\t\t\t\033[31mBot: Thank you\033[0m";
        }
    } else if(task == 4){
        cout << "\t\t\t\033[32mBot: Thank you !\033[0m";
    }
}

int main(){
    concert_variable v;
    organizecss (v);
    Phase();
    return 0;
}