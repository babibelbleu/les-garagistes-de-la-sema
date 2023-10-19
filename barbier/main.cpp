/**
 * Fichier représentant le problème du barbier
 * Sur LittleBookOfSemaphores
*/


// importation des variables
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>

using namespace std;

// constantes

/**
 * @brief NUM_CHAIRS
 *
 * Représente le nombre de chaises dans la salle d'attente.
 */
const int NUM_CHAIRS = 5;

/**
 * @brief NUM_CUSTOMERS
 *
 * Représente le nombre de clients que l'on va générer.
 */
const int NUM_CUSTOMERS = 1000;

/**
 * @brief mtx
 *
 * mutex
 */
mutex mtx;

/**
 * @brief customer_cv
 *
 * variable conditionnelle
 */
condition_variable customer_cv, barber_cv;

/**
 * @brief barber_sleeping
 *
 * Booléen vérifiant si le barbier est en train de dormir
 */
bool barber_sleeping = true;

/**
 * @brief num_customers_waiting
 *
 * Nombre de clients qui attendent dans la salle d'attente.
 */
int num_customers_waiting = 0;

void barber_thread() {
    while (true) {
        unique_lock<mutex> lock(mtx);

        if (num_customers_waiting == 0) {
            barber_sleeping = true;
            lock.unlock();

            cout << "Barber is sleeping..." << endl;
            this_thread::sleep_for(chrono::seconds(1));
        } else {
            num_customers_waiting--;
            barber_sleeping = false;
            customer_cv.notify_one();

            cout << "Barber is cutting hair." << endl;
            lock.unlock();

            this_thread::sleep_for(chrono::seconds(2));
        }
    }
}

void customer_thread(int id) {
    this_thread::sleep_for(chrono::milliseconds(1000));
    unique_lock<mutex> lock(mtx);

    if (num_customers_waiting < NUM_CHAIRS) {
        num_customers_waiting++;
        cout << "Customer " << id << " takes a seat. Customers waiting: " << num_customers_waiting << endl;
        customer_cv.notify_one();
    } else {
        cout << "Customer " << id << " leaves because all chairs are occupied." << endl;
    }
}

void customer_generator() {
    for (int i = 1; i <= NUM_CUSTOMERS; i++) {
        this_thread::sleep_for(chrono::seconds(1)); // New customer arrives every 1 second
        thread(customer_thread, i).detach();
    }
}

int main() {
    thread barber(barber_thread);
    thread generator(customer_generator);

    barber.join();
    generator.join();

    return 0;
}
