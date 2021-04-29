#pragma once

#include <napi.h>

#include "../../lib/src/financial.hpp"

using Fpp = fpp::Financialcpp_t<>;

class ArgValidator {
public:
  enum class Status { SUCCESS, JAVASCRIPT_EXCEPTION };

  // ArgLength must be equal to numArgs
  template <int numArgs>
  static Status ArgLength(const Napi::CallbackInfo &, Napi::Env);
};

class Financialcpp : public Napi::ObjectWrap<Financialcpp> {
public:
  Financialcpp(const Napi::CallbackInfo &);

  Napi::Value download(const Napi::CallbackInfo &);
  Napi::Value tick(const Napi::CallbackInfo &);
  Napi::Value tickerSymbols(const Napi::CallbackInfo &);
  Napi::Value submitBacktest(const Napi::CallbackInfo &);

  static Napi::Function GetClass(Napi::Env);

private:
   Fpp fpp;

  std::string _greeterName;
  int _my_int;
};