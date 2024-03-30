#ifndef ICOSPHERE_H
#define ICOSPHERE_H

#include "vector3.h"
#include <vector>
#include <map>

class Icosphere {
public:
	// Constructor
	Icosphere();

	// Destructor
	~Icosphere();

	// Copy constructor
	Icosphere(const Icosphere& other);

	// Assignment operator
	Icosphere& operator=(const Icosphere& other);

	// Methods for icosphere generation and manipulation
	// void generate(int subdivisionLevel);
	void subdivide(int levels); // Adds a method to subdivide the icosphere
	std::vector<Vector3> getVertices() const;
	std::vector<int> getIndices() const;

private:
	std::vector<Vector3> vertices;
	std::vector<int> indices;

	// Helper methods
	void subdivide();
	int addVertex(const Vector3& vertex);
	void addFace(int v1, int v2, int v3);

	int getOrCreateMidpointIndex(int index1, int index2); // Helper to handle midpoint vertices
	std::map<std::pair<int, int>, int> midpointIndexCache; // Cache to store midpoints
};

#endif // ICOSPHERE_H
