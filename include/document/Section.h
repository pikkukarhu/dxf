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

	virtual std::string to_json();
	virtual void write_json(const std::string& file);
};

} /* namespace dxf */

#endif /* DOCUMENT_SECTION_H_ */
