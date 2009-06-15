// =============================================================================
//                    .___.__                                                 //
//   _____   __ __  __| _/|__| ____  ______ ____ _____  ______   ____         //
//   \__  \ |  |  \/ __ | |  |/  _ \/  ___// ___\\__  \ \____ \_/ __ \        //
//    / __ \|  |  / /_/ | |  (  <_> )___ \\  \___ / __ \|  |_> >  ___/        //
//   (____  /____/\____ | |__|\____/____  >\___  >____  /   __/ \___  >       //
//        \/           \/               \/     \/     \/|__|        \/  .ORG  //
//                                                                            //
// =============================================================================
// The Audioscape Project :: www.audioscape.org
// Copyright (c) 2009
//
//
// Organizations:
// - McGill University, Shared Reality Lab (SRE) :: www.cim.mcgill.ca/sre
// - La Societe des Arts Technologiques (SAT) :: www.sat.qc.ca
// - Universite de Montreal :: www.umontreal.ca
//
// Development Team:
// - Mike Wozniewski (www.mikewoz.com): Head Developer, Researcher
// - Zack Settel (www.sheefa.net/zack): Conception, Research, Artist, Programmer
// - Jeremy Cooperstock (cim.mcgill.ca/~jer): Project Coordinator
// - Sylvain Cormier: Programmer, Tester
// - Jean-Michel Dumas: Assistant, Programmer
// - Pierre-Olivier Charlebois: Programmer
//
// Funding by / Subventionne par:
//    Natural Sciences and Engineering Research Council of Canada (NSERC)
//    Canada Council for the Arts
//
// =============================================================================
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
// =============================================================================

#include <string>
#include <iostream>
#include <pthread.h>

#include <osgDB/Registry>
#include <osgIntrospection/Type>

#include "asUtil.h"
#include "vessThreads.h"

using namespace std;


pthread_mutex_t pthreadLock = PTHREAD_MUTEX_INITIALIZER;


// *****************************************************************************
// The vessMaster is a simple extension of vessListener, adding the ability to
// re-transmit messages that it recieves (done via sceneManager->setTXaddress)
//
// Additionally, the vessMaster will periodically broadcast a ping on infoport,
// and will perform an update traversal on the scene graph for any nodes who
// need periodic processing.
vessMaster::vessMaster() : vessListener()
{

    // note that master needs to listen to a unicast port:
    rxAddr = getMyIPaddress();
	rxPort = "54324";

	txAddr = "224.0.0.1";
	txPort = "54323";

	threadFunction = &vessMasterThread;
}

vessMaster::~vessMaster()
{
	sceneManager->clear();
}

// *****************************************************************************
// The vessListener is a simple class that starts a sceneManager, which listens
// to incoming vess messages. It does NOT re-transmit those messages, and it
// does NOT perform an update traversal.
vessListener::vessListener()
{
	
	// Load the libAudioscape library:
	osgDB::Registry *reg = osgDB::Registry::instance();
	osgDB::DynamicLibrary::loadLibrary(reg->createLibraryNameForNodeKit("libAudioscape"));

	// Make sure that our OSG wrapper library is loaded
	// (by checking for existance of asReferenced type):
	const osgIntrospection::Type &asReferencedType = osgIntrospection::Reflection::getType("asReferenced");
	if (!asReferencedType.isDefined())
	{
		std::cout << "ERROR: libAudioscape was not found. Please check dynamic libraries. Could not start VESS." << std::endl;
		exit(1);
	}
	
	
	
	// defaults:

	id = "default";
	rxAddr = "224.0.0.1";
	rxPort = "54323";
	infoAddr = "224.0.0.1";
	infoPort = "54320";

	// override txAddr and infoPort based on environment variable:
	string tmpStr = getenv("AS_INFOPORT");
	if (!infoPort.empty())
	{
		infoAddr = tmpStr.substr(0,tmpStr.rfind(":"));
		infoPort = tmpStr.substr(tmpStr.find(":")+1);
	}

	lo_infoAddr = lo_address_new(infoAddr.c_str(), infoPort.c_str());

	if (isMulticastAddress(infoAddr))
	{
		lo_infoServ = lo_server_new_multicast(infoAddr.c_str(), NULL, oscParser_error);

	} else if (isBroadcastAddress(infoAddr))
	{
		lo_infoServ = lo_server_new(NULL, oscParser_error);
		int sock = lo_server_get_socket_fd(lo_infoServ);
		int sockopt = 1;
		setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &sockopt, sizeof(sockopt));

	} else {
		lo_infoServ = lo_server_new(NULL, oscParser_error);
	}

	std::cout << "  INFO channel: " << lo_address_get_url(lo_infoAddr) << std::endl;

	//lo_server_thread_start(lo_infoServ);


	running = false;
	threadFunction = &vessListenerThread;
}

vessListener::~vessListener()
{
	sceneManager->clear();
	if (lo_infoAddr)
	{
		lo_address_free(lo_infoAddr);
	}

	if (lo_infoServ)
	{
		//lo_server_thread_stop(lo_infoServ);
		lo_server_free(lo_infoServ);
	}

}

void vessListener::start()
{

	// create thread:
	if (pthread_attr_init(&pthreadAttr) < 0)
	{
		printf("vessServer: warning: could not prepare child thread\n");
		return;
	}
	if (pthread_attr_setdetachstate(&pthreadAttr, PTHREAD_CREATE_DETACHED) < 0)
	{
		printf("vessServer: warning: could not prepare child thread\n");
		return;
	}
	if (pthread_create( &pthreadID, &pthreadAttr, threadFunction, this) < 0)
	{
		printf("vessServer: could not create new thread\n");
		return;
	}

	//pthread_join(pthreadID, NULL); // if not DETACHED thread

	// wait until the thread gets into it's loop before returning:
	while (!running) usleep(10);
}

void vessListener::stop()
{
	std::cout << "Stopping VESS" << std::endl;
	this->running = false;
}

// *****************************************************************************

static void *vessListenerThread(void *arg)
{
	vessListener *vess = (vessListener*) arg;

	std::cout << "  VESS listener started." << std::endl;

	vess->sceneManager = new asSceneManager(vess->id, vess->rxAddr, vess->rxPort);

	vess->running = true;
	while (vess->isRunning())
	{
		sleep(1);
		// do nothing (assume the app is doing updates - eg, in a draw loop)
	}

	pthread_exit(NULL);
}

static void *vessMasterThread(void *arg)
{
	vessMaster *vess = (vessMaster*) arg;

	std::cout << "  VESS started in Master mode." << std::endl;
	std::cout << "  broadcasting info messages on " << vess->txAddr << ", port: " << vess->infoPort << std::endl;

	vess->sceneManager = new asSceneManager(vess->id, vess->rxAddr, vess->rxPort);
	vess->sceneManager->setTXaddress(vess->txAddr, vess->txPort);

	string myIP = getMyIPaddress();
	osg::Timer_t lastTick = osg::Timer::instance()->tick();
	osg::Timer_t frameTick = lastTick;

	// convert ports to integers for sending:
	int i_rxPort, i_txPort;
	fromString<int>(i_rxPort, vess->rxPort);
	fromString<int>(i_txPort, vess->txPort);

	asSceneUpdateVisitor visitor;

	vess->running = true;
	while (vess->isRunning())
	{
		frameTick = osg::Timer::instance()->tick();
		if (osg::Timer::instance()->delta_s(lastTick,frameTick) > 5) // every 5 seconds
		{
			lo_send_from(vess->lo_infoAddr, vess->lo_infoServ, LO_TT_IMMEDIATE, "/ping/vess", "ssisi", vess->id.c_str(), myIP.c_str(), i_rxPort, vess->txAddr.c_str(), i_txPort);
			lastTick = frameTick;
		}

		pthread_mutex_lock(&pthreadLock);
		visitor.apply(*(vess->sceneManager->rootNode.get())); // only vessMaster should do this
		pthread_mutex_unlock(&pthreadLock);


		pthread_mutex_lock(&pthreadLock);
		vess->sceneManager->updateGraph();
		pthread_mutex_unlock(&pthreadLock);


		usleep(10);
	}

	pthread_exit(NULL);
}
