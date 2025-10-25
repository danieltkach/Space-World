#ifndef PLANET_H
#define PLANET_H

typedef struct {
    double x;
    double y;
    double z;
} Vector3D;

typedef struct {
    char name[32];
    double orbitRadius;    // in AU
    double orbitalPeriod;  // in days
} Planet;

Vector3D getPlanetPosition(Planet planet, double time);
double calculateDistance(Vector3D a, Vector3D b);

#endif
