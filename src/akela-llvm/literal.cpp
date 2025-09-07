#include "tools.h"
#include "zinc/os.h"

using namespace llvm;
using namespace llvm::orc;

namespace Akela_llvm {
    Value* Handle_number(Jit_data* jd, Ake_Ast* n)
    {
        Ake_Type* type = n->type;
        if (type->kind == AKE_TYPE_INTEGER) {
            Type* t = Get_type(jd, n->type);
            Zinc_string_finish(&n->data.number.value);
            long v = strtol(n->data.number.value.buf, nullptr, 10);
            return ConstantInt::get(t, APInt(type->data.integer.bit_count, v, true));
        }

        if (type->kind == AKE_TYPE_NATURAL) {
            Type* t = Get_type(jd, n->type);
            Zinc_string_finish(&n->data.number.value);
            long v = strtol(n->data.number.value.buf, nullptr, 10);
            return ConstantInt::get(t, APInt(type->data.natural.bit_count, v, false));
        }

        if (type->kind == AKE_TYPE_REAL) {
            Zinc_string_finish(&n->data.number.value);
            if (type->data.real.bit_count == 64) {
                double v = strtod(n->data.number.value.buf, nullptr);
                Type* t = Get_type(jd, n->type);
                return ConstantFP::get(*jd->TheContext, APFloat(v));
            }

            if (type->data.real.bit_count == 32) {
                float v = strtof(n->data.number.value.buf, nullptr);
                return ConstantFP::get(*jd->TheContext, APFloat(v));
            }

            if (type->data.real.bit_count == 16) {
#if IS_UNIX
                _Float16 v = (_Float16)strtof(n->data.number.value.buf, nullptr);
                Type* t = Type::getHalfTy(*jd->TheContext);
                return ConstantFP::get(t, v);
#elif IS_WIN
				assert(false && "16-bit float not supported");
#endif
            }

            assert(false && "unhandled floating point value");
        }

        assert(false);
    }

    Value* Handle_boolean(Jit_data* jd, Ake_Ast* n)
    {
        if (Zinc_string_compare_str(&n->data.boolean.value, "true")) {
            Type* t = Type::getInt1Ty(*jd->TheContext);
            return ConstantInt::get(t, APInt(1, 1, false));
        } else if (Zinc_string_compare_str(&n->data.boolean.value, "false")) {
            Type* t = Type::getInt1Ty(*jd->TheContext);
            return ConstantInt::get(t, APInt(1, 0, false));
        }
        assert(false && "invalid boolean identifier");
    }

    Value* Handle_string(Jit_data* jd, Ake_Ast* n)
    {
        Zinc_string_finish(&n->data.string.value);
        Value* str_value = jd->Builder->CreateGlobalString(n->data.string.value.buf, ".str");
        return str_value;
    }
}
