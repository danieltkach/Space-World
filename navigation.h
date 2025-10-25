#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "planet.h"

// ShipState structure encapsulates the ship's state.
typedef struct {
    double currentTime;
    Vector3D shipPosition;
    struct {
        char name[64];
        char description[256];
        Vector3D position;
        double arrivalTime;
    } currentDestination;
} ShipState;

// Function prototypes for navigation functions.
void printInfo(ShipState *state);
void printFormulae(void);
double computeHohmannTransferTime(double r1, double r2);
double computePhasingTime(Vector3D current, Vector3D target, double orbitalPeriod);
void hohmannTransferTime(ShipState *state);
void travelSystemExecute(ShipState *state);
void determineDestination(Vector3D pos, double time, ShipState *state);
void updateCurrentDestination(ShipState *state, double arrivalTime);

#endif
