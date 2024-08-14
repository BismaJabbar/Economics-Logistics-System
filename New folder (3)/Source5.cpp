#include <iostream>
#include <string>
#include <vector>
#include<unordered_map>
#include<algorithm>
#include<numeric>
using namespace std;

struct Point {
    int x, y;
};
constexpr int V = 5;
int tableSize = 100;

int hashFunction(int key) {
    return key % tableSize;
}

class Customer {
public:
    int id;
    string name;
    string CNIC;
    string contact;
    string address;
    int hashIndex; // New member to store the index

    Customer() : id(0), name(""), CNIC(""), contact(""), address(""), hashIndex(-1) {}

    Customer(int customerID, string customerName, string customerCNIC, string customerContact, string customerAddress, int index) {
        id = customerID;
        name = customerName;
        CNIC = customerCNIC;
        contact = customerContact;
        address = customerAddress;
        hashIndex = index; // Initialize the hash index
    }
};

class CustomerManager {
private:
    vector<Customer> customerList;

public:
    CustomerManager() {
        customerList.resize(tableSize, Customer(0, "", "", "", "", -1));
    }

    void registerCustomer(Customer customer) {
        int index = hashFunction(customer.id);

        while (customerList[index].id != 0) {
            index = (index + 1) % tableSize;

            if (index == hashFunction(customer.id)) {
                cerr << "Error: Customer list is full. Unable to register customer." << endl;
                return;
            }
        }

        customer.hashIndex = index; // Store the index in the Customer object
        customerList[index] = customer;
    }

    void printCustomers() {
        for (size_t i = 0; i < customerList.size(); ++i) {
            Customer& customer = customerList[i];
            if (customer.id != 0) {
                cout << "Customer ID: " << customer.id << "\nName: " << customer.name << "\nCNIC: " << customer.CNIC
                    << "\nContact: " << customer.contact << "\nAddress: " << customer.address
                    << "\nHash Index: " << customer.hashIndex << "\n\n";
            }
        }
    }

    Customer* findCustomerById(int customerId) {
        int index = hashFunction(customerId);

        while (customerList[index].id != 0) {
            if (customerList[index].id == customerId) {
                return &customerList[index];
            }

            index = (index + 1) % tableSize;

            if (index == hashFunction(customerId)) {
                return nullptr;
            }
        }

        return nullptr;
    }
};

class Item {
public:
    string name;
    double price;
    int quantity;
    Item* next;
    Item* prev;

    Item() : name(""), price(0.0), quantity(0), next(nullptr), prev(nullptr) {}

    Item(string itemName, double itemPrice, int itemQuantity) {
        name = itemName;
        price = itemPrice;
        quantity = itemQuantity;
        next = nullptr;
        prev = nullptr;
    }
};

class InventoryManager {
private:
    Item* head;
    Item* tail;

public:
    InventoryManager() {
        head = nullptr;
        tail = nullptr;
    }

    void stockNewItem(Item item) {
        Item* newItem = new Item(item.name, item.price, item.quantity);

        if (head == nullptr) {
            head = tail = newItem;
        }
        else {
            newItem->prev = tail;
            tail->next = newItem;
            tail = newItem;
        }
    }
    vector<Item> getStockedItems() const {
        vector<Item> stockedItems;
        Item* ptr = head;

        while (ptr != nullptr) {
            stockedItems.push_back(*ptr);
            ptr = ptr->next;
        }

        return stockedItems;
    }

    void displayStockedItems() {
        Item* ptr = head;

        if (ptr == nullptr) {
            cout << "No available items" << endl;
        }

        while (ptr != nullptr) {
            cout << "Item name: " << ptr->name << "\nItem price: " << ptr->price << "\nItem quantity: " << ptr->quantity << "\n\n";
            ptr = ptr->next;
        }
    }

    void discontinueItem(string& name) {
        Item* ptr = head;
        bool found = false;

        while (ptr != nullptr) {
            if (ptr->name == name) {
                found = true;
                Item* tempNext = ptr->next;

                if (ptr->prev != nullptr) {
                    ptr->prev->next = ptr->next;
                }
                else {
                    head = ptr->next;
                }

                if (ptr->next != nullptr) {
                    ptr->next->prev = ptr->prev;
                }
                else {
                    tail = ptr->prev;
                }

                delete ptr;
                ptr = tempNext;
            }
            else {
                ptr = ptr->next;
            }
        }

        if (!found) {
            cout << "No such item found." << endl;
        }
    }

    void updateStock(string& name, int quantityToAdd) {
        Item* ptr = head;
        bool found = false;

        while (ptr != nullptr) {
            if (ptr->name == name) {
                found = true;
                ptr->quantity += quantityToAdd;
                cout << "Stock updated for " << name << ". New quantity: " << ptr->quantity << endl;
                break;
            }
            else {
                ptr = ptr->next;
            }
        }

        if (!found) {
            cout << "No such item found." << endl;
        }
    }

    void removeStock(string& name, int quantityToRemove) {
        Item* ptr = head;
        bool found = false;

        while (ptr != nullptr) {
            if (ptr->name == name) {
                found = true;
                if (ptr->quantity >= quantityToRemove) {
                    ptr->quantity -= quantityToRemove;
                    cout << "Stock removed for " << name << ". New quantity: " << ptr->quantity << endl;
                }
                else {
                    cout << "Not enough stock available for " << name << ". Current quantity: " << ptr->quantity << endl;
                }
                break;
            }
            else {
                ptr = ptr->next;
            }
        }

        if (!found) {
            cout << "No such item found." << endl;
        }
    }
};
class Order {
public:
    int orderid;
    int priority;
    Customer customer;
    Item item;
    int quantity;
    int deliverypoint;
    string trackingStatus; // New member to store tracking status

    Order(int orderid, int priority, const Customer cstmr, Item& items, int quant, int dpoint)
        : orderid(orderid), priority(priority), customer(cstmr), item(items), quantity(quant), deliverypoint(dpoint),
        trackingStatus("In the warehouse") {} // Initialize tracking status

};
class Invoice
{
public:
    int invoiceno;
    Order order2;
    Invoice* next;
    Invoice* previous;

    Invoice(int invono, Order orderr)
        : invoiceno(invono), order2(orderr), next(nullptr), previous(nullptr) {}
};
class billing
{
private:
    Invoice* head;
    Invoice* tail;

public:
    billing()
    {
        head = nullptr;
        tail = nullptr;
    }
    void generate_bill(int invono, Order orderr)
    {
        Invoice* newinvoice = new Invoice(invono, orderr);
        cout << "           -------INVOICE-------  " << endl;
        cout << "|  INVOICE NO:" << invono <<  endl;
        cout << "|  CUSTOMER ID:" << orderr.customer.id  << endl;
        cout << "|  CUSTOMER NAME:" << orderr.customer.name  << endl;
        cout << "|  ITEM BOUGHT:" << orderr.item.name <<  endl;
        cout << "|  QUANTITY:" << orderr.quantity  << endl;
        cout << "|  ITEM PRICE:" << orderr.item.price  << endl;
        cout << "|  Total BILL:" << orderr.quantity * orderr.item.price << endl;
        if (head == nullptr) {
            head = tail = newinvoice;
        }
        else {
            newinvoice->previous = tail;
            tail->next = newinvoice;
            tail = newinvoice;
        }
    }

    void del_end()
    {
        if (tail == nullptr) {
            // The list is already empty
            cout << "No orders to delete." << endl;
            return;
        }

        if (tail == head) {
            // Only one element in the list
            delete tail;
            head = tail = nullptr;
        }
        else {
            Invoice* temp = tail;
            tail = tail->previous;
            tail->next = nullptr;
            delete temp;
        }
    }



};







class TrackingStatus {
private:
    unordered_map<int, string> orderStatus; // Map order ID to its status

public:
    TrackingStatus() {}

    void updateStatus(int orderId, const string& status) {
        orderStatus[orderId] = status;
    }

    string getStatus(int orderId) const {
        auto it = orderStatus.find(orderId);
        if (it != orderStatus.end()) {
            return it->second;
        }
        return "Status not available";
    }
};
class ParcelDeliverySystem {
public:
    vector<Point> points;
    unordered_map<int, vector<pair<int, double>>> adjacencyList; // Node, List of (Neighbor, Distance)
    vector<int> route;

public:
    ParcelDeliverySystem(const vector<Point>& points) : points(points) {
        route.resize(points.size());
        iota(route.begin(), route.end(), 0.0);


        // Populate the adjacency list
        for (int i = 0; i < points.size(); ++i) {
            for (int j = 0; j < points.size(); ++j) {
                if (i != j) {
                    double dist = distance(i, j);
                    adjacencyList[i].emplace_back(j, dist);
                }
            }
        }
    }

    void solveTSPSubset(const vector<int>& subset) {
        auto it = find(route.begin(), route.end(), 0);
        if (it != route.begin()) {
            iter_swap(it, route.begin());
        }

        sort(route.begin() + 1, route.end(), [&](int i, int j) {
            return find(subset.begin(), subset.end(), i) != subset.end() &&
            find(subset.begin(), subset.end(), j) != subset.end() &&
            getDistance(route[0], i) < getDistance(route[0], j);
            });
    }

    void displayRoute(const vector<int>& subset) const {
        cout << "\n\n======= Optimal Route =======\n";
        for (int city : route) {
            if (find(subset.begin(), subset.end(), city) != subset.end()) {
                cout << city << " ";
            }
        }
        double distancePerLiter = 30;
        double costPerLiter = 9.33;
        double fuelConsumption = calculateTotalDistance(subset) / distancePerLiter;
        double totalCost = calculateTotalDistance(subset) * costPerLiter;
        cout << "\n\nTotal Distance Covered: " << calculateTotalDistance(subset) << " units\n";
        cout << "\n======= Fuel Consumption =======\n";
        cout << "Fuel Consumption: " << fuelConsumption << "litres\n";
        cout << "Total Cost: Rs " << totalCost << endl;
    }
    int minDistance(double dist[], bool sptSet[])
    {

        int min = INT_MAX, min_index;

        for (int v = 0; v < V; v++)
            if (sptSet[v] == false && dist[v] <= min)
                min = dist[v], min_index = v;

        return min_index;
    }
    void printSolution(double dist[])
    {
        cout << "\n\nVertex \t Distance from Source" << endl;
        for (int i = 0; i < V; i++)
            cout << i << " \t\t\t" << dist[i] << endl;
    }


    void dijkstra(const ParcelDeliverySystem& deliverySystem, int src)
    {
        double dist[V];
        bool sptSet[V];

        for (int i = 0; i < V; i++) {
            dist[i] = DBL_MAX;
            sptSet[i] = false;
        }

        dist[src] = 0;

        for (int count = 0; count < V - 1; count++) {
            int u = minDistance(dist, sptSet);
            sptSet[u] = true;

            for (int v = 0; v < V; v++) {
                if (!sptSet[v] && deliverySystem.getDistance(u, v) && dist[u] != DBL_MAX &&
                    dist[u] + deliverySystem.getDistance(u, v) < dist[v]) {
                    dist[v] = dist[u] + deliverySystem.getDistance(u, v);
                }
            }
        }

        printSolution(dist);
    }

    double calculateTotalDistance(const vector<int>& subset) const {
        double totalDistance = 0.0;
        for (size_t j = 0; j < route.size() - 1; ++j) {
            if (find(subset.begin(), subset.end(), route[j]) != subset.end() && find(subset.begin(), subset.end(), route[j + 1]) != subset.end()) {
                totalDistance += getDistance(route[j], route[j + 1]);
            }
        }
        totalDistance += getDistance(route.back(), 0); // Return to the starting point
        return totalDistance; // Add this line to fix the error
    }

    double getDistance(int node1, int node2) const {
        auto it = find_if(adjacencyList.at(node1).begin(), adjacencyList.at(node1).end(), [&](const auto& neighbor) { return neighbor.first == node2; });
        if (it != adjacencyList.at(node1).end()) {
            return it->second;
        }
        return 0.0;
    }

    double distance(int point1, int point2) const {
        const double distances[][5] = {
            {0, 8.236, 2.162, 5.071, 3.606},
            {8.236, 0, 8.236, 5.831, 1.414},
            {2.162, 8.236, 0, 5.0, 8.236},
            {5.071, 5.831, 5.0, 0, 4.472},
            {3.606, 1.414, 8.236, 4.472, 0}
        };

        return distances[point1][point2];
    }

};
class OrderManagement {
public:
    vector<Order> orders;
    vector<int> totalDeliveryPointsSubset = { 0 };
    TrackingStatus trackingStatus;
    int i=0;

public:
    billing& bills;
    InventoryManager& inventory;
    ParcelDeliverySystem& deliverySystem;

    OrderManagement(billing& bill, InventoryManager& inv, ParcelDeliverySystem& deliverySys)
        :bills(bill), inventory(inv), deliverySystem(deliverySys) {}

    size_t sizee() const {
        return orders.size();
    }

    void max_heapify(int i) {
        size_t size = sizee();

        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < size && orders[left].priority > orders[largest].priority) {
            largest = left;
        }
        if (right < size && orders[right].priority > orders[largest].priority) {
            largest = right;
        }

        if (largest != i) {
            swap(orders[i], orders[largest]);
            max_heapify(largest);
        }
    }

    void build_heap() {
        size_t n = sizee();

        for (size_t i = (n / 2) - 1; i != static_cast<size_t>(-1); --i) {
            max_heapify(static_cast<int>(i));
        }
    }

    void enqueue(const Order& order) {
        orders.push_back(order);
        build_heap();
        trackingStatus.updateStatus(order.orderid, "In the warehouse");
        i++;
        bills.generate_bill(i, order);
        
    }

    bool isEmpty() const {
        return orders.empty();
    }

    void displayQueue() const {
        if (isEmpty()) {
            cout << "No orders received." << endl;
            return; // Exit the function if there are no orders
        }
        cout << "======Orders=====" << endl << endl;
        for (const auto& order : orders) {
            cout << "Order ID: " << order.orderid << ", Delivery Point: " << order.deliverypoint << ", Item Quantity: " << order.quantity << ", Item Name: " << order.item.name << endl;
        }
        cout << endl;
    }

    void ProcessOrder() {
        if (isEmpty()) {
            cout << "No orders received." << endl;
            return; // Exit the function if there are no orders
        }

        vector<Order> processedOrders; // Store processed orders separately

        while (!isEmpty()) {
            Order frontOrder = orders.front();
            cout << "Processing Order with ID: " << frontOrder.orderid << ", Priority: " << frontOrder.priority << " ," << frontOrder.trackingStatus << endl;

            // Update tracking status to "In Transit"
            trackingStatus.updateStatus(frontOrder.orderid, "In Transit");

            // Store the processed order before removing it from the max-heap
            processedOrders.push_back(frontOrder);

            // Remove the processed order from the max-heap
            orders.erase(orders.begin());
            inventory.removeStock(frontOrder.item.name, frontOrder.quantity);

            // Update the total delivery points subset
            totalDeliveryPointsSubset.push_back(frontOrder.deliverypoint);

            // Build the heap after processing each order
            build_heap();
        }

        // Display the tracking status for the stored processed orders
        cout << "======= Tracking Status after Processing Orders =======\n";
        for (const auto& order : processedOrders) {
            cout << "Order ID: " << order.orderid << ", Status: " << trackingStatus.getStatus(order.orderid) << endl;
        }

        // Use delivery system to solve TSP and display the total optimal route
        deliverySystem.solveTSPSubset(totalDeliveryPointsSubset);
        deliverySystem.displayRoute(totalDeliveryPointsSubset);

        for (const auto& order : processedOrders) {
            trackingStatus.updateStatus(order.orderid, "Delivered");
        }
    }

    void checkOrderStatus(int orderId) const {
        string status = trackingStatus.getStatus(orderId);
        cout << "Order ID: " << orderId << " ,Status: " << status << endl;
    }
};





int main()
{
    cout << endl << "--------------------------------------------------ECONOMICS LOGISTICS SYSTEM-----------------------------------------------------" << endl << endl;
    Item item1 = Item("gatorade", 100.0, 25);
    Item item2 = Item("fruitaVitals", 150.0, 50);
    Item item3 = Item("slice", 100.0, 25);
    Item item4 = Item("fanta", 150.0, 50);

    InventoryManager inventory;
    inventory.stockNewItem(item1);
    inventory.stockNewItem(item2);
    inventory.stockNewItem(item3);
    inventory.stockNewItem(item4);
    CustomerManager manager;


    ParcelDeliverySystem deliverySystem({ {0, 0}, {1, 2}, {3, 1}, {5, 5}, {2, 3} });

    billing bill;
    OrderManagement orderManager(bill, inventory, deliverySystem);
    Customer customer1;


    /*  menu code*/
    int portalchoice;
    string username = "bisma";
    int password = 1234;
    string username2;
    int password2;
    string loginchoice = "no";
    string managerportalcontinue;
    string continuecustomerportal;
    string customerlpcontinue;
    string menuchoice1;
    string menuchoice;
    string orderagain;
    string transactionchoice;
    int mchoice;
    int pchoice;
    int j = 0;

    do
    {
        cout << "                                                 1.    MANAGER PORTAL" << endl;
        cout << "                                                 2.    CUSTOMER PORTAL" << endl;
        cin >> portalchoice;
        if (portalchoice == 1)
        {
            do
            {

                cout << "                                           -----WELCOME TO MANAGERS PORTAL----" << endl;
                cout << "PLEASE LOGIN FIRST" << endl;
                cout << "USERNAME=";
                cin >> username2;
                cout << "PASSWORD=";
                cin >> password2;
                if (username2 == username && password2 == password)
                {
                    do {
                        cout << "                                                 ---LOGIN SUCCESSFUL--" << endl;
                        cout << endl;
                        cout << "                                                   1. Display Stock" << endl;
                        cout << "                                                   2. Update Stock" << endl;
                        cout << "                                                   3. remove stock" << endl;
                        cout << "                                                   4. Display Orders" << endl;
                        cout << "                                                   5. Dispatch orders" << endl;
                        cout << "                                                   6. Order status" << endl;

                        cin >> mchoice;
                        if (mchoice == 1)
                        {
                            cout << "======= Stock =======\n";
                            inventory.displayStockedItems();
                            cout << endl;
                        }
                        else if (mchoice == 2)
                        {
                            string updatestockname;
                            int updatequantity;
                            cout << "Name of stock to update: ";
                            cin >> updatestockname;
                            cout << endl << "quantity: ";
                            cin >> updatequantity;
                            inventory.updateStock(updatestockname, updatequantity);
                        }
                        else if (mchoice == 3)
                        {
                            string itemNameToRemove;
                            int removequantity;
                            cout << "Item to remove= ";
                            cin >> itemNameToRemove;
                            cout << endl << "quantity to remove= ";
                            cin >> removequantity;
                            inventory.removeStock(itemNameToRemove, removequantity);

                            cout << endl << "======= Updated Stock =======\n";
                            inventory.displayStockedItems();
                            cout << endl;
                        }
                        else if (mchoice == 4)
                        {

                            orderManager.displayQueue();
                        }
                        else if (mchoice == 5)
                        {
                            cout << "======= Orders processing =======\n";
                            orderManager.ProcessOrder();


                            double adjacencyMatrix[V][V];
                            for (int i = 0; i < V; ++i) {
                                for (int j = 0; j < V; ++j) {
                                    adjacencyMatrix[i][j] = static_cast<int>(deliverySystem.getDistance(i, j));
                                }
                            }

                            cout << endl << endl;
                            deliverySystem.dijkstra(deliverySystem, 0);
                            cout << endl << endl;
                        }
                        else if (mchoice == 6)
                        {
                            // Check if there are orders
                            if (orderManager.sizee() > 0)
                            {
                                // Display final order statuses
                                cout << "======= Final Order Statuses =======\n";
                                for (const auto& order : orderManager.orders)
                                {
                                    cout << "Order ID: " << order.orderid << " ,Status: " << orderManager.trackingStatus.getStatus(order.orderid) << endl;
                                }
                            }
                            else
                            {
                                cout << "No orders available.\n";
                            }
                        }
                        cout << "Do u want to continue on your portal?" << endl;
                        cin >> managerportalcontinue;
                    } while (managerportalcontinue == "yes");


                }
                else {
                    cout << "Incorrect password/username" << endl;
                    cout << "Do you want to try logging again?" << endl;
                    cin >> loginchoice;
                }
            } while (loginchoice == "yes");

        }


        else if (portalchoice == 2)
        {
            do
            { 
                cout << "                                     ------------WELCOME TO CUSTOMERS PORTAL----------" << endl;
                cout << "                                                 1. REGISTER CUSTOMER" << endl;
                cout << "                                                 2. LOGIN CUSTOMER" << endl;
                cin >> pchoice;


                if (pchoice == 1)
                {
                    //customer management
                    cout << "----REGISTRATION----" << endl;
                    int regID;
                    string name;
                    string cnic;
                    string phonenumber;
                    string address;
                    cout << "Registration ID: ";
                    cin >> regID;
                    cin.ignore();
                    cout << endl << "Customer Name: ";
                    cin >> name;
                    cin.ignore();
                    cout << endl << "CNIC: ";
                    getline(cin, cnic);

                    cout << endl << "Phone Number: ";
                    getline(cin, phonenumber);

                    cout << endl << "Address: ";
                    getline(cin, address);



                    customer1 = Customer(regID, name, cnic, phonenumber, address, -1);

                    manager.registerCustomer(customer1);

                }
                else if (pchoice == 2)
                {
                    int searchcustomerbyid;
                    cout << "\n\nenter your Registration ID no. to login" << endl << endl;
                    cin >> searchcustomerbyid;
                    Customer* searchCustomer = manager.findCustomerById(searchcustomerbyid);
                    if (searchCustomer)
                    {
                        do
                        {
                            int bookorder;
                            cout << "======= Customer found: =======\n";
                            cout << "Customer ID: " << searchCustomer->id << "\nName: " << searchCustomer->name << "\nCNIC: " << searchCustomer->CNIC << "\nContact: " << searchCustomer->contact << "\nAddress: " << searchCustomer->address << endl << endl;
                            cout << "        Press 1. to ORDER" << endl;
                            cout << "        Press 2. to see status of your order" << endl;

                            cin >> bookorder;
                            if (bookorder == 1)
                            {
                                do {
                                    int Orderpriority;
                                    int orderquantity;
                                    int deliverypointt;
                                    string itemName;  // User input for the item name
                                    Item* selectedItem = nullptr;

                                    cout << "\n\n----------Placing an order--------" << endl << endl;
                                    cout << "----Items available for purchase---" << endl;
                                    inventory.displayStockedItems();
                                    cout << endl;

                                    cout << "Press 1 for express order" << endl;
                                    cout << "Press 0 for normal order" << endl;
                                    cin >> Orderpriority;

                                    cout << "Enter the name of the item you want to order: ";
                                    cin >> itemName;

                                    // Find the selected item from the inventory
                                    for (const Item& item : inventory.getStockedItems())
                                    {
                                        if (item.name == itemName)
                                        {
                                            selectedItem = new Item(item);
                                            break;
                                        }
                                    }

                                    if (selectedItem)
                                    {
                                       
                                        cout << "Quantity: ";
                                        cin >> orderquantity;
                                        do {
                                            cout << endl << "Delivery Point: ";
                                            cin >> deliverypointt;
                                            if (deliverypointt >= 1 && deliverypointt <= 4) {
                                                break;  // Exit the loop if the delivery point is valid
                                            }
                                            else {
                                                cout << "Error: Invalid delivery point. Please enter a value between 1 and 4." << endl;
                                            }
                                        } while (true);
                                        j++;
                                        orderManager.enqueue(Order(j, Orderpriority, *searchCustomer, *selectedItem, orderquantity, deliverypointt));
                                        
                                        cout << "Do you confirm transaction" << endl;
                                        cin >> transactionchoice;
                                        if (transactionchoice == "yes")
                                        {
                                            cout << "Transaction Succesful" << endl;
                                        }
                                        else if(transactionchoice=="no") {
                                            bill.del_end();
                                            cout << "order deleted/no transaction" << endl;
                                        }
                                        
                                    }
                                    else
                                    {
                                        cout << "Invalid item name. Order placement failed." << endl;
                                    }
                                    cout << "Do you want to buy another item?" << endl;
                                    cin >> orderagain;
                                } while (orderagain == "yes");

                            }
                            else if (bookorder == 2)
                            {
                                // Display order status of the logged-in customer
                                cout << "======= Your Order Statuses =======\n";
                                bool foundOrders = false;

                                for (const auto& order : orderManager.orders) {
                                    if (order.customer.id == searchCustomer->id) {
                                        foundOrders = true;
                                        cout << "Order ID: " << order.orderid << ", Status: " << orderManager.trackingStatus.getStatus(order.orderid) << endl;
                                    }
                                }

                                if (!foundOrders) {
                                    cout << "No orders found for the logged-in customer." << endl;
                                }
                            }
                            else {
                                cout << "Invalid option.\n" << endl;
                            }



                            cout << "Do u want to continue?" << endl;
                            cin >> customerlpcontinue;
                        } while (customerlpcontinue == "yes");
                    }

                    else {
                        cout << "Customer not found/not registered.\n" << endl;
                    }
                    cout << endl;
                }


                cout << "Do u want to continue on customer portal?" << endl;
                cin >> continuecustomerportal;
            } while (continuecustomerportal == "yes");
        }

        cout << "Menu portal display again?" << endl;
        cin >> menuchoice;
    } while (menuchoice == "yes");
}

        



    
