#include "V8Matrix.h"
#include "../core/Matrix.h"
#include "../../third_party/v8/include/v8.h"

namespace {

#define GETTER_ARGUMENT v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info

#define GETTER_CONTENT(className, name) \
    auto self = info.Holder(); \
    auto ptr = self->GetAlignedPointerFromInternalField(0); \
    auto value = static_cast<className*>(ptr)->name; \
    info.GetReturnValue().Set(value);

#define SETTER_ARGUMENT v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info

#define SETTER_CONTENT(className, name) \
    auto self = info.Holder(); \
    auto ptr = self->GetAlignedPointerFromInternalField(0); \
    static_cast<className*>(ptr)->name = (float) value->NumberValue();

#define SET_ACCESSOR(name, getter, setter) self->SetAccessor(v8::String::NewFromUtf8(isolate, name), getter, setter);

#define SET_METHOD(name, method) prototypeTemplate->Set(v8::String::NewFromUtf8(isolate, name), \
                                                            v8::FunctionTemplate::New(isolate, method));

    void aGetter(GETTER_ARGUMENT) { GETTER_CONTENT(Matrix, a); }

    void aSetter(SETTER_ARGUMENT) { SETTER_CONTENT(Matrix, a); }

    void bGetter(GETTER_ARGUMENT) { GETTER_CONTENT(Matrix, b); }

    void bSetter(SETTER_ARGUMENT) { SETTER_CONTENT(Matrix, b); }

    void cGetter(GETTER_ARGUMENT) { GETTER_CONTENT(Matrix, c); }

    void cSetter(SETTER_ARGUMENT) { SETTER_CONTENT(Matrix, c); }

    void dGetter(GETTER_ARGUMENT) { GETTER_CONTENT(Matrix, d); }

    void dSetter(SETTER_ARGUMENT) { SETTER_CONTENT(Matrix, d); }

    void txGetter(GETTER_ARGUMENT) { GETTER_CONTENT(Matrix, tx); }

    void txSetter(SETTER_ARGUMENT) { SETTER_CONTENT(Matrix, tx); }

    void tyGetter(GETTER_ARGUMENT) { GETTER_CONTENT(Matrix, ty); }

    void tySetter(SETTER_ARGUMENT) { SETTER_CONTENT(Matrix, ty); }

    void setToMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        float a = float(args[0]->IsUndefined() ? 1 : args[0]->NumberValue());
        float b = float(args[1]->IsUndefined() ? 0 : args[1]->NumberValue());
        float c = float(args[2]->IsUndefined() ? 0 : args[2]->NumberValue());
        float d = float(args[3]->IsUndefined() ? 1 : args[3]->NumberValue());
        float tx = float(args[4]->IsUndefined() ? 0 : args[4]->NumberValue());
        float ty = float(args[5]->IsUndefined() ? 0 : args[5]->NumberValue());
        auto self = args.Holder();
        auto ptr = self->GetAlignedPointerFromInternalField(0);
        static_cast<Matrix*>(ptr)->setTo(a, b, c, d, tx, ty);
    }

    void transformBoundsMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto obj = v8::Local<v8::Object>::Cast(args[0]);
        auto rect = static_cast<Rectangle*>(obj->GetAlignedPointerFromInternalField(0));
        auto self = args.Holder();
        auto matrix = static_cast<Matrix*>(self->GetAlignedPointerFromInternalField(0));
        matrix->transformBounds(rect);
    }

    void toStringMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto self = args.Holder();
        auto matrix = static_cast<Matrix*>(self->GetAlignedPointerFromInternalField(0));
        auto value = matrix->toString();
        auto utf = v8::String::NewFromUtf8(args.GetIsolate(),value.c_str());
        args.GetReturnValue().Set(utf);
    }

    void constructor(const v8::FunctionCallbackInfo<v8::Value>& args) {
        float a = float(args[0]->IsUndefined() ? 1 : args[0]->NumberValue());
        float b = float(args[1]->IsUndefined() ? 0 : args[1]->NumberValue());
        float c = float(args[2]->IsUndefined() ? 0 : args[2]->NumberValue());
        float d = float(args[3]->IsUndefined() ? 1 : args[3]->NumberValue());
        float tx = float(args[4]->IsUndefined() ? 0 : args[4]->NumberValue());
        float ty = float(args[5]->IsUndefined() ? 0 : args[5]->NumberValue());
        Matrix* matrix = new Matrix(a, b, c, d, tx, ty);
        auto self = args.Holder();
        self->SetAlignedPointerInInternalField(0,matrix);
        auto isolate = args.GetIsolate();
        SET_ACCESSOR("a", aGetter, aSetter);
        SET_ACCESSOR("b", bGetter, bSetter);
        SET_ACCESSOR("c", cGetter, cSetter);
        SET_ACCESSOR("d", dGetter, dSetter);
        SET_ACCESSOR("tx", txGetter, txSetter);
        SET_ACCESSOR("ty", tyGetter, tySetter);
        args.GetReturnValue().Set(self);
    }


}

void V8Matrix::install(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> parent) {
    auto classTemplate = v8::FunctionTemplate::New(isolate, constructor);
    classTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Matrix"));
    auto prototypeTemplate = classTemplate->PrototypeTemplate();
    SET_METHOD("setTo", setToMethod);
    SET_METHOD("transformBounds", transformBoundsMethod);
    SET_METHOD("toString", toStringMethod);
    classTemplate->InstanceTemplate()->SetInternalFieldCount(1);

    parent->Set(v8::String::NewFromUtf8(isolate, "Matrix"), classTemplate);
}
