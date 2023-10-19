#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

using namespace std;

const int NB_MECANICIENS = 20; // Nombre de mécaniciens
const int NB_CLIENTS = 10; // Nombre de clients

mutex mtx; // Mutex pour la synchronisation
condition_variable cv_clients; // Variable conditionnelle pour la gestion des clients
bool mecanicien_disponible[NB_MECANICIENS] = {true}; // Disponibilité des mécaniciens
int nb_clients_en_attente = 0; // Nombre de clients en attente

/**
 * @brief Fonction exécutée par chaque mécanicien.
 *
 * Cette fonction simule le comportement d'un mécanicien qui attend les clients
 * et répare les véhicules.
 *
 * @param id Identifiant du mécanicien.
 */
void thread_mecanicien(int id) {
    while (true) {
        unique_lock<mutex> verrou(mtx);

        if (nb_clients_en_attente == 0) {
            mecanicien_disponible[id] = true;
            cout << "Mécanicien " << id << " attend un client..." << endl;
            cv_clients.wait(verrou, [id] { return mecanicien_disponible[id] && nb_clients_en_attente > 0; });
        }

        nb_clients_en_attente--;
        mecanicien_disponible[id] = false;

        cout << "Mécanicien " << id << " répare un véhicule." << endl;
        verrou.unlock();

        this_thread::sleep_for(chrono::seconds(2));
    }
}

/**
 * @brief Fonction exécutée par chaque client.
 *
 * Cette fonction simule le comportement d'un client qui arrive au garage
 * et demande des réparations pour son véhicule.
 *
 * @param id Identifiant du client.
 */
void thread_client(int id) {
    this_thread::sleep_for(chrono::milliseconds(1000));
    unique_lock<mutex> verrou(mtx);

    if (nb_clients_en_attente < NB_CLIENTS) {
        nb_clients_en_attente++;
        cout << "Client " << id << " arrive au garage. Clients en attente : " << nb_clients_en_attente << endl;
        cv_clients.notify_one();
    } else {
        cout << "Client " << id << " part car tous les mécaniciens sont occupés." << endl;
    }
}

/**
 * @brief Fonction pour générer des clients de manière asynchrone.
 *
 * Cette fonction génère des clients de manière asynchrone, chaque client
 * arrivant au garage à intervalles réguliers.
 */
void generateur_clients() {
    for (int i = 1; i <= NB_CLIENTS; i++) {
        this_thread::sleep_for(chrono::seconds(1)); // Nouveau client arrive toutes les 1 seconde
        thread(thread_client, i).detach();
    }
}

int main() {
    vector<thread> mecaniciens;

    for (int i = 0; i < NB_MECANICIENS; i++) {
        mecaniciens.push_back(thread(thread_mecanicien, i));
    }

    thread generateur(generateur_clients);

    for (int i = 0; i < NB_MECANICIENS; i++) {
        mecaniciens[i].join();
    }

    generateur.join();

    return 0;
}
