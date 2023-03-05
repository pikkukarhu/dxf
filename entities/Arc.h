/*
 * Arc.h
 *
 *  Created on: 16 Apr 2022
 *      Author: hobbes
 */

#ifndef ENTITIES_ARC_H_
#define ENTITIES_ARC_H_

#include <vector>
#include <string>

#include "document/Entity.h"

namespace dxf {

using std::vector;
using std::string;

class Arc final : public Entity {
private:
	Point_ centerPoint_;	// 10, 20, 30
	double radius_;			// 40
	double startAngle_;		// 50
	double endAngle_;		// 51

	double thicness_ = 0.0;	// 39
	Point_ extrDir_ = {0.0, 0.0, 1.0};	// 210, 220, 230

public:
	Arc(const vector<Group> &properties);
	virtual ~Arc();

	virtual string toJson();
};

} /* namespace dxf */

#endif /* ENTITIES_ARC_H_ */
