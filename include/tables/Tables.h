/*
 * Tables.h
 *
 *  Created on: 6 Apr 2022
 *      Author: hobbes
 */
#pragma once

#ifndef TABLES_TABLES_H_
#define TABLES_TABLES_H_

#include "document/Section.h"

#include "tables/Table.h"

namespace dxf {

class Tables final : public Section {
private:
	vector<Table *> tables_;

public:
	Tables();
	virtual ~Tables();

	virtual void read(File* f);
};

} /* namespace dxf */

#endif /* TABLES_TABLES_H_ */
