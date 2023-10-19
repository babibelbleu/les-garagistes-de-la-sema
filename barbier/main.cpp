#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>

using namespace std;

const int NOMBRE_DE_CHAISES = 5;
const int NOMBRE_DE_CLIENTS = 1000;

mutex mtx;
condition_variable condition_clients, condition_barbier;

bool barbier_endormi = true;
int nombre_de_clients_en_attente = 0;

void thread_barbier() {
    while (true) {
        unique_lock<mutex> verrou(mtx);

        if (nombre_de_clients_en_attente == 0) {
            barbier_endormi = true;
            verrou.unlock();

            cout << "Le barbier dort..." << endl;
            this_thread::sleep_for(chrono::seconds(1));
        } else {
            nombre_de_clients_en_attente--;
            barbier_endormi = false;
            condition_clients.notify_one();

            cout << "Le barbier coupe les cheveux d'un client." << endl;
            verrou.unlock();

            this_thread::sleep_for(chrono::seconds(2));
        }
    }
}

void thread_client(int id) {
    this_thread::sleep_for(chrono::milliseconds(1000));
    unique_lock<mutex> verrou(mtx);

    if (nombre_de_clients_en_attente < NOMBRE_DE_CHAISES) {
        nombre_de_clients_en_attente++;
        cout << "Le client " << id << " prend place. Clients en attente : " << nombre_de_clients_en_attente << endl;
        condition_clients.notify_one();
    } else {
        cout << "Le client " << id << " part car toutes les chaises sont occupées." << endl;
    }
}

void generateur_de_clients() {
    for (int i = 1; i <= NOMBRE_DE_CLIENTS; i++) {
        this_thread::sleep_for(chrono::seconds(1)); // Nouveau client toutes les 1 seconde
        thread(thread_client, i).detach();
    }
}

int main() {
    thread barbier_thread(thread_barbier);
    thread generateur_clients_thread(generateur_de_clients);

    barbier_thread.join();
    generateur_clients_thread.join();

    return 0;
}