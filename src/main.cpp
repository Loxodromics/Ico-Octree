
#include "icosphere.h"
#include "datasettingvisitor.h"
#include <iostream>

int main() {

	Icosphere icosphere;
	icosphere.subdivide(2);

	DataSettingVisitor dataVisitor;
	icosphere.applyVisitor(dataVisitor);

	return 0;
}
