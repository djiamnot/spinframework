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

#include <RayNode.h>
#include <SceneManager.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(RayNode)
	I_DeclaringFile("RayNode.h");
	I_BaseType(ReferencedNode);
	I_Constructor2(IN, SceneManager *, sceneManager, IN, char *, initID,
	               ____RayNode__SceneManager_P1__char_P1,
	               "",
	               "");
	I_Method1(void, setVisible, IN, int, visibilityFlag,
	          Properties::NON_VIRTUAL,
	          __void__setVisible__int,
	          "",
	          "");
	I_Method1(void, setLength, IN, float, length,
	          Properties::NON_VIRTUAL,
	          __void__setLength__float,
	          "",
	          "");
	I_Method1(void, setThickness, IN, float, thickness,
	          Properties::NON_VIRTUAL,
	          __void__setThickness__float,
	          "",
	          "");
	I_Method4(void, setColor, IN, float, red, IN, float, green, IN, float, blue, IN, float, alpha,
	          Properties::NON_VIRTUAL,
	          __void__setColor__float__float__float__float,
	          "",
	          "");
	I_Method0(int, getVisible,
	          Properties::NON_VIRTUAL,
	          __int__getVisible,
	          "",
	          "");
	I_Method0(float, getLength,
	          Properties::NON_VIRTUAL,
	          __float__getLength,
	          "",
	          "");
	I_Method0(float, getThickness,
	          Properties::NON_VIRTUAL,
	          __float__getThickness,
	          "",
	          "");
	I_Method0(osg::Vec4, getColor,
	          Properties::NON_VIRTUAL,
	          __osg_Vec4__getColor,
	          "",
	          "");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "For each subclass of ReferencedNode, we override the getState() method to fill the vector with the correct set of methods for this particular node ");
	I_SimpleProperty(osg::Vec4, Color, 
	                 __osg_Vec4__getColor, 
	                 0);
	I_SimpleProperty(float, Length, 
	                 __float__getLength, 
	                 __void__setLength__float);
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
	I_SimpleProperty(float, Thickness, 
	                 __float__getThickness, 
	                 __void__setThickness__float);
	I_SimpleProperty(int, Visible, 
	                 __int__getVisible, 
	                 __void__setVisible__int);
END_REFLECTOR

