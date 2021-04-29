#include "financial.h"

#include <assert.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

template <int numArgs>
ArgValidator::Status ArgValidator::ArgLength(const Napi::CallbackInfo &info,
                                             Napi::Env env) {
  if (info.Length() < numArgs) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return ArgValidator::Status::JAVASCRIPT_EXCEPTION;
  }
  return ArgValidator::Status::SUCCESS;
}

Financialcpp::Financialcpp(const Napi::CallbackInfo &info) : ObjectWrap(info) {
  Napi::Env env = info.Env();

  // if (ArgValidator::ArgLength<2>(info, env) ==
  // ArgValidator::Status::JAVASCRIPT_EXCEPTION)
  // {
  //     return;
  // }

  // if (info.Length() >= 1) {

  // }

  Napi::Object args = info[0].As<Napi::Object>();

  fpp = fpp::makeFinancialcpp();
  
//   fpp.start();
}

Napi::Value Financialcpp::download(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  auto deferred = Napi::Promise::Deferred::New(env);

  deferred.Resolve(Napi::Object::New(env));

  return deferred.Promise();
}

Napi::Value Financialcpp::tick(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Napi::Object args = info[0].As<Napi::Object>();

  std::string tickerSymbol = args.Get("symbol").As<Napi::String>();
  std::string dateRange = args.Get("dateRange").As<Napi::String>();
  std::string provider = args.Get("provider").As<Napi::String>();

  // submitting job to fpp engine
//   fpp.submitTick(tickerSymbol, dateRange, provider);
  size_t i = 0;
  Napi::Array timestamps = Napi::Array::New(env, 1);
  timestamps.Set(i, Napi::Number::New(env, 120000));

  Napi::Array ticks = Napi::Array::New(env, 1);
  Napi::Object tick = Napi::Object::New(env);
  tick.Set("open", Napi::Number::New(env, 2.32));
  tick.Set("volume", Napi::Number::New(env, 10000));
  ticks.Set(i, tick);

  Napi::Object obj = Napi::Object::New(env);
  obj.Set("timestamps", timestamps);
  obj.Set("ohlcv", ticks);

  auto deferred = Napi::Promise::Deferred::New(env);
  deferred.Resolve(obj);
  return deferred.Promise();
}

Napi::Value Financialcpp::submitBacktest(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Object args = info[0].As<Napi::Object>();

  std::string symbols = args.Get("symbols").As<Napi::String>();
  int fromDate = args.Get("fromDate").As<Napi::Number>();
  int toDate = args.Get("toDate").As<Napi::Number>();
  std::string strategy = args.Get("strategy").As<Napi::String>();
  std::string provider = args.Get("provider").As<Napi::String>();

  // submitting job to fpp engine
  auto backtest = fpp.submitBacktest(symbols, fromDate, toDate, strategy, provider);

    // don't care about errors yet
    if (!backtest.id) {
    }

  Napi::Array timestamps = Napi::Array::New(env, 1);



  Napi::Object backtestJS = Napi::Object::New(env);
  backtestJS.Set("id", Napi::Number::New(env, backtest.id));
  backtestJS.Set("start", Napi::Number::New(env, 0));
  backtestJS.Set("progress", Napi::Number::New(env, 0));
    

  Napi::Object resultJS = Napi::Object::New(env);
  resultJS.Set("success", Napi::Boolean::New(env, true));
  resultJS.Set("message", Napi::String::New(env, "Backtest has been submitted"));
  resultJS.Set("backtest", backtestJS);

  return resultJS;
}

Napi::Value Financialcpp::tickerSymbols(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

//   const std::vector<std::string> &symbols = fpp.getTickerSymbols();

//   Napi::Array tickerSymbols = Napi::Array::New(info.Env(), symbols.size());
//   for (size_t idx = 0; idx < symbols.size(); idx++) {
//     // The HandleScope is recommended especially when the loop has many
//     // iterations.

//     std::cout << symbols[idx] << std::endl;

//     Napi::HandleScope scope(env);
//     Napi::Object obj = Napi::Object::New(env);
//     obj.Set("name", Napi::String::New(env, symbols[idx]));
//     tickerSymbols[idx] = obj;
//   }
//   return tickerSymbols;

return Napi::Array::New(info.Env(), 0);

}

constexpr size_t ARRAY_LENGTH = 10;

// // Data structure representing our thread-safe function context.
// struct TsfnContext {

//   TsfnContext(Napi::Env env) : deferred(Napi::Promise::Deferred::New(env)) {
//     for (size_t i = 0; i < ARRAY_LENGTH; ++i)
//       ints[i] = i;
//   };

//   // Native Promise returned to JavaScript
//   Napi::Promise::Deferred deferred;

//   // Native thread
//   std::thread nativeThread;

//   // Some data to pass around
//   int ints[ARRAY_LENGTH];
// };

// // The thread entry point. This takes as its arguments the specific
// // threadsafe-function context created inside the main thread.
// void threadEntry(TsfnContext *context);

// // The thread-safe function finalizer callback. This callback executes
// // at destruction of thread-safe function, taking as arguments the finalizer
// // data and threadsafe-function context.
// void FinalizerCallback(Napi::Env env, void *finalizeData, TsfnContext
// *context);

// // Exported JavaScript function. Creates the thread-safe function and native
// // thread. Promise is resolved in the thread-safe function's finalizer.
// Napi::Value Financialcpp::CreateTSFN(const Napi::CallbackInfo &info) {
//   Napi::Env env = info.Env();

//   // Construct context data
//   auto testData = new TsfnContext(env);

//   // Create a new ThreadSafeFunction.
//   testData->tsfn = Napi::ThreadSafeFunction::New(
//       env,                          // Environment
//       info[0].As<Napi::Function>(), // JS function from caller
//       "TSFN",                       // Resource name
//       0,                            // Max queue size (0 = unlimited).
//       1,                            // Initial thread count
//       testData,                     // Context,
//       FinalizerCallback,            // Finalizer
//       (void *)nullptr               // Finalizer data
//   );
//   testData->nativeThread = std::thread(threadEntry, testData);

//   // Return the deferred's Promise. This Promise is resolved in the
//   thread-safe
//   // function's finalizer callback.
//   return testData->deferred.Promise();
// }

// // The thread entry point. This takes as its arguments the specific
// // threadsafe-function context created inside the main thread.
// void threadEntry(TsfnContext *context) {

//   // This callback transforms the native addon data (int *data) to JavaScript
//   // values. It also receives the treadsafe-function's registered callback,
//   and
//   // may choose to call it.
//   auto callback = [](Napi::Env env, Napi::Function jsCallback, int *data) {
//     jsCallback.Call({Napi::Number::New(env, *data),
//                      Napi::Number::New(env, *data),
//                      Napi::Number::New(env, *data)});
//   };

//   for (size_t index = 0; index < ARRAY_LENGTH; ++index) {
//     // Perform a call into JavaScript.
//     napi_status status =
//         context->tsfn.BlockingCall(&context->ints[index], callback);

//     if (status != napi_ok) {
//       Napi::Error::Fatal(
//           "ThreadEntry",
//           "Napi::ThreadSafeNapi::Function.BlockingCall() failed");
//     }
//     // Sleep for some time.
//     std::this_thread::sleep_for(std::chrono::milliseconds(200));
//   }

//   // Release the thread-safe function. This decrements the internal thread
//   // count, and will perform finalization since the count will reach 0.
//   context->tsfn.Release();
// }

// void FinalizerCallback(Napi::Env env, void *finalizeData,
//                        TsfnContext *context) {
//   // Join the thread
//   context->nativeThread.join();

//   // Resolve the Promise previously returned to JS via the CreateTSFN method.
//   context->deferred.Resolve(Napi::Boolean::New(env, true));
//   delete context;
// }

Napi::Function Financialcpp::GetClass(Napi::Env env) {
  return DefineClass(
      env, "Financialcpp",
      {
          Financialcpp::InstanceMethod("download", &Financialcpp::download),
          Financialcpp::InstanceMethod("tick", &Financialcpp::tick),
          //   Financialcpp::InstanceMethod("createTSFN",
          //   &Financialcpp::CreateTSFN),
          Financialcpp::InstanceMethod("tickerSymbols",
                                       &Financialcpp::tickerSymbols),
          Financialcpp::InstanceMethod("submitBacktest",
                                       &Financialcpp::submitBacktest),
      });
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  Napi::String name = Napi::String::New(env, "Financialcpp");
  exports.Set(name, Financialcpp::GetClass(env));
  return exports;
}

NODE_API_MODULE(addon, Init)