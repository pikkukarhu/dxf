/*
 * Section.h
 *
 *  Created on: 5 Apr 2022
 *      Author: hobbes
 */

#ifndef DOCUMENT_SECTION_H_
#define DOCUMENT_SECTION_H_

#include "file/File.h"

namespace dxf {

class Section {
public:
	inline Section() {};
	inline virtual ~Section() {};

	virtual void read(File* f);
};

} /* namespace dxf */

#endif /* DOCUMENT_SECTION_H_ */
