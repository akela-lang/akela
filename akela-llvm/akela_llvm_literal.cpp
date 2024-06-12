#include "akela_llvm_tools.h"

using namespace llvm;
using namespace llvm::orc;

Value* CodeGenLLVMNumber(JITData* jd, struct Ast_node* n)
{
    struct Ast_node* tu = n->tu;
    struct type_def *td = tu->td;
    if (td->type == type_integer) {
        Type* t = CodeGenLLVMGetType(jd, n->tu);
        buffer_finish(&n->value);
        long v = strtol(n->value.buf, nullptr, 10);
        return ConstantInt::get(t, APInt(td->bit_count, v, n->tu->td->is_signed));
    } else if (td->type == type_float) {
        buffer_finish(&n->value);
        double v = strtod(n->value.buf, nullptr);
        return ConstantFP::get(*jd->TheContext, APFloat(v));
    }
    assert(false);
}

Value* CodeGenLLVMBoolean(JITData* jd, struct Ast_node* n)
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

Value* CodeGenLLVMString(JITData* jd, struct Ast_node* n)
{
    buffer_finish(&n->value);
    return jd->Builder->CreateGlobalString(n->value.buf, ".str");
}