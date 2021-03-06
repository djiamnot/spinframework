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
#include <SceneManager.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_ENUM_REFLECTOR(spin::GroupNode::interactionMode)
	I_DeclaringFile("GroupNode.h");
	I_EnumLabel(spin::GroupNode::STATIC);
	I_EnumLabel(spin::GroupNode::SELECT);
	I_EnumLabel(spin::GroupNode::DRAG);
	I_EnumLabel(spin::GroupNode::THROW);
	I_EnumLabel(spin::GroupNode::DRAW);
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(spin::GroupNode::globalsReportMode)
	I_DeclaringFile("GroupNode.h");
	I_EnumLabel(spin::GroupNode::NONE);
	I_EnumLabel(spin::GroupNode::GLOBAL_6DOF);
	I_EnumLabel(spin::GroupNode::GLOBAL_ALL);
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(spin::GroupNode::velocityMode)
	I_DeclaringFile("GroupNode.h");
	I_EnumLabel(spin::GroupNode::TRANSLATE);
	I_EnumLabel(spin::GroupNode::MOVE);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(spin::GroupNode)
	I_DeclaringFile("GroupNode.h");
	I_BaseType(spin::ReferencedNode);
	I_Constructor2(IN, spin::SceneManager *, sceneManager, IN, char *, initID,
	               ____GroupNode__SceneManager_P1__char_P1,
	               "",
	               " param initID will be converted into a t_symbol  ");
	I_Method0(void, callbackUpdate,
	          Properties::VIRTUAL,
	          __void__callbackUpdate,
	          "",
	          "For nodes that require regular programmatic control, there is a callback that is evaluated with every refresh. This function can thus be used for animations, or any other periodic updates.Note that changes to the scene graph structure (eg, moving/deleting nodes should NOT be done within this callback because traversals stacks will become corrupted. The technique is rather to enable a flag and then do the actual change in the SceneManager::updateGraph() method. ");
	I_MethodWithDefaults1(void, updateNodePath, IN, bool, updateChildren, true,
	                      Properties::VIRTUAL,
	                      __void__updateNodePath__bool,
	                      "",
	                      "IMPORTANT: subclasses of ReferencedNode are allowed to contain complicated subgraphs, and can also change their attachmentNode so that children are attached anywhere in that subgraph. If that is the case, the updateNodePath() function MUST be overridden, and extra nodes must be manually pushed onto currentNodePath. ");
	I_Method5(void, mouseEvent, IN, int, event, IN, int, keyMask, IN, int, buttonMask, IN, float, x, IN, float, y,
	          Properties::NON_VIRTUAL,
	          __void__mouseEvent__int__int__int__float__float,
	          "",
	          "");
	I_Method7(void, event, IN, int, event, IN, const char *, userString, IN, float, eData1, IN, float, eData2, IN, float, x, IN, float, y, IN, float, z,
	          Properties::NON_VIRTUAL,
	          __void__event__int__C5_char_P1__float__float__float__float__float,
	          "",
	          "");
	I_Method0(void, debug,
	          Properties::VIRTUAL,
	          __void__debug,
	          "",
	          "Debug print (to log/console) ");
	I_Method1(void, setReportMode, IN, spin::GroupNode::globalsReportMode, mode,
	          Properties::NON_VIRTUAL,
	          __void__setReportMode__globalsReportMode,
	          "",
	          "");
	I_Method1(void, setInteractionMode, IN, spin::GroupNode::interactionMode, mode,
	          Properties::NON_VIRTUAL,
	          __void__setInteractionMode__interactionMode,
	          "",
	          "");
	I_Method3(void, setClipping, IN, float, x, IN, float, y, IN, float, z,
	          Properties::NON_VIRTUAL,
	          __void__setClipping__float__float__float,
	          "",
	          "Set a clipping rectangle for the model so that geometry outside of the region (+-x, +-y, +-z) will not be shown (or used in interactive events) ");
	I_Method3(void, setTranslation, IN, float, x, IN, float, y, IN, float, z,
	          Properties::VIRTUAL,
	          __void__setTranslation__float__float__float,
	          "",
	          "The local translation offset for this node with respect to it's parent ");
	I_Method3(void, setOrientation, IN, float, pitch, IN, float, roll, IN, float, yaw,
	          Properties::VIRTUAL,
	          __void__setOrientation__float__float__float,
	          "",
	          "The local orientation offset for this node with respect to it's parent ");
	I_Method4(void, setOrientationQuat, IN, float, x, IN, float, y, IN, float, z, IN, float, w,
	          Properties::VIRTUAL,
	          __void__setOrientationQuat__float__float__float__float,
	          "",
	          "Set the orientation offset as a quaternion ");
	I_Method3(void, setScale, IN, float, x, IN, float, y, IN, float, z,
	          Properties::VIRTUAL,
	          __void__setScale__float__float__float,
	          "",
	          "A grouped scale operation ");
	I_Method3(void, setVelocity, IN, float, dx, IN, float, dy, IN, float, dz,
	          Properties::VIRTUAL,
	          __void__setVelocity__float__float__float,
	          "",
	          "A translational velocity (m/s). This is computed in the callbackUpdate() function. ");
	I_Method1(void, setVelocityMode, IN, spin::GroupNode::velocityMode, mode,
	          Properties::VIRTUAL,
	          __void__setVelocityMode__velocityMode,
	          "",
	          "Applying velocity to an object can either result in translational motion, where velocityMode is TRANSLATE (0). This is the default and applies and motion is relative to the local coordinate system of the node. When velocityMode is MOVE (1), then motion is relative to the current orientation of the node, analogous to the move() command. ");
	I_Method3(void, setSpin, IN, float, dp, IN, float, dr, IN, float, dy,
	          Properties::VIRTUAL,
	          __void__setSpin__float__float__float,
	          "",
	          "A rotational velocity (deg/sec), computed in callbackUpdate(). ");
	I_Method1(void, setDamping, IN, float, d,
	          Properties::VIRTUAL,
	          __void__setDamping__float,
	          "",
	          "A Damping value (negative acceleration) that gets applied to velocity and spin over time. Units are in -m/sec2 or -deg/sec2, meaning that: zero damping will not change velocity/spin valuesany positive value will decrease the speeds  ");
	I_Method3(void, translate, IN, float, x, IN, float, y, IN, float, z,
	          Properties::VIRTUAL,
	          __void__translate__float__float__float,
	          "",
	          "The translate command increments the node's current translation values (ie, it's position in the scene with respect to it's parent) ");
	I_Method3(void, move, IN, float, x, IN, float, y, IN, float, z,
	          Properties::VIRTUAL,
	          __void__move__float__float__float,
	          "",
	          "The move command adds a relative translation with respect to the node's current orientation. That is, the node will translate along it's direction vector by the supplied number of units. ");
	I_Method3(void, rotate, IN, float, pitch, IN, float, roll, IN, float, yaw,
	          Properties::VIRTUAL,
	          __void__rotate__float__float__float,
	          "",
	          "The rotate command adds a relative rotation to the node's current orientation. ");
	I_Method0(int, getReportMode,
	          Properties::NON_VIRTUAL,
	          __int__getReportMode,
	          "",
	          "");
	I_Method0(int, getInteractionMode,
	          Properties::NON_VIRTUAL,
	          __int__getInteractionMode,
	          "",
	          "");
	I_Method0(osg::Vec3, getClipping,
	          Properties::NON_VIRTUAL,
	          __osg_Vec3__getClipping,
	          "",
	          "");
	I_Method0(osg::Vec3, getTranslation,
	          Properties::NON_VIRTUAL,
	          __osg_Vec3__getTranslation,
	          "",
	          "");
	I_Method0(osg::Vec3, getOrientation,
	          Properties::NON_VIRTUAL,
	          __osg_Vec3__getOrientation,
	          "",
	          "");
	I_Method0(osg::Vec3, getScale,
	          Properties::NON_VIRTUAL,
	          __osg_Vec3__getScale,
	          "",
	          "");
	I_Method0(osg::Vec3, getVelocity,
	          Properties::NON_VIRTUAL,
	          __osg_Vec3__getVelocity,
	          "",
	          "");
	I_Method0(int, getVelocityMode,
	          Properties::NON_VIRTUAL,
	          __int__getVelocityMode,
	          "",
	          "");
	I_Method0(float, getDamping,
	          Properties::NON_VIRTUAL,
	          __float__getDamping,
	          "",
	          "");
	I_Method0(osg::Matrix, getGlobalMatrix,
	          Properties::NON_VIRTUAL,
	          __osg_Matrix__getGlobalMatrix,
	          "",
	          "");
	I_Method0(osg::Vec3, getCenter,
	          Properties::NON_VIRTUAL,
	          __osg_Vec3__getCenter,
	          "",
	          "");
	I_Method1(bool, dumpGlobals, IN, bool, forced,
	          Properties::NON_VIRTUAL,
	          __bool__dumpGlobals__bool,
	          "",
	          "The dumpGlobals method results in a broadcast of this node's translation and orientation. It is called by callbackUpdate() every frame, however the 'forced' flag will be set to false, so it will only send a message if the node's matrix has changed. If the 'forced' flag is set to true, it will definitely result in a message broadcast. This should only be used when necessary (eg, when a stateDump is requested).Note: the return value is only to fool wx so that it doesn't consider this as an editable property. ");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "For each subclass of ReferencedNode, we override the getState() method to fill the vector with the correct set of methods for this particular node ");
	I_Method0(void, stateDump,
	          Properties::VIRTUAL,
	          __void__stateDump,
	          "",
	          "We override stateDump so that we can additionally force a dumpGlobals() call whenever a dump is requested ");
	I_SimpleProperty(osg::Vec3, Center, 
	                 __osg_Vec3__getCenter, 
	                 0);
	I_SimpleProperty(osg::Vec3, Clipping, 
	                 __osg_Vec3__getClipping, 
	                 0);
	I_SimpleProperty(float, Damping, 
	                 __float__getDamping, 
	                 __void__setDamping__float);
	I_SimpleProperty(osg::Matrix, GlobalMatrix, 
	                 __osg_Matrix__getGlobalMatrix, 
	                 0);
	I_SimpleProperty(int, InteractionMode, 
	                 __int__getInteractionMode, 
	                 0);
	I_SimpleProperty(osg::Vec3, Orientation, 
	                 __osg_Vec3__getOrientation, 
	                 0);
	I_SimpleProperty(int, ReportMode, 
	                 __int__getReportMode, 
	                 0);
	I_SimpleProperty(osg::Vec3, Scale, 
	                 __osg_Vec3__getScale, 
	                 0);
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
	I_SimpleProperty(osg::Vec3, Translation, 
	                 __osg_Vec3__getTranslation, 
	                 0);
	I_SimpleProperty(osg::Vec3, Velocity, 
	                 __osg_Vec3__getVelocity, 
	                 0);
	I_SimpleProperty(int, VelocityMode, 
	                 __int__getVelocityMode, 
	                 0);
END_REFLECTOR

