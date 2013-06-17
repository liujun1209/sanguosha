#ifndef __MACROS_H__
#define __MACROS_H__
#include <boost\algorithm\string.hpp>

#define CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(varName, funName)\
protected: bool varName;\
public: virtual const bool& is##funName(void) const { return varName; }\
public: virtual void set##funName(const bool& var){ varName = var; }

#define CC_SYNTHESIZE_READONLY_FOR_BOOL(varName, funName)\
protected: bool varName;\
public: virtual bool is##funName(void) const { return varName; }

#define IS_KIND_OF_DECLARE_FOR_ROOT \
public: virtual bool isKindOf(string className) const { return  boost::iequals(name_ ,className); }

#define IS_KIND_OF_DECLARE_FOR_ONE_PARENT(parentName) \
protected: string name_;\
public: virtual bool isKindOf(string className) const { return  boost::iequals(name_ ,className) && ##parentName::isKindOf(className); }

/*
    char* G2U(const char* gb2312)    
    {    
     int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);    
     wchar_t* wstr = new wchar_t[len+1];    
     memset(wstr, 0, len+1);    
     MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);    
     len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);    
     char* str = new char[len+1];    
     memset(str, 0, len+1);    
     WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);    
     if(wstr) delete[] wstr;    
     return str;    
    }  
    */
#endif