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
	[[nodiscard]] std::vector<Vector3> getVertices() const;
	[[nodiscard]] std::vector<unsigned int> getIndices() const;

	/// Visitor
	static void applyVisitorToFace(const std::shared_ptr<Face> &face, FaceVisitor& visitor);
	void applyVisitor(FaceVisitor& visitor) const;

private:
	/// Copy constructor
	Icosphere(const Icosphere& other);
	/// Assignment operator
	Icosphere& operator=(const Icosphere& other);

	void initializeBaseIcosahedron();

	/// Helper methods
	unsigned int addVertex(Vector3 vertex);
	std::shared_ptr<Face> addFace(unsigned int v1, unsigned int v2, unsigned int v3);

	unsigned int getOrCreateMidpointIndex(unsigned int index1, unsigned int index2); /// Helper to handle midpoint vertices
	void subdivideFace(const std::shared_ptr<Face> &face, unsigned int currentLevel, unsigned int targetLevel);

	void setNeighbors();
	void setNeighborsForBaseFaces() const;
	void setNeighborsForFace(const std::shared_ptr<Face>& face);

	/// Data
	std::vector<Vector3> vertices;
	std::vector<unsigned int> indices;
	std::map<std::pair<unsigned int, unsigned int>, unsigned int> midpointIndexCache; /// Cache to store midpoints
	std::vector<std::shared_ptr<Face>> baseFaces;
};
