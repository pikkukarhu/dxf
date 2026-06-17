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

	virtual void resolve(const Tables& tables, bool isBlackBackground = true);

	virtual std::string to_json();
	virtual void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer);
};

} /* namespace dxf */

#endif /* BLOCKS_H_ */
