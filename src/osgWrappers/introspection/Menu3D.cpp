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

#include <Menu3D.h>
#include <SceneManager.h>
#include <TextNode.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(spin::Menu3D)
	I_DeclaringFile("Menu3D.h");
	I_BaseType(spin::GroupNode);
	I_Constructor2(IN, spin::SceneManager *, sceneManager, IN, char *, initID,
	               ____Menu3D__SceneManager_P1__char_P1,
	               "",
	               "");
	I_Method0(void, updateNodePath,
	          Properties::VIRTUAL,
	          __void__updateNodePath,
	          "",
	          "");
	I_Method1(void, setEnabled, IN, int, i,
	          Properties::NON_VIRTUAL,
	          __void__setEnabled__int,
	          "",
	          "");
	I_Method0(int, getEnabled,
	          Properties::NON_VIRTUAL,
	          __int__getEnabled,
	          "",
	          "");
	I_Method1(void, addItem, IN, const char *, itemText,
	          Properties::NON_VIRTUAL,
	          __void__addItem__C5_char_P1,
	          "",
	          "Add an item (TextNode) to the list ");
	I_Method1(void, removeItem, IN, int, itemIndex,
	          Properties::NON_VIRTUAL,
	          __void__removeItem__int,
	          "",
	          "Remove an item from the list ");
	I_Method1(void, removeItem, IN, const char *, itemID,
	          Properties::NON_VIRTUAL,
	          __void__removeItem__C5_char_P1,
	          "",
	          "");
	I_Method1(int, doRemoveItem, IN, osg::observer_ptr< spin::TextNode >, n,
	          Properties::NON_VIRTUAL,
	          __int__doRemoveItem__osg_observer_ptrT1_TextNode_,
	          "",
	          "");
	I_Method0(void, clearItems,
	          Properties::NON_VIRTUAL,
	          __void__clearItems,
	          "",
	          "");
	I_Method0(void, redraw,
	          Properties::NON_VIRTUAL,
	          __void__redraw,
	          "",
	          "");
	I_Method1(void, setHighlighted, IN, int, itemIndex,
	          Properties::NON_VIRTUAL,
	          __void__setHighlighted__int,
	          "",
	          "Highlight an item from the list ");
	I_Method1(void, setHighlighted, IN, const char *, itemID,
	          Properties::NON_VIRTUAL,
	          __void__setHighlighted__C5_char_P1,
	          "",
	          "");
	I_Method1(int, doHighlight, IN, osg::observer_ptr< spin::TextNode >, n,
	          Properties::NON_VIRTUAL,
	          __int__doHighlight__osg_observer_ptrT1_TextNode_,
	          "",
	          "");
	I_Method0(const char *, getHighlighted,
	          Properties::NON_VIRTUAL,
	          __C5_char_P1__getHighlighted,
	          "",
	          "");
	I_Method4(void, setHighlightColor, IN, float, r, IN, float, g, IN, float, b, IN, float, a,
	          Properties::NON_VIRTUAL,
	          __void__setHighlightColor__float__float__float__float,
	          "",
	          "Set the color of the font when highlighted ");
	I_Method0(osg::Vec4, getHighlightColor,
	          Properties::NON_VIRTUAL,
	          __osg_Vec4__getHighlightColor,
	          "",
	          "");
	I_Method0(void, highlightNext,
	          Properties::NON_VIRTUAL,
	          __void__highlightNext,
	          "",
	          "");
	I_Method0(void, highlightPrev,
	          Properties::NON_VIRTUAL,
	          __void__highlightPrev,
	          "",
	          "");
	I_Method0(void, select,
	          Properties::NON_VIRTUAL,
	          __void__select,
	          "",
	          "");
	I_Method3(void, setItemOffset, IN, float, x, IN, float, y, IN, float, z,
	          Properties::NON_VIRTUAL,
	          __void__setItemOffset__float__float__float,
	          "",
	          "Each successive menu item will appear at an offset from the previous ");
	I_Method0(osg::Vec3, getItemOffset,
	          Properties::NON_VIRTUAL,
	          __osg_Vec3__getItemOffset,
	          "",
	          "");
	I_Method1(void, setFont, IN, const char *, s,
	          Properties::NON_VIRTUAL,
	          __void__setFont__C5_char_P1,
	          "",
	          "wrapped from TextNode: ");
	I_Method1(void, setBillboard, IN, spin::TextNode::billboardType, t,
	          Properties::NON_VIRTUAL,
	          __void__setBillboard__TextNode_billboardType,
	          "",
	          "wrapped from TextNode: ");
	I_Method4(void, setColor, IN, float, red, IN, float, green, IN, float, blue, IN, float, alpha,
	          Properties::NON_VIRTUAL,
	          __void__setColor__float__float__float__float,
	          "",
	          "wrapped from TextNode: ");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "For each subclass of ReferencedNode, we override the getState() method to fill the vector with the correct set of methods for this particular node ");
	I_SimpleProperty(spin::TextNode::billboardType, Billboard, 
	                 0, 
	                 __void__setBillboard__TextNode_billboardType);
	I_SimpleProperty(int, Enabled, 
	                 __int__getEnabled, 
	                 __void__setEnabled__int);
	I_SimpleProperty(const char *, Font, 
	                 0, 
	                 __void__setFont__C5_char_P1);
	I_SimpleProperty(osg::Vec4, HighlightColor, 
	                 __osg_Vec4__getHighlightColor, 
	                 0);
	I_SimpleProperty(const char *, Highlighted, 
	                 __C5_char_P1__getHighlighted, 
	                 __void__setHighlighted__C5_char_P1);
	I_SimpleProperty(osg::Vec3, ItemOffset, 
	                 __osg_Vec3__getItemOffset, 
	                 0);
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
END_REFLECTOR

TYPE_NAME_ALIAS(std::vector< osg::observer_ptr< spin::TextNode > >, spin::MenuVector)

STD_VECTOR_REFLECTOR(std::vector< osg::observer_ptr< spin::TextNode > >)

