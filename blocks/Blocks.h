/*
 * Blocks.h
 *
 *  Created on: 8 Apr 2022
 *      Author: hobbes
 */

#ifndef BLOCKS_H_
#define BLOCKS_H_

#include <vector>

#include "blocks/Block.h"
#include "document/Section.h"
#include "entities/Entities.h"

using std::vector;

namespace dxf {

class Blocks final : public Section {
private:
	vector<Block *> blocks_;
	Entities entities_;

public:
	Blocks();
	virtual ~Blocks();

	virtual void read(File* f);
};

} /* namespace dxf */

#endif /* BLOCKS_H_ */
