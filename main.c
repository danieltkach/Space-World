#include "navigation.h"
#include "planet.h"
#include "destinations.h"  // If you want to use printDestinations() or getDestinationByName() elsewhere.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printMenu(void) {
    printf("\n--- Navigation Console ---\n");
    printf("I > Space-Time Information\n");
    printf("D > Display Known Destinations\n");
    printf("F > Formulae\n");
    printf("H > Hohmann Transfer Time\n");
    printf("T > TRAVEL SYSTEM\n");
    printf("M > Menu\n");
    printf("0 > Quit\n");
}

int main(void) {
    // Retrieve Earth from the destinations module.
    Planet *earth = getDestinationByName("Earth");
    if (earth == NULL) {
        printf("Error: Earth not found!\n");
        exit(1);
    }
    
    // Retrieve Mars similarly if needed.
    Planet *mars = getDestinationByName("Mars");
    
    // Initialize your ship state using Earth.
    ShipState state;
    state.currentTime = 100.0;
    state.shipPosition = getPlanetPosition(*earth, state.currentTime);
    strcpy(state.currentDestination.name, earth->name);
    strcpy(state.currentDestination.description, "Earth: our vibrant blue home planet.");
    state.currentDestination.position = state.shipPosition;
    state.currentDestination.arrivalTime = state.currentTime;
    
    printf("\nYou are on << Mineral-Raider-1 >>\n");
    printInfo(&state);
    
    printMenu();
    char choice;
    while (1) {
        printf("System Option ('M' for menu) >> ");
        scanf(" %c", &choice);
        
        if (choice == 'D' || choice == 'd') {
            printDestinations();
        } else if (choice == 'F' || choice == 'f') {
            printFormulae();
        } else if (choice == 'H' || choice == 'h') {
            hohmannTransferTime(&state);
        } else if (choice == 'T' || choice == 't') {
            travelSystemExecute(&state);
        } else if (choice == 'I' || choice == 'i') {
            printInfo(&state);
        } else if (choice == 'M' || choice == 'm') {
            printMenu();
        } else if (choice == '0') {
            printf("Exiting Navigation Console. Safe travels!\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}
