# les-garagistes-de-la-sema
Gestionnaire de Garage

Description :

Ce programme simule la gestion d'un garage avec des mécaniciens et des clients. Il s'inspire du problème classique du coiffeur, où les mécaniciens représentent les coiffeurs et les clients, les clients qui arrivent au garage pour des réparations de véhicules.

Le problème est formulé de la manière suivante :

    • Le garage est composé de plusieurs mécaniciens.
    • Les clients arrivent au garage et demandent des réparations pour leurs véhicules.
    • Les mécaniciens réparent les véhicules des clients en attente.
    • Si tous les mécaniciens sont occupés, un client peut choisir de partir.
    • Les mécaniciens attendent qu'un client arrive pour effectuer des réparations.

Fonctionnement :

Le programme utilise des threads pour représenter les mécaniciens et les clients. Voici comment il fonctionne :

    • Les mécaniciens sont en attente de clients. Lorsqu'un client arrive, un mécanicien disponible le prend en charge.
    • Si tous les mécaniciens sont occupés, un client peut choisir de partir.
    • Les clients arrivent de manière asynchrone au garage et demandent des réparations.
    • Chaque mécanicien répare un véhicule à la fois.
    • Le programme génère des clients de manière asynchrone à intervalles réguliers.

Personnalisation :

Vous pouvez personnaliser le nombre de mécaniciens et de clients en ajustant les constantes NB_MECANICIENS et NB_CLIENTS dans le code.
Dépendances
Le programme utilise les bibliothèques C++ standard, notamment <iostream>, <thread>, <mutex>, <condition_variable>, et <chrono>.

Auteur:

    • Bodin Bastien
    • Sarti Alex
    • Rifi Sara
    
