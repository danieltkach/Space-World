#include "planet.h"
#include <math.h>
#define PI 3.141592653589793

Vector3D getPlanetPosition(Planet planet, double time) {
    double angle = 2 * PI * (time / planet.orbitalPeriod);
    Vector3D pos;
    pos.x = planet.orbitRadius * cos(angle);
    pos.y = planet.orbitRadius * sin(angle);
    pos.z = 0.0;
    return pos;
}

double calculateDistance(Vector3D a, Vector3D b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}
