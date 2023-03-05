/*
 * Blocks.cpp
 *
 *  Created on: 8 Apr 2022
 *      Author: hobbes
 */

#include "blocks/Blocks.h"
#include <fstream>
#include <iostream>

#define __UNKNOWN 0
#define __BLOCK 1
#define __ENDBLK 2
#define __ENTITIES 3

using std::cout;
using std::endl;
using std::cerr;

namespace dxf {

Blocks::Blocks() {
	// TODO Auto-generated constructor stub

}

Blocks::~Blocks() {
	// TODO Auto-generated destructor stub
}


void Blocks::read(File* f) {

    vector<Group> buffer;
    Block* currentBlock = nullptr;
    unsigned int paragraph = __UNKNOWN;

    Group g;
    while (f->readGroup(g)) {

    	// Some point read entities of block with blocs Entities - section;
    	if (g.groupcode == 0) {
    		// Finalize reading this pat (BLOCK, BLKEND, ENTITYES
    		if (paragraph == __BLOCK){
    			currentBlock->initBlock(buffer);
    		}
    		else if (paragraph == __ENDBLK) {
    			currentBlock->finalizeBlock(buffer);
    		}
    		else if (paragraph == __ENTITIES) {
    			currentBlock->addEntity(buffer);
    		}
    		else {
    			// Do nothing, just dropped in
    		}
    		buffer.clear();

    		if ( g.value == "ENDSEC") {
    			// Print blocks
    			for (unsigned i = 0; i < this->blocks_.size(); ++i) {
    				cout << this->blocks_[i]->toJson() << endl;
    			}
    			cout << "] {Blocks} ENDSEC " << endl;
    		    return;
    		}
    		else if (g.value == "BLOCK") {
    			// Start new block
    			currentBlock = new Block();
    			this->blocks_.push_back(currentBlock);
    			paragraph = __BLOCK;
    		}
    		else if (g.value == "ENDBLK") { // table ready
    			// Finalize block
    			paragraph = __ENDBLK;
    		}
    		else {
    			// .... ENtity-part;
    			paragraph = __ENTITIES;
    			buffer.push_back(g); // Save 0-group, it has entity type
    		}
    	}
    	else {
    		buffer.push_back(g);	// Add to data vector, used to create new object (Table or table entry) when next start or table ends.
    	}
    }
    cerr << "Unexpected end of file" << endl;
}

} /* namespace dxf */
