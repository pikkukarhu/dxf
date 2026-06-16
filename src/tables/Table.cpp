/*
 * Table.cpp
 *
 *  Created on: 25 Feb 2022
 *      Author: hobbes
 */

#include "tables/Table.h"


#include <vector>
#include <string>

#include "file/File.h"

using std::stoi;

namespace dxf {

Table::Table(const vector<Group> &properties) {

	for (unsigned int i = 0; i < properties.size(); ++ i) {
		Group g = properties[i];
		switch (g.groupcode) {
		case 0:			// 0	Object type (TABLE)
			this->objectType_ = g.value;
			break;
		case 2:
			this->tableName_ = g.value;		// 2	Table name
			break;
		case 5:
			this->handle_ = g.value;			// 5	Handle
			break;
		case 330:
			this->ownerSOftPtr_ = g.value;	// 330	Soft-pointer ID/handle to owner object
			break;
		case 100:
			this->subclassMarker_ = g.value;	// 100	Subclass marker (AcDbSymbolTable)
			break;
		case 70:
			this->entriesMaxNro_ = stoi(g.value);		// 70	Maximum number of entries in table
			break;
		}
	}

}

Table::~Table() {
	// TODO  Debug -- print this table.
	for (unsigned int i = 0; i < this->entryes_.size(); ++i) {

		delete this->entryes_[i];
	}
}

string Table::toString() {
	string s;
	s += 	"\"object_type\" : \"" + this->objectType_ + "\", ";
	s +=   "\"table_name\" : \"" + this->tableName_ + "\", ";
	s +=   "\"handle\" : \"" + this->handle_ + "\", ";
	s +=   "\"owner_soft_pointer\" : \"" + this->ownerSOftPtr_ + "\", ";
	s +=   "\"subclass_marker\" : \"" + this->subclassMarker_ + "\", ";
	s +=   "\"subclass_marker\" : \"" + this->subclassMarker_ + "\"";
	return s;
}

TableEntry* Table::getEntry(const string& name) const {
	for (auto entry : this->entryes_) {
		if (entry != nullptr && entry->getName() == name) {
			return entry;
		}
	}
	return nullptr;
}

string Table::toJson() {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	write_to_json_writer(writer);
	return buffer.GetString();
}

void Table::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	writer.Key("table_name");      writer.String(this->tableName_.c_str());
	writer.Key("handle");          writer.String(this->handle_.c_str());
	writer.Key("owner_soft_ptr");  writer.String(this->ownerSOftPtr_.c_str());
	writer.Key("subclass_marker"); writer.String(this->subclassMarker_.c_str());
	writer.Key("entries_max");     writer.Int(this->entriesMaxNro_);

	writer.Key("entries");
	writer.StartArray();
	for (size_t i = 0; i < this->entryes_.size(); ++i) {
		if (this->entryes_[i] != nullptr) {
			this->entryes_[i]->write_to_json_writer(writer);
		}
	}
	writer.EndArray();
	writer.EndObject();
}


} /* namespace dxf */
