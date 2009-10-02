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
//    La Societe des Arts Technologiques (http://www.sat.qc.ca)
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
//  You should have received a copy of the GNU Lesser General Public License
//  along with SPIN Framework. If not, see <http://www.gnu.org/licenses/>.
// -----------------------------------------------------------------------------

#include "MeasurementNode.h"
#include "SceneManager.h"
#include "osgUtil.h"

using namespace std;

//extern SceneManager *sceneManager;



// *****************************************************************************
// constructor:
MeasurementNode::MeasurementNode (SceneManager *sceneManager, char *initID) : ReferencedNode(sceneManager, initID)
{
	this->setName(string(id->s_name) + ".MeasurementNode");
	nodeType = "MeasurementNode";

	targetName = gensym("NULL");
	reportingLevel = 2;
	
	thisMatrix.makeIdentity();
	targetMatrix.makeIdentity();
	
	this->setNodeMask(STATSDATA_NODE_MASK); // nodemask info in spinUtil.h

	
	//thisMatrix = new osg::Matrixd();
	//targetMatrix = new osg::Matrixd();
}

// *****************************************************************************
// destructor
MeasurementNode::~MeasurementNode()
{

}

void MeasurementNode::callbackUpdate()
{
	osg::ref_ptr<ReferencedNode> targetNode = targetName->s_thing;
	if (!targetNode.valid()) return;
	
	osg::Matrix mthis = osg::computeLocalToWorld(this->currentNodePath);
	osg::Matrix mtarget = osg::computeLocalToWorld(targetNode->currentNodePath);
	
	// check if there is a change in the matrices:
	if ((thisMatrix==mthis) && (targetMatrix==mtarget))
	{
		return;
	} else
	{
		thisMatrix = mthis;
		targetMatrix = mtarget;
	}
	
	osg::Vec3 connection_vector = targetMatrix.getTrans() - thisMatrix.getTrans();
	
	
	osg::Quat srcQuat, snkQuat;
	thisMatrix.get(srcQuat);
	targetMatrix.get(snkQuat);
	
	// let's also compute the orientations projected on the (local) horizontal and vertical plane:
	// (ie, azimuth and elevation respectively)
	osg::Vec3 src_dir   = srcQuat * osg::Vec3(0,1,0);
	osg::Vec3 src_right = srcQuat * osg::Vec3(1,0,0);
	osg::Vec3 src_up    = srcQuat * osg::Vec3(0,0,1);
	osg::Vec3 snk_dir   = snkQuat * osg::Vec3(0,1,0);
	osg::Vec3 snk_right = snkQuat * osg::Vec3(1,0,0);
	osg::Vec3 snk_up    = snkQuat * osg::Vec3(0,0,1);
		

	// incidence (radians) between source and the connection_vector:
	float srcIncidence = AngleBetweenVectors(src_dir, connection_vector);
	float srcIncidenceAzim = (osg::PI/2) - AngleBetweenVectors(src_right, connection_vector);
	float srcIncidenceElev = (osg::PI/2) - AngleBetweenVectors(src_up, connection_vector);

	// incidence (radians) between sink and the connection_vector:
	float snkIncidence = AngleBetweenVectors(osg::Vec3(0,0,0)-snk_dir, connection_vector);		
	float snkIncidenceAzim = (osg::PI/2) - AngleBetweenVectors(snk_right, -connection_vector);
	float snkIncidenceElev = (osg::PI/2) - AngleBetweenVectors(snk_up, -connection_vector);

	if (reportingLevel>0)
	{
		BROADCAST(this, "sf", "distance", connection_vector.length());
		BROADCAST(this, "sf", "angle", srcIncidence);
	}
	
	if (reportingLevel>1)
	{
		BROADCAST(this, "sf", "azimuth", srcIncidenceAzim);
		BROADCAST(this, "sf", "elevation", srcIncidenceElev);
		BROADCAST(this, "sf", "incidence", (srcIncidence / osg::PI) * (snkIncidence / osg::PI) );
	}
	
}

// *****************************************************************************
void MeasurementNode::setTarget (const char *targetID)
{
	if (targetName != gensym(targetID))
	{
		targetName = gensym(targetID);
		BROADCAST(this, "ss", "setTarget", getTarget());	
	}
}

void MeasurementNode::setReportingLevel (int level)
{
	this->reportingLevel = level;
	BROADCAST(this, "si", "setReportingLevel", getReportingLevel());	
}


// *****************************************************************************

std::vector<lo_message> MeasurementNode::getState ()
{
	// inherit state from base class
	std::vector<lo_message> ret = ReferencedNode::getState();
	
	lo_message msg;
	
	
	msg = lo_message_new();
	lo_message_add(msg, "ss", "setTarget", this->getTarget());
	ret.push_back(msg);
	
	msg = lo_message_new();
	lo_message_add(msg, "si", "setReportingLevel", this->getReportingLevel() );
	ret.push_back(msg);
	
	return ret;
}
