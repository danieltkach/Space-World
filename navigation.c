#include "navigation.h"
#include "planet.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define THRESHOLD 0.1  // in AU
#define PI 3.141592653589793

// Prints ship status info with custom formatting.
void printInfo(ShipState *state) {
    double distanceFromSun = sqrt(state->shipPosition.x * state->shipPosition.x +
                                  state->shipPosition.y * state->shipPosition.y);
    printf("Space-Time Information:\n");
    printf("-T- Current Time (departure time): %.2f days\n", state->currentTime);
    printf("-P- Current Ship Position:\n");
    printf("  x = %.4f AU\n  y = %.4f AU\n  z = %.4f AU\n",
           state->shipPosition.x, state->shipPosition.y, state->shipPosition.z);
    printf("\n-L- Destination data: \n");
    printf("  Distance from the Sun: %.4f AU\n", distanceFromSun);
    printf("  Name: %s\n", state->currentDestination.name);
    printf("  Description: %s\n", state->currentDestination.description);
    printf("\n<-> Oxygen levels: Not available\n");
    printf("@^@ Fuel levels: Not available\n");
    printf("-+- Food levels: Unknown\n");
}

// Prints the orbital formulae.
void printFormulae(void) {
    printf("\nANGLE = 2 * PI * (time / planet.orbitalPeriod)\n");
    printf("X = planet.orbitRadius * cos(ANGLE)\n");
    printf("Y = planet.orbitRadius * sin(ANGLE)\n");
    printf("Z = 0.0 (2D approximation: movement in the ecliptic plane only)\n");
}

// Computes Hohmann transfer time (in days) between two orbits.
// Returns 0 if radii are nearly identical.
double computeHohmannTransferTime(double r1, double r2) {
    if (fabs(r1 - r2) < 1e-6)
        return 0.0;
    double a_transfer = (r1 + r2) / 2.0;
    double period_transfer = 365.25 * pow(a_transfer, 1.5);
    return period_transfer / 2.0;
}

// Computes phasing time for same-orbit transfers based on angular difference.
double computePhasingTime(Vector3D current, Vector3D target, double orbitalPeriod) {
    double angleCurrent = atan2(current.y, current.x);
    double angleTarget  = atan2(target.y, target.x);
    double dtheta = fabs(angleTarget - angleCurrent);
    if (dtheta > PI)
        dtheta = 2 * PI - dtheta;
    return (dtheta / (2 * PI)) * orbitalPeriod;
}

// Displays Hohmann or phasing transfer time based on input orbit radii.
void hohmannTransferTime(ShipState *state) {
    double playerOrbitRadius, destinationOrbitRadius;
    printf("\nYour orbit radius (AU): ");
    scanf("%lf", &playerOrbitRadius);
    printf("Destination orbit radius (AU): ");
    scanf("%lf", &destinationOrbitRadius);
    
    if (fabs(playerOrbitRadius - destinationOrbitRadius) < 1e-6) {
        Vector3D target;
        printf("Same orbit detected. Enter the target X coordinate (AU): ");
        scanf("%lf", &target.x);
        printf("Enter the target Y coordinate (AU): ");
        scanf("%lf", &target.y);
        target.z = 0.0;
        double orbitalPeriod = 365.25 * pow(playerOrbitRadius, 1.5);
        double phasingTime = computePhasingTime(state->shipPosition, target, orbitalPeriod);
        printf("Calculated phasing transfer time: %.2f days\n", phasingTime);
    } else {
        double transferTime = computeHohmannTransferTime(playerOrbitRadius, destinationOrbitRadius);
        printf("Calculated Hohmann transfer time: %.2f days\n", transferTime);
    }
}

// TRAVEL SYSTEM: Prompts for trajectory and arrival time, then updates ShipState.
void travelSystemExecute(ShipState *state) {
  Vector3D playerCalculated;
  double travelDuration;
  printf("\nEnter your calculated X coordinate: ");
  scanf("%lf", &playerCalculated.x);
  printf("Enter your calculated Y coordinate: ");
  scanf("%lf", &playerCalculated.y);
  printf("Enter your calculated Z coordinate: ");
  scanf("%lf", &playerCalculated.z);
  printf("Enter your travel duration (in days): ");
  scanf("%lf", &travelDuration);
  
  // Update the ship's state.
  state->shipPosition = playerCalculated;
  state->currentTime += travelDuration;  // Add travel duration to current time.
  updateCurrentDestination(state, state->currentTime);
}


// Determines the current destination by comparing the ship's position with known planets.
#include "destinations.h" // Include your destinations module

void determineDestination(Vector3D pos, double time, ShipState *state) {
    // Loop over all known destinations.
    Planet *destinationFound = NULL;
    for (int i = 0; i < knownDestinationsCount; i++) {
        Vector3D destPos = getPlanetPosition(knownDestinations[i], time);
        double d = calculateDistance(pos, destPos);
        if (d < THRESHOLD) {
            destinationFound = &knownDestinations[i];
            break;
        }
    }

    if (destinationFound != NULL) {
         strcpy(state->currentDestination.name, destinationFound->name);
         // For description, you might hard-code some or call a helper function.
         if (strcmp(destinationFound->name, "Earth") == 0) {
             strcpy(state->currentDestination.description, "Earth: our vibrant blue home planet.");
         } else if (strcmp(destinationFound->name, "Mars") == 0) {
             strcpy(state->currentDestination.description, "Mars: the Red Planet, a potential destination for exploration.");
         } else if (strcmp(destinationFound->name, "Saturn") == 0) {
             strcpy(state->currentDestination.description, "Saturn: adorned with magnificent rings.");
         } else {
             strcpy(state->currentDestination.description, "A known celestial destination.");
         }
         state->currentDestination.position = getPlanetPosition(*destinationFound, time);
         state->currentDestination.arrivalTime = time;
    } else {
         strcpy(state->currentDestination.name, "Unknown");
         strcpy(state->currentDestination.description, "You have arrived at an unknown celestial destination.");
         state->currentDestination.position = pos;
         state->currentDestination.arrivalTime = time;
    }
}


// Updates the current destination in the ShipState.
void updateCurrentDestination(ShipState *state, double arrivalTime) {
    state->currentTime = arrivalTime;
    determineDestination(state->shipPosition, state->currentTime, state);
    printf("You have arrived at %s.\n", state->currentDestination.name);
}
