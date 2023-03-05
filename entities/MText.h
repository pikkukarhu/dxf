/*
 * MText.h
 *
 *  Created on: 21 Apr 2022
 *      Author: hobbes
 */

#ifndef ENTITIES_MTEXT_H_
#define ENTITIES_MTEXT_H_

#include "document/Entity.h"

namespace dxf {

class MText final : public Entity {
public:
	MText(const vector<Group> &properties);
	virtual ~MText();
};

} /* namespace dxf */

#endif /* ENTITIES_MTEXT_H_ */
