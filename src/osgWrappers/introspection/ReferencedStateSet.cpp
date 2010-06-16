// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/StaticMethodInfo>
#include <osgIntrospection/Attributes>

#include <ReferencedStateSet.h>
#include <SceneManager.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_ABSTRACT_OBJECT_REFLECTOR(ReferencedStateSet)
	I_DeclaringFile("ReferencedStateSet.h");
	I_Constructor2(IN, SceneManager *, sceneManager, IN, const char *, initID,
	               ____ReferencedStateSet__SceneManager_P1__C5_char_P1,
	               "",
	               "");
	I_Method0(void, updateCallback,
	          Properties::VIRTUAL,
	          __void__updateCallback,
	          "",
	          "");
	I_Method0(const char *, getPath,
	          Properties::PURE_VIRTUAL,
	          __C5_char_P1__getPath,
	          "",
	          "Abstract method getPath needs to be implemented ");
	I_Method0(void, removeFromScene,
	          Properties::NON_VIRTUAL,
	          __void__removeFromScene,
	          "",
	          "Remove this stateset from all parents... essentially destroying the state, since no reference to it will exist anymore, and OSG will kill it. ");
	I_Method1(void, replace, IN, osg::StateSet *, ss,
	          Properties::NON_VIRTUAL,
	          __void__replace__osg_StateSet_P1,
	          "",
	          "Replaces a StateSet in the scene graph with this one. ie, goes through all parents of the provided stateset and replaces the object's state with this. ");
	I_Method0(void, debug,
	          Properties::VIRTUAL,
	          __void__debug,
	          "",
	          "");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "Just like a ReferencedNode, each subclass of ReferencedStateSet must override the getState() method to pass it's current state. ");
	I_Method0(void, stateDump,
	          Properties::VIRTUAL,
	          __void__stateDump,
	          "",
	          "StateDump() is a request to broadcast the node state via SceneManager. ");
	I_SimpleProperty(const char *, Path, 
	                 __C5_char_P1__getPath, 
	                 0);
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
	I_PublicMemberProperty(SceneManager *, sceneManager);
	I_PublicMemberProperty(t_symbol *, id);
	I_PublicMemberProperty(std::string, classType);
END_REFLECTOR

BEGIN_VALUE_REFLECTOR(ReferencedStateSet_callback)
	I_DeclaringFile("ReferencedStateSet.h");
	I_Constructor0(____ReferencedStateSet_callback,
	               "",
	               "");
END_REFLECTOR

