// ===================================================================
// Audioscape library for PureData
// Copyright (c) 2007
//
// Collaborators:
//    Shared Reality Lab (SRE), McGill University Centre for Intelligent Machines (CIM)
//       www.cim.mcgill.ca/sre
//    La Société des Arts Technologiques (SAT)
//       www.sat.qc.ca
//
// Project Directors:
//    Science - Jeremy R. Cooperstock (SRE/CIM)
//    Arts - Zack Settel
//
// Conception:
//    Zack Settel
//
// Development Team:
//    Mike Wozniewski (SRE/CIM): Researcher, Head Developer
//    Zack Settel: Artist, Researcher, Audio/DSP programming
//    Jean-Michel Dumas (SAT): Assistant Researcher
//    Mitchel Benovoy (SRE/CIM): Video Texture Programming
//    Stéphane Pelletier (SRE/CIM): Video Texture Programming
//    Pierre-Olivier Charlebois (SRE/CIM): Former Developer
//
// Funding by / Souventionné par:
//    Natural Sciences and Engineering Research Council of Canada (NSERC)
//    Canada Council for the Arts
//    NSERC/Canada Council for the Arts - New Media Initiative
//
// ===================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// ===================================================================

#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
//#include <osgUtil/Optimizer>
//#include <osgUtil/SmoothingVisitor>
//#include <osg/TessellationHints>

#include "asSoundNode.h"
#include "asSceneManager.h"
#include "osgUtil.h"

using namespace std;

//extern asSceneManager *sceneManager;


// ===================================================================
// constructor:
asSoundNode::asSoundNode (asSceneManager *sceneManager, char *initID) : asDSPnode(sceneManager, initID)
{
	nodeType = "asSoundNode";

	_rolloff = "default";
	_spread = 1.0f;
	_length = 10.0f;
	
	directivityFlag = 0;
	laserFlag = 0;
	VUmeterFlag = 0;	
	
	currentSoundIntensity = 0.0;
	currentSoundColor = osg::Vec3(0.0,1.0,0.0); //green

	// OSG Stuff:
	laserGeode = NULL;
	directivityGeode = NULL;
	VUmeterTransform = NULL;
	
	


}

// ===================================================================
// destructor
asSoundNode::~asSoundNode()
{
	//std::cout << "In asSoundNode destructor... node: " << this->id->s_name << std::endl;
}



// ===================================================================
// ======================== SET METHODS: =============================
// ===================================================================

void asSoundNode::setRolloff (char *newvalue)
{
	// We store just the id instead of the whole table. If we want, we can always get the table
	// from the database (eg, for drawing).
	_rolloff = newvalue;
	drawDirectivity();
	BROADCAST(this, "ss", "setRolloff", _rolloff.c_str());
}

void asSoundNode::setSpread (float newvalue)
{
	_spread = newvalue;
	drawDirectivity();
	BROADCAST(this, "sf", "setSpread", _spread);
}

void asSoundNode::setLength (float newvalue)
{
	_length = newvalue;
	drawDirectivity();
	BROADCAST(this, "sf", "setLength", _length);
}

void asSoundNode::setDirectivityFlag (float newFlag)
{
	directivityFlag = newFlag;
	drawDirectivity();
	BROADCAST(this, "sf", "setDirectivityFlag", directivityFlag);
}

void asSoundNode::setLaserFlag (float newFlag)
{
	laserFlag = newFlag; // note continuous value is used for alpha
	drawLaser();
	BROADCAST(this, "sf", "setLaserFlag", laserFlag);
}

void asSoundNode::setVUmeterFlag (float newFlag)
{
	VUmeterFlag = newFlag; // note continuous value is used for alpha
	drawVUmeter();
	BROADCAST(this, "sf", "setVUmeterFlag", VUmeterFlag);
}

void asSoundNode::setIntensity (float newvalue)
{
	currentSoundIntensity = newvalue;
	
	float r = currentSoundIntensity / 0.896909;
	if (r > 1.0) r=1.0;
	float g = (1.0 - currentSoundIntensity) / 0.103091;
	if (g > 1.0) g=1.0;
	currentSoundColor = osg::Vec3(r, g, 0.0);
	
	drawVUmeter();
	drawLaser();
	
	BROADCAST(this, "sf", "setIntensity", currentSoundIntensity);
}


// ===================================================================
// ======================= DRAW METHODS: =============================
// ===================================================================

void asSoundNode::drawVUmeter()
{
		
	if (this->containsNode(VUmeterTransform.get()) )
	{
		this->removeChild(VUmeterTransform.get());
		VUmeterTransform = NULL;
	}
		
	if (VUmeterFlag>0)
	{
		// draw intensity geodes (cone and cap)

		osg::StateSet* VUmeterStateSet = new osg::StateSet();
		VUmeterStateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
		VUmeterStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF);

		osg::Geode *VUmeterBaseGeode = new osg::Geode();
		osg::Geode *VUmeterCapGeode = new osg::Geode();
		
		osg::TessellationHints* hints = new osg::TessellationHints;
		hints->setDetailRatio(0.5f);
		
		osg::ShapeDrawable* VUmeterBaseDrawable = new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(0.0f,0.0f,0.0f),AS_UNIT_SCALE*.1, AS_UNIT_SCALE),hints);
		VUmeterBaseDrawable->setColor( osg::Vec4(currentSoundColor, VUmeterFlag) );
		VUmeterBaseGeode->setStateSet( VUmeterStateSet );
		VUmeterBaseGeode->addDrawable( VUmeterBaseDrawable );
	
		osg::ShapeDrawable *VUmeterCapDrawable = new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0.0f,0.0f,AS_UNIT_SCALE*.5),AS_UNIT_SCALE*.2,AS_UNIT_SCALE*.2),hints);
		VUmeterCapDrawable->setColor( osg::Vec4(currentSoundColor, VUmeterFlag) );
		VUmeterCapGeode->setStateSet(VUmeterStateSet );
		VUmeterCapGeode->addDrawable(VUmeterCapDrawable);
		
		VUmeterTransform = new osg::PositionAttitudeTransform();
		VUmeterTransform->addChild(VUmeterBaseGeode);
		VUmeterTransform->addChild(VUmeterCapGeode);
			
		VUmeterTransform->setScale( osg::Vec3(1, 1, 1 + currentSoundIntensity) );

		VUmeterTransform->setName(string(id->s_name) + ".VUmeterTransform");
		this->addChild(VUmeterTransform.get());
   }

}



t_float cardioid_to_cone_map[] = {180.0, 155.047, 147.605, 140.163, 116.907, 113.651, 106.674, 101.344, 97.1575, 93.6512, 90.8605, 89, 82.3043, 79.0484, 74.3973, 71.6065, 69.7461, 67.4205, 64.6298, 59.5135, 57.653, 55.7926, 54.8624, 53.467, 52.0717, 49.7461, 48.3507, 45, 44.9285, 42.9762, 41.8048, 41.4143, 40.6334, 39.8525, 38.6811, 38.2907, 37.1193, 36.3384, 35.9479, 35.167, 34.7766, 34.3861, 33.6052, 32.4338, 31.6529, 30.872, 30.4816, 30.4816, 29.7006, 29.3102, 28.5293, 28.1388, 27.7484, 26.9675, 26.9675, 26.577, 25.7961, 25.4056, 24.6247, 24.2343, 23.0629, 23.0629, 22.3043, 20.9089, 20.9089, 20.4438, 19.9787, 19.5136, 19.0485, 18.5834, 18.1182, 17.6531, 17.188, 16.7229, 16.7229, 16.2578, 16.2578, 15.7927, 15.7927, 15.3275, 15.3275, 15.0515, 14.4993, 14.3973, 13.9322, 13.4671, 13.002, 13.002, 12.7259, 12.4498, 12.4498, 12.4498, 12.1737, 11.6066, 11.6066, 11.6066, 11.6066, 11.3305, 11.3305, 10.6764, 10, 10.1242, 9.84816, 9.28103, 8.81591, 8.81591, 8.81591, 8.53984, 8.53984, 7.88568, 7.42056, 7.14449, 7.14449, 6.49033, 6.49033, 6.0066, 6.0066, 5.70254, 5.39849, 5.39849, 5.09443, 5.38037, 5.38037, 5.10429, 5.10429, 4.82822, 4.82822, 4.82822, 4.82822, 4.82822, 4.55215, 4.27607, 4.27607, 4.27607, 4, 3.72393, 3.72393, 3.72393, 3.72393, 3.72393, 3.72393, 3.72393, 3.72393, 3.72393, 3.72393, 3.72393, 3.72393, 3.72393, 3.72393, 3.72393, 4, 3.72393, 3.72393, 3.44785, 3.17178, 3.17178, 3.17178, 3.17178, 3.17178, 3.17178, 3.17178, 2.89571, 2.89571, 2.89571, 2.89571, 2.61963, 2.61963, 2.61963, 2.34356, 2.34356, 2.34356, 2.34356, 2.06748, 2.06748, 2.06748, 2.06748, 2.06748, 2.06748, 2.06748, 2.06748, 2.27607, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

// ===================================================================
void asSoundNode::drawDirectivity()
{
	
	if (this->containsNode(directivityGeode.get()))
	{
        this->removeChild(directivityGeode.get());
		directivityGeode = NULL;
	}

	if (directivityFlag > 0)
	{
		t_float val;
		t_float index = _spread * 16.6666;
		if ( (index < 0) || ((int)index >= 199) ) return;
		else val = cardioid_to_cone_map[(int)index];

		//printf("drawing directivity: _spread=%.3f  index=%.3f  val=%.3f\n", _spread, index, val);

		
		// *** NEW METHOD (NOT WORKING YET):
		// directivityGeode = createWireframeRolloff( this->_rolloff, this->_spread, AS_DEBUG_SCALE, osg::Vec4(DEFAULT_DIRECTIVITY_COLOR,1.0) );

		// *** OLD METHOD:
		
		if (val < 90)
		{
			// Place a cone pointing along the +Y axis
			directivityGeode = createHollowCone( _length*AS_DEBUG_SCALE, AS_DEBUG_SCALE*_length*tan(osg::DegreesToRadians(val)), osg::Vec4(DEFAULT_DIRECTIVITY_COLOR,1.0) );
		} else if (val > 90 && val < 180) {
			// Place a cone pointing along the -Y axis
			directivityGeode = createHollowCone( -_length*AS_DEBUG_SCALE, AS_DEBUG_SCALE*_length*tan(osg::DegreesToRadians(val)), osg::Vec4(DEFAULT_DIRECTIVITY_COLOR,1.0) );
		} else {
			// Sphere
			directivityGeode= createHollowSphere(_length*AS_DEBUG_SCALE, osg::Vec4(DEFAULT_DIRECTIVITY_COLOR,1.0) );
		}
		
		// ***
		
		
		
		osg::StateSet *wireframeStateSet = new osg::StateSet();
		osg::PolygonMode *polymode = new osg::PolygonMode;
		polymode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		wireframeStateSet->setAttributeAndModes(polymode,osg::StateAttribute::ON);
		wireframeStateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
		wireframeStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
		
		directivityGeode->setStateSet(wireframeStateSet);
		
		directivityGeode->setName(string(id->s_name) + ".directivityGeode");
        this->addChild(directivityGeode.get());

	}
	
}

// ===================================================================
void asSoundNode::drawLaser()
{
	
	if (this->containsNode(laserGeode.get()))
	{
        this->removeChild(laserGeode.get());
		laserGeode = NULL;
	}

	if (laserFlag > 0)
	{  
		// create geode to hold lase:
		laserGeode = new osg::Geode();
		laserGeode->setName(string(id->s_name) + ".laserGeode");
 		 
		// draw laser as a cylinder:
		osg::Vec3 center;
		osg::Cylinder *laser;
		osg::Quat rot;
		center = osg::Vec3(0.0f,AS_DEBUG_SCALE*_length*.5,0.0f);
		laser = new osg::Cylinder(center, AS_LASER_RADIUS, AS_DEBUG_SCALE*_length);
		rot.makeRotate(osg::Vec3(0,0,1),center);
		laser->setRotation(rot);
		
		// make drawable and add to geode:
		osg::TessellationHints* hints = new osg::TessellationHints;
		hints->setDetailRatio(GENERIC_SHAPE_RESOLUTION);
		osg::ShapeDrawable* laserDrawable = new osg::ShapeDrawable(laser,hints);
		laserDrawable->setColor( osg::Vec4(currentSoundColor, laserFlag) );
		laserGeode->addDrawable(laserDrawable);
		
		// turn off lighting effects:
		osg::StateSet *laserStateSet = new osg::StateSet;
		laserStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF);			
		laserGeode->setStateSet ( laserStateSet );
		
		// add it to the node:
		this->addChild( laserGeode.get() );
	}
}

std::vector<lo_message> asSoundNode::getState ()
{
	// inherit state from base class
	std::vector<lo_message> ret = asDSPnode::getState();
	
	lo_message msg;
	
	msg = lo_message_new();
	lo_message_add(msg, "ss", "setRolloff", _rolloff.c_str());
	ret.push_back(msg);

	msg = lo_message_new();
	lo_message_add(msg, "sf", "setSpread", _spread);
	ret.push_back(msg);
	
	msg = lo_message_new();
	lo_message_add(msg, "sf", "setLength", _length);
	ret.push_back(msg);

	msg = lo_message_new();
	lo_message_add(msg, "sf", "setDirectivityFlag", directivityFlag);
	ret.push_back(msg);

	msg = lo_message_new();
	lo_message_add(msg, "sf", "setLaserFlag", laserFlag);
	ret.push_back(msg);

	msg = lo_message_new();
	lo_message_add(msg, "sf", "setVUmeterFlag", VUmeterFlag);
	ret.push_back(msg);

	msg = lo_message_new();
	lo_message_add(msg, "sf", "setIntensity", currentSoundIntensity);
	ret.push_back(msg);
	
	
	
	return ret;
}