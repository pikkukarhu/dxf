/*
 * Layer.cpp
 *
 *  Created on: 25 Feb 2022
 *      Author: hobbes
 */

#include "Layer.h"

#include <vector>
#include <string>

#include "tables/TableEntry.h"
#include "file/File.h"

using std::vector;
using std::string;
using std::to_string;

namespace dxf {

Layer::Layer(const vector<Group> &properties) : TableEntry(properties) {

	for (unsigned int i = 0; i < properties.size(); ++i) {
		Group g = properties[i];

		switch (g.groupcode) {
			case 2:		// 2	Layer name
				this->layerName_ = g.value;
				break;
			case 70:	// 70	Standard flags (bit-coded values):
				this->flags_ |= stoi(g.value);
				break;
			case 62:	// 62	Color number (if negative, layer is off)
				this->colorNumber_ = stoi(g.value);
				break;
			case 6:		// 6	Linetype name
				this->linetypeName_ = g.value;
				break;
			case 290:	// 290	Plotting flag. If set to 0, do not plot this layer
				this->plottingFlag_ = stoi(g.value);
				break;
			case 370:	// 370	Lineweight enum value
				this->lineWeight_ = stoi(g.value);
				break;
			case 390:	// 390 	Hard-pointer ID/handle of PlotStyleName object
				this->plotStylePtr_ = g.value;
				break;
			case 347:	// 347	Hard-pointer ID/handle to Material object
				this->materialPtr_ = g.value;
				break;
		}
	}

}

Layer::~Layer() {
	// TODO Auto-generated destructor stub
}

string Layer::toString() {
	string s = TableEntry::toString();

	s += "\"layer_name\" : \"" + this->layerName_ + "\", ";
	s += "\"flags\" : \"" + to_string(this->flags_) + "\", ";
	s += "\"color_number\" : " + to_string(this->colorNumber_) + ", ";
	s += "\"linetype_name\" : \"" + this->linetypeName_ + "\", ";
	s += "\"plotting_flag\" : " + to_string(this->plottingFlag_) + ", ";
	s += "\"line_weight\" : " + to_string(this->lineWeight_) + ", ";
	s += "\"plot_style_pointer\" : \"" + this->plotStylePtr_ + "\", ";
	s += "\"material_pointer\" : \"" + this->materialPtr_ + "\", ";
	s += "\"color_number\" : " + to_string(this->colorNumber_) + ", ";

	return s;
}

string Layer::toJson() {
	return "{" + toString() + "}";
}

} /* namespace dxf */
