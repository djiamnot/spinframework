// -----------------------------------------------------------------------------
// |    ___  ___  _  _ _     ___                                        _      |
// |   / __>| . \| || \ |   | __>_ _  ___ ._ _ _  ___  _ _ _  ___  _ _ | |__   |
// |   \__ \|  _/| ||   |   | _>| '_><_> || ' ' |/ ._>| | | |/ . \| '_>| / /   |
// |   <___/|_|  |_||_\_|   |_| |_|  <___||_|_|_|\___.|__/_/ \___/|_|  |_\_\   |
// |                                                                           |
// |---------------------------------------------------------------------------|
//
// http://spinframework.sourceforge.net
// Copyright (C) 2009 Mike Wozniewski, Zack Settel
//
// Developed/Maintained by:
//    Mike Wozniewski (http://www.mikewoz.com)
//    Zack Settel (http://www.sheefa.net/zack)
// 
// Principle Partners:
//    Shared Reality Lab, McGill University (http://www.cim.mcgill.ca/sre)
//    La Soci�t� des Arts Technologiques (http://www.sat.qc.ca)
//
// Funding by:
//    NSERC/Canada Council for the Arts - New Media Initiative
//    Heritage Canada
//    Ministere du Developpement economique, de l'Innovation et de l'Exportation
//
// -----------------------------------------------------------------------------
//  This file is part of the SPIN Framework.
//
//  SPIN Framework is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  SPIN Framework is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the Lesser GNU General Public License
//  along with SPIN Framework. If not, see <http://www.gnu.org/licenses/>.
// -----------------------------------------------------------------------------


#ifndef __ASBASICNODE_H
#define __ASBASICNODE_H

#include "asGlobals.h"
#include "asReferenced.h"

#include <osg/Group>
#include <osg/PositionAttitudeTransform> 
#include <osg/NodeVisitor>

#include <string>
#include <vector>
#include <map>

/**
 * \brief A basic node to manage translation/orientation/scale of a subgraph.
 *        Allows for grouping of nodes.
 * 
 * Most nodes in the scene do not have a method to offset their position in the
 * scene. This node provides a mechanism to group and offset nodes.
 */
class asBasicNode : public asReferenced
{

public:
	
	/**
	 * The constructor takes a string id that will be converted to a t_symbol,
	 * which contains a pointer to this object. Thus functions will be able to
	 * acquire the pointer using gensym(), which uses a hash table lookup.
	 * @param initID will be converted into a t_symbol
	 */

	asBasicNode(asSceneManager *sceneManager, char *initID);
	virtual ~asBasicNode();
	
	virtual void callbackUpdate();

	
	/**
	 * IMPORTANT:
	 * subclasses of asReferenced are allowed to contain complicated subgraphs,
	 * and can also change their attachmentNode so that children are attached
	 * anywhere in this subgraph. If that is the case, the updateNodePath()
	 * function MUST be overridden, and extra nodes must be manually pushed onto
	 * the currentNodePath.
	 */
	virtual void updateNodePath();
	
	void reportGlobals (int b);
	void setTranslation (float x, float y, float z);
	void setOrientation (float p, float r, float y);
	void move (float x, float y, float z);
	void rotate (float p, float r, float y);
	
	int getReportGlobals() { return (int)_reportGlobals; };
	osg::Vec3 getTranslation() { return mainTransform->getPosition(); };
	osg::Vec3 getOrientation() { return _orientation; };
	//osg::Vec3 getOrientation() { return Vec3inDegrees((mainTransform->getAttitude()).asVec3()); };
	
	
	// extra method for globals:
	void dumpGlobals(bool forced);
	
	
	/**
	 * For each subclass of asReferenced, we override the getState() method to
	 * fill the vector with the correct set of methods for this particular node
	 */
	virtual std::vector<lo_message> getState();
	
	/**
	 * We must include a stateDump() method that simply invokes the base class
	 * method. Simple C++ inheritance is not enough, because osg::Introspection
	 * won't see it.
	 */
	virtual void stateDump() { asReferenced::stateDump(); dumpGlobals(true); };


	
	// ***********************************************************
	// data:
	
	osg::ref_ptr<osg::PositionAttitudeTransform> mainTransform;

	
private:
	bool _reportGlobals;
	osg::Vec3 _orientation; // store the orientation as it comes in (in degrees)
	osg::Matrix _globalMatrix;
	osg::Vec3 _globalScale;
	float _globalRadius;
		
};


	
#endif
