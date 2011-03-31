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

#include <MeasurementNode.h>
#include <SceneManager.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_ENUM_REFLECTOR(spin::MeasurementNode::reportMode)
	I_DeclaringFile("MeasurementNode.h");
	I_EnumLabel(spin::MeasurementNode::REPORT_NONE);
	I_EnumLabel(spin::MeasurementNode::REPORT_BASIC);
	I_EnumLabel(spin::MeasurementNode::REPORT_ANGLES);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(spin::MeasurementNode)
	I_DeclaringFile("MeasurementNode.h");
	I_BaseType(spin::ReferencedNode);
	I_Constructor2(IN, spin::SceneManager *, sceneManager, IN, char *, initID,
	               ____MeasurementNode__SceneManager_P1__char_P1,
	               "",
	               "");
	I_Method0(void, callbackUpdate,
	          Properties::VIRTUAL,
	          __void__callbackUpdate,
	          "",
	          "The update callback for MeasurementNode checks to see if the target's or the MeasurementNode's global matrix has changed (ie, whether it has been moved or not). If so, it updates the internal matrices, and calls sendMeasurements() ");
	I_Method0(void, sendMeasurements,
	          Properties::NON_VIRTUAL,
	          __void__sendMeasurements,
	          "",
	          "sendMeasurements is where the actual computation takes place, and, depending on the reportMode, the measurements are sent out on the network ");
	I_Method1(void, setTarget, IN, const char *, targetID,
	          Properties::NON_VIRTUAL,
	          __void__setTarget__C5_char_P1,
	          "",
	          "MeasurementNode requires a targetNode to be set, which defines which node in the scene is being measured. ");
	I_Method1(void, setReportingLevel, IN, spin::MeasurementNode::reportMode, level,
	          Properties::NON_VIRTUAL,
	          __void__setReportingLevel__reportMode,
	          "",
	          "This sets the level of reporting (choose a reportMode) ");
	I_Method0(const char *, getTarget,
	          Properties::NON_VIRTUAL,
	          __C5_char_P1__getTarget,
	          "",
	          "");
	I_Method0(int, getReportingLevel,
	          Properties::NON_VIRTUAL,
	          __int__getReportingLevel,
	          "",
	          "");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "For each subclass of ReferencedNode, we override the getState() method to fill the vector with the correct set of methods for this particular node ");
	I_SimpleProperty(int, ReportingLevel, 
	                 __int__getReportingLevel, 
	                 0);
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
	I_SimpleProperty(const char *, Target, 
	                 __C5_char_P1__getTarget, 
	                 __void__setTarget__C5_char_P1);
END_REFLECTOR

