/*
 * Entity.h
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */
#pragma once

#ifndef DOCUMENT_ENTITY_H_
#define DOCUMENT_ENTITY_H_

#include <vector>
#include <string>

#include "file/File.h"

using std::vector;
using std::string;

namespace dxf {

struct RGB {
	unsigned char red_;
	unsigned char green_;
	unsigned char blue_;
};

class Entity {
private:
	/* Should these be references? Or there,s name from file *and* reference, solved after read whole file
	 *  -- depends how much we use dxf::entitity / dxf::document in code?
	 *  -- name better, can save to Json- file or document db as-is.
	 */
	string	type_;					// Entity type name. Eg LINE, POINT, CIRCLE ...
	string	handle_;				// Entity handle
	string	lineStyle_;				// Linestyle name;
	string	subclassMarker_;		// Subclass marker (AcDbEntity) not omitted
	bool	modelSpace_ = true;		// Is entity in modelspace (false = paperspace)
	string	layer_	= "0";			// Layer name;
	string	lineType_ = "BYLAYER";	// Default 256 Byblock
	int		colorNumber_ = 0;		// Default 0 Bylayer
	int		lineWeight_;
	double	linetypeScale_ = 1.0;
	bool	visible_	= true;		// Visiblity of entity, default = true;
	RGB		rgb_;
	int		shadowMode_ = 3;		// 0 = Casts and receives shadows
									// 1 = Casts shadows
									// 2 = Receives shadows
									// 3 = Ignores shadows
protected:

	struct Point_ {
		double x_;
		double y_;
		double z_;
	};

public:

	Entity(const std::vector<dxf::Group>&);
	virtual ~Entity();


	inline string	getType() { return this->type_;	}
	inline string	getHandle() { return this->handle_;}
	inline string	getLinestyle() { return this->lineStyle_;}
	inline string	getSubclassMarker() { return this->subclassMarker_;}
	inline bool		isModelspace() { return this->modelSpace_;}
	inline string	getLayer() { return this->layer_;}
	inline string	getLinetype() { return this->lineType_;}
	inline int		getColorNumber() { return this->colorNumber_ ;}
	inline int		getLineWeight() { return this->lineWeight_;}
	inline double	getLinetypeScale() { return this->linetypeScale_;}
	inline bool		isVisible() { return this->visible_;}
	inline RGB		getRgb() { return this->rgb_;}
	inline int		getShadowMode() { return this->shadowMode_;}

	virtual string toString();
	virtual string toJson() = 0;
};

} /* namespace dxf */

#endif /* DOCUMENT_ENTITY_H_ */
