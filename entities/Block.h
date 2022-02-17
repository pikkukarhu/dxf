/*
 * Block.h
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */

#pragma once

#ifndef BLOCK_BLOCK_H_
#define BLOCK_BLOCK_H_

namespace dxf {

class Block {
public:
	Block();
	virtual ~Block();
	Block(const Block &other);
	Block(Block &&other);
};

} /* namespace dxf */

#endif /* BLOCK_BLOCK_H_ */
