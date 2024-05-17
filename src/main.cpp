
#include "icosphere.h"
#include "datasettingvisitor.h"

int main() {

	Icosphere icosphere;
	icosphere.subdivide(2);

	DataSettingVisitor dataVisitor;
	icosphere.applyVisitor(dataVisitor);

	return 0;
}
