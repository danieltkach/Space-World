#ifndef DESTINATIONS_H
#define DESTINATIONS_H

#include "planet.h"

// Array of known destinations (planets, etc.)
extern Planet knownDestinations[];

// Number of known destinations.
extern int knownDestinationsCount;

// Function to print all loaded destinations.
void printDestinations(void);

// Helper function: Find a destination by name.
Planet *getDestinationByName(const char *name);

#endif
