/*
 * Style.h
 *
 *  Created on: Jun 16, 2026
 *      Author: Gemini CLI
 */

#ifndef ENTRIES_STYLE_H_
#define ENTRIES_STYLE_H_

#include <vector>
#include <string>

#include "tables/TableEntry.h"
#include "file/File.h"

using std::vector;
using std::string;

namespace dxf {

class Style final : public TableEntry {
private:
	string styleName_;			// 2	Style name
	int flags_ = 0;				// 70	Standard flags
	double fixedTextHeight_ = 0.0;	// 40	Fixed text height; 0 if not fixed
	double widthFactor_ = 1.0;		// 41	Width factor
	double obliqueAngle_ = 0.0;		// 50	Oblique angle
	int textGenerationFlags_ = 0;	// 71	Text generation flags
	double lastHeightUsed_ = 0.0;	// 42	Last height used
	string primaryFontFile_;		// 3	Primary font file name
	string bigFontFile_;			// 4	Big font file name (blank if none)

public:
	Style(const vector<Group> &properties);
	virtual ~Style();

	virtual string getName() const override { return styleName_; }
	inline string getPrimaryFontFile() const { return primaryFontFile_; }

	virtual string toString();
	virtual string toJson();
	virtual void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
};

} /* namespace dxf */

#endif /* ENTRIES_STYLE_H_ */
