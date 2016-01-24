
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __javax_naming_spi_DirectoryManager__
#define __javax_naming_spi_DirectoryManager__

#pragma interface

#include <javax/naming/spi/NamingManager.h>
extern "Java"
{
  namespace javax
  {
    namespace naming
    {
        class CannotProceedException;
        class Context;
        class Name;
      namespace directory
      {
          class Attributes;
          class DirContext;
      }
      namespace spi
      {
          class DirStateFactory$Result;
          class DirectoryManager;
          class ObjectFactory;
      }
    }
  }
}

class javax::naming::spi::DirectoryManager : public ::javax::naming::spi::NamingManager
{

public: // actually package-private
  DirectoryManager();
public:
  static ::javax::naming::directory::DirContext * getContinuationDirContext(::javax::naming::CannotProceedException *);
private:
  static ::java::lang::Object * tryCreateObject(::javax::naming::spi::ObjectFactory *, ::java::lang::Object *, ::javax::naming::Name *, ::javax::naming::Context *, ::java::util::Hashtable *, ::javax::naming::directory::Attributes *);
public:
  static ::java::lang::Object * getObjectInstance(::java::lang::Object *, ::javax::naming::Name *, ::javax::naming::Context *, ::java::util::Hashtable *, ::javax::naming::directory::Attributes *);
  static ::javax::naming::spi::DirStateFactory$Result * getStateToBind(::java::lang::Object *, ::javax::naming::Name *, ::javax::naming::Context *, ::java::util::Hashtable *, ::javax::naming::directory::Attributes *);
  static ::java::lang::Class class$;
};

#endif // __javax_naming_spi_DirectoryManager__
