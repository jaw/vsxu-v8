/**
* @author Dinesh Manajipet, Vovoid Media Technologies - Copyright (C) 2012-2020
* @see The GNU Public License (GPL)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/


#ifndef VSX_SAMPLE_MODULE_H
#define VSX_SAMPLE_MODULE_H
#include <v8.h>

#include "vsx_math_3d.h"
#include "vsx_param.h"
#include "vsx_module.h"
#include "vsx_timer.h"

using namespace v8;

class vsx_sample_module : public vsx_module
{
  vsx_module_param_float* param1;
  vsx_module_param_float* param2;
  vsx_module_param_float* result;

  vsx_timer tt;
  HandleScope handle_scope;
  Handle<Script> script;
  Handle<String> source;
  //Persistent<Context> context;
  Handle<Context> context;
  Handle<Value> v8result;

public:

  void module_info(vsx_module_info* info)
  {
    info->identifier = "sample;module";
    info->description = "Adds two input floats. And Returns an output float.";
    info->in_param_spec ="param1:float,param2:float";;
    info->out_param_spec = "result:float";
    info->component_class = "parameters";
  }

  void declare_params(vsx_module_param_list& in_parameters, vsx_module_param_list& out_parameters)
  {
    param1 = (vsx_module_param_float*)in_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,"param1");
    param2 = (vsx_module_param_float*)in_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,"param2");

    result = (vsx_module_param_float*)out_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,"result");

    param1->set(0);
    param2->set(0);
    result->set(0);

    loading_done = true;
  }


  bool init()
  {
    // Create a new context.
    context = Context::New();


    // Enter the created context for compiling and
    // running the hello world script.
    Context::Scope context_scope(context);

    // Create a string containing the JavaScript source code.
    source = String::New(
      ""
      "foo = 1.0; "
      "function test_function()"
      "{ "
      "   foo+= 0.1;"
      "   return foo;"
      "}"
    );
    /*
      "  var match = 0;"
      "  if(arguments[0] == arguments[1])"
      "   {"
      "     match = 1;"
      "   }"
    */
    // Compile the source code.
    script = Script::Compile(source);

    // Run the script to get the result.
    v8result = script->Run();

    //context.Dispose();
    return true;
  }



  void run()
  {
    tt.start();
    Context::Scope context_scope(context);

    Handle<Object> global = context->Global();
    Handle<Value> value = global->Get(String::New("test_function"));
    Handle<Function> func = Handle<Function>::Cast(value);
    Handle<Value> args[2];
    Handle<Value> js_result;
    int final_result;

    args[0] = String::New("1");
    args[1] = String::New("1");

    js_result = func->Call(global, 2, args);
    String::AsciiValue ascii(js_result);

    Number* js_numresult = Number::Cast( *js_result );

    final_result = atoi(*ascii);

    //context.Dispose();

    float t = tt.dtime();
    printf("%d result %d run in %f seconds\n", this, final_result, t);
    printf("     %f\n", js_numresult->Value());

    //result->set(param1->get()+param2->get());
  }


};

#endif // VSX_SAMPLE_MODULE_H
