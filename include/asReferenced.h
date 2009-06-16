// -----------------------------------------------------------------------------
// |    ___  ___  _  _ _     ___                                        _      |
// |   / __>| . \| || \ |   | __>_ _  ___ ._ _ _  ___  _ _ _  ___  _ _ | |__   |
// |   \__ \|  _/| ||   |   | _>| '_><_> || ' ' |/ ._>| | | |/ . \| '_>| / /   |
// |   <___/|_|  |_||_\_|   |_| |_|  <___||_|_|_|\___.|__/_/ \___/|_|  |_\_\   |
// |                                                                           |
// |---------------------------------------------------------------------------|
//
// http://spinframework.sourceforge.net
// Copyright (c) 2009
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
// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------


#ifndef __ASREFERENCED_H
#define __ASREFERENCED_H

#include <string>

#include "asUtil.h"
#include "asGlobals.h"
#include "oscParser.h"
#include <osg/Group>
#include <osg/Node>
#include <osg/Geode>


class asSceneManager;
class asMediaManager;

/**
 * \brief The base class for all VESS scene graph nodes
 *
 * Any node that is to be attached to the scene graph needs to be extended from
 * the asReferenced class. By doing so, the OSC networking interface is created
 * automatically, and the wxVess gui is automatically populated with the nodes
 * editable properties.
 */
class asReferenced : virtual public osg::Group
{

public:


	// ***********************************************************
	// methods:

	asReferenced(asSceneManager *sceneManager, char *initID);
	~asReferenced();

	void registerNode(asSceneManager *s);

	/**
	 * For nodes that require regular programmatic control, there is a callback
	 * that is evaluated with every refresh. This function can thus be used for
	 * animations, or any other periodic updates.
	 *
	 * Note that changes to the scene graph structure (eg, moving/deleting nodes
	 * should NOT be done within this callback because traversals stacks will
	 * become corrupted. The technique is rather to enable a flag and then do
	 * the actual change in the asSceneManager::updateGraph() method.
	 */
	virtual void callbackUpdate();

	/**
	 * The attach() method is used to properly add a node to the scene graph.
	 * Each node derived from asReferenced has an attachmentNode parameter that
	 * can be overridden in derived classes, but to keep code simple, this is
	 * the only this method that actually performs the attachment.
	 *
	 * In the default case, nodes should are attached directly to the parent
	 * asReferenced instance.
	 */
	void attach();
	/**
	 * The detach() method is removes a node from the scene graph, depending on
	 * it's attachment position (specified by attachmentNode).
	 */
	void detach();

	/**
	 * IMPORTANT:
	 * subclasses of asReferenced are allowed to contain complicated subgraphs,
	 * and can also change their attachmentNode so that children are attached
	 * anywhere in that subgraph. If that is the case, the updateNodePath()
	 * function MUST be overridden, and extra nodes must be manually pushed onto
	 * currentNodePath.
	 */
	virtual void updateNodePath();

	/**
	 * An internal method that keeps track of the nodepath (for efficient
	 * computation of global position, etc.
	 */
	void updateChildNodePaths();

	/**
	 * TO BE DEPRECATED?
	 * It used to be that we wanted to keep a list of all child asReferenced
	 * nodes that are attached. Was this so that we could differentiate VESS
	 * nodes from other scene graph node? That seems silly, since one can always
	 * try a dynamic_cast or use osgIntrospection
	 */
	asReferenced *as_getChild(asReferenced *child);
	/**
	 * TO BE DEPRECATED?
	 */
	void as_addChild(asReferenced *child);
	/**
	 * TO BE DEPRECATED?
	 */
	void as_removeChild(asReferenced *child);

	/**
	 * TO BE DEPRECATED?
	 * The idea is that one type of node can only be attached to certain types
	 * of other nodes, but that has not been implemented. Currently, the only
	 * illegal parents include the node itself, or any children.
	 */
	bool legalParent (t_symbol *newParent);

	/**
	 * This method schedules a change in parent for this node. The setParent()
	 * does not immediately change the scenegraph, since it can be called at any
	 * time, even while in a traversal. The graph is updated later using the
	 * attach() method, which is called by asSceneManager->updateGraph() when
	 * there is a legal time to re-order the scenegraph.
	 *
	 * Internally, this method just sets the newParent property.
	 */
	void setParent (const char *newvalue);

	/**
	 * Returns the current parent name (string)
	 */
	char *getParent() { return parent->s_name; }

	/**
	 * Returns the current parent as an osg::Group
	 */
	osg::Group *getParent(unsigned int i) { return osg::Group::getParent(i); }

	/**
	 * Enables a simple text label. For more control, use the asText node.
	 */
	void setTextFlag (int b);
	int  getTextFlag () { return (int) this->textFlag; }

	/**
	 * Debug print (to log/console)
	 */
	void debug();

	/**
	 * For each subclass of asReferenced, we override the getState() method to
	 * fill the vector with the correct set of methods for this particular node.
	 */
	virtual std::vector<lo_message> getState();

	/**
	 * StateDump() is a request to broadcast the node state via asSceneManager.
	 *
	 * Note that even though this function is declared virtual, each derived
	 * class must declare it in it's class definition. Simple C++ inheritance is
	 * not enough, because osg::Introspection won't see it.
	 */
	virtual void stateDump();


	// ***********************************************************
	// data:


	t_symbol *id;
	std::string nodeType;

	int pd_mail_id;
	lo_method oscHandler;

	t_symbol *parent, *newParent;

	// debug

	bool textFlag;

	// the node that children get attached to:
	//osg::ref_ptr<osg::Group> attachmentNode;
	osg::Group *attachmentNode;

	osg::NodePath currentNodePath;

	std::vector<asReferenced*> children;


	//osg::ref_ptr<osg::PositionAttitudeTransform> textTransform;
	//osg::ref_ptr<osgText::Text> textLabel;
	osg::ref_ptr<osg::Geode> textGeode;

	asSceneManager *sceneManager;
	asMediaManager *mediaManager;


private:

};



// ***********************************************************
// TODO: we should only be doing the callback for VESS. Currently,
//       asViewer (maybe other clients) will also process these events
//       during their draw loops
class asReferenced_callback : public osg::NodeCallback
{

	public:
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
		{
			osg::ref_ptr<asReferenced> thisNode = dynamic_cast<asReferenced*> (node->getUserData());

			if (thisNode != NULL)
			{
				//std::cout << "in asReferenced_callback for node: " << thisNode->id->s_name << std::endl;
				thisNode->callbackUpdate();
			}
			traverse(node, nv);
		}
};


#endif
