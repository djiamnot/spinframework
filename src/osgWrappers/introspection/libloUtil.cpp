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

#include <libloUtil.h>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_VALUE_REFLECTOR(_lo_method)
	I_DeclaringFile("libloUtil.h");
	I_Constructor0(_____lo_method,
	               "",
	               "");
	I_PublicMemberProperty(lo_method_handler, handler);
	I_PublicMemberProperty(char *, user_data);
	I_PublicMemberProperty(struct _lo_method *, next);
END_REFLECTOR

BEGIN_VALUE_REFLECTOR(_lo_server)
	I_DeclaringFile("libloUtil.h");
	I_Constructor0(_____lo_server,
	               "",
	               "");
	I_PublicMemberProperty(struct addrinfo *, ai);
	I_PublicMemberProperty(lo_method, first);
	I_PublicMemberProperty(lo_err_handler, err_h);
	I_PublicMemberProperty(int, port);
	I_PublicMemberProperty(char *, hostname);
	I_PublicMemberProperty(char *, path);
	I_PublicMemberProperty(int, protocol);
	I_PublicMemberProperty(void *, queued);
	I_PublicMemberProperty(struct sockaddr_storage, addr);
	I_PublicMemberProperty(socklen_t, addr_len);
	I_PublicMemberProperty(int, sockets_len);
	I_PublicMemberProperty(int, sockets_alloc);
	I_PublicMemberProperty(int, fd);
	I_PublicMemberProperty(struct _lo_server::@0 *, sockets);
END_REFLECTOR

