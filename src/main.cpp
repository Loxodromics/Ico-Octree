#include "vector3.h"
#include "icosphere.h"
#include "face.h"
#include <iostream>

int main() {
	Vector3 myVector(1.0f, 2.0f, 3.0f);

	std::cout << "Vector3 Created: (" << myVector.x << ", " << myVector.y << ", " << myVector.z << ")" << std::endl;

	Icosphere icosphere;
	icosphere.subdivide(8);
	Face face;

	return 0;
}
