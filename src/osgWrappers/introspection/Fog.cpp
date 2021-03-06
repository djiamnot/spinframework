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

#include <Fog.h>
#include <SceneManager.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(spin::Fog)
	I_DeclaringFile("Fog.h");
	I_BaseType(spin::ReferencedStateSet);
	I_Constructor2(IN, spin::SceneManager *, sceneManager, IN, const char *, initID,
	               ____Fog__SceneManager_P1__C5_char_P1,
	               "",
	               "");
	I_Method0(const char *, getPath,
	          Properties::VIRTUAL,
	          __C5_char_P1__getPath,
	          "",
	          "need to implement abstract method... ?! ");
	I_Method1(void, setFogDensity, IN, float, density,
	          Properties::NON_VIRTUAL,
	          __void__setFogDensity__float,
	          "",
	          "Set fog density (good values are around 0.001 - 0.1) ");
	I_Method0(float, getFogDensity,
	          Properties::NON_VIRTUAL,
	          __float__getFogDensity,
	          "",
	          "Get fog density (good values are around 0.001 - 0.1) ");
	I_Method4(void, setFogColor, IN, float, r, IN, float, g, IN, float, b, IN, float, a,
	          Properties::NON_VIRTUAL,
	          __void__setFogColor__float__float__float__float,
	          "",
	          "Set fog color ");
	I_Method0(osg::Vec4, getFogColor,
	          Properties::NON_VIRTUAL,
	          __osg_Vec4__getFogColor,
	          "",
	          "Get fog color ");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "must reimplement ");
	I_SimpleProperty(osg::Vec4, FogColor, 
	                 __osg_Vec4__getFogColor, 
	                 0);
	I_SimpleProperty(float, FogDensity, 
	                 __float__getFogDensity, 
	                 __void__setFogDensity__float);
	I_SimpleProperty(const char *, Path, 
	                 __C5_char_P1__getPath, 
	                 0);
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
END_REFLECTOR

