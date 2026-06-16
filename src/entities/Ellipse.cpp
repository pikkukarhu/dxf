/*
 * Eclipse.cpp
 *
 *  Created on: 6 Jan 2024
 *      Author: hobbes
 */

#include "entities/Ellipse.h"
#include <cmath>

namespace dxf {


struct BoundingBox {
    Entity::Point_ low;
    Entity::Point_ high;
    double width;
    double height;
};

BoundingBox bounds(const Entity::Point_& point1, const Entity::Point_ point2) {
    BoundingBox result;

    result.low.x_ = std::min(point1.x_, point2.x_);
    result.low.y_ = std::min(point1.y_, point2.y_);

    result.high.x_ = std::max(point1.x_, point2.x_);
    result.high.y_ = std::max(point1.y_, point2.y_);

    result.width = result.high.x_ - result.low.x_;
    result.height = result.high.y_ - result.low.y_;

    return result;
}


Entity::Point_ rotate(const Entity::Point_& point, const Entity::Point_& origin, double theta) {
    double cosTheta = std::cos(theta);
    double sinTheta = std::sin(theta);

    // Translate the point back to the origin, rotate, and translate back
    double x = cosTheta * (point.x_ - origin.x_) - sinTheta * (point.y_ - origin.y_) + origin.x_;
    double y = sinTheta * (point.x_ - origin.x_) + cosTheta * (point.y_ - origin.y_) + origin.y_;

    return {x, y};
}

Entity::Point_ multiply(const Entity::Point_& point, const Entity::Point_& mul) {
    return {point.x_ * mul.x_, point.y_ * mul.y_};
}

void resolveUV(const Entity::Point_& r, double theta, Entity::Point_& pu, Entity::Point_& pv) {
    pu.x_ = std::cos(M_PI / 2.0);
    pu.y_ = std::sin(M_PI / 2.0);

    pv.x_ = std::cos(0);
    pv.y_ = std::sin(0);

    // Assuming you have a function to rotate a point
    Entity::Point_ origin{0, 0};

    Entity::Point_ rotatedPu = rotate(multiply(pu, r), origin, theta);
    Entity::Point_ rotatedPv = rotate(multiply(pv, r), origin, theta);

    pu.x_ = rotatedPu.x_;
    pu.y_ = rotatedPu.y_;

    pv.x_ = rotatedPv.x_;
    pv.y_ = rotatedPv.y_;
}


BoundingBox getBBox(const Entity::Point_& c, const Entity::Point_& end, double ratio, double theta) {
    // Assuming you have a function to get UV vectors
	Entity::Point_ pu;
	Entity::Point_ pv;
	Entity::Point_ r;

	r.x_ = std::hypot(end.x_, end.y_);
	r.y_ = r.x_ * ratio;;


    resolveUV(r, theta, pu, pv);

    // Calculate squared components of e vector
    double exsqr = pu.x_ * pu.x_ + pv.x_ * pv.x_;
    double eysqr = pu.y_ * pu.y_ + pv.y_ * pv.y_;

    // Calculate e vector
    Entity::Point_ e{std::sqrt(exsqr), std::sqrt(eysqr)};

    // Calculate cMinusE and cPlusE vectors
    Entity::Point_ cMinusE = {c.x_ - e.x_, c.y_ - e.y_};
    Entity::Point_ cPlusE = {c.x_ + e.x_, c.y_ + e.y_};

    // Calculate bounding box
    BoundingBox bbox{cMinusE, cPlusE, cPlusE.x_ - cMinusE.x_, cPlusE.y_ - cMinusE.y_};

    return bbox;
}


double distance(Entity::Point_ p1, Entity::Point_ p2) {
    return std::hypot(p1.x_ - p2.x_, p1.y_ - p2.y_) / 2.0;
}

const Entity::Point_ rotate_point(double x, double y, double cx, double cy, double angle) {
    // # Rotate point (x, y) around center (cx, cy) by angle (in radians)
	Entity::Point_ rot;
	rot.x_ = (x - cx) * std::cos(angle) - (y - cy) * std::sin(angle) + cx;
    rot.y_ = (x - cx) * std::sin(angle) + (y - cy) * std::cos(angle) + cy;
    return rot;
}

void Ellipse::calc_bounding_box() {

	double rotation =  std::atan2(end_point_.y_,  end_point_.x_);
	BoundingBox bb = getBBox(center_point_, end_point_, ratio_, rotation);

	bounding_box_.x = bb.low.x_;
	bounding_box_.y = bb.low.y_;
	bounding_box_.width = bb.width;
	bounding_box_.heigth = bb.height;
}


Ellipse::Ellipse(const vector<Group> &properties)  :Entity(properties) {
	// Read end & start from groups. (Other line specific) Entity should read directly

	for (unsigned int i = 0; i < properties.size(); ++i) {

		Group g = properties[i];
		switch (g.groupcode) {
			case 10:	// Start point (in WCS)
				this->center_point_.x_ = stod(g.value);
				break;
			case 20:
				this->center_point_.y_ = stod(g.value);
				break;
			case 30:
				this->center_point_.z_ = stod(g.value);
				break;
			case 11:	// Endpoint (in WCS)
				this->end_point_.x_ = stod(g.value);
				break;
			case 21:
				this->end_point_.y_ = stod(g.value);
				break;
			case 31:
				this->end_point_.z_ = stod(g.value);
				break;
			case 40 :
				this->ratio_ = stod(g.value);
				break;
			case 210: // Extrusion direction (optional; default = 0, 0, 1)
				this->extrusion_dir_.x_ = stod(g.value);
				break;
			case 220:
				this->extrusion_dir_.y_ = stod(g.value);
				break;
			case 230:
				this->extrusion_dir_.z_ = stod(g.value);
				break;
			case 50:
				this->start_angle_ =  stod(g.value);
				break;
			case 51:
				this->end_angle_ =  stod(g.value);
				break;
			default:
				break;
		}
	}
	calc_bounding_box();
}



string Ellipse::to_json() {
	string s;
		s += "{\"ellipse\" : {";
		s += to_string();
		s += ", \"center_point\" : " + this->center_point_.as_string();
		s += ", \"end_point\" : " + this->end_point_.as_string();
		s += ", \"start_angle\" : "  + std::to_string(this->start_angle_) + ", ";
		s += ", \"end_angle\" : "  + std::to_string(this->end_angle_) + "} ";
		s += "}}";

	return s;
}

void Ellipse::write_to_json_writer(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
	writer.StartObject();
	writer.Key("ellipse");
	writer.StartObject();
	Entity::write_to_json_writer(writer);
	writer.Key("center_point"); this->center_point_.write_to_json_writer(writer);
	writer.Key("end_point"); this->end_point_.write_to_json_writer(writer);
	writer.Key("start_angle"); writer.Double(this->start_angle_);
	writer.Key("end_angle"); writer.Double(this->end_angle_);
	writer.EndObject();
	writer.EndObject();
}

void Ellipse::to_svg(pugi::xml_node& svg_node) {
    double x = center_point_.x_; // Center point x
    double y = center_point_.y_; // Center point y

	double a = std::hypot(end_point_.x_, end_point_.y_); // std::abs(center_point_.x_ - end_point_.x_);//distance(center_point_, end_point_); // Major axle
	double b = a * ratio_;							// Minor axle

	double theta = std::atan2(end_point_.y_,  end_point_.x_);
    double angle_deg = theta * 180 / M_PI;

    // Create a group element for rotation
    pugi::xml_node group = svg_node.append_child("g");
	group.append_attribute("transform").set_value(("rotate(" + std::to_string(angle_deg) + " " + std::to_string(x) + " " + std::to_string(y) + ")").c_str());

    // Create an ellipse element inside the group
    pugi::xml_node ellipse = group.append_child("ellipse");

    // Set SVG ellipse attributes
    ellipse.append_attribute("cx").set_value(x);
    ellipse.append_attribute("cy").set_value(y);
    ellipse.append_attribute("rx").set_value(a);
    ellipse.append_attribute("ry").set_value(b);

    ellipse.append_attribute("fill").set_value("none");
    ellipse.append_attribute("stroke").set_value(get_svg_color().c_str());

    draw_bounding_box(svg_node);
}

} /* namespace dxf */
