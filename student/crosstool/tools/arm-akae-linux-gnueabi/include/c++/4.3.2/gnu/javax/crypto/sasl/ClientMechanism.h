
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __gnu_javax_crypto_sasl_ClientMechanism__
#define __gnu_javax_crypto_sasl_ClientMechanism__

#pragma interface

#include <java/lang/Object.h>
#include <gcj/array.h>

extern "Java"
{
  namespace gnu
  {
    namespace javax
    {
      namespace crypto
      {
        namespace sasl
        {
            class ClientMechanism;
        }
      }
    }
  }
  namespace javax
  {
    namespace security
    {
      namespace auth
      {
        namespace callback
        {
            class CallbackHandler;
        }
      }
    }
  }
}

class gnu::javax::crypto::sasl::ClientMechanism : public ::java::lang::Object
{

public: // actually protected
  ClientMechanism(::java::lang::String *);
  virtual void initMechanism() = 0;
  virtual void resetMechanism() = 0;
public:
  virtual JArray< jbyte > * evaluateChallenge(JArray< jbyte > *) = 0;
  virtual jboolean hasInitialResponse() = 0;
  virtual jboolean isComplete();
  virtual JArray< jbyte > * unwrap(JArray< jbyte > *, jint, jint);
  virtual JArray< jbyte > * wrap(JArray< jbyte > *, jint, jint);
  virtual ::java::lang::String * getMechanismName();
  virtual ::java::lang::Object * getNegotiatedProperty(::java::lang::String *);
  virtual void dispose();
  virtual ::java::lang::String * getAuthorizationID();
public: // actually protected
  virtual ::java::lang::String * getNegotiatedQOP();
  virtual ::java::lang::String * getNegotiatedStrength();
  virtual ::java::lang::String * getNegotiatedServerAuth();
  virtual ::java::lang::String * getNegotiatedMaxBuffer();
  virtual ::java::lang::String * getNegotiatedRawSendSize();
  virtual ::java::lang::String * getNegotiatedPolicyNoPlainText();
  virtual ::java::lang::String * getNegotiatedPolicyNoActive();
  virtual ::java::lang::String * getNegotiatedPolicyNoDictionary();
  virtual ::java::lang::String * getNegotiatedPolicyNoAnonymous();
  virtual ::java::lang::String * getNegotiatedPolicyForwardSecrecy();
  virtual ::java::lang::String * getNegotiatedPolicyPassCredentials();
  virtual ::java::lang::String * getReuse();
  virtual JArray< jbyte > * engineUnwrap(JArray< jbyte > *, jint, jint);
  virtual JArray< jbyte > * engineWrap(JArray< jbyte > *, jint, jint);
public:
  virtual void init(::java::util::Map *);
  virtual void reset();
public: // actually protected
  ::java::lang::String * __attribute__((aligned(__alignof__( ::java::lang::Object)))) mechanism;
  ::java::lang::String * authorizationID;
  ::java::lang::String * protocol;
  ::java::lang::String * serverName;
  ::java::util::Map * properties;
  ::javax::security::auth::callback::CallbackHandler * handler;
  JArray< jbyte > * channelBinding;
  jboolean complete;
  jint state;
public:
  static ::java::lang::Class class$;
};

#endif // __gnu_javax_crypto_sasl_ClientMechanism__
