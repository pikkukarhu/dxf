/*
 * Block.h
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */

#pragma once

#ifndef BLOCK_BLOCK_H_
#define BLOCK_BLOCK_H_

#include <string>
#include <vector>

#include "document/EntityFactory.h"
#include "document/Entity.h"
/*
 * BLock type bit-flags
 */
#define ANONYMOUS_BLOCK 			1
#define HAS_NONCONSTANT_ATTRIBUTES	2
#define EXTERNAL_REFERENCE			4
#define XREF_OVERLAY				8
#define EXTERNALLY_DEPENDENT_BLOCK	16
#define RESOLVED_EXTERNAL_REF		32
#define REFERENCED_EXTERNAL_REF		64


using std::string;
using std::vector;

namespace dxf {

class Block {
private:
	EntityFactory entityFactor_;

	// Header properties
	string handle_;
	/*
	 * 102 .. application defined codes
	 */
	string ownerHandle_;
	string subclassMarker_;	// = "AcDbEntity"
	string blockBegin_;		// ???
	string layerName_;
	string blockName_;
	unsigned int blockType_ = 0;
	Entity::Point_ basePoint_;
	string xrefPath_;
	string description_;
	string blockEnd_;

	// Entities
	vector<Entity*> entities_;



public:
	inline Block() {};
	virtual ~Block();

	void initBlock(const vector<Group> &properties);
	void finalizeBlock(const vector<Group> &properties);
	void addEntity(const vector<Group> &properties);

	inline bool isAnonymous() { return this->blockType_ & ANONYMOUS_BLOCK; }
	inline bool hasNonconstAttributes() { return this->blockType_ & HAS_NONCONSTANT_ATTRIBUTES; }
	inline bool isExternelReference() { return this->blockType_ & EXTERNAL_REFERENCE; }
	inline bool isXRefOverlay() { return this->blockType_ & XREF_OVERLAY; }
	inline bool isExternallyDependent() { return this->blockType_ & EXTERNALLY_DEPENDENT_BLOCK; }
	inline bool isResolvedXRer() { return this->blockType_ & RESOLVED_EXTERNAL_REF; }
	inline bool referencedXRef() { return this->blockType_ & REFERENCED_EXTERNAL_REF; }

	string toString();
	string toJson();
};

} /* namespace dxf */

#endif /* BLOCK_BLOCK_H_ */
