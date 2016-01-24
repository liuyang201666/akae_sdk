
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __javax_swing_plaf_metal_MetalUtils__
#define __javax_swing_plaf_metal_MetalUtils__

#pragma interface

#include <java/lang/Object.h>
#include <gcj/array.h>

extern "Java"
{
  namespace java
  {
    namespace awt
    {
        class Color;
        class Component;
        class Graphics;
        class Graphics2D;
      namespace image
      {
          class BufferedImage;
      }
    }
  }
  namespace javax
  {
    namespace swing
    {
      namespace plaf
      {
        namespace metal
        {
            class MetalUtils;
        }
      }
    }
  }
}

class javax::swing::plaf::metal::MetalUtils : public ::java::lang::Object
{

public: // actually package-private
  MetalUtils();
  static void fillMetalPattern(::java::awt::Component *, ::java::awt::Graphics *, jint, jint, jint, jint, ::java::awt::Color *, ::java::awt::Color *);
  static void fillMetalPattern2D(::java::awt::Graphics2D *, jint, jint, jint, jint, ::java::awt::Color *, ::java::awt::Color *);
  static void initializePattern(::java::awt::Color *, ::java::awt::Color *);
  static void paintGradient(::java::awt::Graphics *, jint, jint, jint, jint, jint, ::java::lang::String *);
  static void paintGradient(::java::awt::Graphics *, jint, jint, jint, jint, jint, ::java::lang::String *, JArray< JArray< jint > * > *);
  static void paintGradient(::java::awt::Graphics *, jint, jint, jint, jint, jfloat, jfloat, ::java::awt::Color *, ::java::awt::Color *, ::java::awt::Color *, jint, JArray< JArray< jint > * > *);
  static void paintHorizontalGradient(::java::awt::Graphics *, jint, jint, jint, jint, jfloat, jfloat, ::java::awt::Color *, ::java::awt::Color *, ::java::awt::Color *, JArray< JArray< jint > * > *);
  static void paintVerticalGradient(::java::awt::Graphics *, jint, jint, jint, jint, jfloat, jfloat, ::java::awt::Color *, ::java::awt::Color *, ::java::awt::Color *, JArray< JArray< jint > * > *);
private:
  static void paintHorizontalGradient2D(::java::awt::Graphics2D *, jint, jint, jint, jint, jfloat, jfloat, ::java::awt::Color *, ::java::awt::Color *, ::java::awt::Color *, JArray< JArray< jint > * > *);
  static void paintVerticalGradient2D(::java::awt::Graphics2D *, jint, jint, jint, jint, jfloat, jfloat, ::java::awt::Color *, ::java::awt::Color *, ::java::awt::Color *, JArray< JArray< jint > * > *);
public: // actually package-private
  static ::java::awt::image::BufferedImage * pattern2D;
  static ::java::awt::Color * lightColor;
  static ::java::awt::Color * darkColor;
public:
  static ::java::lang::Class class$;
};

#endif // __javax_swing_plaf_metal_MetalUtils__
