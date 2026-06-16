/*
 * LType.h
 *
 *  Created on: Jun 16, 2026
 *      Author: Gemini CLI
 */

#ifndef ENTRIES_LTYPE_H_
#define ENTRIES_LTYPE_H_

#include <vector>
#include <string>

#include "tables/TableEntry.h"
#include "file/File.h"

namespace dxf {

class LType final : public TableEntry {
private:
	struct Element {
		double dashLength = 0.0;	// 49
		int complexType = 0;		// 74
		int shapeNumber = 0;		// 75
		string stylePtr;			// 340
		double scale = 1.0;			// 46
		double rotation = 0.0;		// 50
		double xOffset = 0.0;		// 44
		double yOffset = 0.0;		// 45
		string text;				// 9
	};

	string ltypeName_;		// 2	Linetype name
	int flags_ = 0;			// 70	Standard flags
	string description_;	// 3	Descriptive text
	int alignmentCode_ = 0;	// 72	Alignment code; value is always 65 (the ASCII code for A)
	int numElements_ = 0;	// 73	The number of linetype elements
	double patternLength_ = 0.0; // 40	Total pattern length
	std::vector<Element> elements_;

public:
	LType(const std::vector<Group> &properties);
	virtual ~LType();

	virtual std::string getName() const override { return ltypeName_; }
	inline std::string getDescription() const { return description_; }

	virtual std::string toString();
	virtual std::string toJson();
	virtual void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
};

} /* namespace dxf */

#endif /* ENTRIES_LTYPE_H_ */
