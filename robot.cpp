#include <iostream>
#include <string>
using namespace std;

struct Deliveryrequest {
    int id;
    string customername;
    string address;
    string item;
};

struct node {
    Deliveryrequest data;
    node* next;
};

// Linked List Queue (For pending deliveries)
class Linkedlistqueue {
    node* front;
    node* rear;
    int count;

public:
    Linkedlistqueue() {
        front = rear = nullptr;
        count = 0;
    }

    bool isempty() { return front == nullptr; }

    void enqueue(Deliveryrequest r) {
        node* newnode = new node;
        newnode->data = r;
        newnode->next = NULL;

        if (rear)
            rear->next = newnode;
        else
            front = newnode;

        rear = newnode;
        count++;
    }

    void showqueue() {
        if (isempty()) {
            cout << "No pending delivery requests.\n";
            return;
        }

        cout << "\nPending delivery requests:\n";
        node* temp = front;
        int index = 1;
        while (temp) {
            cout << index++ << ". "
                << "ID: " << temp->data.id
                << " | Customer: " << temp->data.customername
                << " | Address: " << temp->data.address
                << " | Item: " << temp->data.item << "\n";
            temp = temp->next;
        }
    }

    int getCount() { return count; }

    Deliveryrequest removeAt(int pos) {
        if (pos < 1 || pos > count || isempty()) {
            cout << "Invalid position.\n";
            return { -1, "", "", "" };
        }

        node* temp = front;
        node* prev = nullptr;

        for (int i = 1; i < pos; i++) {
            prev = temp;
            temp = temp->next;
        }

        if (prev)
            prev->next = temp->next;
        else
            front = temp->next;

        if (temp == rear)
            rear = prev;

        Deliveryrequest removed = temp->data;
        delete temp;
        count--;
        return removed;
    }
};

// Stack for Undo
class Linkedliststack {
    node* top;

public:
    Linkedliststack() {
        top = nullptr;
    }

    bool isempty() { return top == nullptr; }

    void push(Deliveryrequest r) {
 
        node* newnode = new node;
        newnode->data = r;
        newnode->next = top;
        top = newnode;
    }

    Deliveryrequest pop() {
        if (isempty()) {
            cout << "Stack is empty.\n";
            return { -1, "", "", "" };
        }

        node* temp = top;
        Deliveryrequest r = top->data;
        top = top->next;
        delete temp;
        return r;
    }
};

// Linked List for History
class Linkedlisthistory {
    node* head;

public:
    Linkedlisthistory() {
        head = nullptr;
    }

    void add(Deliveryrequest r) {
        node* newnode = new node;
        newnode->data = r;
        newnode->next = head;
        head = newnode;
    }

    void showhistory() {
        if (!head) {
            cout << "No completed deliveries.\n";
            return;
        }

        cout << "\nCompleted Deliveries:\n";
        node* current = head;
        while (current) {
            cout << "ID: " << current->data.id
                << " | Customer: " << current->data.customername
                << " | Address: " << current->data.address
                << " | Item: " << current->data.item << "\n";
            current = current->next;
        }
    }
};

// class include all classes 
class Smartdeliverybot {
    Linkedlistqueue deliveryqueue; // here classes as a data type
    Linkedliststack undostack;
    Linkedlisthistory history;
    int nextId = 1;

public:
    void addRequest(string name, string address, string item) {
        Deliveryrequest r = { nextId++, name, address, item };
        deliveryqueue.enqueue(r);
        cout << "Request added for customer: " << name << "\n";
    }

    void removeRequestByChoice() {
        if (deliveryqueue.isempty()) {
            cout << "No requests to remove.\n";
            return;
        }

        deliveryqueue.showqueue();
        int choice;
        cout << "Enter the request number to remove: ";
        cin >> choice;

        Deliveryrequest removed = deliveryqueue.removeAt(choice);
        if (removed.id != -1) {
            undostack.push(removed);
            cout << "Removed request for: " << removed.customername << "\n";
        }
    }

    void undocancel() {
        if (undostack.isempty()) {
            cout << "Nothing to undo.\n";
            return;
        }

        Deliveryrequest r = undostack.pop();
        deliveryqueue.enqueue(r);
        cout << "Undo: Request restored for " << r.customername << "\n";
    }

    void processnextrequest() {
        if (deliveryqueue.isempty()) {
            cout << "No requests to deliver.\n";
            return;
        }

        Deliveryrequest r = deliveryqueue.removeAt(1);
        history.add(r);

        cout << "Delivered to: " << r.customername
            << " - " << r.address << " - " << r.item << "\n";
    }

    void showqueue() { deliveryqueue.showqueue(); }

    void showhistory() { history.showhistory(); }
};

int main() {
    Smartdeliverybot bot;
    int choice;
    string name, address, item;

    do {
        cout << "\nSmart Delivery Bot Menu\n";
        cout << "1. Add New Request\n";
        cout << "2. Remove Specific Request\n";
        cout << "3. Undo Cancel\n";
        cout << "4. Deliver Next Request\n";
        cout << "5. Show Pending Requests\n";
        cout << "6. Show Delivery History\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            cout << "Customer Name: ";
            getline(cin, name);
            cout << "Address: ";
            getline(cin, address);
            cout << "Item Description: ";
            getline(cin, item);
            bot.addRequest(name, address, item);
            break;
        case 2:
            bot.removeRequestByChoice();
            break;
        case 3:
            bot.undocancel();
            break;
        case 4:
            bot.processnextrequest();
            break;
        case 5:
            bot.showqueue();
            break;
        case 6:
            bot.showhistory();
            break;
        case 0:
            cout << "\nThank you for using the Smart Delivery Bot. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}
