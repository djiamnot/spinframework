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

#include <AttractorNode.h>
#include <SceneManager.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_ENUM_REFLECTOR(spin::AttractorNode::attractorMode)
	I_DeclaringFile("AttractorNode.h");
	I_EnumLabel(spin::AttractorNode::EXTRINSIC);
	I_EnumLabel(spin::AttractorNode::INTRINSIC);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(spin::AttractorNode)
	I_DeclaringFile("AttractorNode.h");
	I_BaseType(spin::GroupNode);
	I_Constructor2(IN, spin::SceneManager *, sceneManager, IN, char *, initID,
	               ____AttractorNode__SceneManager_P1__char_P1,
	               "",
	               "");
	I_Method0(void, callbackUpdate,
	          Properties::VIRTUAL,
	          __void__callbackUpdate,
	          "",
	          "For nodes that require regular programmatic control, there is a callback that is evaluated with every refresh. This function can thus be used for animations, or any other periodic updates.Note that changes to the scene graph structure (eg, moving/deleting nodes should NOT be done within this callback because traversals stacks will become corrupted. The technique is rather to enable a flag and then do the actual change in the SceneManager::updateGraph() method. ");
	I_Method1(void, setDistanceDecay, IN, float, decay,
	          Properties::NON_VIRTUAL,
	          __void__setDistanceDecay__float,
	          "",
	          "setDistanceDecay specifies how the attractive force decays as a function of distance. A decay of 1.0 is a linear decay; greater than 1 is exponential (slow at start, faster as you approach the attractor); less than 1 is logarithmic (fast at start, slower as you approach); and zero implies a constant decay (independent of distance). ");
	I_Method0(float, getDistanceDecay,
	          Properties::NON_VIRTUAL,
	          __float__getDistanceDecay,
	          "",
	          "");
	I_Method1(void, setAngularDecay, IN, float, decay,
	          Properties::NON_VIRTUAL,
	          __void__setAngularDecay__float,
	          "",
	          "setAngularDecay specifies how the attractive force decays as a function of incidence to the attractor's current orientation. ie, whether the force falls off linearly (1.0), exponentially (>1), logarithmically (<1), or not at all (0.0) if the attractor is not pointing directly at the target. ");
	I_Method0(float, getAngularDecay,
	          Properties::NON_VIRTUAL,
	          __float__getAngularDecay,
	          "",
	          "");
	I_Method1(void, setAttractorMode, IN, spin::AttractorNode::attractorMode, m,
	          Properties::NON_VIRTUAL,
	          __void__setAttractorMode__attractorMode,
	          "",
	          "Change the mode (see attractorMode enum). ");
	I_Method0(int, getAttractorMode,
	          Properties::NON_VIRTUAL,
	          __int__getAttractorMode,
	          "",
	          "");
	I_Method1(void, setForce, IN, float, force,
	          Properties::NON_VIRTUAL,
	          __void__setForce__float,
	          "",
	          "Set the attractive force (negative force for repulsion) ");
	I_Method0(float, getForce,
	          Properties::NON_VIRTUAL,
	          __float__getForce,
	          "",
	          "");
	I_Method1(void, addTarget, IN, const char *, targetID,
	          Properties::NON_VIRTUAL,
	          __void__addTarget__C5_char_P1,
	          "",
	          "Add a target node to the list ");
	I_Method1(void, removeTarget, IN, const char *, targetID,
	          Properties::NON_VIRTUAL,
	          __void__removeTarget__C5_char_P1,
	          "",
	          "Remove a target from the list ");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "For each subclass of ReferencedNode, we override the getState() method to fill the vector with the correct set of methods for this particular node ");
	I_SimpleProperty(float, AngularDecay, 
	                 __float__getAngularDecay, 
	                 __void__setAngularDecay__float);
	I_SimpleProperty(int, AttractorMode, 
	                 __int__getAttractorMode, 
	                 0);
	I_SimpleProperty(float, DistanceDecay, 
	                 __float__getDistanceDecay, 
	                 __void__setDistanceDecay__float);
	I_SimpleProperty(float, Force, 
	                 __float__getForce, 
	                 __void__setForce__float);
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
END_REFLECTOR

