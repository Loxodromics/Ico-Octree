#pragma once

#include "face.h"

class DataSettingVisitor : public FaceVisitor {
public:
	void visit(std::shared_ptr<Face> face) override;
	
private:
	float calculateDataForFace(std::shared_ptr<Face> face);
};
