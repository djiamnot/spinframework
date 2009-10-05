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

#include <osgText/Text>
#include <osg/Billboard>

#include "osgUtil.h"
#include "TextNode.h"
#include "SceneManager.h"
#include "MediaManager.h"



using namespace std;


extern pthread_mutex_t pthreadLock;


// ===================================================================
// constructor:
TextNode::TextNode (SceneManager *sceneManager, char *initID) : ReferencedNode(sceneManager, initID)
{
	this->setName(string(id->s_name) + ".TextNode");
	nodeType = "TextNode";

	textTransform = new osg::PositionAttitudeTransform();
	textTransform->setName(string(id->s_name) + ".textTransform");
	this->addChild(textTransform.get());

	// When children are attached to this, they get added to the attachNode:
	// NOTE: by changing this, we MUST override the updateNodePath() method!
	setAttachmentNode(textTransform.get());

	_text = "";
	_font = "arial.ttf";
	_color = osg::Vec4(1.0,1.0,1.0,1.0);
	_billboard = RELATIVE; // ie, no billboard
	
}

// ===================================================================
// destructor
TextNode::~TextNode()
{

}

// ===================================================================

// IMPORTANT:
// subclasses of ReferencedNode are allowed to contain complicated subgraphs, and
// can also change their attachmentNode so that children are attached anywhere
// in this subgraph. If that is the case, the updateNodePath() function MUST be
// overridden, and extra nodes must be manually pushed onto the currentNodePath.

void TextNode::updateNodePath()
{
	currentNodePath.clear();
	if ((parent!=WORLD_SYMBOL) && (parent!=NULL_SYMBOL))
	{
		osg::ref_ptr<ReferencedNode> parentNode = parent->s_thing;
		if (parentNode.valid())
		{
			currentNodePath = parentNode->currentNodePath;
		}
	}

	// here, the nodePath includes the base osg::group, PLUS the textTransform
	currentNodePath.push_back(this);
	currentNodePath.push_back(textTransform.get());

	// now update NodePaths for all children:
	updateChildNodePaths();

}

// ===================================================================
// ======================== SET METHODS: =============================
// ===================================================================


void TextNode::setText (const char *s)
{
	if (this->_text != string(s))
	{
		//getText().createUTF8EncodedString();
		
		this->_text = string(s);

		drawText();

		BROADCAST(this, "ss", "setText", getText());
	}
}

void TextNode::setFont (const char *s)
{
	if (this->_font != string(s))
	{
		this->_font = string(s);

		drawText();

		BROADCAST(this, "ss", "setFont", getFont());
	}
}

void TextNode::setBillboard (billboardType t)
{
	if (t == _billboard) return;
	else _billboard = t;

	drawText();

	BROADCAST(this, "si", "setBillboard", (int) _billboard);

}

void TextNode::setColor (float r, float g, float b, float a)
{
	_color = osg::Vec4(r,g,b,a);

	drawText();

	BROADCAST(this, "sffff", "setColor", r, g, b, a);
}

// ===================================================================

void TextNode::setTranslation (float x, float y, float z)
{
	textTransform->setPosition(osg::Vec3(x,y,z));

	BROADCAST(this, "sfff", "setTranslation", x, y, z);
}

void TextNode::setOrientation (float p, float r, float y)
{
	_orientation = osg::Vec3(p, r, y);

	osg::Quat q = osg::Quat( osg::DegreesToRadians(p), osg::Vec3d(1,0,0),
							 osg::DegreesToRadians(r), osg::Vec3d(0,1,0),
							 osg::DegreesToRadians(y), osg::Vec3d(0,0,1));

	textTransform->setAttitude(q);

	BROADCAST(this, "sfff", "setOrientation", p, r, y);
}

void TextNode::setScale (float x, float y, float z)
{
	textTransform->setScale(osg::Vec3(x,y,z));

	BROADCAST(this, "sfff", "setScale", x, y, z);
}

// =============================================================================
void TextNode::drawText()
{

    pthread_mutex_lock(&pthreadLock);

	// first remove existing text:
	if (this->getAttachmentNode()->containsNode(textGeode.get()))
	{
		this->getAttachmentNode()->removeChild(textGeode.get());
		textGeode = NULL;
	}

	// create a new text label if the labelFlag is set:
	if (_text.length())
	{
		
		if (_billboard)
		{
			osg::Billboard *b = new osg::Billboard();
			switch (_billboard)
			{
				case POINT_EYE:
					b->setMode(osg::Billboard::POINT_ROT_EYE);
					break;
				case STAY_UP:
					b->setMode(osg::Billboard::AXIAL_ROT);
					b->setAxis(osg::Vec3(0.0f,0.0f,1.0f));
					b->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));
					break;
			}
			textGeode = b;
			
		} else {
			textGeode = new osg::Geode();
		}
		textGeode->setName(string(id->s_name) + ".textGeode");
		
		// attach geode and text node:
		this->getAttachmentNode()->addChild(textGeode.get());

		osgText::Text *textLabel = new osgText::Text();
		textGeode->addDrawable(textLabel);

		// set text:
		textLabel->setText(this->_text);

		// set some parameters for the text:
		textLabel->setCharacterSize(0.01f);
		//textLabel->setFont(0); // inbuilt font (small)
		textLabel->setFont( sceneManager->resourcesPath + _font );
		textLabel->setFontResolution(40,40);
		textLabel->setColor( _color );

		textLabel->setDrawMode(osgText::Text::TEXT);
		//textLabel->setDrawMode(osgText::Text::ALIGNMENT);
		//textLabel->setDrawMode(osgText::Text::BOUNDINGBOX);
		
		//textLabel->setAlignment(osgText::Text::CENTER_BOTTOM); // means text bottom

		// disable lighting effects on the text, and allow transparency:
		osg::StateSet *labelStateSet = new osg::StateSet;
		labelStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
		labelStateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
		labelStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		textLabel->setStateSet( labelStateSet );
	}

	pthread_mutex_unlock(&pthreadLock);

}

// =============================================================================
std::vector<lo_message> TextNode::getState ()
{
	// inherit state from base class
	std::vector<lo_message> ret = ReferencedNode::getState();

	lo_message msg;
	osg::Vec3 v;
	osg::Vec4 v4;


	msg = lo_message_new();
	v = this->getTranslation();
	lo_message_add(msg, "sfff", "setTranslation", v.x(), v.y(), v.z());
	ret.push_back(msg);

	msg = lo_message_new();
	v = this->getOrientation();
	lo_message_add(msg, "sfff", "setOrientation", v.x(), v.y(), v.z());
	ret.push_back(msg);

	msg = lo_message_new();
	v = this->getScale();
	lo_message_add(msg, "sfff", "setScale", v.x(), v.y(), v.z());
	ret.push_back(msg);

	msg = lo_message_new();
	lo_message_add(msg, "ss", "setText", getText());
	ret.push_back(msg);

	msg = lo_message_new();
	lo_message_add(msg, "ss", "setFont", getFont());
	ret.push_back(msg);

	msg = lo_message_new();
	lo_message_add(msg, "si", "setBillboard", getBillboard());
	ret.push_back(msg);
	
	msg = lo_message_new();
	v4 = this->getColor();
	lo_message_add(msg, "sffff", "setColor", v4.x(), v4.y(), v4.z(), v4.w());
	ret.push_back(msg);

	return ret;
}
