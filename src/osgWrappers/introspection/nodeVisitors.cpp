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

#include <nodeVisitors.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_VALUE_REFLECTOR(spin::DebugVisitor)
	I_DeclaringFile("nodeVisitors.h");
	I_Constructor0(____DebugVisitor,
	               "",
	               "");
	I_Method1(void, apply, IN, osg::Node &, node,
	          Properties::VIRTUAL,
	          __void__apply__osg_Node_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::PositionAttitudeTransform &, node,
	          Properties::VIRTUAL,
	          __void__apply__osg_PositionAttitudeTransform_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Geode &, node,
	          Properties::VIRTUAL,
	          __void__apply__osg_Geode_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Group &, node,
	          Properties::VIRTUAL,
	          __void__apply__osg_Group_R1,
	          "",
	          "");
END_REFLECTOR

BEGIN_VALUE_REFLECTOR(spin::NodeSearcher)
	I_DeclaringFile("nodeVisitors.h");
	I_Constructor1(IN, NodeList &, list,
	               Properties::NON_EXPLICIT,
	               ____NodeSearcher__NodeList_R1,
	               "",
	               "");
	I_Method1(void, apply, IN, osg::Node &, node,
	          Properties::VIRTUAL,
	          __void__apply__osg_Node_R1,
	          "",
	          "");
	I_Method2(void, search, IN, osg::Node *, subgraph, IN, std::string, s,
	          Properties::NON_VIRTUAL,
	          __void__search__osg_Node_P1__std_string,
	          "",
	          "");
	I_PublicMemberProperty(std::string, _searchName);
END_REFLECTOR

BEGIN_VALUE_REFLECTOR(spin::SearchVisitor)
	I_DeclaringFile("nodeVisitors.h");
	I_Constructor0(____SearchVisitor,
	               "",
	               "");
	I_Method1(void, apply, IN, osg::Node &, searchNode,
	          Properties::VIRTUAL,
	          __void__apply__osg_Node_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Group &, searchNode,
	          Properties::VIRTUAL,
	          __void__apply__osg_Group_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Geode &, searchNode,
	          Properties::VIRTUAL,
	          __void__apply__osg_Geode_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::MatrixTransform &, searchNode,
	          Properties::VIRTUAL,
	          __void__apply__osg_MatrixTransform_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::PositionAttitudeTransform &, searchNode,
	          Properties::VIRTUAL,
	          __void__apply__osg_PositionAttitudeTransform_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Switch &, searchNode,
	          Properties::VIRTUAL,
	          __void__apply__osg_Switch_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Sequence &, searchNode,
	          Properties::VIRTUAL,
	          __void__apply__osg_Sequence_R1,
	          "",
	          "");
	I_Method2(void, searchNode, IN, osg::Node *, searchFromMe, IN, std::string, searchName,
	          Properties::NON_VIRTUAL,
	          __void__searchNode__osg_Node_P1__std_string,
	          "",
	          "");
	I_Method0(osg::ref_ptr< osg::Group >, getGroup,
	          Properties::NON_VIRTUAL,
	          __osg_ref_ptrT1_osg_Group___getGroup,
	          "",
	          "");
	I_Method0(osg::ref_ptr< osg::MatrixTransform >, getMT,
	          Properties::NON_VIRTUAL,
	          __osg_ref_ptrT1_osg_MatrixTransform___getMT,
	          "",
	          "");
	I_Method0(osg::ref_ptr< osg::Geode >, getGeode,
	          Properties::NON_VIRTUAL,
	          __osg_ref_ptrT1_osg_Geode___getGeode,
	          "",
	          "");
	I_Method0(osg::ref_ptr< osg::PositionAttitudeTransform >, getPAT,
	          Properties::NON_VIRTUAL,
	          __osg_ref_ptrT1_osg_PositionAttitudeTransform___getPAT,
	          "",
	          "");
	I_Method0(osg::ref_ptr< osg::Switch >, getSwitchNode,
	          Properties::NON_VIRTUAL,
	          __osg_ref_ptrT1_osg_Switch___getSwitchNode,
	          "",
	          "");
	I_Method0(osg::ref_ptr< osg::Sequence >, getSequenceNode,
	          Properties::NON_VIRTUAL,
	          __osg_ref_ptrT1_osg_Sequence___getSequenceNode,
	          "",
	          "");
	I_SimpleProperty(osg::ref_ptr< osg::Geode >, Geode, 
	                 __osg_ref_ptrT1_osg_Geode___getGeode, 
	                 0);
	I_SimpleProperty(osg::ref_ptr< osg::Group >, Group, 
	                 __osg_ref_ptrT1_osg_Group___getGroup, 
	                 0);
	I_SimpleProperty(osg::ref_ptr< osg::MatrixTransform >, MT, 
	                 __osg_ref_ptrT1_osg_MatrixTransform___getMT, 
	                 0);
	I_SimpleProperty(osg::ref_ptr< osg::PositionAttitudeTransform >, PAT, 
	                 __osg_ref_ptrT1_osg_PositionAttitudeTransform___getPAT, 
	                 0);
	I_SimpleProperty(osg::ref_ptr< osg::Sequence >, SequenceNode, 
	                 __osg_ref_ptrT1_osg_Sequence___getSequenceNode, 
	                 0);
	I_SimpleProperty(osg::ref_ptr< osg::Switch >, SwitchNode, 
	                 __osg_ref_ptrT1_osg_Switch___getSwitchNode, 
	                 0);
END_REFLECTOR

BEGIN_VALUE_REFLECTOR(spin::TextureStateSetFinder)
	I_DeclaringFile("nodeVisitors.h");
	I_Constructor1(IN, spin::StateSetList &, list,
	               Properties::NON_EXPLICIT,
	               ____TextureStateSetFinder__StateSetList_R1,
	               "",
	               "");
	I_Method1(void, apply, IN, osg::Node &, node,
	          Properties::VIRTUAL,
	          __void__apply__osg_Node_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Geode &, geode,
	          Properties::VIRTUAL,
	          __void__apply__osg_Geode_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::StateSet *, stateset,
	          Properties::VIRTUAL,
	          __void__apply__osg_StateSet_P1,
	          "",
	          "");
END_REFLECTOR

BEGIN_VALUE_REFLECTOR(spin::UpdateSceneVisitor)
	I_DeclaringFile("nodeVisitors.h");
	I_Constructor0(____UpdateSceneVisitor,
	               "",
	               "");
	I_Method1(void, apply, IN, osg::Node &, node,
	          Properties::VIRTUAL,
	          __void__apply__osg_Node_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Group &, node,
	          Properties::VIRTUAL,
	          __void__apply__osg_Group_R1,
	          "",
	          "");
END_REFLECTOR

TYPE_NAME_ALIAS(std::vector< osg::ref_ptr< osg::StateSet > >, spin::StateSetList)

STD_VECTOR_REFLECTOR(std::vector< osg::ref_ptr< osg::StateSet > >)

