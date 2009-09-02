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


#include "SceneManager.h"
#include "MediaManager.h"
#include "ReferencedNode.h"
//#include "GroupNode.h"
//#include "DSPNode.h"
//#include "SoundNode.h"
//#include "SoundSpace.h"
#include "SoundConnection.h"
//#include "ModelNode.h"
#include "spinUtil.h"
#include "osgUtil.h"


#include "nodeVisitors.h"

#include <lo/lo.h>
#include <lo/lo_lowlevel.h>

#include <sstream>

#include <osgDB/WriteFile>


#include <osgIntrospection/Reflection>
#include <osgIntrospection/Type>
#include <osgIntrospection/Value>
#include <osgIntrospection/variant_cast>
#include <osgIntrospection/Exceptions>
#include <osgIntrospection/MethodInfo>
#include <osgIntrospection/PropertyInfo>


#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/StaticMethodInfo>
#include <osgIntrospection/Attributes>

#include <osgIntrospection/ExtendedTypeInfo>

using namespace std;
using namespace osgIntrospection;


extern pthread_mutex_t pthreadLock;

// *****************************************************************************
// constructors:

SceneManager::SceneManager (std::string id, std::string addr, std::string port)
{
	int i;

	this->sceneID = id;

	graphicalMode = false;


	rxAddr = lo_address_new(addr.c_str(), port.c_str());

	// initialize storage vectors:
	nodeTypes.clear();
	nodeMap.clear();
	nodeList.clear();

	// set up OSC event listener:

	if (isMulticastAddress(addr))
	{
		rxServ = lo_server_thread_new_multicast(addr.c_str(), port.c_str(), oscParser_error);
	} else {
		rxServ = lo_server_thread_new(port.c_str(), oscParser_error);
	}

	txServ = NULL;

	// add OSC callback methods to match various incoming messages:

#ifdef OSCDEBUG
	// oscCallback_debug() will match any path and args:
	lo_server_thread_add_method(rxServ, NULL, NULL, SceneManagerCallback_debug, NULL);
#endif

	// generic admin callback:
	lo_server_thread_add_method(rxServ, string("/SPIN/"+sceneID).c_str(), NULL, SceneManagerCallback_admin, this);


	
	// start the listener:
	lo_server_thread_start(rxServ);

	std::cout << "  SceneManager '" << id << "' is listening on " << addr << ", port: " << port << std::endl;


	// discover all relevant nodeTypes by introspection, and fill the nodeMap
	// with empty vectors:
	const osgIntrospection::Type &ReferencedNodeType = osgIntrospection::Reflection::getType("ReferencedNode");
	nodeTypes.clear();
	const osgIntrospection::TypeMap &allTypes = osgIntrospection::Reflection::getTypes();
	osgIntrospection::TypeMap::const_iterator it;
	for ( it=allTypes.begin(); it!=allTypes.end(); it++)
	{
		if (((*it).second)->isDefined())
		{
			//std::cout << ((*it).second)->getName() << " isSubclassOf(ReferencedNode)? " << ((*it).second)->isSubclassOf(ReferencedNodeType) << std::endl;
			if ( ((*it).second)->isSubclassOf(ReferencedNodeType) )
			{
				string theType = ((*it).second)->getName();
				nodeTypes.push_back(theType);
				nodeListType emptyVector;
				nodeMap.insert(nodeMapPair(theType, emptyVector));
			}
		}
	}

	// need to remove DSPNode???

	/*
	std::cout << "Found the following nodeTypes:" << std::endl;
	vector<string>::const_iterator si;
	for( si=nodeTypes.begin(); si!=nodeTypes.end(); si++)
	{
		cout << *si << endl;
	}
	*/


	// create some initial nodeS:
	rootNode = new osg::Group();
	rootNode->setName("root");
	worldNode = new osg::Group();
	worldNode->setName("world");
	rootNode->addChild(worldNode.get());

	for (i=0; i<OSG_NUM_LIGHTS; i++)
	{
		activeLights[i] = false;
	}

	// why do we do this?:
	osg::StateSet* rootStateSet = new osg::StateSet;
	for (i=0; i<OSG_NUM_LIGHTS; i++)
	{
		rootStateSet->setMode(GL_LIGHT0 + i, osg::StateAttribute::ON);
		//rootStateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
	}
	rootNode->setStateSet(rootStateSet);

	std::string dataPath = "~/Documents/Audioscape-Data"; // this isn't used anymore
	mediaManager = new MediaManager(dataPath);

	this->sendSceneMessage("s", "userRefresh");
}

// *****************************************************************************
// destructor
SceneManager::~SceneManager()
{
	lo_server_thread_stop(rxServ);
	usleep(50);
	
	if (txServ) lo_server_free(txServ);
	if (rxServ) lo_server_thread_free(rxServ);
    //if (txAddr) lo_address_free(txAddr);
	//if (rxAddr) lo_address_free(rxAddr);

}

//void SceneManager::setLog(const char* filename)
void SceneManager::setLog(spinLog &log)
{
	// remove existing callback:
	

			
	lo_server_thread_add_method(rxServ, NULL, NULL, SceneManagerCallback_log, &log);
}

void SceneManager::setTXaddress (std::string addr, std::string port)
{
	// Note that only a spinServer will set the txAddr, so that events are
	// re-broadcasted. Other apps, such as simple renderers/editors/etc will
	// leave the txAddr null.

	// events for this scene are broadcasted to the following address:
	txAddr = lo_address_new(addr.c_str(), port.c_str());

	if (isMulticastAddress(addr))
	{
		txServ = lo_server_new_multicast(addr.c_str(), NULL, oscParser_error);
	}

	else if (isBroadcastAddress(addr))
	{
		txServ = lo_server_new(NULL, oscParser_error);
		int sock = lo_server_get_socket_fd(txServ);
		int sockopt = 1;
		setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &sockopt, sizeof(sockopt));
	}

	else {
		txServ = lo_server_new(NULL, oscParser_error);
	}

	if (!txServ) std::cout << "ERROR: SceneManager::setTXaddress(). Bad address?: " << addr << ":" << port << std::endl;
	else
	{
		std::cout << "  SceneManager is sending scene updates to: " << lo_address_get_url(txAddr) << std::endl;
		//std::cout << "  ... via liblo server: " << lo_server_get_url(txServ) << std::endl;
	}
}

void SceneManager::sendSceneMessage(const char *types, ...)
{
	if (!txServ) return;


	lo_message msg = lo_message_new();

	va_list ap;
	va_start(ap, types);

	int err = lo_message_add_varargs(msg, types, ap);

	if (!err)
	{
		string OSCpath = "/SPIN/" + sceneID;
		lo_send_message_from(txAddr, txServ, OSCpath.c_str(), msg);
	} else {
		std::cout << "ERROR (lo_message_add_varargs): " << err << std::endl;
	}

	//lo_message_free(msg);
}

void SceneManager::sendNodeList(std::string typeFilter)
{
	// TODO: typeFilter not used yet

	if (!txServ) return;

	int i;
	string OSCpath;
	lo_message msg;

	nodeMapType::iterator it;
	nodeListType::iterator iter;

	if ((typeFilter.empty()) || (typeFilter=="*"))
	{
		// for each type, send an OSC message:
		for ( it=nodeMap.begin(); it!=nodeMap.end(); it++ )
		{
			//std::cout << "Nodes of type '" << (*it).first << "':";
			//for (i=0; i<(*it).second.size(); i++) std::cout << " " << (*it).second[i];
			//std::cout << std::endl;

			msg = lo_message_new();
			OSCpath = "/SPIN/" + sceneID; // + "/nodeList/" + (*it).first;

			lo_message_add_string(msg, "nodeList" );
			lo_message_add_string(msg, (*it).first.c_str() );

			if ( (*it).second.size() )
			{
				for (iter = (*it).second.begin(); iter != (*it).second.end(); iter++)
				{
					lo_message_add_string(msg, (char*) (*iter)->id->s_name );
				}
			} else {
				lo_message_add_string(msg, "NULL");
			}

			lo_send_message_from(txAddr, txServ, OSCpath.c_str(), msg);
		}
	}

	// just send a list for the desired type:
	else {

		it = nodeMap.find(typeFilter);
		if ( it != nodeMap.end() )
		{
			msg = lo_message_new();
			OSCpath = "/SPIN/" + sceneID; // + "/nodeList/" + (*it).first;

			lo_message_add_string(msg, "nodeList" );
			lo_message_add_string(msg, (*it).first.c_str() );
			if ( (*it).second.size() )
			{
				for (iter = (*it).second.begin(); iter != (*it).second.end(); iter++)
				{
					lo_message_add_string(msg, (char*) (*iter)->id->s_name );
				}
			} else {
				lo_message_add_string(msg, "NULL");
			}

			lo_send_message_from(txAddr, txServ, OSCpath.c_str(), msg);
		}
	}
}

void SceneManager::sendNodeMessage(t_symbol *nodeSym, lo_message msg)
{
	if (!txServ) return;

	string OSCpath = "/SPIN/" + sceneID + "/" + string(nodeSym->s_name);
	lo_send_message_from(txAddr, txServ, OSCpath.c_str(), msg);

	//lo_message_pp(msg);
	lo_message_free(msg);

}

void SceneManager::sendNodeMessage(t_symbol *nodeSym, const char *types, ...)
{
	if (!txServ) return;

	int err;
	lo_message msg = lo_message_new();

	va_list ap;
	va_start(ap, types);

	/*
    while (types && *types) {
        count++;
        switch (*types++) {
        */

	/* The data will be added in OSC byteorder (bigendian).
	 * IMPORTANT: args list must be terminated with LO_ARGS_END, or this call will
	 * fail.  This is used to do simple error checking on the sizes of parameters
	 * passed.
	*/

	err = lo_message_add_varargs(msg, types, ap);

	if (!err)
	{
		sendNodeMessage(nodeSym, msg);
	} else {
		std::cout << "ERROR (lo_message_add_varargs): " << err << std::endl;
	}

}



// *****************************************************************************
void SceneManager::debug()
{

	std::cout << "****************************************" << std::endl;
	std::cout << "************* SCENE DEBUG: *************" << std::endl;

	std::cout << "\n NODE LIST for scene with id '" << sceneID << "':" << std::endl;

	/*
	vector< osg::ref_ptr<ReferencedNode> >::iterator iter;
	for (iter = nodeList.begin(); iter != nodeList.end() ; iter++)
	{
		std::cout << (*iter)->id->s_name << " (parent=" << (*iter)->parent->s_name << ")" << std::endl;
	}
	*/


	nodeMapType::iterator it;
	for (it = nodeMap.begin(); it != nodeMap.end(); it++)
	{
		std::cout << "  nodes with type '" << (*it).first << "':" << std::endl;

		nodeListType::iterator iter;
		for (iter = (*it).second.begin(); iter != (*it).second.end(); iter++)
		{
			std::cout << "    " << (*iter)->id->s_name << " (parent=" << (*iter)->parent->s_name << ")" << std::endl;
		}
	}


	std::cout << "\n SCENE GRAPH:" << std::endl;
	DebugVisitor ev;
	ev.apply(*(this->rootNode.get()));
}


// *****************************************************************************

ReferencedNode* SceneManager::createNode(string id, string type)
{
    const char *charID = id.c_str();
    const char *charType = type.c_str();
    return createNode(charID, charType);
}

ReferencedNode* SceneManager::createNode(const char *id, const char *type)
{
	t_symbol *nodeID = gensym(id);
	string nodeType = string(type);

	// ignore SoundConnection messages (these should be created by
	// the DSPNode::connect() method
	if (nodeType == "SoundConnection") return NULL;

	// check if a node with that name already exists:
	osg::ref_ptr<ReferencedNode> n = nodeID->s_thing;

	if (n.valid())
	{
		if (n->nodeType != nodeType)
		{
			std::cout << "ERROR: Tried to create " << type << " with id '" << id << "', but that id already exists as an " << n->nodeType << "." << std::endl;
			return NULL;
		} else return n.get();
	}


	osgIntrospection::Value sceneManagerPtr = osgIntrospection::Value(this);

	/*
	std::cout << "[DEBUG] These are all possible types:" << std::endl;
	const osgIntrospection::TypeMap &allTypes = osgIntrospection::Reflection::getTypes();
	for (osgIntrospection::TypeMap::const_iterator it = allTypes.begin (); it != allTypes.end (); ++it)
	{
		std::cout << ((*it).second)->getName() << " isAtomic? " << ((*it).second)->isAtomic() << std::endl;
	}
	*/

	/*
	std::cout << "sceneManagerPtr::" << std::endl;
	std::cout << "is value a typed pointer? " << sceneManagerPtr.isTypedPointer() << std::endl;
	std::cout << "is value null? " << sceneManagerPtr.isNullPointer() << std::endl;
	std::cout << "is value empty? " << sceneManagerPtr.isEmpty() << std::endl;

	const osgIntrospection::Type &tt = sceneManagerPtr.getType();
	std::cout << "the type is:" << std::endl;
	//introspect_print_type(tt);
	std::cout << "name():" << tt.getStdTypeInfo().name() << std::endl;
	std::cout << "is type defined? " << tt.isDefined() << std::endl;
	*/


	// Let's use osgIntrospection to create a node of the proper type:
	const osgIntrospection::Type &t = osgIntrospection::Reflection::getType(type);
	if (t.isDefined())
	{
		//std::cout << "... about to create node of type [" << t.getStdTypeInfo().name() << "]" << std::endl;
		//introspect_print_type(t);

		osgIntrospection::ValueList args;
		args.push_back(sceneManagerPtr);
		//args.push_back(this);
		args.push_back(nodeID->s_name);
		osgIntrospection::Value v = t.createInstance(args);


		/*
		std::cout << "is value a typed pointer? " << v.isTypedPointer() << std::endl;
		std::cout << "is value null? " << v.isNullPointer() << std::endl;
		std::cout << "is value empty? " << v.isEmpty() << std::endl;

		const osgIntrospection::Type &tt = v.getType();
		std::cout << "the reg type is:" << std::endl;
		introspect_print_type(tt);

		const osgIntrospection::Type &ttt = v.getInstanceType();
		std::cout << "the instantiated type is:" << std::endl;
		introspect_print_type(ttt);



		std::cout << "type of ReferencedNode = " << typeid(ReferencedNode).name() << std::endl;
		std::cout << "type of ReferencedNode* = " << typeid(ReferencedNode*).name() << std::endl;
		std::cout << "type of getInstance = " << typeid(osgIntrospection::getInstance<ReferencedNode>(v)).name() << std::endl;
		std::cout << "type returned from variant_cast = " << typeid(osgIntrospection::variant_cast<ReferencedNode>(v)).name() << std::endl;
		std::cout << "type returned from variant_cast* = " << typeid(osgIntrospection::variant_cast<ReferencedNode*>(v)).name() << std::endl;
		std::cout << "type returned from extract_raw_data = " << typeid(osgIntrospection::extract_raw_data<ReferencedNode>(v)).name() << std::endl;
		std::cout << "type returned from extract_raw_data (with reinterpret_cast)= " << typeid(reinterpret_cast<ReferencedNode*>(osgIntrospection::extract_raw_data<ReferencedNode>(v))).name() << std::endl;
		std::cout << "type returned from extract_raw_data (with dynamic_cast)= " << typeid(dynamic_cast<ReferencedNode*>(osgIntrospection::extract_raw_data<ReferencedNode>(v))).name() << std::endl;


		//ReferencedNode test = (osgIntrospection::getInstance<ReferencedNode>(v));
		//ReferencedNode test = osgIntrospection::variant_cast<ReferencedNode>(v);
		ReferencedNode *test = osgIntrospection::variant_cast<ReferencedNode*>(v);
		//ReferencedNode *test = osgIntrospection::extract_raw_data<ReferencedNode>(v);
		//ReferencedNode *test = reinterpret_cast<ReferencedNode*>(osgIntrospection::extract_raw_data<ReferencedNode>(v));
		//ReferencedNode *test = dynamic_cast<ReferencedNode*>(osgIntrospection::extract_raw_data<ReferencedNode>(v));

		//std::cout << "test type = " << typeid(test).name() << std::endl;
		//std::cout << "test id = " << test.id->s_name << std::endl;
		std::cout << "test id = " << test->id->s_name << std::endl;


		n = test;
		*/
		n = osgIntrospection::variant_cast<ReferencedNode*>(v);


	} else {
		std::cout << "ERROR: Type " << type << " is not defined." << std::endl;
		return NULL;
	}

	if (n.valid())
	{

		// Must call this for newly created nodes. the function registers the node
		// with an OSC callback, and sets a pointer to this SceneManager
		//n->registerNode(this);


		// NOTE: by pushing the referenced pointer onto the nodeList, we make sure
		// that at least one reference to the node is always maintianed. This allows
		// us to detach it from the scene graph, yet still keep its memory. However,
		// it also means that we have to explicitely remove it.
		//nodeList.push_back(n);
		nodeMap[n->nodeType].push_back(n);

		// broadcast:
		//std::cout << "sending createNode message: " << ("/SPIN/"+sceneID).c_str() << " createNode " << id << " " << type << std::endl;
		if (txServ) lo_send_from(txAddr, txServ, LO_TT_IMMEDIATE, ("/SPIN/"+sceneID).c_str(), "sss", "createNode", id, type);
		//sendNodeList(n->nodeType);

		return n.get();
	}
	else
	{
		std::cout << "ERROR: Could not create " << nodeType << ". Invalid type?" << std::endl;
		return NULL;
	}

}

// *****************************************************************************
// returns a pointer to a node given an id:
ReferencedNode* SceneManager::getNode(string id)
{
    char *charID = (char*) id.c_str();
    return getNode(charID);
}
ReferencedNode* SceneManager::getNode(const char *id)
{
	osg::ref_ptr<ReferencedNode> n = gensym(id)->s_thing;

	if (n.valid()) return n.get();
	else return NULL;
}


// *****************************************************************************
// returns a pointer to an node given an id and type:
ReferencedNode* SceneManager::getNode(const char *id, const char *type)
{
	osg::ref_ptr<ReferencedNode> n = gensym(id)->s_thing;

	if (n.valid())
	{
		if (n->nodeType == string(type))
		{
			return n.get();
		}
	}

	return NULL;
}

// *****************************************************************************
ReferencedNode* SceneManager::getOrCreateNode(const char *id, const char *type)
{
	osg::ref_ptr<ReferencedNode> n = getNode(id, type);
	if (n.valid()) return n.get();
	else return createNode(id, type);
}



// *****************************************************************************
void SceneManager::deleteNode(const char *id)
{
	// don't use ref_ptr here, otherwise node will stay alive until the end of
	// the function, and we want to ensure the destructor is called in doDelete
	ReferencedNode *n = getNode(id);

	if (n)
	{
	
		// for the deleteNode method, all children nodes will remain, so we just
		// change their parent to the "world":
		vector<ReferencedNode*>::iterator childIter;
		for (childIter = n->children.begin(); childIter!=n->children.end(); childIter++)
		{
			std::cout << "sstting parent for " << (*childIter)->id->s_name << " (who's current parent is " << (*childIter)->parent->s_name << ") to world" << std::endl;
			(*childIter)->setParent("world");
		}
		
		
		doDelete(n);
		
	}
	else std::cout << "ERROR: tried to deleteNode " << id << ", but that node does not exist." << std::endl;

	// if delete was successful and removed all other references to the node,
	// then by this point, the node will be deleted, and it's destructor will
	// have been called.

}


// *****************************************************************************
void SceneManager::deleteGraph(const char *id)
{
	// don't use ref_ptr here, otherwise node will stay alive until the end of
	// the function, and we want to ensure the destructor is called in doDelete
	ReferencedNode *n = getNode(id);

	if (n)
	{
		// for the deleteGraph method, we also delete all children:
		while (n->children.size())
		{
			doDelete(*(n->children.begin()));
		}
	
		doDelete(n);	
	}
	else std::cout << "ERROR: tried to deleteGraph " << id << ", but that node does not exist." << std::endl;
	
	// if delete was successful and removed all other references to the node,
	// then by this point, the node will be deleted, and it's destructor will
	// have been called.
}

// *****************************************************************************
void SceneManager::doDelete(ReferencedNode *nodeToDelete)
{

	// hold on to a referenced pointer, while we remove all others
	osg::ref_ptr<ReferencedNode> n = nodeToDelete;

	// remove the node from the scenegraph:
	n->detach();

	// remove from our storage nodeMap:
	nodeMapType::iterator it;
	for ( it=nodeMap.begin(); it!=nodeMap.end(); it++)
	{
		vector< osg::ref_ptr<ReferencedNode> >::iterator iter;
		iter = std::find( (*it).second.begin(), (*it).second.end(), n );
		if ( iter != (*it).second.end() ) (*it).second.erase(iter);
		//else std::cout << "ERROR: node " << n->id->s_name << " was not found on the nodeList in SceneManager::removeNode()" << std::endl;
	}

	// TODO: more efficient way (since we know the nodetype):
	/*
	nodeListType::iterator iter;
	iter = std::find( nodeMap[n->nodeType].second.begin(), nodeMap[n->nodeType].second.end(), n );
	if ( iter != nodeMap[n->nodeType].second.end() ) nodeMap[n->nodeType].second.erase(iter);
	*/


	// have to unregister the callback function to remove the last ref_ptr:
	pthread_mutex_lock(&pthreadLock);
	n->setUserData( NULL );
	pthread_mutex_unlock(&pthreadLock);
	
	// now force the actual delete by nulling this referenced pointer. At that
	// time, the destructor for the node should be called
	char *nodeID = n->id->s_name; // but remember the name for the broadcast
	n = NULL;

	// finally, broadcast:
	if (txServ) lo_send_from(txAddr, txServ, LO_TT_IMMEDIATE, ("/SPIN/"+sceneID).c_str(), "ss", "deleteNode", nodeID);

}


// *****************************************************************************
void SceneManager::clear()
{

	
	// first find all UserNodes and check move them to the worldNode:
	nodeListType::iterator iter;
	for (iter = nodeMap[string("UserNode")].begin(); iter != nodeMap[string("UserNode")].end(); iter++)
	{
		(*iter)->setParent("world");
	}
	
	
	/*
	// now go through all children of worldNode and do a deleteGraph on any node
	// that is not a UserNode:
	// ERROR: only does odd ones
	for (int i=0; i<worldNode->getNumChildren(); i++)
	{
		ReferencedNode *n = dynamic_cast<ReferencedNode*>(worldNode->getChild(i));
		if (n)
		{
			// as long as this isn't a UserNode, delete 
			if (!dynamic_cast<UserNode*>(n))
			{
				deleteGraph(n->id->s_name);
			}
		}
	}
	*/
	
	int i = 0;
	ReferencedNode *n;
	while (i < worldNode->getNumChildren())
	{
		if (n=dynamic_cast<UserNode*>(worldNode->getChild(i)))
		{
			// skip UserNodes
			i++;
		}
		else if (n=dynamic_cast<ReferencedNode*>(worldNode->getChild(i)))
		{
			// delete the graph of any ReferencedNode:
			deleteGraph(n->id->s_name);
		}
		else
		{
			// it's possible that there are other nodes attached to worldNode,
			// so just skip them:
			i++;
		}
	}
	
	/*
	// now remove any remaining nodes attached to world (that are not UserNodes)
	// including their subgraph via deleteGraph()
	// ERROR: only does odd ones
	nodeMapType::iterator it;
	for (it = nodeMap.begin(); it != nodeMap.end(); it++)
	{
		if ((*it).first != "UserNode")
		{
			
			nodeListType::iterator iter;
			for (iter = (*it).second.begin(); iter != (*it).second.end(); iter++)
			{
				if (strcmp((*iter)->getParent(),"world")==0)
				{
					deleteGraph((*iter)->id->s_name);
				}
			}
		}
	}	
	*/
	
	/*
	ClearSceneVisitor visitor;
	worldNode->accept(visitor);
	*/
	
	
	if (txServ) lo_send_from(txAddr, txServ, LO_TT_IMMEDIATE, ("/SPIN/"+sceneID).c_str(), "s", "clear");

	sendNodeList("*");
	
	std::cout << "Cleared scene." << std::endl;

}

void SceneManager::clearUsers()
{
	while (nodeMap[string("UserNode")].size())
	{
		deleteGraph(nodeMap[string("UserNode")][0]->id->s_name);
	}
	
	if (txServ) lo_send_from(txAddr, txServ, LO_TT_IMMEDIATE, ("/SPIN/"+sceneID).c_str(), "s", "clearUsers");

	sendNodeList("*");
	
	std::cout << "Cleared all users." << std::endl;

}

// *****************************************************************************
void SceneManager::refresh()
{
	sendNodeList("*");

	nodeMapType::iterator it;
	for (it = nodeMap.begin(); it != nodeMap.end(); it++)
	{
		nodeListType::iterator iter;
		for (iter = (*it).second.begin(); iter != (*it).second.end(); iter++)
		{
			(*iter)->stateDump();
		}
	}

    if (txServ) lo_send_from(txAddr, txServ, LO_TT_IMMEDIATE, ("/SPIN/"+sceneID).c_str(), "s", "refresh");

}

// *****************************************************************************
// This function returns a pointer to an SoundConnection object
/*
SoundConnection* SceneManager::getConnection(char *from, char *to)
{
	vector<SoundConnection*>::iterator iter;

	osg::ref_ptr<DSPNode> srcNode = dynamic_cast<DSPNode*>( getNode(from) );
	osg::ref_ptr<DSPNode> snkNode = dynamic_cast<DSPNode*>( getNode(to) );

	if ( srcNode.valid() && snkNode.valid() ) {
		for (iter = srcNode->connectTO.begin(); iter != srcNode->connectTO.end(); iter++)
		{
			if ((*iter)->sink->id == snkNode->id) return (*iter);
		}
	}

	// SoundConnection not found:
	return NULL;

}
*/

// *****************************************************************************
// Here is where we change the graph (ie, attach nodes to their new parents,
// detach them, move them to different parts of the scene graph, etc).
//
// IMPORTANT: We do not want to change the graph during a traversal because this
// will invalidate iterators that are on the stack (NodeVisitor uses nested calls
// to do traversal).
//
// So, take care to only call this function outside of a traversal. For example,
// during the GroupNode callback, we set the newParent symbol, and only after the
// traversal do we call this function which then re-arranges the graph to reflect
// the new parent relationships.

void SceneManager::updateGraph()
{
    // TODO: remove this and just place a thread mutex around the setParent
    // stuff in ReferencedNode.


/*
	nodeMapType::iterator it;
	for (it = nodeMap.begin(); it != nodeMap.end(); it++)
	{
		nodeListType::iterator iter;
		for (iter = (*it).second.begin(); iter != (*it).second.end(); iter++)
		{
			if ((*iter)->newParent != NULL_SYMBOL) (*iter)->attach();
		}
	}
*/

}

// save scene as .osg
void SceneManager::exportScene (const char *nodeID, const char *filename)
{
	std:: string fullPath = string(filename);
	if (fullPath.substr(fullPath.size()-4) != ".osg") fullPath += ".osg";

	if (strcmp(nodeID,"world")==0)
	{
		osgDB::writeNodeFile(*worldNode.get(), fullPath);
		std::cout << "Exported entire scene to: " << fullPath << std::endl;
	}
	else {
		osg::ref_ptr<ReferencedNode> subgraph = getNode(nodeID);
		if (subgraph.valid())
		{
			osgDB::writeNodeFile(*subgraph.get(), fullPath);
			std::cout << "Exported subgraph starting at node '" << subgraph->id->s_name << "' to: " << fullPath << std::endl;
		}
		else std::cout << "Could not find node " << nodeID << ". Export failed." << std::endl;
	}

}


std::string SceneManager::getStateAsXML(vector<lo_message> nodeState)
{
	ostringstream output("");

	lo_arg **args;
	int i, argc;

	char *argTypes;


	// iterate through all state messages and write as xml output:
	vector<lo_message>::iterator nodeStateIterator = nodeState.begin();
	while (nodeStateIterator != nodeState.end())
	{

	    argTypes = lo_message_get_types(*nodeStateIterator);
	    argc = lo_message_get_argc(*nodeStateIterator);
	    args = lo_message_get_argv(*nodeStateIterator);

	    output << "<" << (char*)args[0] << " types=" << argTypes+1 << ">";

	    for (i = 1; i<argc; i++) {
	    	if (i>1) output << " ";
	    	if (lo_is_numerical_type((lo_type)argTypes[i]))
	    	{
	    		output << (float) lo_hires_val( (lo_type)argTypes[i], args[i] );
	    	} else if (strlen((char*) args[i])) {
	    		output << (char*) args[i];
	    	} else {
	    		output << "NULL";
	    	}
	    }

	    output << "</" << (char*)args[0] << ">";

		lo_message_free(*nodeStateIterator); // do we need to do this? isn't it automatic if we do an erase()?
		nodeState.erase(nodeStateIterator); //note: iterator automatically advances after erase()
	}

	return output.str();
}

std::string SceneManager::getNodeAsXML(ReferencedNode *n)
{
	ostringstream output("");

	// open tag for this node:
	output << "<" << n->nodeType << " id=" << n->id->s_name << ">\n";

	output << getStateAsXML( n->getState() );

	// check for children:
	if (!n->children.empty())
	{
		output << "<subgraph>\n";
		vector<ReferencedNode*>::iterator childIter;
		for (childIter = n->children.begin(); childIter != n->children.end(); childIter++)
		{
			output << getNodeAsXML(*childIter);
		}
		output << "</subgraph>\n";
	}

	// remember to close tag:
	output << "</" << n->nodeType << ">\n";

	return output.str();
}

std::string SceneManager::getConnectionsAsXML()
{
	ostringstream output("");
	output << "<connections>\n";

	// go through all DSPNodes (actually, all nodes that are subclasses of
	// DSPNode), and save the connection info at end of file:

	nodeMapType::iterator it;
	nodeListType::iterator iter;
	for (it = nodeMap.begin(); it != nodeMap.end(); it++)
	{
		string nodeType = (*it).first;

		const osgIntrospection::Type &t = osgIntrospection::Reflection::getType(nodeType);
		if (t.isDefined())
		{
			// check if the nodeType is a subclass of DSPNode:
			if (t.getBaseType(0).getName() == "DSPNode")
			{

				for (iter = (*it).second.begin(); iter != (*it).second.end(); iter++)
				{
					osg::ref_ptr<DSPNode> dspNode = dynamic_cast<DSPNode*>((*iter).get());
					
					if ((*iter).valid())
					{
						vector<SoundConnection*>::iterator connIter;
						for (connIter = dspNode->connectTO.begin(); connIter != dspNode->connectTO.end(); connIter++)
						{
							// open tag for this node:
							output << "<SoundConnection id=" << (*connIter)->id->s_name << ">\n";
	
							output << getStateAsXML( (*connIter)->getState() );
	
							// close tag
							output << "</SoundConnection>";
						}
					} //else std::cout << "bad dspnode: " << (*iter)->id->s_name << std::endl;
				}
			}
		}
	}

	output << "</connections>\n";
	return output.str();
}

// *****************************************************************************
bool SceneManager::saveXML(const char *s)
{
	string filename;

	// replace tilde with $HOME variable
	if (s[0]=='~') filename = getenv("HOME") + string(s).substr(1);
	else filename = string(s);


	// start with XML Header:
	ostringstream output("");
	output << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n"
			<< "<!DOCTYPE SPIN SYSTEM>\n"
			<< "<spinScene>\n";


	/*
	vector< osg::ref_ptr<ReferencedNode> >::iterator iter;
	for (iter = nodeList.begin(); iter != nodeList.end() ; iter++)
	{
		// Add the state for this node only if it is a top-level node. Children
		// will be added recursively:
		if ((*iter)->parent == WORLD_SYMBOL)
			output << getNodeStateAsXML( (*iter).get() );
	}
	*/

	nodeMapType::iterator it;
	nodeListType::iterator iter;

	for (it = nodeMap.begin(); it != nodeMap.end(); it++)
	{
		for (iter = (*it).second.begin(); iter != (*it).second.end(); iter++)
		{
			if ((*iter)->parent == WORLD_SYMBOL)
			{
				output << getNodeAsXML((*iter).get());
			}
		}
	}


	output << "</spinScene>\n";


	output << getConnectionsAsXML();


	// now write to file:
	TiXmlDocument outfile( filename.c_str() );
	outfile.Parse( output.str().c_str() );
	if ( outfile.Error() )
	{
		// error!
		std::cout << "ERROR: failed to save " << filename << std::endl;
		return false;
	} else {
		// success!
		outfile.SaveFile();
		std::cout << "Saved scene to: " << filename << std::endl;
		return true;
	}

}


bool SceneManager::createNodeFromXML(TiXmlElement *XMLnode, const char *parentNode= "")
{
	TiXmlElement *child1, *child2;
	char *types;
	string method;
	std::vector<std::string> argVector;
	osgIntrospection::ValueList args;
	int i;
	float f;

	char *nodeType = (char*) XMLnode->Value();

	if (osgIntrospection::Reflection::getType(nodeType).isDefined())
	{
		if (XMLnode->Attribute("id"))
		{
			char *nodeID = (char*) XMLnode->Attribute("id");
			osg::ref_ptr<ReferencedNode> n = getOrCreateNode(nodeID, nodeType);

			// get node as an osgInrospection::Value (note that type will be ReferencedNode pointer):
			const osgIntrospection::Value introspectValue = osgIntrospection::Value(n.get());

			// the getInstanceType() method however, gives us the real type being pointed at:
			const osgIntrospection::Type &introspectType = introspectValue.getInstanceType();




			for ( child1 = XMLnode->FirstChildElement(); child1; child1 = child1->NextSiblingElement() )
			{
				// first check if this XML node is a list of Audiocape children:
				if (child1->Value() == string("subgraph"))
				{

					for (child2 = child1->FirstChildElement(); child2; child2 = child2->NextSiblingElement() )
					{
						createNodeFromXML(child2, n->id->s_name);
					}
					continue;
				}

				// look for child nodes with a 'types' attribute
				if (types = (char*) child1->Attribute("types"))
				{
					method = child1->Value();
					argVector = tokenize(child1->FirstChild()->Value());
				} else continue;

				// special case if method is setParent():
				if (method == "setParent")
				{
					if (!argVector.empty()) n->setParent((char*) argVector[0].c_str());
					continue;
				}

				if (argVector.size() != strlen(types))
				{
					std::cout << "ERROR: could not call '" << method << "' for node " << nodeID << ". Type mismatch." << std::endl;
					continue;
				}

				args.clear();
				for (i=0; i<strlen(types); i++)
				{
					if (lo_is_numerical_type((lo_type)types[i]))
					{
						if (fromString<float>(f, argVector[i])) args.push_back(f);
					} else {
						args.push_back( (const char*) argVector[i].c_str() );
					}
				}

				// now we can finally call the method:
				try {
					introspectType.invokeMethod(method, introspectValue, args, true); // the true means that it will try base classes as well
				}

				catch (osgIntrospection::Exception & ex)
				{
					std::cerr << "catch exception in loadXML: " << ex.what() << std::endl;
				}
			}

			if (parentNode)
			{
				n->setParent(parentNode);
			}

		} else {
			std::cout << "ERROR: Found XML node of type " << nodeType << " with no id attribute. Could not create." << std::endl;
		}

	} else {
		std::cout << "ERROR: Found XML node of type " << nodeType << ", but no such type is registered." << std::endl;
	}


	return true;
}

bool SceneManager::createConnectionsFromXML(TiXmlElement *XMLnode)
{

	// NOTE: This assumes all nodes have already been created, but will create
	//       connections if they do not exist.

	TiXmlElement *child;

	if (XMLnode->Value() == string("SoundConnection"))
	{

		//std::cout << "found <SoundConnection>: " << XMLnode->Attribute("id") << std::endl;
		//std::cout << "src: " << XMLnode->FirstChild("src")->FirstChild()->Value() << std::endl;
		//std::cout << "snk: " << XMLnode->FirstChild("snk")->FirstChild()->Value() << std::endl;

		osg::ref_ptr<ReferencedNode> n = getNode(XMLnode->FirstChild("src")->FirstChild()->Value());
		char *snkName = (char*)XMLnode->FirstChild("snk")->FirstChild()->Value();


		if (!n.valid()) return false;

		// we can call the connect() method again, to ensure the connection is actually made:
		osg::ref_ptr<DSPNode> srcNode = dynamic_cast<DSPNode*>(n.get());
		srcNode->connect(snkName);

		// check if it exists now (it might not, if the sink didn't exist)
		SoundConnection *conn = srcNode->getConnection(snkName);

		if (!conn) return false;

		// now we have it, so we can go through the rest of the xml children
		// and update the connection's parameters:

		for (child = XMLnode->FirstChildElement(); child; child = child->NextSiblingElement() )
		{
			//std::cout << "child = " << child->Value() << ", value = " << child->FirstChild()->Value() << std::endl;

			// we know that each method takes exactly one float arg:
			float f;
			if (child->Value() == string("setThru"))
			{
				if (fromString<float>(f, child->FirstChild()->Value())) conn->setThru(f);
			}
			else if (child->Value() == string("setDistanceEffect"))
			{
				if (fromString<float>(f, child->FirstChild()->Value())) conn->setDistanceEffect(f);
			}
			else if (child->Value() == string("setRolloffEffect"))
			{
				if (fromString<float>(f, child->FirstChild()->Value())) conn->setRolloffEffect(f);
			}
			else if (child->Value() == string("setDopplerEffect"))
			{
				if (fromString<float>(f, child->FirstChild()->Value())) conn->setDopplerEffect(f);
			}
			else if (child->Value() == string("setDiffractionEffect"))
			{
				if (fromString<float>(f, child->FirstChild()->Value())) conn->setDiffractionEffect(f);
			}

		}
	}

}

/*
bool SceneManager::createConnectionsFromXML(TiXmlElement *XMLnode)
{

	// NOTE: this assumes all nodes have already been created

	TiXmlElement *child1, *child2;

	for ( child1 = XMLnode->FirstChildElement(); child1; child1 = child1->NextSiblingElement() )
	{
		// first check children:
		if (child1->Value() == string("subgraph"))
		{
			for (child2 = child1->FirstChildElement(); child2; child2 = child2->NextSiblingElement() )
			{
				createConnectionsFromXML(child2);
			}
		}

		// now if this node has a "connect" message,
		else if (child1->Value() == string("connect"))
		{
			// don't care about nodeType (should be SoundNode or SoundSpace)
			//char *nodeType = (char*) XMLnode->Value();

			if (XMLnode->Attribute("id"))
			{
				osg::ref_ptr<ReferencedNode> n = getNode((char*) XMLnode->Attribute("id"));
				if (n.valid())
				{
					osg::ref_ptr<DSPNode> srcNode = dynamic_cast<DSPNode*>(n.get());
					if (srcNode.valid()) srcNode->connect((char*)child1->FirstChild()->Value());
				}
			}
		}

	} // for ( child1
}
*/


// *****************************************************************************
bool SceneManager::loadXML(const char *s)
{
	string filename;

	// replace tilde with $HOME variable
	if (s[0]=='~') filename = getenv("HOME") + string(s).substr(1);
	else filename = string(s);

	std::cout << "Loading scene: " << filename << std::endl;

	TiXmlDocument doc( filename.c_str() );

	TiXmlNode *root = 0;
	TiXmlElement *child = 0;

	// Load the XML file and verify:
	if ( !doc.LoadFile() ) {
		std::cout << "ERROR: failed to load " << filename << ". Invalid XML format." << std::endl;
		return false;
	}

	// get the <spinScene> tag and verify:
	if (!(root = doc.FirstChild( "spinScene" )))
	{
		std::cout << "ERROR: failed to load " << filename << ". XML file has no <spinScene> tag." << std::endl;
		return false;
	}

	// okay.. we have a valid xml file.

	for( child = root->FirstChildElement(); child; child = child->NextSiblingElement() )
	{
		createNodeFromXML(child);
	}

	// Now see if there is a <connections> tag:
	if (root = doc.FirstChild( "connections" ))
	{
		// go through the file again, making sure that connections get created:
		for( child = root->FirstChildElement(); child; child = child->NextSiblingElement() )
		{
			createConnectionsFromXML(child);
		}
	}


	// note: the setParent() just set the intended parent. We still have to call
	// sceneManager::updateGraph() to force the changes to the scene graph:

	this->updateGraph();



	std::cout << "Successfully loaded scene from " << filename << std::endl;
	return true;

}



// *****************************************************************************
void SceneManager::setGrid(int gridSize)
{
	std::cout << "called setGrid() with : " << gridSize << std::endl;

	// first remove existing grid (if any):
	if (gridGeode.valid())
	{
		if (worldNode->containsNode(gridGeode.get()))
		{
			worldNode->removeChild(gridGeode.get());
			gridGeode = NULL;
		}
	}

	// now create a new grid and add it to the worldNode:
	if (gridSize > 0)
	{
		gridGeode = createGrid(gridSize, osg::Vec4(0.3,0.3,0.3,0.0));
		worldNode->addChild(gridGeode.get());
	}

	sendSceneMessage("si", "setGrid", gridSize, LO_ARGS_END);
}






// *****************************************************************************
// *****************************************************************************
// helper methods:





static bool nodeSortFunction (osg::ref_ptr<ReferencedNode> n1, osg::ref_ptr<ReferencedNode> n2)
{
	return ( string(n1->id->s_name) < string(n2->id->s_name) );
}




// *****************************************************************************
// OSC callback functions below (need to be valid C function pointers, so they
// are declared here as static functions):




/**
 * Recursive function to invoke a method for a particular class, that will try
 * all base classes as well
 */
static int invokeMethod(const osgIntrospection::Value classInstance, const osgIntrospection::Type &classType, std::string method, ValueList theArgs)
{

	// TODO: we should try to store this globally somewhere, so that we don't do
	// a lookup every time there is a message:
	const osgIntrospection::Type &ReferencedNodeType = osgIntrospection::Reflection::getType("ReferencedNode");


	if ((classType==ReferencedNodeType) || (classType.isSubclassOf(ReferencedNodeType)))
    {
    	try {
    		classType.invokeMethod(method, classInstance, theArgs, true);
    		// if we get this far, then the method invocation succeeded and
    		// we can return:
    		return 1;
    	}
    	catch (osgIntrospection::Exception & ex)
    	{
   			//std::cerr << "catch exception: " << ex.what() << std::endl;
  		}

        // If the method wasn't found in the classInstance, then we need to go
    	// through all base classes to see if method is contained in a parent class:
    	for (int i=0; i<classType.getNumBaseTypes(); i++)
    	{
    		if (invokeMethod(classInstance, classType.getBaseType(i), method, theArgs)) return 1;
    	}
    }

	return 0;
}

int SceneManagerCallback_node(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	// NOTE: user_data is a t_symbol pointer

	int i;
	string    theMethod, nodeStr;
	ValueList theArgs;


	// make sure there is at least one argument (ie, a method to call):
	if (!argc) return 0;

	// get the method (argv[0]):
	if (lo_is_string_type((lo_type)types[0]))
	{
		theMethod = string((char *)argv[0]);
	}
	else return 0;

	// get the instance of the node, which is the last token of the OSCpath:
	// TODO: use user_data instead!
	nodeStr = string(path);
	nodeStr = nodeStr.substr(nodeStr.rfind("/")+1);
	t_symbol *s = gensym(nodeStr.c_str());
	osg::ref_ptr<ReferencedNode> n = s->s_thing;
	if (!n.valid())
	{
		std::cout << "oscParser: Could not find node: " << nodeStr << std::endl;
		return 0;
	}

	//pthread_mutex_lock(&pthreadLock);


	// get node as an osgInrospection::Value (note that type will be ReferencedNode pointer):
	const osgIntrospection::Value classInstance = osgIntrospection::Value(n.get());


	// the getInstanceType() method however, gives us the real type being pointed at:
	const osgIntrospection::Type &classType = classInstance.getInstanceType();


    if (!classType.isDefined())
    {
        std::cout << "ERROR: oscParser cound not process message '" << path << ". osgIntrospection has no data for that node." << std::endl;
        return 0;
    }

	//introspect_print_type(classType);


	// If we have found a valid Type, then let's build an argument list and see
	// if we can find a method that takes this list of argumets:
	for (i=1; i<argc; i++)
	{
		if (lo_is_numerical_type((lo_type)types[i]))
		{
			theArgs.push_back( (float) lo_hires_val((lo_type)types[i], argv[i]) );
		} else {
			theArgs.push_back( (const char*) argv[i] );
		}
	}

	// invoke the method on the node, and if it doesn't work, then just forward
	// the message:
	if (!invokeMethod(classInstance, classType, theMethod, theArgs))
	{
		if (n->sceneManager->txServ)
		{
			//std::cout << "Ignoring method '" << theMethod << "' for [" << n->id->s_name << "], but forwarding message anyway..." << std::endl; 
			lo_message msg = lo_message_new();
			for (i=0; i<argc; i++)
			{
				if (lo_is_numerical_type((lo_type)types[i]))
				{
					lo_message_add_float(msg, (float) lo_hires_val((lo_type)types[i], argv[i]));
				} else {
					lo_message_add_string(msg, (const char*) argv[i] );
				}
			}
				
			n->sceneManager->sendNodeMessage(n->id, msg);
		}
	}


	//pthread_mutex_unlock(&pthreadLock);

	return 1;
}

int SceneManagerCallback_log(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	spinLog *log = (spinLog*) user_data;
	
	*log << path << ' ' << types << ' ';
	for (int i=0; i<argc; i++)
	{
		if (lo_is_numerical_type((lo_type)types[i]))
		{
			*log << (float) lo_hires_val((lo_type)types[i], argv[i]);
		} else {
			*log << (const char*) argv[i];
		}
		*log << ' ';
	}
	*log << std::endl;

	return 1;
}

int SceneManagerCallback_debug(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	printf("************ oscCallback_debug() got message: %s\n", (char*)path);
	printf("user_data: %s\n", (char*) user_data);
	for (int i=0; i<argc; i++) {
		printf("arg %d '%c' ", i, types[i]);
    	lo_arg_pp((lo_type) types[i], argv[i]);
    	printf("\n");
	}
	printf("\n");
	fflush(stdout);

	return 1;
}

int SceneManagerCallback_admin(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	SceneManager *sceneManager = (SceneManager*) user_data;

	// make sure there is at least one argument (ie, a method to call):
	if (!argc) return 0;

	// get the method (argv[0]):
	string theMethod;
	if (lo_is_string_type((lo_type)types[0]))
	{
		theMethod = string((char *)argv[0]);
	}
	else return 0;

	//std::cout << "in callback_admin. got method: " << theMethod << std::endl;

	//pthread_mutex_lock(&pthreadLock);

	// note that args start at argv[1] now:
	if (theMethod=="debug")
		sceneManager->debug();
	else if (theMethod=="clear")
		sceneManager->clear();
	else if (theMethod=="clearUsers")
		sceneManager->clearUsers();
	else if (theMethod=="userRefresh")
		sceneManager->sendSceneMessage("s", "userRefresh", LO_ARGS_END);
	else if (theMethod=="refresh")
		sceneManager->refresh();
	else if (theMethod=="getNodeList")
		sceneManager->sendNodeList("*");
	else if ((theMethod=="nodeList") && (argc>2))
	{
		for (int i=2; i<argc; i++)
		{
			if (strcmp((char*)argv[i],"NULL")!=0) sceneManager->createNode((char*)argv[i], (char*)argv[1]);
		}
	}
	
	else if ((theMethod=="exportScene") && (argc==3))
		sceneManager->exportScene((char*)argv[1], (char*)argv[2]);
	else if ((theMethod=="load") && (argc==2))
		sceneManager->loadXML((char*)argv[1]);
	else if ((theMethod=="save") && (argc==2))
		sceneManager->saveXML((char*)argv[1]);
	else if ((theMethod=="createNode") && (argc==3))
		sceneManager->createNode((char*)argv[1], (char*)argv[2]);
	else if ((theMethod=="deleteNode") && (argc==2))
		sceneManager->deleteNode((char*)argv[1]);
	else if ((theMethod=="deleteGraph") && (argc==2))
		sceneManager->deleteGraph((char*)argv[1]);
	else if ((theMethod=="setGrid") && (argc==2))
		if (lo_is_numerical_type((lo_type)types[1])) sceneManager->setGrid((int) lo_hires_val((lo_type)types[1], argv[1]));

	else {
		std::cout << "Unknown OSC command: " << path << " " << theMethod << " (with " << argc-1 << " args)" << std::endl;
	}

	//pthread_mutex_unlock(&pthreadLock);

	return 1;
}

int SceneManagerCallback_conn(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	int i;
	string    theMethod, idStr;


	// make sure there is at least one argument (ie, a method to call):
	if (!argc) return 0;

	// get the method (argv[0]):
	if (lo_is_string_type((lo_type)types[0]))
	{
		theMethod = string((char *)argv[0]);
	}
	else return 0;

	// get the instance of the connection:
	SoundConnection *conn = (SoundConnection*) user_data;
	if (!conn)
	{
		std::cout << "oscParser: Could not find connection: " << idStr << std::endl;
		return 0;
	}

	// TODO: replace method call with osg::Introspection

	if (theMethod=="stateDump")
		conn->stateDump();
	else if (theMethod=="debug")
		conn->debug();
	else if ((argc==2) && (lo_is_numerical_type((lo_type)types[1])))
	{
		float value = lo_hires_val((lo_type)types[1], argv[1]);

		if (theMethod=="setThru")
			conn->setThru((bool) value);
		else if (theMethod=="setDistanceEffect")
			conn->setDistanceEffect(value);
		else if (theMethod=="setRolloffEffect")
			conn->setRolloffEffect(value);
		else if (theMethod=="setDopplerEffect")
			conn->setDopplerEffect(value);
		else if (theMethod=="setDiffractionEffect")
			conn->setDiffractionEffect(value);
		else
			std::cout << "Unknown OSC command: " << path << " " << theMethod << " (with " << argc-1 << " args)" << std::endl;
	}

	else
		std::cout << "Unknown OSC command: " << path << " " << theMethod << " (with " << argc-1 << " args)" << std::endl;

}
void oscParser_error(int num, const char *msg, const char *path)
{
	printf("OSC (liblo) error %d in path %s: %s\n", num, path, msg);
	fflush(stdout);
}