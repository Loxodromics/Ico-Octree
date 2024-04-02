#include "icosphere.h"
#include "vector3.h"

#include <algorithm> /// For std::min and std::max
#include <map>
#include <utility> /// For std::pair
#include <iostream>

Icosphere::Icosphere() {
	this->initializeBaseIcosahedron();
}


Icosphere::~Icosphere() {
	/// Cleanup if needed
}

Icosphere::Icosphere(const Icosphere& other) : vertices(other.vertices), indices(other.indices) {
	/// Copy constructor implementation
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

int Icosphere::addVertex(const Vector3 vertex) {
	vertices.push_back(vertex);
	/// Assuming the vertices vector is zero-indexed, the position of the newly added vertex
	/// is at the size of the vector before the push_back minus 1.
	return vertices.size() - 1;
}

std::shared_ptr<Face> Icosphere::addFace(int v1, int v2, int v3) {
	std::cout << "addFace(" << v1 << ", " << v2 << ", " << v3 <<")\n";
	/// Adding indices for a triangular face
	indices.push_back(v3);
	indices.push_back(v2);
	indices.push_back(v1);

	/// Create and store the Face object
	std::shared_ptr<Face> face = std::make_shared<Face>(std::array<int, 3>{v3, v2, v1});
	return face;
}

void Icosphere::subdivide(int levels) {
	for (auto& baseFace : baseFaces) {
		subdivideFace(baseFace, 0, levels);
	}

	// After subdivision, we run a separate function
	// to recursively set neighbors for each face.
	this->setNeighbors();
}

int Icosphere::getOrCreateMidpointIndex(int index1, int index2) {
	/// Ensure the first index is always the smaller one to avoid duplicates
	std::pair<int, int> key(std::min(index1, index2), std::max(index1, index2));
	std::cout << "getOrCreateMidpointIndex(" << key.first << ", " << key.second << "): ";

	/// Check if this midpoint has already been created
	if (midpointIndexCache.find(key) != midpointIndexCache.end()) {
		std::cout << "found key: " << midpointIndexCache[key] << std::endl;
		return midpointIndexCache[key];
	}

	/// Create a new midpoint vertex, then normalize it to ensure it's on the unit sphere
	Vector3 midpoint = (vertices[index1] + vertices[index2]) * 0.5f;
	midpoint.normalize();
	int midpointIndex = addVertex(midpoint);
	std::cout << "add vertex: " << midpointIndex << std::endl;

	/// Add to cache
	midpointIndexCache[key] = midpointIndex;

	return midpointIndex;
}

void Icosphere::initializeBaseIcosahedron() {
	this->vertices.clear();
	this->indices.clear();
	this->midpointIndexCache.clear();

	float phi = (1.0f + sqrt(5.0f)) * 0.5f; /// golden ratio
	float a = 1.0f;
	float b = 1.0f / phi;

	/// Add vertices
	this->addVertex(Vector3(0, b, -a).normalized());  // v0
	this->addVertex(Vector3(b, a, 0).normalized());   // v1
	this->addVertex(Vector3(-b, a, 0).normalized());  // v2
	// this->addVertex(Vector3(0, b, a).normalized());   // v3
	// this->addVertex(Vector3(0, -b, a).normalized());  // v4
	// this->addVertex(Vector3(-a, 0, b).normalized());  // v5
	// this->addVertex(Vector3(0, -b, -a).normalized()); // v6
	// this->addVertex(Vector3(a, 0, -b).normalized());  // v7
	// this->addVertex(Vector3(a, 0, b).normalized());   // v8
	// this->addVertex(Vector3(-a, 0, -b).normalized()); // v9
	// this->addVertex(Vector3(b, -a, 0).normalized());  // v10
	// this->addVertex(Vector3(-b, -a, 0).normalized()); // v11

	/// Add faces
	baseFaces.push_back(this->addFace(2, 1, 0));
	// baseFaces.push_back(this->addFace(2, 3, 1));
	// baseFaces.push_back(this->addFace(5, 4, 3));
	// baseFaces.push_back(this->addFace(4, 8, 3));
	// baseFaces.push_back(this->addFace(7, 6, 0));
	// baseFaces.push_back(this->addFace(6, 9, 0));
	// baseFaces.push_back(this->addFace(11, 10, 4));
	// baseFaces.push_back(this->addFace(10, 11, 6));
	// baseFaces.push_back(this->addFace(9, 5, 2));
	// baseFaces.push_back(this->addFace(5, 9, 11));
	// baseFaces.push_back(this->addFace(8, 7, 1));
	// baseFaces.push_back(this->addFace(7, 8, 10));
	// baseFaces.push_back(this->addFace(2, 5, 3));
	// baseFaces.push_back(this->addFace(8, 1, 3));
	// baseFaces.push_back(this->addFace(9, 2, 0));
	// baseFaces.push_back(this->addFace(1, 7, 0));
	// baseFaces.push_back(this->addFace(11, 9, 6));
	// baseFaces.push_back(this->addFace(7, 10, 6));
	// baseFaces.push_back(this->addFace(5, 11, 4));
	// baseFaces.push_back(this->addFace(10, 8, 4));
}

void Icosphere::subdivideFace(std::shared_ptr<Face> face, int currentLevel, int targetLevel) {
	if (currentLevel >= targetLevel) {
		return; /// Base case: Reached the desired level of subdivision
	}
	std::cout << "subdivideFace(" << face->getVertexIndices()[0] << ", "
		<< face->getVertexIndices()[1] << ", "
		<< face->getVertexIndices()[2] << "): " << currentLevel << " : " << targetLevel << std::endl;

	/// Calculate midpoints and create new vertices (if necessary)
	int mid1 = getOrCreateMidpointIndex(face->getVertexIndices()[0], face->getVertexIndices()[1]);
	int mid2 = getOrCreateMidpointIndex(face->getVertexIndices()[1], face->getVertexIndices()[2]);
	int mid3 = getOrCreateMidpointIndex(face->getVertexIndices()[2], face->getVertexIndices()[0]);

	/// Create new faces using the original vertices and the new midpoints
	// std::array<std::shared_ptr<Face>, 4> newFaces = {
	// 	std::make_shared<Face>(std::array<int, 3>{face->getVertexIndices()[0], mid1, mid3}),
	// 	std::make_shared<Face>(std::array<int, 3>{mid1, face->getVertexIndices()[1], mid2}),
	// 	std::make_shared<Face>(std::array<int, 3>{mid3, mid2, face->getVertexIndices()[2]}),
	// 	std::make_shared<Face>(std::array<int, 3>{mid1, mid2, mid3})
	// };
	std::array<std::shared_ptr<Face>, 4> newFaces = {
		this->addFace(face->getVertexIndices()[0], mid1, mid3),
		this->addFace(mid1, face->getVertexIndices()[1], mid2),
		this->addFace(mid3, mid2, face->getVertexIndices()[2]),
		this->addFace(mid1, mid2, mid3)
	};

	/// Set parent-child relationships
	for (auto& newFace : newFaces) {
		newFace->setParent(face);
		face->addChild(newFace);
	}

	/// Recursively subdivide the new faces
	for (auto& newFace : newFaces) {
		subdivideFace(newFace, currentLevel + 1, targetLevel);
	}
}

void Icosphere::setNeighbors() {
	this->setNeighborsForBaseFaces();

	for (auto baseFace : this->baseFaces) {
		for (auto face : baseFace->getChildren()) {
			if (face)
				this->setNeighborsForFace(face);
			else
				std::cout << "no child\n";
		}
	}
}

void Icosphere::setNeighborsForBaseFaces() {
	for (auto& currentFace : this->baseFaces) {
		std::array<int, 3> currentIndices = currentFace->getVertexIndices();
		std::sort(currentIndices.begin(), currentIndices.end());

		int neighborCount = 0;

		for (auto& potentialNeighbor : this->baseFaces) {
			if (currentFace == potentialNeighbor) continue; /// Skip the same face

			std::array<int, 3> neighborIndices = potentialNeighbor->getVertexIndices();
			std::sort(neighborIndices.begin(), neighborIndices.end());

			std::array<int, 3> intersection;
			auto it = std::set_intersection(currentIndices.begin(), currentIndices.end(),
											neighborIndices.begin(), neighborIndices.end(),
											intersection.begin());
			size_t matches = it - intersection.begin();

			if (matches == 2) { /// If exactly two indices match, it's a neighbor
				std::cout << "setNeighbor(" << potentialNeighbor->getVertexIndices()[0] << ", "
					<< potentialNeighbor->getVertexIndices()[1] << ", "
					<< potentialNeighbor->getVertexIndices()[2] << ")\n";
				currentFace->setNeighbor(neighborCount++, potentialNeighbor);
				if (neighborCount == 3) break; /// Each face has exactly 3 neighbors
			}
		}
		std::cout << "setNeighborsForBaseFaces found " << neighborCount << " neighbors\n";
	}
}


void Icosphere::setNeighborsForFace(std::shared_ptr<Face> face) {
	auto parent = face->getParent();
	if (!parent)
		return;

	std::cout << "setNeighborsForFace\n";

	int neighborCount = 0;
	std::array<int, 3> myIndices = face->getVertexIndices();
	std::sort(myIndices.begin(), myIndices.end());

	/// check my siblings first
	for (auto sibling : parent->getChildren()) {
		if (!sibling)
			continue; /// Skip if sibling has no child at this index

		std::array<int, 3> siblingIndices = sibling->getVertexIndices();
		std::sort(siblingIndices.begin(), siblingIndices.end());

		/// Count matching indices
		std::array<int, 3> intersection;
		auto it = std::set_intersection(myIndices.begin(), myIndices.end(),
										siblingIndices.begin(), siblingIndices.end(),
										intersection.begin());
		size_t matches = it - intersection.begin();
		std::cout << "myIndices:      " << myIndices[0] << ", " << myIndices[1] << ", " << myIndices[2] << "\n";
		std::cout << "siblingIndices: " << siblingIndices[0] << ", " << siblingIndices[1] << ", " << siblingIndices[2] << "\n";
		std::cout << "setNeighborsForFace, intersections: " << matches << "\n";

		/// If exactly two indices match, it's a neighbor
		if (matches == 2) {
			std::cout << "setNeighbor(" << sibling->getVertexIndices()[0] << ", "
				<< sibling->getVertexIndices()[1] << ", "
				<< sibling->getVertexIndices()[2] << ")\n";
			// face->setNeighbor(neighborCount++, sibling);
			neighborCount++;
			face->addNeighbor(sibling);
		}
	}

	/// Now check my cusins 
	auto grandparent = parent->getParent();
	if (grandparent) {

		std::array<std::shared_ptr<Face>, 4> siblings = grandparent->getChildren();

		/// Check siblings of the parent (which are my siblings and cousins) to find neighbors
		for (auto& sibling : siblings) {
			if (sibling == parent || !sibling)
				continue; /// Skip my parent (did that already) and any null siblings

			// std::array<int, 3> myIndices = face->getVertexIndices();
			// std::sort(myIndices.begin(), myIndices.end());

			for (int i = 0; i < 4; ++i) { /// Checking all 4 children
				auto siblingChild = sibling->getChild(i);
				if (!siblingChild)
					continue; /// Skip if sibling has no child at this index

				std::array<int, 3> siblingIndices = siblingChild->getVertexIndices();
				std::sort(siblingIndices.begin(), siblingIndices.end());

				/// Count matching indices
				std::array<int, 3> intersection;
				auto it = std::set_intersection(myIndices.begin(), myIndices.end(),
												siblingIndices.begin(), siblingIndices.end(),
												intersection.begin());
				size_t matches = it - intersection.begin();
				std::cout << "myIndices:      " << myIndices[0] << ", " << myIndices[1] << ", " << myIndices[2] << "\n";
				std::cout << "siblingIndices: " << siblingIndices[0] << ", " << siblingIndices[1] << ", " << siblingIndices[2] << "\n";
				std::cout << "setNeighborsForFace, intersections: " << matches << "\n";

				/// If exactly two indices match, it's a neighbor
				if (matches == 2) {
					std::cout << "setNeighbor(" << siblingChild->getVertexIndices()[0] << ", "
						<< siblingChild->getVertexIndices()[1] << ", "
						<< siblingChild->getVertexIndices()[2] << ")\n";
					// face->setNeighbor(neighborCount++, siblingChild);
					neighborCount++;
					face->addNeighbor(siblingChild);
				}
			}
		}
	}
	std::cout << "setNeighborsForFace found " << neighborCount << " neighbors\n";

	/// Recursively set neighbors for children
	for (int i = 0; i < 4; ++i) { /// Updated to iterate over all 4 children
		if (face->getChild(i))
			this->setNeighborsForFace(face->getChild(i));
		else
			std::cout << "no child\n";
	}
}
