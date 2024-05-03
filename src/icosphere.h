#pragma once

#include "vector3.h"
#include "face.h"
#include <vector>
#include <map>

class Icosphere {
public:
	Icosphere();
	~Icosphere();

	/// Methods for icosphere generation and manipulation
	void subdivide(int levels); // Adds a method to subdivide the icosphere

	/// Accessors
	std::vector<Vector3> getVertices() const;
	std::vector<int> getIndices() const;

	/// Visitor
	void applyVisitorToFace(std::shared_ptr<Face> face, FaceVisitor& visitor);
	void applyVisitor(FaceVisitor& visitor);

private:
	/// Copy constructor
	Icosphere(const Icosphere& other);
	/// Assignment operator
	Icosphere& operator=(const Icosphere& other);

	void initializeBaseIcosahedron();

	/// Helper methods
	int addVertex(const Vector3 vertex);
	std::shared_ptr<Face> addFace(int v1, int v2, int v3);

	int getOrCreateMidpointIndex(int index1, int index2); /// Helper to handle midpoint vertices
	void subdivideFace(std::shared_ptr<Face> face, int currentLevel, int targetLevel);

	void setNeighbors();
	void setNeighborsForBaseFaces();
	void setNeighborsForFace(std::shared_ptr<Face> face);

	/// Data
	std::vector<Vector3> vertices;
	std::vector<int> indices;
	std::map<std::pair<int, int>, int> midpointIndexCache; /// Cache to store midpoints
	std::vector<std::shared_ptr<Face>> baseFaces;
};
