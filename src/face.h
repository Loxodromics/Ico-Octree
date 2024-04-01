#pragma once

#include "vector3.h"
#include <array>
#include <memory> /// Include for smart pointers

class Face {
public:
	/// Constructor with vertex indices
	explicit Face(const std::array<int, 3>& vertexIndices);

	/// Default destructor - smart pointers handle their own memory
	~Face() = default;

	/// Rule of five: Define or delete copy/move constructors and assignment operators as needed
	Face(const Face& Other) = delete;
	Face& operator=(const Face& other) = delete;
	Face(Face&& Other) noexcept = default;
	Face& operator=(Face&& other) noexcept = default;

	void setData(float value);
	float getData() const;

	void setNeighbor(int Index, std::shared_ptr<Face> neighbor);
	std::shared_ptr<Face> getNeighbor(int index) const;
	void setChild(int Index, std::shared_ptr<Face> child);
	std::shared_ptr<Face> getChild(int index) const;
	std::array<std::shared_ptr<Face>, 3> getChildren() const;
	void setParent(std::weak_ptr<Face> parent);
	std::shared_ptr<Face> getParent() const; /// Gets the parent, converting the weak pointer to a shared pointer
	void setVertexIndices(const std::array<int, 3>& indices);
	std::array<int, 3> getVertexIndices() const;

private:
	/// Default constructor
	Face() = default;

	/// Data
	std::array<std::shared_ptr<Face>, 3> children;
	std::array<std::shared_ptr<Face>, 3> neighbors;
	std::weak_ptr<Face> parent; /// weak_ptr for a non-owning, nullable reference to the parent
	float data{0.0f};
	std::array<int, 3> vertexIndices{{-1, -1, -1}};
};