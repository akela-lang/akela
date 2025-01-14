#include "akela_llvm_tools.h"

using namespace llvm;
using namespace llvm::orc;

namespace Akela_llvm {
    Value* Handle_number(Jit_data* jd, Ake_ast* n)
    {
        Type_use* tu = n->tu;
        struct Ake_type_def *td = tu->td;
        if (td->type == Ake_type_integer) {
            Type* t = Get_type(jd, n->tu);
            buffer_finish(&n->value);
            long v = strtol(n->value.buf, nullptr, 10);
            return ConstantInt::get(t, APInt(td->bit_count, v, n->tu->td->is_signed));
        } else if (td->type == Ake_type_float) {
            buffer_finish(&n->value);
            double v = strtod(n->value.buf, nullptr);
            return ConstantFP::get(*jd->TheContext, APFloat(v));
        }
        assert(false);
    }

    Value* Handle_boolean(Jit_data* jd, Ake_ast* n)
    {
        if (buffer_compare_str(&n->value, "true")) {
            Type* t = Type::getInt1Ty(*jd->TheContext);
            return ConstantInt::get(t, APInt(1, 1, true));
        } else if (buffer_compare_str(&n->value, "false")) {
            Type* t = Type::getInt1Ty(*jd->TheContext);
            return ConstantInt::get(t, APInt(1, 0, true));
        }
        assert(false && "invalid boolean identifier");
    }

    Value* Handle_string(Jit_data* jd, Ake_ast* n)
    {
        buffer_finish(&n->value);
        Value* str_value = jd->Builder->CreateGlobalString(n->value.buf, ".str");
        return str_value;
    }
}
