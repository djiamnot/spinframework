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

#include <Listener.h>
#include <SceneManager.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(spin::Listener)
	I_DeclaringFile("Listener.h");
	I_BaseType(spin::SoundNode);
	I_Constructor2(IN, spin::SceneManager *, sceneManager, IN, char *, initID,
	               ____Listener__SceneManager_P1__char_P1,
	               "",
	               "");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "For each subclass of ReferencedNode, we override the getState() method to fill the vector with the correct set of methods for this particular node ");
	I_Method1(void, setType, IN, const char *, t,
	          Properties::NON_VIRTUAL,
	          __void__setType__C5_char_P1,
	          "",
	          "");
	I_Method0(const char *, getType,
	          Properties::NON_VIRTUAL,
	          __C5_char_P1__getType,
	          "",
	          "");
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
	I_SimpleProperty(const char *, Type, 
	                 __C5_char_P1__getType, 
	                 __void__setType__C5_char_P1);
	I_PublicMemberProperty(std::string, type);
END_REFLECTOR

