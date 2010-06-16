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

#include <ReferencedNode.h>
#include <SceneManager.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_VALUE_REFLECTOR(ReferencedNode)
	I_DeclaringFile("ReferencedNode.h");
	I_Constructor2(IN, SceneManager *, sceneManager, IN, char *, initID,
	               ____ReferencedNode__SceneManager_P1__char_P1,
	               "",
	               "");
	I_Method1(void, registerNode, IN, SceneManager *, s,
	          Properties::NON_VIRTUAL,
	          __void__registerNode__SceneManager_P1,
	          "",
	          "");
	I_Method0(void, callbackUpdate,
	          Properties::VIRTUAL,
	          __void__callbackUpdate,
	          "",
	          "For nodes that require regular programmatic control, there is a callback that is evaluated with every refresh. This function can thus be used for animations, or any other periodic updates.Note that changes to the scene graph structure (eg, moving/deleting nodes should NOT be done within this callback because traversals stacks will become corrupted. The technique is rather to enable a flag and then do the actual change in the SceneManager::updateGraph() method. ");
	I_Method0(void, attach,
	          Properties::NON_VIRTUAL,
	          __void__attach,
	          "",
	          "The attach() method is used to properly add a node to the scene graph. Each node derived from ReferencedNode has an attachmentNode parameter that can be overridden in derived classes, but to keep code simple, this is the only this method that actually performs the attachment.In the default case, nodes should are attached directly to the parent ReferencedNode instance. ");
	I_Method0(void, detach,
	          Properties::NON_VIRTUAL,
	          __void__detach,
	          "",
	          "The detach() method is removes a node from the scene graph, depending on it's attachment position (specified by attachmentNode). ");
	I_Method0(void, updateNodePath,
	          Properties::VIRTUAL,
	          __void__updateNodePath,
	          "",
	          "IMPORTANT: subclasses of ReferencedNode are allowed to contain complicated subgraphs, and can also change their attachmentNode so that children are attached anywhere in that subgraph. If that is the case, the updateNodePath() function MUST be overridden, and extra nodes must be manually pushed onto currentNodePath. ");
	I_Method1(int, setAttachmentNode, IN, osg::Group *, n,
	          Properties::NON_VIRTUAL,
	          __int__setAttachmentNode__osg_Group_P1,
	          "",
	          "");
	I_Method0(void, updateChildNodePaths,
	          Properties::NON_VIRTUAL,
	          __void__updateChildNodePaths,
	          "",
	          "An internal method that keeps track of the nodepath (for efficient computation of global position, etc. ");
	I_Method1(void, setParent, IN, const char *, newvalue,
	          Properties::NON_VIRTUAL,
	          __void__setParent__C5_char_P1,
	          "",
	          "This method schedules a change in parent for this node. The setParent() does not immediately change the scenegraph, since it can be called at any time, even while in a traversal. The graph is updated later using the attach() method, which is called by SceneManager->updateGraph() when there is a legal time to re-order the scenegraph.Internally, this method just sets the newParent property. ");
	I_Method0(char *, getParent,
	          Properties::NON_VIRTUAL,
	          __char_P1__getParent,
	          "",
	          "Returns the current parent name (string) ");
	I_Method1(osg::Group *, getParent, IN, unsigned int, i,
	          Properties::NON_VIRTUAL,
	          __osg_Group_P1__getParent__unsigned_int,
	          "",
	          "Returns the current parent as an osg::Group ");
	I_Method1(void, setContext, IN, const char *, newvalue,
	          Properties::VIRTUAL,
	          __void__setContext__C5_char_P1,
	          "",
	          "A node can 'belong' to a certain host machine, allowing it to be rendered or behave differently than on other machines.NOTE: the \"NULL\" string means that it belongs to no specific context.NOTE: a scene operating in SERVER_MODE will always create the node, so this feature is only really relevant for clients applications. ");
	I_Method0(const char *, getContext,
	          Properties::NON_VIRTUAL,
	          __C5_char_P1__getContext,
	          "",
	          "Returns the current host ");
	I_Method2(void, setParam, IN, const char *, paramName, IN, const char *, paramValue,
	          Properties::NON_VIRTUAL,
	          __void__setParam__C5_char_P1__C5_char_P1,
	          "",
	          "");
	I_Method2(void, setParam, IN, const char *, paramName, IN, float, paramValue,
	          Properties::NON_VIRTUAL,
	          __void__setParam__C5_char_P1__float,
	          "",
	          "");
	I_Method0(osg::Group *, getAttachmentNode,
	          Properties::NON_VIRTUAL,
	          __osg_Group_P1__getAttachmentNode,
	          "",
	          "subclasses of ReferencedNode may contain complicated subgraphs, and any children get attached not to the node pointer itself, but to an attachmentNode. This attachmentNode essentially defines the origin of the local coordinate system of this node (according to the subgraph). This function returns a pointer to this node. ");
	I_Method0(void, debug,
	          Properties::VIRTUAL,
	          __void__debug,
	          "",
	          "Debug print (to log/console) ");
	I_Method0(std::vector< lo_message >, getState,
	          Properties::VIRTUAL,
	          __std_vectorT1_lo_message___getState,
	          "",
	          "For each subclass of ReferencedNode, we override the getState() method to fill the vector with the correct set of methods for this particular node. ");
	I_Method0(void, stateDump,
	          Properties::VIRTUAL,
	          __void__stateDump,
	          "",
	          "StateDump() is a request to broadcast the node state via SceneManager. ");
	I_Method1(bool, setScript, IN, const char *, scriptPath,
	          Properties::NON_VIRTUAL,
	          __bool__setScript__C5_char_P1,
	          "",
	          "");
	I_Method3(bool, addEventScript, IN, const std::string &, eventName, IN, const std::string &, scr, IN, const std::string &, params,
	          Properties::NON_VIRTUAL,
	          __bool__addEventScript__C5_std_string_R1__C5_std_string_R1__C5_std_string_R1,
	          "",
	          "");
	I_Method1(bool, callEventScript, IN, const std::string &, eventName,
	          Properties::NON_VIRTUAL,
	          __bool__callEventScript__C5_std_string_R1,
	          "",
	          "");
	I_SimpleProperty(osg::Group *, AttachmentNode, 
	                 __osg_Group_P1__getAttachmentNode, 
	                 __int__setAttachmentNode__osg_Group_P1);
	I_SimpleProperty(const char *, Context, 
	                 __C5_char_P1__getContext, 
	                 __void__setContext__C5_char_P1);
	I_SimpleProperty(char *, Parent, 
	                 __char_P1__getParent, 
	                 0);
	I_SimpleProperty(const char *, Script, 
	                 0, 
	                 __bool__setScript__C5_char_P1);
	I_SimpleProperty(std::vector< lo_message >, State, 
	                 __std_vectorT1_lo_message___getState, 
	                 0);
	I_PublicMemberProperty(t_symbol *, id);
	I_PublicMemberProperty(std::string, nodeType);
	I_PublicMemberProperty(std::string, contextString);
	I_PublicMemberProperty(int, pd_mail_id);
	I_PublicMemberProperty(lo_method, oscHandler);
	I_PublicMemberProperty(t_symbol *, parent);
	I_PublicMemberProperty(t_symbol *, newParent);
	I_PublicMemberProperty(bool, textFlag);
	I_PublicMemberProperty(stringParamType, stringParams);
	I_PublicMemberProperty(floatParamType, floatParams);
	I_PublicMemberProperty(osg::NodePath, currentNodePath);
	I_PublicMemberProperty(std::vector< ReferencedNode * >, children);
	I_PublicMemberProperty(SceneManager *, sceneManager);
	I_PublicMemberProperty(MediaManager *, mediaManager);
END_REFLECTOR

BEGIN_VALUE_REFLECTOR(ReferencedNode_callback)
	I_DeclaringFile("ReferencedNode.h");
	I_Constructor0(____ReferencedNode_callback,
	               "",
	               "");
END_REFLECTOR

