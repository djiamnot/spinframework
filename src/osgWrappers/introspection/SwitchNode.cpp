// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <cppintrospection/ReflectionMacros>
#include <cppintrospection/TypedMethodInfo>
#include <cppintrospection/StaticMethodInfo>
#include <cppintrospection/Attributes>

#include <SceneManager.h>
#include <SwitchNode.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(spin::SwitchNode)
	I_DeclaringFile("SwitchNode.h");
	I_BaseType(spin::GroupNode);
	I_Constructor2(IN, spin::SceneManager *, sceneManager, IN, char *, initID,
	               ____SwitchNode__SceneManager_P1__char_P1,
	               "",
	               "");
	I_Method0(void, updateNodePath,
	          Properties::VIRTUAL,
	          __void__updateNodePath,
	          "",
	          "IMPORTANT: subclasses of ReferencedNode are allowed to contain complicated subgraphs, and can also change their attachmentNode so that children are attached anywhere in that subgraph. If that is the case, the updateNodePath() function MUST be overridden, and extra nodes must be manually pushed onto currentNodePath. ");
	I_Method2(void, setEnabled, IN, const char *, id, IN, int, enabled,
	          Properties::NON_VIRTUAL,
	          __void__setEnabled__C5_char_P1__int,
	          "",
	          "This enables/disables a child of a certain id ");
	I_Method1(void, setAll, IN, int, enabled,
	          Properties::NON_VIRTUAL,
	          __void__setAll__int,
	          "",
	          "Set all child noded to be either disabled or enabled ");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "For each subclass of ReferencedNode, we override the getState() method to fill the vector with the correct set of methods for this particular node ");
	I_SimpleProperty(int, All, 
	                 0, 
	                 __void__setAll__int);
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
END_REFLECTOR

