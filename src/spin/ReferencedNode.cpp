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

#include <string>
#include <vector>
#include <iostream>


#include <exception>

#include "ReferencedNode.h"
#include "SceneManager.h"


using namespace std;

extern pthread_mutex_t pthreadLock;

// ***********************************************************
// constructor (one arg required: the node ID)
ReferencedNode::ReferencedNode (SceneManager *sceneManager, char *initID)
{

    id = gensym(initID);
    id->s_thing = this;
    id->s_type = REFERENCED_NODE;

    nodeType = "ReferencedNode";

    contextString = "NULL";

    this->setName(string(id->s_name) + ".ReferencedNode");

    // set some initial symbols:
    parent = WORLD_SYMBOL;
    newParent = WORLD_SYMBOL;

    textFlag = false;

    // When children are attached to this, they get added to the attachmentNode:
    attachmentNode = this;

    // We need to set up a callback. This should be on the topmost node, so that during node
    // traversal, we update our parameters before anything is drawn.
    this->setUserData( dynamic_cast<osg::Referenced*>(this) );
    this->setUpdateCallback(new ReferencedNode_callback);

    // set initial nodepath:
    currentNodePath.clear();

    registerNode(sceneManager);


    if (strcmp(initID, "potato") == 0) {
        //setScript("hello.py");
        printf("I am potato\n");
        addEventScript("setScale","hello.py", "");
    }


    this->setNodeMask(GEOMETRIC_NODE_MASK); // nodemask info in spinUtil.h

    attach();
    //printf("referencednode ctor done.\n");

}

// ***********************************************************
// destructor
ReferencedNode::~ReferencedNode()
{
    //std::cout << "In ReferencedNode destructor... node: " << this->id->s_name << std::endl;

    if (sceneManager)
    {
        // unregister with OSC parser:
        string oscPattern = "/SPIN/" + sceneManager->sceneID + "/" + string(id->s_name);
        lo_server_thread_del_method(sceneManager->rxServ, oscPattern.c_str(), NULL);
    }

    id->s_thing = 0;
}


void ReferencedNode::registerNode(SceneManager *s)
{
    sceneManager = s;
    mediaManager = sceneManager->mediaManager;

    // register with OSC parser:
    string oscPattern = "/SPIN/" + sceneManager->sceneID + "/" + string(id->s_name);
    lo_server_thread_add_method(sceneManager->rxServ, oscPattern.c_str(), NULL, SceneManagerCallback_node, (void*)id);
#ifdef OSCDEBUG
    std::cout << "oscParser registered: " << oscPattern << std::endl;
#endif
}

// *****************************************************************************

std::string currentException()
{
  using namespace boost::python;
  namespace py = boost::python;
  printf("currentException...\n");
  PyObject *exc,*val,*tb;
  PyErr_Fetch(&exc,&val,&tb);
  handle<> hexc(exc),hval(val),htb(tb);
  if(!htb || !hval)
  {
      //MYAPP_ASSERT_BUG(hexc);
    return extract<std::string>(str(hexc));
  }
  else
  {
    object traceback(py::import("traceback"));
    object format_exception(traceback.attr("format_exception"));
    object formatted_list(format_exception(hexc,hval,htb));
    object formatted(str("\n").join(formatted_list));
    return extract<std::string>(formatted);
  }
}


void squat() {
    printf("doing squat.\n");
}


void ReferencedNode::callbackUpdate()
{

    if ( _scriptFile != "" ) {

        try {

            _scriptRun();

        } catch (boost::python::error_already_set & ) {
            std::cout << currentException() << std::endl;
            std::cout << "2: Python error: [";
            PyErr_Print();
            std::cout << "]" << std::endl;
            abort();

        } catch ( std::exception& e ) {
            std::cout << "2:  what? "<< e.what() << std::endl;
            abort();
        } catch(...) {                        // catch all other exceptions
            std::cout << "2: Caught... something??\n";
            abort();
        }


    }

}


// *****************************************************************************

void ReferencedNode::attach()
{
    if (this->newParent==NULL_SYMBOL) return;

    pthread_mutex_lock(&pthreadLock);

    osg::ref_ptr<ReferencedNode> newParentNode = dynamic_cast<ReferencedNode*>(newParent->s_thing);

    // if the parent is invalid (which will be the case, for example, if the user
    // specified 'world' as the parent), we attach to the worldNode:
    if (!newParentNode.valid())
    {
        if (!(sceneManager->worldNode->containsNode( this ))) sceneManager->worldNode->addChild(this);
        this->newParent = WORLD_SYMBOL;
    }

    // Otherwise attach to the parent:
    else
    {
        if (!(newParentNode->attachmentNode->containsNode(this)))
        {
            newParentNode->attachmentNode->addChild(this);
            newParentNode->as_addChild(this);
        }
    }

    pthread_mutex_unlock(&pthreadLock);

    // remove node from current parent (make sure to release the mutex first!)
    if (this->parent != this->newParent)
    {
        this->detach();
    }

    // update the new parent symbols:
    this->parent = this->newParent;
    this->newParent = NULL_SYMBOL;

    // update currentNodePath:
    this->updateNodePath();

    // broadcast this change to any remote clients:
    BROADCAST(this, "ss", "setParent", this->parent->s_name);

}

// ***********************************************************
// remove this node from the scenegraph:
void ReferencedNode::detach()
{
    pthread_mutex_lock(&pthreadLock);

    if (parent == WORLD_SYMBOL)
    {
        if (sceneManager->worldNode->containsNode(this)) sceneManager->worldNode->removeChild(this);
    }

    else {
        osg::ref_ptr<ReferencedNode> pNode = dynamic_cast<ReferencedNode*>(parent->s_thing);
        if (pNode.valid())
        {
            if (pNode->attachmentNode->containsNode(this))
            {
                pNode->attachmentNode->removeChild(this);
                pNode->as_removeChild(this);
            }
        }
    }

    pthread_mutex_unlock(&pthreadLock);
}


// *****************************************************************************
// IMPORTANT:
// subclasses of ReferencedNode are allowed to contain complicated subgraphs, and
// can also change their attachmentNode so that children are attached anywhere
// in this subgraph. If that is the case, the updateNodePath() function MUST be
// overridden, and extra nodes must be manually pushed onto the currentNodePath.

void ReferencedNode::updateNodePath()
{
    currentNodePath.clear();
    if ((parent!=WORLD_SYMBOL) && (parent!=NULL_SYMBOL))
    {
        osg::ref_ptr<ReferencedNode> parentNode = dynamic_cast<ReferencedNode*>(parent->s_thing);
        if (parentNode.valid())
        {
            currentNodePath = parentNode->currentNodePath;
        }
    }

    // this nodePath only stores the path until this node (osg::Group).
    currentNodePath.push_back(this);

    // now update NodePaths for all children:
    updateChildNodePaths();

}


void ReferencedNode::updateChildNodePaths()
{
    vector<ReferencedNode*>::iterator childIter;
    for (childIter = children.begin(); childIter != children.end() ; childIter++)
    {
        (*childIter)->updateNodePath();
    }
}

int ReferencedNode::setAttachmentNode(osg::Group *n)
{
    if (n)
    {
        attachmentNode = n;

        // update the nodepath now that we've defined a new attachmentNode
        this->updateNodePath();

        return 1;

    }

    return 0;
}


// *****************************************************************************
ReferencedNode *ReferencedNode::as_getChild(ReferencedNode *child)
{
    vector<ReferencedNode*>::iterator iter;
    for (iter = children.begin(); iter != children.end() ; iter++)
    {
        if ((*iter) == child) return (*iter);
    }
    return NULL;
}

void ReferencedNode::as_addChild(ReferencedNode *child)
{
    children.push_back(child);
}

void ReferencedNode::as_removeChild(ReferencedNode *child)
{
    vector<ReferencedNode*>::iterator iter;
    for (iter = children.begin(); iter != children.end() ; iter++)
    {
        if ((*iter) == child)
        {
            children.erase(iter);
            break;
        }
    }
}



// *****************************************************************************
bool ReferencedNode::legalParent (t_symbol *newParent)
{
    vector<ReferencedNode*>::iterator childIter;

    if (newParent == this->id)
    {
        return false;
    }

    else
    {
        for (childIter = children.begin(); childIter != children.end() ; childIter++)
        {
            if ((*childIter)->id == newParent) return false;
        }
    }

    return true;
}

// *****************************************************************************

void ReferencedNode::setParent (const char *newvalue)
{
    t_symbol *s = gensym(newvalue);
    if (parent != s)
    {
        newParent = s;
        attach();
    }
}

void ReferencedNode::setContext (const char *newvalue)
{
    contextString = string(newvalue);
    BROADCAST(this, "ss", "setContext", getContext());
}

void ReferencedNode::setParam (const char *paramName, const char *paramValue)
{
    //std::cout << id->s_name << " got setParam: " << paramValue << std::endl;
    stringParams[string(paramName)] = string(paramValue);
    BROADCAST(this, "sss", "setParam", paramName, paramValue);
}

void ReferencedNode::setParam (const char *paramName, float paramValue)
{
    floatParams[string(paramName)] = paramValue;
    BROADCAST(this, "ssf", "setParam", paramName, paramValue);
}

// *****************************************************************************

void ReferencedNode::debug()
{
    lo_arg **args;
    int i, argc;
    char *argTypes;

    std::cout << "****************************************" << std::endl;
    std::cout << "************* NODE  DEBUG: *************" << std::endl;

    std::cout << "\nnode: " << id->s_name << ", type: " << nodeType << std::endl;

    vector<lo_message> nodeState = this->getState();
    vector<lo_message>::iterator nodeStateIterator;
    for (nodeStateIterator = nodeState.begin(); nodeStateIterator != nodeState.end(); ++nodeStateIterator)
    {
        argTypes = lo_message_get_types(*nodeStateIterator);
        argc = lo_message_get_argc(*nodeStateIterator);
        args = lo_message_get_argv(*nodeStateIterator);

        std::cout << "  ";
        for (i = 0; i<argc; i++) {
            std::cout << " ";
            if (lo_is_numerical_type((lo_type)argTypes[i]))
            {
                std::cout << (float) lo_hires_val( (lo_type)argTypes[i], args[i] );
            } else if (strlen((char*) args[i])) {
                std::cout << (char*) args[i];
            } else {
                std::cout << "NULL";
            }
        }
        std::cout << std::endl;
    }


    if (!this->children.empty())
    {
        std::cout << "   children:" << std::endl;
        vector<ReferencedNode*>::iterator childIter;
        for (childIter = this->children.begin(); childIter != this->children.end(); ++childIter)
        {
            std::cout << "      " << (*childIter)->id->s_name << std::endl;
        }
    }

    BROADCAST(this, "s", "debug");
}

std::vector<lo_message> ReferencedNode::getState ()
{
    std::vector<lo_message> ret;

    lo_message msg;

    msg = lo_message_new();
    lo_message_add(msg, "ss", "setParent", this->getParent());
    ret.push_back(msg);

    msg = lo_message_new();
    lo_message_add(msg, "ss", "setContext", this->getContext());
    ret.push_back(msg);

    stringParamType::iterator stringIter;
    for (stringIter=stringParams.begin(); stringIter!=stringParams.end(); stringIter++ )
    {
        msg = lo_message_new();
        lo_message_add(msg, "sss", "setParam", (*stringIter).first.c_str(), (const char*)(*stringIter).second.c_str());
        ret.push_back(msg);
    }

    floatParamType::iterator floatIter;
    for (floatIter=floatParams.begin(); floatIter!=floatParams.end(); floatIter++ )
    {
        msg = lo_message_new();
        lo_message_add(msg, "ssf", "setParam", (*floatIter).first.c_str(), (*floatIter).second);
        ret.push_back(msg);
    }


    return ret;
}

// *****************************************************************************
void ReferencedNode::stateDump ()
{

    sceneManager->sendNodeBundle(this->id, this->getState());

    /*
    vector<lo_message> nodeState = this->getState();

    vector<lo_message>::iterator iter = nodeState.begin();
    while (iter != nodeState.end())
    {
        sceneManager->sendNodeMessage(this->id, (*iter));
        //if (sceneManager->txServ) lo_send_message_from(sceneManager->txAddr, sceneManager->txServ, ("/node/"+string(this->id->s_name)).c_str(), (*iter));
        //lo_message_free(*iter);
        nodeState.erase(iter); //note: iterator automatically advances after erase()
    }
    */
}


// *****************************************************************************

bool ReferencedNode::setScript( const std::string& scr, const std::string& params ) {

    spinContext &spin = spinContext::Instance();
    printf("moo?\n");
    _scriptFile = sceneManager->resourcesPath + "/scripts/" + scr;
    printf("moo! [%s]\n", _scriptFile.c_str());

    boost::python::object s;
    //char hexAddr[20];
    //sprintf(hexAddr, "%p", this);
    printf("ReferencedNode: address = %p\n", this);
    printf("ReferencedNode: id = %s\n", id->s_name);
    char cmd[100];

    try {
        sprintf(cmd, "mod%p = spin.load_module('%s')", this, _scriptFile.c_str());
        std::cout << "Python cmd: " << cmd << std::endl;
        exec(cmd, spin._pyNamespace, spin._pyNamespace);

        if (params != "") sprintf(cmd, "script%p = mod%p.Script('%s')", this, this, id->s_name);
        else sprintf(cmd, "script%p = mod%p.Script('%s', %s)", this, this, id->s_name, params.c_str());
        std::cout << "Python cmd: " << cmd << std::endl;
        exec(cmd, spin._pyNamespace, spin._pyNamespace);

        sprintf(cmd, "script%p", this);
        s = spin._pyNamespace[cmd];
        _scriptRun = s.attr("run"); // extract instead?

    } catch (boost::python::error_already_set const & ) {
        ///if (PyErr_ExceptionMatches(PyExc_ZeroDivisionError))

        std::cout << "0: Python error: " << std::endl;
        PyErr_Print();
        //abort();
        _scriptFile = "";
        return false;
    } catch ( std::exception& e ) {
        std::cout << "0: what? " << e.what() << std::endl;
        //abort();
        _scriptFile = "";
        return false;
    } catch(...) {                        // catch all other exceptions
        std::cout << "0: Caught... something??\n";
        //abort();
        _scriptFile = "";
        return false;
    }

    return true;

}




// *****************************************************************************

bool ReferencedNode::addEventScript( const std::string& eventName, const std::string& scr, const std::string& params ) {

    spinContext &spin = spinContext::Instance();
    printf("moo?\n");


    std::string sf = sceneManager->resourcesPath + "/scripts/" + scr;
    printf("moo! [%s]\n", sf.c_str());

    boost::python::object s, p;
    //char hexAddr[20];
    //sprintf(hexAddr, "%p", this);
    //printf("ReferencedNode: address = %p\n", this);
    //printf("ReferencedNode: id = %s\n", id->s_name);
    char cmd[100];

    try {
        sprintf(cmd, "mod%p = spin.load_module('%s')", this, sf.c_str());
        std::cout << "Python cmd: " << cmd << std::endl;
        exec(cmd, spin._pyNamespace, spin._pyNamespace);

        if (params != "") sprintf(cmd, "script%p = mod%p.Script('%s')", this, this, id->s_name);
        else sprintf(cmd, "script%p = mod%p.Script('%s', %s)", this, this, id->s_name, params.c_str());
        std::cout << "Python cmd: " << cmd << std::endl;
        exec(cmd, spin._pyNamespace, spin._pyNamespace);

        sprintf(cmd, "script%p", this);
        s = spin._pyNamespace[cmd];
        p = s.attr("run");
        //_scriptRun = s.attr("run"); // extract instead?
        _eventScriptList.insert( pair<const std::string, boost::python::object>(eventName, p) );

    } catch (boost::python::error_already_set const & ) {
        std::cout << "0: Python error: " << std::endl;
        PyErr_Print();
        return false;
    } catch ( std::exception& e ) {
        std::cout << "0: what? " << e.what() << std::endl;

        return false;
    } catch(...) {                        // catch all other exceptions
        std::cout << "0: Caught... something??\n";
        return false;
    }

    return true;

}


// *****************************************************************************

bool ReferencedNode::callEventScript( const std::string& eventName ) {

    printf("callEventScript( '%s' )...\n", eventName.c_str());
    if (_eventScriptList.empty()) return false;
    printf("callEventScript... doing stuff\n");
    boost::python::object p;
    EventScriptList::iterator iter;

    try {

        iter = _eventScriptList.find( eventName );
        if( iter != _eventScriptList.end() ) {
            cout << "Value is: " << eventName << '\n';
            p = iter->second;
            p();

        }
        else {
            cout << "Key is not in myMap" << '\n';

        }



    } catch (boost::python::error_already_set const & ) {
        std::cout << "0: Python error: " << std::endl;
        PyErr_Print();
        return false;
    } catch ( std::exception& e ) {
        std::cout << "0: what? " << e.what() << std::endl;
        return false;
    } catch(...) {
        std::cout << "0: Caught... something??\n";
        return false;
    }


    return true;

}
