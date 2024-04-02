#include "face.h"
#include <iostream>


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
	if (index >= 0 && index < 4) {
		this->children[index] = std::move(child);
	}
}

void Face::addChild(std::shared_ptr<Face> child) {
	for (size_t index = 0; index < this->children.size(); ++index) {
		if (this->children[index] == nullptr) {
			this->setChild(index, child);
			return;
		}
	}
	std::cout << "added no child\n";
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

std::array<std::shared_ptr<Face>, 4> Face::getChildren() const {
	return this->children;
}

std::shared_ptr<Face> Face::getParent() const {
	if (!this->parent.expired())
		return this->parent.lock();
	else
		return nullptr;
}

std::array<int, 3> Face::getVertexIndices() const {
	return this->vertexIndices;
}
