#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.141592653589793
#define THRESHOLD 0.1  // distance tolerance (in AU) to consider as "arrived"

// Structure to store a point or vector in 3D space.
typedef struct {
    double x;
    double y;
    double z;
} Vector3D;

// Structure to store planetary data.
typedef struct {
    char name[32];
    double orbitRadius;    // in Astronomical Units (AU)
    double orbitalPeriod;  // in days
} Planet;

// Structure to store a location.
typedef struct {
    char name[64];
    char description[256];
    Vector3D position;
    double arrivalTime;
} Location;

// Prints current ship status info.
void printInfo(double departureTime, Vector3D shipPosition, const char *locName, const char *locDescription) {
  double distanceFromSun = sqrt(shipPosition.x * shipPosition.x + shipPosition.y * shipPosition.y);
  printf("Space-Time Information\n");
  printf("-T- Current Time (departure time): %.2f days\n", departureTime);
  printf("-P- Current Ship Position:\n");
  printf("  x = %.4f AU\n  y = %.4f AU\n  z = %.4f AU\n", shipPosition.x, shipPosition.y, shipPosition.z);
  printf("-L- Location data: \n");
  printf("  Distance from the Sun: %.4f AU\n", distanceFromSun);
  printf("  Name: %s\n", locName);
  printf("  Description: %s\n", locDescription);
  printf("\n<-> Oxygen levels: Not available\n");
  printf("@^@ Fuel levels: Not available\n");
  printf("-+- Food levels: Unknown\n");
}

// Calculates a planet's position (circular orbit in the ecliptic plane) at a given time.
Vector3D getPlanetPosition(Planet planet, double time) {
    double angle = 2 * PI * (time / planet.orbitalPeriod);
    Vector3D pos;
    pos.x = planet.orbitRadius * cos(angle);
    pos.y = planet.orbitRadius * sin(angle);
    pos.z = 0.0; // 2D approximation
    return pos;
}

// Computes the distance between two 3D points.
double calculateDistance(Vector3D a, Vector3D b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}

// Computes the Hohmann transfer time (in days) between two circular orbits.
// Returns 0 if the orbit radii are (almost) identical.
double computeHohmannTransferTime(double r1, double r2) {
    if (fabs(r1 - r2) < 1e-6) {
        return 0.0;
    }
    double a_transfer = (r1 + r2) / 2.0;  // semi-major axis of the transfer orbit
    double period_transfer = 365.25 * pow(a_transfer, 1.5);  // full period (using Earth's period as reference)
    return period_transfer / 2.0;         // transfer time is half the period of the elliptical orbit
}

void hohmannTransferTime() {
  double playerOrbitRadius, destinationOrbitRadius;
  printf("\nYour orbit radius (AU): ");
  scanf("%lf", &playerOrbitRadius);
  printf("Destination orbit radius (AU): ");
  scanf("%lf", &destinationOrbitRadius);
  double transferTime = computeHohmannTransferTime(playerOrbitRadius, destinationOrbitRadius);
  printf("Calculated Hohmann transfer time: %.2f days\n", transferTime);
}

// Checks the player's calculated trajectory against a target position.
int checkNavigation(Vector3D playerInput, Vector3D targetPosition) {
    double error = calculateDistance(playerInput, targetPosition);
    const double tolerance = 0.05; // in AU
    if (error <= tolerance) {
         printf("Correct! You have set the proper trajectory.\n");
         return 1;
    } else {
         printf("Incorrect trajectory: error = %.4lf AU. Adjust your calculations.\n", error);
         return 0;
    }
}

// (Optional) Prints an ephemeris table for a planet over a given time range.
void printEphemeris(Planet planet, double startTime, double endTime, double interval) {
    printf("\nEphemeris for %s:\n", planet.name);
    printf("Time (days)    x (AU)      y (AU)\n");
    for (double t = startTime; t <= endTime; t += interval) {
        Vector3D pos = getPlanetPosition(planet, t);
        printf("%8.2f    %8.4f    %8.4f\n", t, pos.x, pos.y);
    }
    printf("\n");
}

// Global variables for the ship's state.
double currentTime = 100.0; // initial departure time (in days)
Vector3D shipPosition;      // current ship position
Location currentLocation;   // current location info

// Determines the current location based on shipPosition and currentTime.
// If the ship is close to Earth or Mars (within THRESHOLD), update accordingly; otherwise, mark as "Unknown."
void determineLocation(Vector3D pos, double time, Location *loc) {
    Planet earth = { "Earth", 1.0, 365.25 };
    Planet mars  = { "Mars", 1.523, 687.0 };
    Vector3D earthPos = getPlanetPosition(earth, time);
    Vector3D marsPos  = getPlanetPosition(mars, time);
    
    double dEarth = calculateDistance(pos, earthPos);
    double dMars  = calculateDistance(pos, marsPos);
    
    if (dEarth < THRESHOLD) {
         strcpy(loc->name, "Earth");
         strcpy(loc->description, "Earth: our vibrant blue home planet.");
         loc->position = earthPos;
         loc->arrivalTime = time;
    } else if (dMars < THRESHOLD) {
         strcpy(loc->name, "Mars");
         strcpy(loc->description, "Mars: the Red Planet, a potential destination for exploration.");
         loc->position = marsPos;
         loc->arrivalTime = time;
    } else {
         strcpy(loc->name, "Unknown");
         strcpy(loc->description, "You have arrived at an unknown celestial location.");
         loc->position = pos;
         loc->arrivalTime = time;
    }
}

// Updates the current location based on the new shipPosition and arrival time.
void updateCurrentLocation(double arrivalTime) {
    currentTime = arrivalTime;
    determineLocation(shipPosition, currentTime, &currentLocation);
    printf("You have arrived at %s.\n", currentLocation.name);
}

void printMenu() {
  printf("\n--- Navigation Console ---\n");
  printf("I > Info for current time and space\n");
  printf("P > Display Planet Data\n");
  printf("F > Formulae\n");
  printf("H > Hohmann Transfer Time\n");
  printf("T > TRAVEL SYSTEM\n");
  // printf("E > Show Ephemeris Table (optional)\n");
  printf("M > Menu\n");
  printf("0 > Quit\n");
}

void printFormulae() {
  printf("\nANGLE = 2 * PI * (time / planet.orbitalPeriod)\n");
  printf("X = planet.orbitRadius * cos(ANGLE)\n");
  printf("Y = planet.orbitRadius * sin(ANGLE)\n");
  printf("Z = 0.0 (2D approximation: movement in the ecliptic plane only)\n");
}

// TRAVEL SYSTEM: Let the player input their calculated trajectory.
void travelSystemExecute() {
  Vector3D playerCalculated;
  printf("\nEnter your calculated X coordinate: ");
  scanf("%lf", &playerCalculated.x);
  printf("Enter your calculated Y coordinate: ");
  scanf("%lf", &playerCalculated.y);
  printf("Enter your calculated Z coordinate: ");
  scanf("%lf", &playerCalculated.z);
  double arrivalTime;
  printf("Enter your calculated Arrival Time (in days): ");
  scanf("%lf", &arrivalTime);
  
  // Update ship state from player's input.
  shipPosition = playerCalculated;
  currentTime = arrivalTime;
  // Determine the new location.
  updateCurrentLocation(arrivalTime);
}

int main(void) {
    // Define known planets.
    Planet earth = { "Earth", 1.0, 365.25 };
    Planet mars  = { "Mars", 1.523, 687.0 };
    
    // Initialize ship's starting position at Earth.
    shipPosition = getPlanetPosition(earth, currentTime);
    strcpy(currentLocation.name, "Earth");
    strcpy(currentLocation.description, "Earth: our vibrant blue home planet.");
    currentLocation.position = shipPosition;
    currentLocation.arrivalTime = currentTime;
    
    // Initial mission briefing.
    printf("You are on << Mineral-Raider-1 >>\n\n");
    printInfo(currentTime, shipPosition, currentLocation.name, currentLocation.description);
    
    // Interactive Menu Loop.
    char choice;
    printMenu();
    while (1) {
        printf("\nSystem Option >> ");
        scanf(" %c", &choice);
        
        if (choice == 'P' || choice == 'p') {
            printf("\nEarth: Orbit Radius = %.3f AU, Orbital Period = %.2f days\n", earth.orbitRadius, earth.orbitalPeriod);
            printf("Mars:  Orbit Radius = %.3f AU, Orbital Period = %.2f days\n", mars.orbitRadius, mars.orbitalPeriod);
        } else if (choice == 'F' || choice == 'f') {
           printFormulae();
        } else if (choice == 'H' || choice == 'h') {
            hohmannTransferTime();
        } else if (choice == 'T' || choice == 't') {
            travelSystemExecute();
        } else if (choice == 'I' || choice == 'i') {
            printInfo(currentTime, shipPosition, currentLocation.name, currentLocation.description);
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
