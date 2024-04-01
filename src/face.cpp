#include "face.h"

Face::Face(const std::array<int, 3>& vertexIndices)
: vertexIndices(vertexIndices) {

}

/// Setters
void Face::setData(float value) {
	this->data = value;
}

void Face::setNeighbor(int index, std::shared_ptr<Face> neighbor) {
	if (index >= 0 && index < 3) {
		this->neighbors[index] = std::move(neighbor);
	}
}

void Face::setChild(int index, std::shared_ptr<Face> child) {
	if (index >= 0 && index < 3) {
		this->children[index] = std::move(child);
	}
}

void Face::setParent(std::weak_ptr<Face> parent) {
	this->parent = std::move(parent);
}

void Face::setVertexIndices(const std::array<int, 3>& indices) {
	this->vertexIndices = indices;
}

/// Getters
float Face::getData() const {
	return this->data;
}

std::shared_ptr<Face> Face::getNeighbor(int index) const {
	if (index >= 0 && index < 3) {
		return this->neighbors[index];
	}
	return nullptr;
}

std::shared_ptr<Face> Face::getChild(int index) const {
	if (index >= 0 && index < 3) {
		return this->children[index];
	}
	return nullptr;
}

std::array<std::shared_ptr<Face>, 3> Face::getChildren() const {
	return this->children;
}

std::shared_ptr<Face> Face::getParent() const {
	return this->parent.lock();
}

std::array<int, 3> Face::getVertexIndices() const {
	return this->vertexIndices;
}
