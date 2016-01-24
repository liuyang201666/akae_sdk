
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __javax_accessibility_AccessibleAction__
#define __javax_accessibility_AccessibleAction__

#pragma interface

#include <java/lang/Object.h>
extern "Java"
{
  namespace javax
  {
    namespace accessibility
    {
        class AccessibleAction;
    }
  }
}

class javax::accessibility::AccessibleAction : public ::java::lang::Object
{

public:
  virtual jint getAccessibleActionCount() = 0;
  virtual ::java::lang::String * getAccessibleActionDescription(jint) = 0;
  virtual jboolean doAccessibleAction(jint) = 0;
  static ::java::lang::String * DECREMENT;
  static ::java::lang::String * INCREMENT;
  static ::java::lang::String * TOGGLE_EXPAND;
  static ::java::lang::Class class$;
} __attribute__ ((java_interface));

#endif // __javax_accessibility_AccessibleAction__
