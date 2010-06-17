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

#ifndef __spinApp_H
#define __spinApp_H

#include <boost/python.hpp>
#include <osg/Timer>

#include "UserNode.h"
#include "spinUtil.h"

class spinBaseContext;
class SceneManager;
class MediaManager;

/**
 * \brief A singleton class to facilitate communication with SPIN.
 *
 * By instantiating this class, we load the OSG nodekit for SPIN -- otherwise
 * known as libSPIN, and create
 *
 */
class spinApp
{
    public:
        // Meyers Singleton design pattern:
        static spinApp& Instance();


        void setContext(spinBaseContext *c);
        spinBaseContext *getContext() { return context; }


        /**
         * This method should be used to register a user for a listener-style
         * SPIN client. The user is definitively created and stored in the
         * current application context, even if a server is not running.
         */
        void registerUser(const char *id);

        /**
         * This sends a variable length message.
         *
         * IMPORTANT: the list must be terminated with LO_ARGS_END, or this call
         * will fail.  This is used to do simple error checking on the sizes of
         * parameters passed.
         */
        void InfoMessage(std::string OSCpath, const char *types, ...);
        void InfoMessage(std::string OSCpath, const char *types, va_list ap);
        void InfoMessage(std::string OSCpath, lo_message msg);

        void SceneMessage(const char *types, ...);
        void SceneMessage(const char *types, va_list ap);
        void SceneMessage(lo_message msg);

        void NodeMessage(const char *nodeId, const char *types, ...);
        void NodeMessage(const char *nodeId, const char *types, va_list ap);
        void NodeMessage(const char *nodeId, lo_message msg);

        void NodeBundle(t_symbol *nodeSym, std::vector<lo_message> msgs);
        void SceneBundle(std::vector<lo_message> msgs);

        void setSceneID(std::string s) { sceneID = s; }
        std::string getSceneID() { return sceneID; }


        void setSyncStart(osg::Timer_t t) { _syncStartTick = t; }
        osg::Timer_t getSyncStart() { return _syncStartTick; }


        /**
         * initializes the embedded python interpreter
         */
        bool initPython();
        bool execPython( const std::string& cmd );
        boost::python::object _pyMainModule;
        boost::python::object _pyNamespace;
        bool _pyInitialized;


        osg::ref_ptr<UserNode> userNode;


        SceneManager *sceneManager;
        MediaManager *mediaManager;

    private:
        void sendBundle(std::string OSCpath, std::vector<lo_message> msgs);

        // singleton constructors & desctructor (hidden):
        spinApp();
        spinApp(spinApp const&); // copy constructor
        // hide the assignment operator, otherwise it would be possible to
        // assign the singleton spinApp to itself:
        spinApp& operator=(spinApp const&);
        ~spinApp();


        std::string sceneID;

        osg::Timer_t _syncStartTick;

        spinBaseContext *context;
};


/*
#define BROADCAST(pNode, types, ...) \
    if (sceneManager->isServer()) \
    lo_send_from(sceneManager->txAddr, sceneManager->txServ, LO_TT_IMMEDIATE, ("/SPIN/"+sceneManager->sceneID+"/"+std::string(pNode->id->s_name)).c_str(), types, ##__VA_ARGS__)

#define BROADCAST_MSG(pNode, msg) \
    if (sceneManager->isServer()) \
    lo_send_message_from(sceneManager->txAddr, sceneManager->txServ, ("/SPIN/"+sceneManager->sceneID+"/"+std::string(pNode->id->s_name)).c_str(), msg)
*/


// Internal server-side MACROS for sending messages. Clients should NEVER use
// these macros, and should rather use spinContext::send* methods. But just in
// case, the macros always check that the passed SceneManager (s) is a server.

#define SCENE_MSG(types, ...) \
    if (spinApp::Instance().getContext()->isServer()) \
    lo_send(spinApp::Instance().getContext()->lo_txAddr, \
            ("/SPIN/" + spinApp::Instance().getSceneID()).c_str(), types, ##__VA_ARGS__, LO_ARGS_END)

#define SCENE_LO_MSG(msg) \
    if (spinApp::Instance().getContext()->isServer()) \
    lo_send_message(spinApp::Instance().getContext()->lo_txAddr, ("/SPIN/" + spinApp::Instance().getSceneID()).c_str(), msg)

#define NODE_MSG(pNode, types, ...) \
    if (spinApp::Instance().getContext()->isServer()) \
    lo_send(spinApp::Instance().getContext()->lo_txAddr, \
            ("/SPIN/" + spinApp::Instance().getSceneID() + "/" + std::string(pNode->id->s_name)).c_str(), \
            types, ##__VA_ARGS__, LO_ARGS_END)

#define NODE_LO_MSG(s, pNode, msg) \
    if (spinApp::Instance().getContext()->isServer()) \
    lo_send_message(spinApp::Instance().getContext()->lo_txAddr, \
            ("/SPIN/" + spinApp::Instance().getSceneID() + "/" + std::string(pNode->id->s_name)).c_str(), msg)


// backwards compatibility (TODO: replace all BROADCAST messages with NODE_MSG)
#define BROADCAST(pNode, types, ...) NODE_MSG(pNode, types, ##__VA_ARGS__)

#endif
