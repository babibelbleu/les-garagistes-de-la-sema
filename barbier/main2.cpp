
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <vector>

using namespace std;

const int NUM_MECHANICS = 2; // Nombre de mécaniciens
const int NUM_CUSTOMERS = 10; // Nombre de clients

mutex mtx;
condition_variable customer_cv;
bool mechanics_available[NUM_MECHANICS] = {true};
int num_customers_waiting = 0;

void mechanic_thread(int id) {
    while (true) {
        unique_lock<mutex> lock(mtx);

        if (num_customers_waiting == 0) {
            mechanics_available[id] = true;
            cout << "Mechanic " << id << " is waiting for a customer..." << endl;
            customer_cv.wait(lock, [id] { return mechanics_available[id] && num_customers_waiting > 0; });
        }

        num_customers_waiting--;
        mechanics_available[id] = false;

        cout << "Mechanic " << id << " is repairing a vehicle." << endl;
        lock.unlock();

        this_thread::sleep_for(chrono::seconds(2));
    }
}

void customer_thread2(int id) {
    this_thread::sleep_for(chrono::milliseconds(1000));
    unique_lock<mutex> lock(mtx);

    if (num_customers_waiting < NUM_CUSTOMERS) {
        num_customers_waiting++;
        cout << "Customer " << id << " arrives at the garage. Customers waiting: " << num_customers_waiting << endl;
        customer_cv.notify_one();
    } else {
        cout << "Customer " << id << " leaves because all mechanics are busy." << endl;
    }
}

void customer_generator2() {
    for (int i = 1; i <= NUM_CUSTOMERS; i++) {
        this_thread::sleep_for(chrono::seconds(1)); // New customer arrives every 1 second
        thread(customer_thread2, i).detach();
    }
}

int main2() {
    vector<thread> mechanics;

    for (int i = 0; i < NUM_MECHANICS; i++) {
        mechanics.push_back(thread(mechanic_thread, i));
    }

    thread generator(customer_generator2);

    for (int i = 0; i < NUM_MECHANICS; i++) {
        mechanics[i].join();
    }

    generator.join();

    return 0;
}
