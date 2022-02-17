/*
 * Entity.h
 *
 *  Created on: 13 Feb 2022
 *      Author: hobbes
 */

#ifndef ENTITIES_ENTITY_H_
#define ENTITIES_ENTITY_H_

namespace dxf {

class Entity {
public:
	Entity();
	virtual ~Entity();

	Entity(const Entity &other);
	Entity(Entity &&other);
};

} /* namespace dxf */

#endif /* ENTITIES_ENTITY_H_ */
