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

#include "Menu3D.h"
#include "spinApp.h"
#include "osgUtil.h"
#include "spinBaseContext.h"
#include "SceneManager.h"

extern pthread_mutex_t sceneMutex;

static unsigned int COUNTER = 0;
static float Z_OFFSET = 0.2;

// -----------------------------------------------------------------------------
// constructor:
Menu3D::Menu3D (SceneManager *sceneManager, char *initID) : GroupNode(sceneManager, initID)
{
	this->setName(std::string(id->s_name) + ".Menu3D");
	nodeType = "Menu3D";

	highlighted_ = 0;

	font_ = "GillSans.ttf";
	billboardType_ = TextNode::STAY_UP;
	color_ = osg::Vec4(0.8,0.8,0.8,1.0);
	highlightColor_ = osg::Vec4(1.0,1.0,1.0,1.0);

}

// -----------------------------------------------------------------------------
// destructor
Menu3D::~Menu3D()
{
	// remove text nodes:
	MenuVector::iterator i;
    for (i = items_.begin(); i != items_.end(); i++)
    {
    	if ((*i).valid()) sceneManager->deleteNode((*i)->id->s_name);
	}
    items_.clear();
}


// -----------------------------------------------------------------------------
void Menu3D::addItem (const char *itemText)
{
	// We only add and remove items on the server side, because the actions will
	// result in messages to create the appropriate nodes on the client anyway:
    if (1) // ( spinApp::Instance().getContext()->isServer() )
    {

    	// check if a menu item for that text already exists:
    	MenuVector::iterator i;
    	for (i = items_.begin(); i != items_.end(); i++)
    	{
    		if ((*i)->getTextString()==std::string(itemText)) return;
    	}

    	// create a new id for the item:
    	char itemID[256];
    	sprintf(itemID, "%s_item_%06d", this->id->s_name, ++COUNTER);

    	std::cout << "adding item: " << itemID << std::endl;

		// get ReferencedNode for the ID:
		osg::observer_ptr<TextNode> n = dynamic_cast<TextNode*>(sceneManager->getOrCreateNode(itemID, "TextNode"));
		if (!n.valid())
		{
			std::cout << "WARNING: Menu3D '" << this->id->s_name << "' could not create TextNode: '" << itemID << "'" << std::endl;
			return;
		}

		// initial properties:
		n->setTranslation(0.0, 0.0, items_.size() * -Z_OFFSET);
		n->setFont(font_.c_str());
		n->setColor(color_.x(),color_.y(),color_.z(),color_.w());
		n->setBillboard(billboardType_);
		n->setInteractionMode(GroupNode::SELECT);
		n->setTextValue(itemText);

		// add it to the list:
		items_.push_back(n);

		// if this is the first item, highlight it:
		if (items_.size() == 1) setHighlighted(0);

		BROADCAST(this, "ss", "addItem", itemText);
    }
}

void Menu3D::removeItem (int itemIndex)
{
	if ((itemIndex>=0) && (itemIndex<(int)items_.size()))
	{
		if (!doRemoveItem(items_[itemIndex]))
			std::cout << "WARNING: Menu3D '" << this->id->s_name << "' failed to remove item " << itemIndex << std::endl;
	}
}
void Menu3D::removeItem (const char *itemID)
{
	osg::observer_ptr<TextNode> n = dynamic_cast<TextNode*>(sceneManager->getNode(itemID));
	if (!doRemoveItem(n))
		std::cout << "WARNING: Menu3D '" << this->id->s_name << "' failed to remove item '" << itemID << "'" << std::endl;
}
int Menu3D::doRemoveItem (osg::observer_ptr<TextNode> n)
{
	if (!n.valid()) return 0;

	// if we are removing the highlighted item, then we need to un-
	// highlight it:
	if (n.get() == highlighted_.get()) highlighted_ = 0;

	// on the server, find it in the list, and remove it:
	int found = 0;
	MenuVector::iterator i;
	for (i = items_.begin(); i != items_.end(); i++)
	{
		if (n.get()==(*i).get())
		{
			BROADCAST(this, "ss", "removeItem", (*i)->id->s_name);
			sceneManager->deleteNode((*i)->id->s_name);
			items_.erase(i);
			found = 1;
		}

		// Notice that we don't stop iterating once we've found and removed the
		// item. This is because we have to shift all the rest of the items up:
		if (found) (*i)->translate(0,0,Z_OFFSET);
	}

	return found;
}
// -----------------------------------------------------------------------------

void Menu3D::setHighlighted(int itemIndex)
{
	if ((itemIndex>=0) && (itemIndex<(int)items_.size()))
	{
		doHighlight(items_[itemIndex]);
	}
}

void Menu3D::setHighlighted(const char *itemID)
{
	osg::observer_ptr<TextNode> n = dynamic_cast<TextNode*>(sceneManager->getNode(itemID));
	doHighlight(n);
}

int Menu3D::doHighlight(osg::observer_ptr<TextNode> n)
{
	// make sure the node is valid and in our list of items:
	if (n.valid())
	{
		// return previous item to non-highlighted color:
		if (highlighted_.valid()) highlighted_->setColor(color_.x(), color_.y(), color_.z(), color_.w());
		// set the new highlight id:
		highlighted_ = n.get();
		// set the highlight color on the new item:
		highlighted_->setColor(highlightColor_.x(), highlightColor_.y(), highlightColor_.z(), highlightColor_.w());

		BROADCAST(this, "ss", "setHighlighted", getHighlighted());
		return 1;
	}
	return 0;
}

void Menu3D::setHighlightColor(float r, float g, float b, float a)
{
	highlightColor_ = osg::Vec4(r,g,b,a);
	BROADCAST(this, "sffff", "setHighlightColor", r, g, b, a);

	if (highlighted_.valid())
	{
		highlighted_->setColor(highlightColor_.x(), highlightColor_.y(), highlightColor_.z(), highlightColor_.w());
	}
}

// -----------------------------------------------------------------------------

// Methods wrapped from TextNode:
//
// these methods set internal variables for any future items, and also update
// current items:

void Menu3D::setFont (const char *s)
{
	font_ = std::string(s);

	MenuVector::iterator i;
    for (i = items_.begin(); i != items_.end(); i++)
    {
    	if ((*i).valid()) (*i)->setFont(s);
    }
}

void Menu3D::setBillboard (TextNode::billboardType t)
{
	billboardType_ = t;

	MenuVector::iterator i;
    for (i = items_.begin(); i != items_.end(); i++)
    {
    	if ((*i).valid()) (*i)->setBillboard(t);
    }
}

void Menu3D::setColor (float r, float g, float b, float a)
{
	color_ = osg::Vec4(r,g,b,a);

	MenuVector::iterator i;
    for (i = items_.begin(); i != items_.end(); i++)
    {
    	if ((*i).valid()) (*i)->setColor(r, g, b, a);
    }
}

// -----------------------------------------------------------------------------
std::vector<lo_message> Menu3D::getState ()
{
	// inherit state from base class
	std::vector<lo_message> ret = GroupNode::getState();

	lo_message msg;
	MenuVector::iterator i;
    for (i = items_.begin(); i != items_.end(); i++)
    {
    	if ((*i).valid())
		{
    		msg = lo_message_new();
    		lo_message_add(msg, "sss", "addItem", (*i)->id->s_name, (*i)->getTextValue());
    		ret.push_back(msg);
		}
    }

	msg = lo_message_new();
	lo_message_add(msg, "ss", "setHighlighted", getHighlighted());
	ret.push_back(msg);

	msg = lo_message_new();
	osg::Vec4 v4 = this->getHighlightColor();
	lo_message_add(msg, "sffff", "setHighlightColor", v4.x(), v4.y(), v4.z(), v4.w());
	ret.push_back(msg);

	return ret;
}