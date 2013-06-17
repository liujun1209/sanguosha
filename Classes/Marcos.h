#ifndef __MACROS_H__
#define __MACROS_H__

#define CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(varName, funName)\
protected: bool varName;\
public: virtual const bool& is##funName(void) const { return varName; }\
public: virtual void set##funName(const bool& var){ varName = var; }

#define CC_SYNTHESIZE_READONLY_FOR_BOOL(varName, funName)\
protected: bool varName;\
public: virtual bool is##funName(void) const { return varName; }

#endif