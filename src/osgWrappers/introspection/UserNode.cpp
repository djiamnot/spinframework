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

#include <SceneManager.h>
#include <UserNode.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(UserNode)
	I_DeclaringFile("UserNode.h");
	I_BaseType(ConstraintsNode);
	I_Constructor2(IN, SceneManager *, sceneManager, IN, char *, initID,
	               ____UserNode__SceneManager_P1__char_P1,
	               "",
	               "");
	I_Method0(void, updateNodePath,
	          Properties::VIRTUAL,
	          __void__updateNodePath,
	          "",
	          "The UserNode is used by OSG's NodeTrackerManipulator to position a camera for the user. However, NodeTrackerManipulator doesn't check if the nodepath has changed, so we override updateNodePath() and set an nodepathUpdate flag for the manipulator to see. ");
	I_Method1(void, setDescription, IN, const char *, s,
	          Properties::NON_VIRTUAL,
	          __void__setDescription__C5_char_P1,
	          "",
	          "");
	I_Method0(const char *, getDescription,
	          Properties::NON_VIRTUAL,
	          __C5_char_P1__getDescription,
	          "",
	          "");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "For each subclass of ReferencedNode, we override the getState() method to fill the vector with the correct set of methods for this particular node ");
	I_SimpleProperty(const char *, Description, 
	                 __C5_char_P1__getDescription, 
	                 __void__setDescription__C5_char_P1);
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
	I_PublicMemberProperty(bool, nodepathUpdate);
END_REFLECTOR

