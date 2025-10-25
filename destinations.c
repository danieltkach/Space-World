#include "destinations.h"
#include <stdio.h>
#include <string.h>

// Define the known destinations.
Planet knownDestinations[] = {
    { "Mercury", 0.387, 87.97 },
    { "Venus",   0.723, 224.70 },
    { "Earth",   1.0,   365.25 },
    { "Mars",    1.523, 687.0 },
    { "Jupiter", 5.203, 4332.59 },
    { "Saturn",  9.537, 10759.22 },
    { "Uranus",  19.191, 30685.4 },
    { "Neptune", 30.068, 60190 }
};

int knownDestinationsCount = sizeof(knownDestinations) / sizeof(knownDestinations[0]);

void printDestinations(void) {
    printf("Loaded Destinations:\n");
    for (int i = 0; i < knownDestinationsCount; i++) {
         printf("%d. %s - Orbit Radius: %.3f AU, Orbital Period: %.2f days\n",
                i + 1, knownDestinations[i].name, knownDestinations[i].orbitRadius, knownDestinations[i].orbitalPeriod);
    }
}

Planet *getDestinationByName(const char *name) {
    for (int i = 0; i < knownDestinationsCount; i++) {
        if (strcmp(knownDestinations[i].name, name) == 0) {
            return &knownDestinations[i];
        }
    }
    return NULL;  // Not found.
}

