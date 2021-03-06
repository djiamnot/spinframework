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

#include <AnimationNode.h>
#include <SceneManager.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_ENUM_REFLECTOR(spin::AnimationNode::LoopMode)
	I_DeclaringFile("AnimationNode.h");
	I_EnumLabel(spin::AnimationNode::SWING);
	I_EnumLabel(spin::AnimationNode::LOOP);
	I_EnumLabel(spin::AnimationNode::NO_LOOPING);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(spin::AnimationNode)
	I_DeclaringFile("AnimationNode.h");
	I_BaseType(spin::GroupNode);
	I_Constructor2(IN, spin::SceneManager *, sceneManager, IN, char *, initID,
	               ____AnimationNode__SceneManager_P1__char_P1,
	               "",
	               "");
	I_Method0(void, callbackUpdate,
	          Properties::VIRTUAL,
	          __void__callbackUpdate,
	          "",
	          "For nodes that require regular programmatic control, there is a callback that is evaluated with every refresh. This function can thus be used for animations, or any other periodic updates.Note that changes to the scene graph structure (eg, moving/deleting nodes should NOT be done within this callback because traversals stacks will become corrupted. The technique is rather to enable a flag and then do the actual change in the SceneManager::updateGraph() method. ");
	I_Method1(bool, doUpdate, IN, double, timestamp,
	          Properties::NON_VIRTUAL,
	          __bool__doUpdate__double,
	          "",
	          "Performs the actual update of translation, orientation, and scale given a timestamp into the animation ");
	I_Method1(void, setIndex, IN, float, index,
	          Properties::NON_VIRTUAL,
	          __void__setIndex__float,
	          "",
	          "A client interface that allows for setting the animation time using a normalized index in the range [0,1] ");
	I_Method1(void, setUpdateRate, IN, float, hz,
	          Properties::NON_VIRTUAL,
	          __void__setUpdateRate__float,
	          "Set the update rate (in Hz). ",
	          "The animation will send setTranslation, setOrientation, and setScale events at this rate (assuming there is a change). Values will be interpolated in between control points. ");
	I_Method0(float, getUpdateRate,
	          Properties::NON_VIRTUAL,
	          __float__getUpdateRate,
	          "",
	          "");
	I_Method1(void, setPlay, IN, int, p,
	          Properties::NON_VIRTUAL,
	          __void__setPlay__int,
	          "",
	          "");
	I_Method0(int, getPlay,
	          Properties::NON_VIRTUAL,
	          __int__getPlay,
	          "",
	          "");
	I_Method1(void, setRecord, IN, int, r,
	          Properties::NON_VIRTUAL,
	          __void__setRecord__int,
	          "",
	          "Turns on/off automatic recording. This implies that whenever the node gets an update of translation or orientation, a new position will be saved. Each position will be added with a timestamp relative to the start of the recording. ");
	I_Method0(int, getRecord,
	          Properties::NON_VIRTUAL,
	          __int__getRecord,
	          "",
	          "");
	I_Method1(void, setLoopMode, IN, spin::AnimationNode::LoopMode, mode,
	          Properties::NON_VIRTUAL,
	          __void__setLoopMode__LoopMode,
	          "",
	          "Sets the loop mode for the animation. eg, SWING, LOOP, NO_LOOPING. ");
	I_Method0(int, getLoopMode,
	          Properties::NON_VIRTUAL,
	          __int__getLoopMode,
	          "",
	          "");
	I_Method3(void, setTranslation, IN, float, x, IN, float, y, IN, float, z,
	          Properties::VIRTUAL,
	          __void__setTranslation__float__float__float,
	          "",
	          "Override setTranslation so we can store updates in record mode ");
	I_Method3(void, setOrientation, IN, float, pitch, IN, float, roll, IN, float, yaw,
	          Properties::VIRTUAL,
	          __void__setOrientation__float__float__float,
	          "",
	          "Override setOrientation so we can store updates in record mode ");
	I_Method3(void, setScale, IN, float, x, IN, float, y, IN, float, z,
	          Properties::VIRTUAL,
	          __void__setScale__float__float__float,
	          "",
	          "Override setScale so we can store updates in record mode ");
	I_Method0(void, storeCurrentPosition,
	          Properties::NON_VIRTUAL,
	          __void__storeCurrentPosition,
	          "",
	          "Stores the node's current position/rotation/scale as a control point in the animation sequence ");
	I_Method1(void, storeCurrentPosition, IN, double, timestamp,
	          Properties::NON_VIRTUAL,
	          __void__storeCurrentPosition__double,
	          "",
	          "Adds the current position/rotation/scale into the animation sequence at the specified time offset, overriding any previous position at that time. ");
	I_Method11(void, controlPoint, IN, double, timestamp, IN, float, x, IN, float, y, IN, float, z, IN, float, rotX, IN, float, rotY, IN, float, rotZ, IN, float, rotW, IN, float, scaleX, IN, float, scaleY, IN, float, scaleZ,
	           Properties::NON_VIRTUAL,
	           __void__controlPoint__double__float__float__float__float__float__float__float__float__float__float,
	           "",
	           "Explicitely adds a control point into the animation sequence ");
	I_Method0(void, clear,
	          Properties::NON_VIRTUAL,
	          __void__clear,
	          "",
	          "Clears the current animation sequence ");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "For each subclass of ReferencedNode, we override the getState() method to fill the vector with the correct set of methods for this particular node ");
	I_SimpleProperty(float, Index, 
	                 0, 
	                 __void__setIndex__float);
	I_SimpleProperty(int, LoopMode, 
	                 __int__getLoopMode, 
	                 0);
	I_SimpleProperty(int, Play, 
	                 __int__getPlay, 
	                 __void__setPlay__int);
	I_SimpleProperty(int, Record, 
	                 __int__getRecord, 
	                 __void__setRecord__int);
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
	I_SimpleProperty(float, UpdateRate, 
	                 __float__getUpdateRate, 
	                 __void__setUpdateRate__float);
END_REFLECTOR

STD_VECTOR_REFLECTOR(std::vector< lo_message >)

