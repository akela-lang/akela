#include "tools.h"

using namespace llvm;
using namespace llvm::orc;

namespace Akela_llvm {
    Value* Handle_number(Jit_data* jd, Ake_ast* n)
    {
        Ake_type_use* tu = n->tu;
        Ake_type_def *td = tu->td;
        if (td->type == Ake_type_integer) {
            Type* t = Get_type(jd, n->tu);
            Zinc_string_finish(&n->value);
            long v = strtol(n->value.buf, nullptr, 10);
            return ConstantInt::get(t, APInt(td->bit_count, v, n->tu->td->is_signed));
        } else if (td->type == Ake_type_float) {
            Zinc_string_finish(&n->value);
            if (td->bit_count == 64) {
                double v = strtod(n->value.buf, nullptr);
                Type* t = Get_type(jd, n->tu);
                return ConstantFP::get(*jd->TheContext, APFloat(v));
            } else if (td->bit_count == 32) {
                float v = strtof(n->value.buf, nullptr);
                return ConstantFP::get(*jd->TheContext, APFloat(v));
            } else if (td->bit_count == 16) {
                _Float16 v = strtof(n->value.buf, nullptr);
                Type* t = Type::getHalfTy(*jd->TheContext);
                return ConstantFP::get(t, v);
            } else {
                assert(false && "unhandled floating point value");
            }
        }
        assert(false);
    }

    Value* Handle_boolean(Jit_data* jd, Ake_ast* n)
    {
        if (Zinc_string_compare_str(&n->value, "true")) {
            Type* t = Type::getInt1Ty(*jd->TheContext);
            return ConstantInt::get(t, APInt(1, 1, true));
        } else if (Zinc_string_compare_str(&n->value, "false")) {
            Type* t = Type::getInt1Ty(*jd->TheContext);
            return ConstantInt::get(t, APInt(1, 0, true));
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
