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
#include <pugixml.hpp>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "file/File.h"

using std::vector;
using std::string;

namespace dxf {


struct RGB {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

struct Rect {
	double x;
	double y;
	double width;
	double heigth;
	/*
	double x_min;
	double y_min;
	double x_max;
	double y_max;*/

	inline Rect() {x = y = width = heigth = 0; }

	inline Rect(double x, double y, double width, double heigth) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->heigth = heigth;
		/*
		x_max = x1 < x2 ? x2 : x1;
		y_min = y1 < y2 ? y1 : y2;
		y_max = y1 < y2 ? y2 : y1;*/
	}
};

class Entity {
private:
	/* Should these be references? Or there,s name from file *and* reference, solved after read whole file
	 *  -- depends how much we use dxf::entitity / dxf::document in code?
	 *  -- name better, can save to Json- file or document db as-is.virtual void to_svg(pugi::xml_node& svgNode);
	 */
	string	type_;					// Entity type name. Eg LINE, POINT, CIRCLE ...
	string	handle_;				// Entity handle
	string	line_style_;			// Linestyle name;
	string	subclass_marker_;		// Subclass marker (AcDbEntity) not omitted
	bool	model_space_ = true;	// Is entity in modelspace (false = paperspace)
	string	layer_	= "0";			// Layer name;
	string	line_type_ = "BYLAYER";	// Default 256 Byblock
	int		color_number_ = 0;		// Default 0 Bylayer
	int		line_weight_;
	double	linetype_scale_ = 1.0;
	bool	visible_	= true;		// Visiblity of entity, default = true;
	RGB		rgb_;
	int		shadow_mode_ = 3;		// 0 = Casts and receives shadows
									// 1 = Casts shadows							// 2 = Receives shadows
protected:							// 3 = Ignores shadows
	Rect bounding_box_; 			// Bounding box of element
	virtual void calc_bounding_box() = 0;
	void draw_bounding_box(pugi::xml_node& svg_node);
public:
	static bool show_bounding_box;
	static inline void setShowBoundingBox(bool show) { show_bounding_box = show; }

	struct Point_ {
		double x_;
		double y_;
		double z_;

		inline std::string as_string() {
			return
				 "{\"x\" : " + std::to_string(x_) + ", "
				+ "\"y\" : " + std::to_string(y_) + ", "
				+ "\"z\" : " + std::to_string(z_) + "}"; }
		
		inline void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
			writer.StartObject();
			writer.Key("x"); writer.Double(x_);
			writer.Key("y"); writer.Double(y_);
			writer.Key("z"); writer.Double(z_);
			writer.EndObject();
		}
	};

	Entity(const std::vector<dxf::Group>&);
	virtual ~Entity();

	inline string	get_type() { return this->type_;	}
	inline string	get_handle() { return this->handle_;}
	inline string	get_linestyle() { return this->line_style_;}
	inline string	get_subclass_marker() { return this->subclass_marker_;}
	inline bool		is_modelspace() { return this->model_space_;}
	inline string	get_layer() { return this->layer_;}
	inline string	get_linetype() { return this->line_type_;}
	inline int		get_colorNumber() { return this->color_number_ ;}
	inline int		get_line_weight() { return this->line_weight_;}
	inline double	get_linetype_scale() { return this->linetype_scale_;}
	inline bool		is_visible() { return this->visible_;}
	inline RGB		get_RGB() { return this->rgb_;}
	inline int		get_shadow_mode() { return this->shadow_mode_;}
	inline Rect 	get_bounding_box() { return this->bounding_box_; };

	virtual string to_string();
	virtual string to_json() = 0;
	virtual void write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer);
	virtual void to_svg(pugi::xml_node& svgNode) = 0;
};

} /* namespace dxf */

#endif /* DOCUMENT_ENTITY_H_ */
