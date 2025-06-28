#include "tools.h"
#include "zinc/os.h"

using namespace llvm;
using namespace llvm::orc;

namespace Akela_llvm {
    Value* Handle_number(Jit_data* jd, Ake_ast* n)
    {
        Ake_TypeDef* tu = n->tu;
        if (tu->kind == AKE_TYPE_INTEGER) {
            Type* t = Get_type(jd, n->tu);
            Zinc_string_finish(&n->value);
            long v = strtol(n->value.buf, nullptr, 10);
            return ConstantInt::get(t, APInt(tu->data.integer.bit_count, v, true));
        }

        if (tu->kind == AKE_TYPE_NATURAL) {
            Type* t = Get_type(jd, n->tu);
            Zinc_string_finish(&n->value);
            long v = strtol(n->value.buf, nullptr, 10);
            return ConstantInt::get(t, APInt(tu->data.natural.bit_count, v, false));
        }

        if (tu->kind == AKE_TYPE_REAL) {
            Zinc_string_finish(&n->value);
            if (tu->data.real.bit_count == 64) {
                double v = strtod(n->value.buf, nullptr);
                Type* t = Get_type(jd, n->tu);
                return ConstantFP::get(*jd->TheContext, APFloat(v));
            }

            if (tu->data.real.bit_count == 32) {
                float v = strtof(n->value.buf, nullptr);
                return ConstantFP::get(*jd->TheContext, APFloat(v));
            }

            if (tu->data.real.bit_count == 16) {
#if IS_UNIX
                _Float16 v = (_Float16)strtof(n->value.buf, nullptr);
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

    Value* Handle_boolean(Jit_data* jd, Ake_ast* n)
    {
        if (Zinc_string_compare_str(&n->value, "true")) {
            Type* t = Type::getInt1Ty(*jd->TheContext);
            return ConstantInt::get(t, APInt(1, 1, false));
        } else if (Zinc_string_compare_str(&n->value, "false")) {
            Type* t = Type::getInt1Ty(*jd->TheContext);
            return ConstantInt::get(t, APInt(1, 0, false));
        }
        assert(false && "invalid boolean identifier");
    }

    Value* Handle_string(Jit_data* jd, Ake_ast* n)
    {
        Zinc_string_finish(&n->value);
        Value* str_value = jd->Builder->CreateGlobalString(n->value.buf, ".str");
        return str_value;
    }
}
