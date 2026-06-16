/*
 * LType.cpp
 *
 *  Created on: Jun 16, 2026
 *      Author: Gemini CLI
 */

#include "entries/LType.h"

#include <vector>
#include <string>

#include "tables/TableEntry.h"
#include "file/File.h"

using std::vector;
using std::string;
using std::to_string;

namespace dxf {

LType::LType(const vector<Group> &properties) : TableEntry(properties) {

	for (unsigned int i = 0; i < properties.size(); ++i) {
		Group g = properties[i];

		switch (g.groupcode) {
			case 2:		// 2	Linetype name
				this->ltypeName_ = g.value;
				break;
			case 70:	// 70	Standard flags
				this->flags_ = stoi(g.value);
				break;
			case 3:		// 3	Descriptive text
				this->description_ = g.value;
				break;
			case 72:	// 72	Alignment code
				this->alignmentCode_ = stoi(g.value);
				break;
			case 73:	// 73	Number of elements
				this->numElements_ = stoi(g.value);
				break;
			case 40:	// 40	Total pattern length
				this->patternLength_ = stod(g.value);
				break;
			case 49:	// 49	Dash length
				elements_.push_back(Element());
				elements_.back().dashLength = stod(g.value);
				break;
			case 74:	// 74	Complex linetype element type
				if (!elements_.empty()) {
					elements_.back().complexType = stoi(g.value);
				}
				break;
			case 75:	// 75	Shape number
				if (!elements_.empty()) {
					elements_.back().shapeNumber = stoi(g.value);
				}
				break;
			case 340:	// 340	Pointer to STYLE object
				if (!elements_.empty()) {
					elements_.back().stylePtr = g.value;
				}
				break;
			case 46:	// 46	Scale value
				if (!elements_.empty()) {
					elements_.back().scale = stod(g.value);
				}
				break;
			case 50:	// 50	Rotation angle
				if (!elements_.empty()) {
					elements_.back().rotation = stod(g.value);
				}
				break;
			case 44:	// 44	X offset
				if (!elements_.empty()) {
					elements_.back().xOffset = stod(g.value);
				}
				break;
			case 45:	// 45	Y offset
				if (!elements_.empty()) {
					elements_.back().yOffset = stod(g.value);
				}
				break;
			case 9:		// 9	Text string
				if (!elements_.empty()) {
					elements_.back().text = g.value;
				}
				break;
		}
	}
}

LType::~LType() {
}

string LType::toString() {
	string s = TableEntry::toString();

	s += "\"ltype_name\" : \"" + this->ltypeName_ + "\", ";
	s += "\"flags\" : " + to_string(this->flags_) + ", ";
	s += "\"description\" : \"" + this->description_ + "\", ";
	s += "\"alignment_code\" : " + to_string(this->alignmentCode_) + ", ";
	s += "\"num_elements\" : " + to_string(this->numElements_) + ", ";
	s += "\"pattern_length\" : " + to_string(this->patternLength_) + ", ";

	return s;
}

string LType::toJson() {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	write_to_json_writer(writer);
	return buffer.GetString();
}

void LType::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	writer.Key("ltype");
	writer.StartObject();
	TableEntry::write_to_json_writer(writer);
	writer.Key("name");             writer.String(this->ltypeName_.c_str());
	writer.Key("flags");            writer.Int(this->flags_);
	writer.Key("description");      writer.String(this->description_.c_str());
	writer.Key("alignment_code");   writer.Int(this->alignmentCode_);
	writer.Key("num_elements");     writer.Int(this->numElements_);
	writer.Key("pattern_length");   writer.Double(this->patternLength_);
	
	writer.Key("elements");
	writer.StartArray();
	for (const auto& e : elements_) {
		writer.StartObject();
		writer.Key("dash_length");  writer.Double(e.dashLength);
		writer.Key("complex_type"); writer.Int(e.complexType);
		writer.Key("shape_number"); writer.Int(e.shapeNumber);
		writer.Key("style_ptr");    writer.String(e.stylePtr.c_str());
		writer.Key("scale");        writer.Double(e.scale);
		writer.Key("rotation");     writer.Double(e.rotation);
		writer.Key("x_offset");     writer.Double(e.xOffset);
		writer.Key("y_offset");     writer.Double(e.yOffset);
		writer.Key("text");         writer.String(e.text.c_str());
		writer.EndObject();
	}
	writer.EndArray();
	writer.EndObject();
	writer.EndObject();
}

} /* namespace dxf */
