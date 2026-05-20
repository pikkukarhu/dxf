/*
 * AttDef.h
 *
 *  Created on: 19 Apr 2022
 *      Author: hobbes
 */

#ifndef ENTITIES_ATTDEF_H_
#define ENTITIES_ATTDEF_H_

#include <vector>
#include "document/Entity.h"

using std::vector;
using std::string;

namespace dxf {

class AttDef final : public Entity {
private:
	string tag_;
	unsigned int flags_ = 0;
	unsigned int fieldLength_ = 0;
	unsigned int verticalTestJustification_ = 0;
	string lockPositionFlag_;
	unsigned int dublicateRecordCloningFlag_ = 0;
	unsigned int mtextFlag_ = 0;
	unsigned int isReallyLockedFlag_ = 0;
	unsigned int numOfSecondaryAttributes_ = 0;
	string secondaryAttributePointer_;
	Point_ aligmentPt_;
	double annotationScale_;
	string tagString_;

public:
	AttDef(const vector<Group> &properties);
	virtual ~AttDef();
};

} /* namespace dxf */

#endif /* ENTITIES_ATTDEF_H_ */
