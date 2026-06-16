/*
 * Style.cpp
 *
 *  Created on: Jun 16, 2026
 *      Author: Gemini CLI
 */

#include "entries/Style.h"

#include <vector>
#include <string>

#include "tables/TableEntry.h"
#include "file/File.h"

using std::vector;
using std::string;
using std::to_string;

namespace dxf {

Style::Style(const vector<Group> &properties) : TableEntry(properties) {

	for (unsigned int i = 0; i < properties.size(); ++i) {
		Group g = properties[i];

		switch (g.groupcode) {
			case 2:		// 2	Style name
				this->styleName_ = g.value;
				break;
			case 70:	// 70	Standard flags
				this->flags_ = stoi(g.value);
				break;
			case 40:	// 40	Fixed text height
				this->fixedTextHeight_ = stod(g.value);
				break;
			case 41:	// 41	Width factor
				this->widthFactor_ = stod(g.value);
				break;
			case 50:	// 50	Oblique angle
				this->obliqueAngle_ = stod(g.value);
				break;
			case 71:	// 71	Text generation flags
				this->textGenerationFlags_ = stoi(g.value);
				break;
			case 42:	// 42	Last height used
				this->lastHeightUsed_ = stod(g.value);
				break;
			case 3:		// 3	Primary font file name
				this->primaryFontFile_ = g.value;
				break;
			case 4:		// 4	Big font file name
				this->bigFontFile_ = g.value;
				break;
		}
	}
}

Style::~Style() {
}

string Style::toString() {
	string s = TableEntry::toString();

	s += "\"style_name\" : \"" + this->styleName_ + "\", ";
	s += "\"flags\" : " + to_string(this->flags_) + ", ";
	s += "\"fixed_text_height\" : " + to_string(this->fixedTextHeight_) + ", ";
	s += "\"width_factor\" : " + to_string(this->widthFactor_) + ", ";
	s += "\"oblique_angle\" : " + to_string(this->obliqueAngle_) + ", ";
	s += "\"text_generation_flags\" : " + to_string(this->textGenerationFlags_) + ", ";
	s += "\"last_height_used\" : " + to_string(this->lastHeightUsed_) + ", ";
	s += "\"primary_font_file\" : \"" + this->primaryFontFile_ + "\", ";
	s += "\"big_font_file\" : \"" + this->bigFontFile_ + "\", ";

	return s;
}

string Style::toJson() {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	write_to_json_writer(writer);
	return buffer.GetString();
}

void Style::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	writer.Key("style");
	writer.StartObject();
	TableEntry::write_to_json_writer(writer);
	writer.Key("name");                   writer.String(this->styleName_.c_str());
	writer.Key("flags");                  writer.Int(this->flags_);
	writer.Key("fixed_text_height");      writer.Double(this->fixedTextHeight_);
	writer.Key("width_factor");           writer.Double(this->widthFactor_);
	writer.Key("oblique_angle");          writer.Double(this->obliqueAngle_);
	writer.Key("text_generation_flags");  writer.Int(this->textGenerationFlags_);
	writer.Key("last_height_used");       writer.Double(this->lastHeightUsed_);
	writer.Key("primary_font_file");      writer.String(this->primaryFontFile_.c_str());
	writer.Key("big_font_file");          writer.String(this->bigFontFile_.c_str());
	writer.EndObject();
	writer.EndObject();
}

} /* namespace dxf */
