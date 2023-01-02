#ifndef YARU_FLUTTER_H_
#define YARU_FLUTTER_H_

#include <windows.h>
//
#include <flutter/encodable_value.h>
#include <flutter/event_channel.h>
#include <flutter/event_sink.h>
#include <flutter/event_stream_handler.h>
#include <flutter/flutter_view.h>
#include <flutter/method_call.h>
#include <flutter/method_channel.h>
#include <flutter/method_result.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

typedef flutter::EncodableValue FlValue;
typedef flutter::EventChannel<FlValue> FlEventChannel;
typedef flutter::EventSink<FlValue> FlEventSink;
typedef flutter::MethodCall<FlValue> FlMethodCall;
typedef flutter::MethodResult<FlValue> FlMethodResult;
typedef flutter::MethodChannel<FlValue> FlMethodChannel;
typedef flutter::Plugin FlPlugin;
typedef flutter::PluginRegistrarManager FlPluginRegistrarManager;
typedef flutter::PluginRegistrarWindows FlPluginRegistrar;
typedef flutter::StandardMethodCodec FlStandardMethodCodec;
typedef flutter::StreamHandler<FlValue> FlStreamHandler;
typedef flutter::StreamHandlerError<FlValue> FlStreamHandlerError;
typedef flutter::FlutterView FlView;

#endif  // YARU_FLUTTER_H_
