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
#include <TextNode.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_ENUM_REFLECTOR(TextNode::billboardType)
	I_DeclaringFile("TextNode.h");
	I_EnumLabel(TextNode::RELATIVE);
	I_EnumLabel(TextNode::POINT_EYE);
	I_EnumLabel(TextNode::STAY_UP);
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(TextNode::decorationType)
	I_DeclaringFile("TextNode.h");
	I_EnumLabel(TextNode::DROP_SHADOW_BOTTOM_RIGHT);
	I_EnumLabel(TextNode::DROP_SHADOW_CENTER_RIGHT);
	I_EnumLabel(TextNode::DROP_SHADOW_TOP_RIGHT);
	I_EnumLabel(TextNode::DROP_SHADOW_BOTTOM_CENTER);
	I_EnumLabel(TextNode::DROP_SHADOW_TOP_CENTER);
	I_EnumLabel(TextNode::DROP_SHADOW_BOTTOM_LEFT);
	I_EnumLabel(TextNode::DROP_SHADOW_CENTER_LEFT);
	I_EnumLabel(TextNode::DROP_SHADOW_TOP_LEFT);
	I_EnumLabel(TextNode::OUTLINE);
	I_EnumLabel(TextNode::NONE);
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(TextNode::backgroundType)
	I_DeclaringFile("TextNode.h");
	I_EnumLabel(TextNode::NO_BACKGROUND);
	I_EnumLabel(TextNode::FILLED);
	I_EnumLabel(TextNode::WIREFRAME);
	I_EnumLabel(TextNode::ALL);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(TextNode)
	I_DeclaringFile("TextNode.h");
	I_BaseType(GroupNode);
	I_Constructor2(IN, SceneManager *, sceneManager, IN, char *, initID,
	               ____TextNode__SceneManager_P1__char_P1,
	               "",
	               "");
	I_Method1(void, setContext, IN, const char *, newvalue,
	          Properties::VIRTUAL,
	          __void__setContext__C5_char_P1,
	          "",
	          "A node can 'belong' to a certain host machine, allowing it to be rendered or behave differently than on other machines.NOTE: the \"NULL\" string means that it belongs to no specific context.NOTE: a scene operating in SERVER_MODE will always create the node, so this feature is only really relevant for clients applications. ");
	I_Method1(void, setTextValue, IN, const char *, s,
	          Properties::NON_VIRTUAL,
	          __void__setTextValue__C5_char_P1,
	          "",
	          "");
	I_Method1(void, setFont, IN, const char *, s,
	          Properties::NON_VIRTUAL,
	          __void__setFont__C5_char_P1,
	          "",
	          "");
	I_Method1(void, setSize, IN, float, s,
	          Properties::NON_VIRTUAL,
	          __void__setSize__float,
	          "",
	          "");
	I_Method4(void, setColor, IN, float, red, IN, float, green, IN, float, blue, IN, float, alpha,
	          Properties::NON_VIRTUAL,
	          __void__setColor__float__float__float__float,
	          "",
	          "");
	I_Method4(void, setBgColor, IN, float, red, IN, float, green, IN, float, blue, IN, float, alpha,
	          Properties::NON_VIRTUAL,
	          __void__setBgColor__float__float__float__float,
	          "",
	          "");
	I_Method1(void, setMargin, IN, float, margin,
	          Properties::NON_VIRTUAL,
	          __void__setMargin__float,
	          "",
	          "");
	I_Method1(void, setBillboard, IN, TextNode::billboardType, t,
	          Properties::NON_VIRTUAL,
	          __void__setBillboard__billboardType,
	          "",
	          "");
	I_Method1(void, setDecoration, IN, TextNode::decorationType, t,
	          Properties::NON_VIRTUAL,
	          __void__setDecoration__decorationType,
	          "",
	          "");
	I_Method1(void, setBackground, IN, TextNode::backgroundType, t,
	          Properties::NON_VIRTUAL,
	          __void__setBackground__backgroundType,
	          "",
	          "");
	I_Method0(const char *, getTextValue,
	          Properties::NON_VIRTUAL,
	          __C5_char_P1__getTextValue,
	          "",
	          "");
	I_Method0(std::string, getTextString,
	          Properties::NON_VIRTUAL,
	          __std_string__getTextString,
	          "",
	          "");
	I_Method0(const char *, getFont,
	          Properties::NON_VIRTUAL,
	          __C5_char_P1__getFont,
	          "",
	          "");
	I_Method0(float, getSize,
	          Properties::NON_VIRTUAL,
	          __float__getSize,
	          "",
	          "");
	I_Method0(osg::Vec4, getColor,
	          Properties::NON_VIRTUAL,
	          __osg_Vec4__getColor,
	          "",
	          "");
	I_Method0(osg::Vec4, getBgColor,
	          Properties::NON_VIRTUAL,
	          __osg_Vec4__getBgColor,
	          "",
	          "");
	I_Method0(float, getMargin,
	          Properties::NON_VIRTUAL,
	          __float__getMargin,
	          "",
	          "");
	I_Method0(int, getBillboard,
	          Properties::NON_VIRTUAL,
	          __int__getBillboard,
	          "",
	          "");
	I_Method0(int, getDecoration,
	          Properties::NON_VIRTUAL,
	          __int__getDecoration,
	          "",
	          "");
	I_Method0(int, getBackround,
	          Properties::NON_VIRTUAL,
	          __int__getBackround,
	          "",
	          "");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "For each subclass of ReferencedNode, we override the getState() method to fill the vector with the correct set of methods for this particular node ");
	I_SimpleProperty(TextNode::backgroundType, Background, 
	                 0, 
	                 __void__setBackground__backgroundType);
	I_SimpleProperty(int, Backround, 
	                 __int__getBackround, 
	                 0);
	I_SimpleProperty(osg::Vec4, BgColor, 
	                 __osg_Vec4__getBgColor, 
	                 0);
	I_SimpleProperty(TextNode::billboardType, Billboard, 
	                 0, 
	                 __void__setBillboard__billboardType);
	I_SimpleProperty(osg::Vec4, Color, 
	                 __osg_Vec4__getColor, 
	                 0);
	I_SimpleProperty(const char *, Context, 
	                 0, 
	                 __void__setContext__C5_char_P1);
	I_SimpleProperty(TextNode::decorationType, Decoration, 
	                 0, 
	                 __void__setDecoration__decorationType);
	I_SimpleProperty(const char *, Font, 
	                 __C5_char_P1__getFont, 
	                 __void__setFont__C5_char_P1);
	I_SimpleProperty(float, Margin, 
	                 __float__getMargin, 
	                 __void__setMargin__float);
	I_SimpleProperty(float, Size, 
	                 __float__getSize, 
	                 __void__setSize__float);
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
	I_SimpleProperty(std::string, TextString, 
	                 __std_string__getTextString, 
	                 0);
	I_SimpleProperty(const char *, TextValue, 
	                 __C5_char_P1__getTextValue, 
	                 __void__setTextValue__C5_char_P1);
END_REFLECTOR

