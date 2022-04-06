/*
 * Layer.h
 *
 *  Created on: 25 Feb 2022
 *      Author: hobbes
 */

#ifndef ENTRIES_LAYER_H_
#define ENTRIES_LAYER_H_

#include <vector>
#include <string>

#include "tables/TableEntry.h"
#include "file/File.h"

using std::vector;
using std::string;

namespace dxf {

class Layer final : public TableEntry {
private:
	string layerName_;		// 2	Layer name
	int	flags_ = 0;			// 70	Standard flags (bit-coded values):
							//		1 = Layer is frozen; otherwise layer is thawed
							//		2 = Layer is frozen by default in new viewports
							//		4 = Layer is locked
							//		16 = If set, table entry is externally dependent on an xref
							//		32 = If both this bit and bit 16 are set, the externally dependent xref has been successfully resolved
							//		64 = If set, the table entry was referenced by at least one entity in the drawing the last time the drawing was edited. (This flag is for the benefit of AutoCAD commands. It can be ignored by most programs that read DXF files and need not be set by programs that write DXF files)
	int colorNumber_;		// 62	Color number (if negative, layer is off)
	string linetypeName_;	// 6	Linetype name
	bool plottingFlag_;		// 290	Plotting flag. If set to 0, do not plot this layer
	int lineWeight_;		// 370	Lineweight enum value
	string plotStylePtr_;	// 390 	Hard-pointer ID/handle of PlotStyleName object
	string materialPtr_;	// 347	Hard-pointer ID/handle to Material object

public:
	Layer(const vector<Group> &properties);
	virtual ~Layer();

	virtual string toString();
	virtual string toJson();
};

} /* namespace dxf */

#endif /* ENTRIES_LAYER_H_ */
