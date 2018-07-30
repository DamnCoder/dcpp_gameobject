// Original author: Paul Varcholik
// Forked author: Shao Voon Wong
// Re-forked author: Jorge López González
// version 0.1: Changed sRunTimeTypeId type from unsigned int to size_t
// version 0.2: Replaced sRunTimeTypeId with a static local variable, so the class is a header only. Credit: Andrew Fedoniouk aka c-smile
// version 0.3: Jorge López: Added macro for base classes, added direct cast methods, removed RTTI class to avoid inheritance in the base class

#pragma once

#include <string.h>

namespace dc
{
#define RTTI_COMMON(Type) \
	template <typename T> \
	T* DirectCast() \
	{ \
		return (T*)this; \
	} \
	\
	template <typename T> \
	const T* DirectCast() const \
	{ \
		return (T*)this; \
	} \
	\
	template <typename T> \
	T* SecureCast() \
	{ \
		if (Is(T::TypeIdClass())) \
		{ \
			return (T*)this; \
		} \
		\
		return 0; \
	} \
	\
	template <typename T> \
	const T* SecureCast() const \
	{ \
		if (Is(T::TypeIdClass())) \
		{ \
			return (T*)this; \
		} \
		\
		return 0; \
	}
	
#define RTTI_BASE_DECLARATIONS(Type) \
	public: \
		static const char* TypeName() { return #Type; } \
		\
		static const size_t TypeIdClass() \
		{ \
			static int d = 0; \
			return (size_t) &d; \
		} \
		\
		virtual const char* InstanceName() const { return TypeName(); } \
		\
		virtual const size_t TypeIdInstance() const \
		{ \
			return Type::TypeIdClass(); \
		} \
		\
		virtual const bool Is(const size_t id) const \
		{ \
			return id == Type::TypeIdClass(); \
		} \
		\
		virtual const bool Is(const char* name) const \
		{ \
			return strcmp (name, Type::TypeName()) == 0; \
		} \
		\
		RTTI_COMMON(Type)


#define RTTI_DECLARATIONS(Type, ParentType) \
    public: \
        static const char* TypeName() { return #Type; } \
        \
		static const size_t TypeIdClass() \
		{ \
			static int d = 0; \
			return (size_t) &d; \
		} \
		\
		const char* InstanceName() const override { return TypeName(); } \
		\
		const size_t TypeIdInstance() const override \
		{ \
			return Type::TypeIdClass(); \
		} \
		\
        const bool Is(const size_t id) const override \
        { \
            if (id == Type::TypeIdClass()) \
			{ \
				return true; \
			} \
            else \
			{ \
				return ParentType::Is(id); \
			} \
        } \
		\
        const bool Is(const char* name) const override \
        { \
            if (strcmp (name, Type::TypeName()) == 0) \
			{ \
				return true; \
			} \
            else \
			{ \
				return ParentType::Is(name); \
			} \
        } \
		\
		RTTI_COMMON(Type)
}