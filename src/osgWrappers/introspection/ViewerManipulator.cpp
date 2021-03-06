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

#include <GroupNode.h>
#include <ViewerManipulator.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(spin::ViewerManipulator)
	I_DeclaringFile("ViewerManipulator.h");
	I_Constructor0(____ViewerManipulator,
	               "",
	               "");
	I_Method1(void, setPicker, IN, bool, b,
	          Properties::NON_VIRTUAL,
	          __void__setPicker__bool,
	          "",
	          "");
	I_Method1(void, setMover, IN, bool, b,
	          Properties::NON_VIRTUAL,
	          __void__setMover__bool,
	          "",
	          "");
	I_Method1(void, setRaw, IN, bool, b,
	          Properties::NON_VIRTUAL,
	          __void__setRaw__bool,
	          "",
	          "");
	I_Method2(bool, handle, IN, const osgGA::GUIEventAdapter &, ea, IN, osgGA::GUIActionAdapter &, aa,
	          Properties::NON_VIRTUAL,
	          __bool__handle__C5_osgGA_GUIEventAdapter_R1__osgGA_GUIActionAdapter_R1,
	          "",
	          "");
	I_Method1(void, handleKeypress, IN, const osgGA::GUIEventAdapter &, ea,
	          Properties::NON_VIRTUAL,
	          __void__handleKeypress__C5_osgGA_GUIEventAdapter_R1,
	          "",
	          "");
	I_Method2(void, handleMouse, IN, osgViewer::View *, view, IN, const osgGA::GUIEventAdapter &, ea,
	          Properties::NON_VIRTUAL,
	          __void__handleMouse__osgViewer_View_P1__C5_osgGA_GUIEventAdapter_R1,
	          "",
	          "");
	I_Method9(void, sendPick, IN, spin::GroupNode *, hitNode, IN, unsigned int, eventType, IN, unsigned int, modKeyMask, IN, unsigned int, buttonMask, IN, float, scrollX, IN, float, scrollY, IN, float, dX, IN, float, dY, IN, osg::Vec3, hitPoint,
	          Properties::NON_VIRTUAL,
	          __void__sendPick__GroupNode_P1__unsigned_int__unsigned_int__unsigned_int__float__float__float__float__osg_Vec3,
	          "",
	          "");
	I_Method3(void, sendEvent, IN, const char *, nodeId, IN, const char *, types, IN, ..., x,
	          Properties::NON_VIRTUAL,
	          __void__sendEvent__C5_char_P1__C5_char_P1__...,
	          "",
	          "");
	I_Method3(void, sendEvent, IN, const char *, nodeId, IN, const char *, types, IN, va_list, ap,
	          Properties::NON_VIRTUAL,
	          __void__sendEvent__C5_char_P1__C5_char_P1__va_list,
	          "",
	          "");
	I_SimpleProperty(bool, Mover, 
	                 0, 
	                 __void__setMover__bool);
	I_SimpleProperty(bool, Picker, 
	                 0, 
	                 __void__setPicker__bool);
	I_SimpleProperty(bool, Raw, 
	                 0, 
	                 __void__setRaw__bool);
END_REFLECTOR

