#pragma once

#include "face.h"

class DataSettingVisitor : public FaceVisitor {
public:
	void visit(std::shared_ptr<Face> face) override;
	
private:
	static float calculateDataForFace(const std::shared_ptr<Face>& face);
};
