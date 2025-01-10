
#include "icosphere.h"
#include "datasettingvisitor.h"

int main() {
	lillugsi::planet::Icosphere icosphere;
	icosphere.subdivide(2);

	lillugsi::planet::DataSettingVisitor dataVisitor;
	icosphere.applyVisitor(dataVisitor);

	return 0;
}
