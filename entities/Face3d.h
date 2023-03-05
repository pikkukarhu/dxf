/*
 * Face3d.h
 *
 *  Created on: 16 Apr 2022
 *      Author: hobbes
 */

#ifndef ENTITIES_FACE3D_H_
#define ENTITIES_FACE3D_H_

#include "document/Entity.h"

#define __FIRST_EDGE_INVISIBLE 1
#define __SECOND_EDGE_INVISIBLE 2
#define __THIRD_EDGE_INVISIBLE 4
#define __FOURTH_EDGE_INVISIBLE 8

namespace dxf {

class Face3d final : public Entity {
private:
	Point_ first_;		// Group codes 10, 20, 30
	Point_ second_;		// Group codes 11, 21, 31
	Point_ third_;		// Group codes 12, 22, 32
	Point_ fourth_;		// Group codes 13, 23, 33
	unsigned int invisible_edges_;

public:
	Face3d(const vector<Group> &properties);
	virtual ~Face3d();

	inline bool* invisibleEdges() {
		static bool r[] = {
			invisible_edges_ & __FIRST_EDGE_INVISIBLE,
			invisible_edges_ & __SECOND_EDGE_INVISIBLE,
			invisible_edges_ & __THIRD_EDGE_INVISIBLE,
			invisible_edges_ & __FOURTH_EDGE_INVISIBLE
		};
		return r;
	}

	virtual string toJson();
};

} /* namespace dxf */

#endif /* ENTITIES_FACE3D_H_ */
