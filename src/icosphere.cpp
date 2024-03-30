#include "icosphere.h"
#include "vector3.h"

#include <algorithm> // For std::min and std::max
#include <map>
#include <utility> // For std::pair

Icosphere::Icosphere() {
	float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
	float a = 1.0f;
	float b = 1.0f / phi;

	// Add vertices
	this->addVertex(Vector3(0, b, -a));  // v0
	this->addVertex(Vector3(b, a, 0));   // v1
	this->addVertex(Vector3(-b, a, 0));  // v2
	this->addVertex(Vector3(0, b, a));   // v3
	this->addVertex(Vector3(0, -b, a));  // v4
	this->addVertex(Vector3(-a, 0, b));  // v5
	this->addVertex(Vector3(0, -b, -a)); // v6
	this->addVertex(Vector3(a, 0, -b));  // v7
	this->addVertex(Vector3(a, 0, b));   // v8
	this->addVertex(Vector3(-a, 0, -b)); // v9
	this->addVertex(Vector3(b, -a, 0));  // v10
	this->addVertex(Vector3(-b, -a, 0)); // v11

	// Add faces
	this->addFace(2, 1, 0);
	this->addFace(2, 3, 1);
	this->addFace(5, 4, 3);
	this->addFace(4, 8, 3);
	this->addFace(7, 6, 0);
	this->addFace(6, 9, 0);
	this->addFace(11, 10, 4);
	this->addFace(10, 11, 6);
	this->addFace(9, 5, 2);
	this->addFace(5, 9, 11);
	this->addFace(8, 7, 1);
	this->addFace(7, 8, 10);
	this->addFace(2, 5, 3);
	this->addFace(8, 1, 3);
	this->addFace(9, 2, 0);
	this->addFace(1, 7, 0);
	this->addFace(11, 9, 6);
	this->addFace(7, 10, 6);
	this->addFace(5, 11, 4);
	this->addFace(10, 8, 4);
}


Icosphere::~Icosphere() {
	// Cleanup if needed
}

Icosphere::Icosphere(const Icosphere& other) : vertices(other.vertices), indices(other.indices) {
	// Copy constructor implementation
}

Icosphere& Icosphere::operator=(const Icosphere& other) {
	if (this != &other) {
		vertices = other.vertices;
		indices = other.indices;
	}
	return *this;
}

std::vector<Vector3> Icosphere::getVertices() const {
	return vertices;
}

std::vector<int> Icosphere::getIndices() const {
	return indices;
}

int Icosphere::addVertex(const Vector3& vertex) {
	vertices.push_back(vertex);
	// Assuming the vertices vector is zero-indexed, the position of the newly added vertex
	// is at the size of the vector before the push_back minus 1.
	return vertices.size() - 1;
}

void Icosphere::addFace(int v1, int v2, int v3) {
	// Adding indices for a triangular face
	indices.push_back(v1);
	indices.push_back(v2);
	indices.push_back(v3);
}

void Icosphere::subdivide(int levels) {
	for (int level = 0; level < levels; ++level) {
		std::vector<int> newIndices;
		midpointIndexCache.clear(); // Reset cache for each subdivision level

		for (size_t i = 0; i < indices.size(); i += 3) {
			int v1 = indices[i];
			int v2 = indices[i + 1];
			int v3 = indices[i + 2];

			// Get new vertices in the middle of each edge of the triangle
			int a = getOrCreateMidpointIndex(v1, v2);
			int b = getOrCreateMidpointIndex(v2, v3);
			int c = getOrCreateMidpointIndex(v3, v1);

			// Replace the current triangle with 4 new triangles
			// The order of vertices for these triangles should ensure normals are consistent
			newIndices.push_back(v1); newIndices.push_back(a); newIndices.push_back(c);
			newIndices.push_back(v2); newIndices.push_back(b); newIndices.push_back(a);
			newIndices.push_back(v3); newIndices.push_back(c); newIndices.push_back(b);
			newIndices.push_back(a); newIndices.push_back(b); newIndices.push_back(c);
		}

		// Use the new set of indices to replace the old one
		indices = newIndices;
	}
}

int Icosphere::getOrCreateMidpointIndex(int index1, int index2) {
	// Ensure the first index is always the smaller one to avoid duplicates
	std::pair<int, int> key(std::min(index1, index2), std::max(index1, index2));

	// Check if this midpoint has already been created
	if (midpointIndexCache.find(key) != midpointIndexCache.end()) {
		return midpointIndexCache[key];
	}

	// Create a new midpoint vertex, then normalize it to ensure it's on the unit sphere
	Vector3 midpoint = (vertices[index1] + vertices[index2]) * 0.5f;
	midpoint.normalize();
	int midpointIndex = addVertex(midpoint);

	// Add to cache
	midpointIndexCache[key] = midpointIndex;

	return midpointIndex;
}
