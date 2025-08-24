#include "zinc/os.h"
#include "akela/parse_types.h"
#include "tools.h"
#include "function.h"
#include "literal.h"
#include "variable.h"
#include "stmts.h"
#include "operator.h"
#include "struct.h"
#include <stdfloat>
#include <float.h>
#include <cstdint>


using namespace llvm;
using namespace llvm::orc;

namespace Akela_llvm {
    void Jit_data_init(Jit_data *jd, Zinc_error_list *el) {
        jd->el = el;
        jd->TheJIT = jd->ExitOnErr(KaleidoscopeJIT::Create());
        jd->TheContext = std::make_unique<LLVMContext>();
        jd->TheModule = std::make_unique<Module>(MODULE_NAME, *jd->TheContext);
        jd->TheModule->setDataLayout(jd->TheJIT->getDataLayout());
        jd->Builder = std::make_unique<IRBuilder<>>(*jd->TheContext);
        jd->toplevel = nullptr;
        jd->abort_function = nullptr;
        jd->printf_function = nullptr;
        jd->exit_function = nullptr;
        jd->current_function = std::vector<Function*>();
        jd->need_exit = false;
        jd->need_abort = false;
        jd->need_printf = false;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    FunctionType *Get_function_type(Jit_data *jd, Ake_Type *type) {
        bool is_variadic = false;
        std::vector<Type *> param_types = std::vector<Type *>();
        Ake_TypeParam* tp = type->data.function.input_head;
        while (tp) {
            if (tp->kind == AKE_TYPE_PARAM_ELLIPSIS) {
                is_variadic = true;
            } else {
                Type *dec_type = Get_type_pointer(jd, tp->type);
                param_types.push_back(dec_type);
            }
            tp = tp->next;
        }

        Type *ret_type;
        if (type->data.function.output) {
            ret_type = Get_type_pointer(jd, type->data.function.output);
        } else {
            ret_type = Type::getVoidTy(*jd->TheContext);
        }

        return FunctionType::get(ret_type, param_types, is_variadic);
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Type *Get_scalar_type(Jit_data *jd, Ake_Type *type) {
        if (!type) {
            return Type::getVoidTy(*jd->TheContext);
        }

        if (type->kind == AKE_TYPE_FUNCTION) {
            return Get_function_type(jd, type);
        }

        if (type->kind == AKE_TYPE_INTEGER) {
            Type *t;
            if (type->data.integer.bit_count == 64) {
                t = Type::getInt64Ty(*jd->TheContext);
            } else if (type->data.integer.bit_count == 32) {
                t = Type::getInt32Ty(*jd->TheContext);
            } else if (type->data.integer.bit_count == 16) {
                t = Type::getInt16Ty(*jd->TheContext);
            } else if (type->data.integer.bit_count == 8) {
                t = Type::getInt8Ty(*jd->TheContext);
            } else {
                assert(false && "unsupported bit count");
            }
            return t;
        }

        if (type->kind == AKE_TYPE_NATURAL) {
            Type *t;
            if (type->data.natural.bit_count == 64) {
                t = Type::getInt64Ty(*jd->TheContext);
            } else if (type->data.natural.bit_count == 32) {
                t = Type::getInt32Ty(*jd->TheContext);
            } else if (type->data.natural.bit_count == 16) {
                t = Type::getInt16Ty(*jd->TheContext);
            } else if (type->data.natural.bit_count == 8) {
                t = Type::getInt8Ty(*jd->TheContext);
            } else {
                assert(false && "unsupported bit count");
            }
            return t;
        }

        if (type->kind == AKE_TYPE_REAL) {
            if (type->data.real.bit_count == 64) {
                return Type::getDoubleTy(*jd->TheContext);
            } else if (type->data.real.bit_count == 32) {
                return Type::getFloatTy(*jd->TheContext);
            } else if (type->data.real.bit_count == 16) {
                return Type::getHalfTy(*jd->TheContext);
            } else {
                assert(false && "unsupported bit count");
            }
        }

        if (type->kind == AKE_TYPE_BOOLEAN) {
            return Type::getInt1Ty(*jd->TheContext);
        }

        if (type->kind == AKE_TYPE_STRUCT) {
            return GetStructTypeFromType(jd, type);
        }

        assert(false);
        return nullptr;
    }

    bool NeedPointer(Ake_TypeKind kind)
    {
        if (kind == AKE_TYPE_ARRAY) {
            return true;
        }

        if (kind == AKE_TYPE_FUNCTION) {
            return true;
        }

        if (kind == AKE_TYPE_STRUCT) {
            return true;
        }

        return false;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Type* Get_type_pointer(Jit_data *jd, Ake_Type *type)
    {
        Type* t = Get_type(jd, type);
        if (type) {
            if (NeedPointer(type->kind)) {
                t = PointerType::get(*jd->TheContext, 0);
            }
        }
        return t;
    }

    bool IsArray(Ake_TypeKind kind)
    {
        if (kind == AKE_TYPE_ARRAY) {
            return true;
        }

        return false;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Type* Get_type(Jit_data* jd, Ake_Type* type)
    {
        Type *t;
        if (type && IsArray(type->kind)) {
            size_t dim = 0;
            if (type->kind == AKE_TYPE_ARRAY) {
                dim = type->data.array.dim;
                Ake_Type* type2 = type->data.array.type;
                t = Get_type(jd, type2);
                if (type2->kind == AKE_TYPE_FUNCTION) {
                    t = PointerType::get(*jd->TheContext, 0);
                }
                t = ArrayType::get(t, dim);
            } else {
                assert(false && "unsupported array type");
            }
        } else {
            return Get_scalar_type(jd, type);
        }

        return t;
    }

    BasicBlock* Get_last_block(Jit_data* jd, Function* f)
    {
        BasicBlock* last_block = nullptr;
        Function::iterator blocks = f->end();
        if (blocks != f->begin()) {
            last_block = &*--blocks;
        }
        return last_block;
    }

    void Run(Jit_data* jd, Ake_Ast* n, Ake_code_gen_result* result)
    {
        Zinc_string* value = &result->value;
        auto ExprSymbol = jd->ExitOnErr(jd->TheJIT->lookup(TOP_LEVEL_NAME));
        if (n->type) {
            Ake_TypeKind kind = n->type->kind;

            if (IsArray(kind)) {
                if (n->type->data.array.is_const) {
                    Ake_Type* type = n->type->data.array.type;
                    if (type->kind == AKE_TYPE_NATURAL && type->data.natural.bit_count == 8) {
                        char* (*fp)() = ExprSymbol.getAddress().toPtr<char*(*)()>();
                        char* v = fp();
                        Zinc_string_add_str(&result->value, v);
                        if (result->return_address) {
                            assert(result->return_size == 4);
                            *(char**)result->return_address = v;
                        }
                    } else {
                        assert(false && "return of arrays not supported");
                    }
                } else {
                    assert(false && "return of arrays not supported");
                }
            } else if (kind == AKE_TYPE_INTEGER) {
                size_t bit_count = n->type->data.integer.bit_count;
                if (bit_count == 64) {
                    int64_t (*fp)() = ExprSymbol.getAddress().toPtr<int64_t(*)()>();
                    int64_t v = fp();
                    Zinc_string_add_format(value, "%ld", v);
                    if (result->return_address) {
                        *(int64_t*)result->return_address = v;
                    }
                } else if (bit_count == 32) {
                    int32_t (*fp)() = ExprSymbol.getAddress().toPtr<int32_t(*)()>();
                    int32_t v = fp();
                    Zinc_string_add_format(value, "%d", v);
                    if (result->return_address) {
                        assert(result->return_size >= 4);
                        *(int32_t*)result->return_address = v;
                    }
                } else if (bit_count == 16) {
                    int16_t (*fp)() = ExprSymbol.getAddress().toPtr<int16_t(*)()>();
                    int16_t v = fp();
                    Zinc_string_add_format(value, "%hd", v);
                    if (result->return_address) {
                        *(int16_t*)result->return_address = v;
                    }
                } else if (bit_count == 8) {
                    int8_t (*fp)() = ExprSymbol.getAddress().toPtr<int8_t(*)()> ();
                    int8_t v = fp();
                    Zinc_string_add_format(value, "%hhd", v);
                    if (result->return_address) {
                        *(int8_t*)result->return_address = v;
                    }
                } else {
                    assert(false);
                }
            } else if (kind == AKE_TYPE_NATURAL) {
                size_t bit_count = n->type->data.natural.bit_count;
                if (bit_count == 64) {
                    uint64_t (*fp)() = ExprSymbol.getAddress().toPtr<uint64_t(*)()>();
                    uint64_t v = fp();
                    Zinc_string_add_format(value, "%lu", v);
                    if (result->return_address) {
                        *(uint64_t*)result->return_address = v;
                    }
                } else if (bit_count == 32) {
                    uint32_t (*fp)() = ExprSymbol.getAddress().toPtr<uint32_t(*)()>();
                    uint32_t v = fp();
                    Zinc_string_add_format(value, "%u", v);
                    if (result->return_address) {
                        *(uint32_t*)result->return_address = v;
                    }
                } else if (bit_count == 16) {
                    uint16_t (*fp)() = ExprSymbol.getAddress().toPtr<uint16_t(*)()>();
                    uint16_t v = fp();
                    Zinc_string_add_format(value, "%hu", v);
                    if (result->return_address) {
                        *(uint16_t*)result->return_address = v;
                    }
                } else if (bit_count == 8) {
                    uint8_t (*fp)() = ExprSymbol.getAddress().toPtr<uint8_t(*)()>();
                    uint8_t v = fp();
                    Zinc_string_add_format(value, "%hhu", v);
                    if (result->return_address) {
                        *(uint8_t*)result->return_address = v;
                    }
                } else {
                    assert(false);
                }
            } else if (kind == AKE_TYPE_REAL) {
                size_t bit_count = n->type->data.real.bit_count;
                if (bit_count == 64) {
                    double (*fp)() = ExprSymbol.getAddress().toPtr <double(*)()>();
                    double v = fp();
                    Zinc_string_add_format(value, "%lf", v);
                    if (result->return_address) {
                        *(double*)result->return_address = v;
                    }
                } else if (bit_count == 32) {
                    float (*fp)() = ExprSymbol.getAddress().toPtr <float(*)()>();
                    float v = fp();
                    Zinc_string_add_format(value, "%f", v);
                    if (result->return_address) {
                        *(float*)result->return_address = v;
                    }
                } else if (bit_count == 16) {
#if IS_UNIX
                    _Float16 (*fp)() = ExprSymbol.getAddress().toPtr <_Float16(*)()>();
                    _Float16 v = fp();
                    Zinc_string_add_format(value, "%f", (float)v);
                    if (result->return_address) {
                        *(_Float16*)result->return_address = v;
                    }
#elif IS_WIN
					assert(false && "16-bit float not supported");
#endif
                } else {
                    assert(false);
                }
            } else if (kind == AKE_TYPE_BOOLEAN) {
                bool (*fp)() = ExprSymbol.getAddress().toPtr <bool(*)()>();
                bool v = fp();
                if (v) {
                    Zinc_string_add_format(value, "true", v);
                } else {
                    Zinc_string_add_format(value, "false", v);
                }
                if (result->return_address) {
                    *(bool*)result->return_address = v;
                }
            } else if (kind == AKE_TYPE_FUNCTION) {
                void* (*fp)() = ExprSymbol.getAddress().toPtr<void*(*)()>();
                void* v = fp();
                Zinc_string_add_format(value, "Function");
            } else {
                Zinc_location loc{};
                Zinc_location_init(&loc);
                Zinc_error_list_set(jd->el, &loc, "type not handled");
            }
        } else {
            void (*fp)() = ExprSymbol.getAddress().toPtr <void(*)()>();
            fp();
        }
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Dispatch(Jit_data* jd, Ake_Ast* n)
    {
        if (n->kind == Ake_ast_type_stmts) {
            return Handle_stmts(jd, n);
        }

        if (n->kind == Ake_ast_type_extern) {
            return Handle_extern(jd, n);
        }

        if (n->kind == Ake_ast_type_if) {
            return Handle_if(jd, n);
        }

        if (n->kind == Ake_ast_type_const || n->kind == Ake_ast_type_var) {
            return Handle_variable_dec(jd, n);
        }

        if (n->kind == Ake_ast_type_function) {
            return Handle_function(jd, n);
        }

        if (n->kind == Ake_ast_type_assign) {
            return Handle_assign(jd, n);
        }

        if (n->kind == Ake_ast_type_mult) {
            return Handle_mult(jd, n);
        }

        if (n->kind == Ake_ast_type_plus) {
            return Handle_add(jd, n);
        }

        if (n->kind == Ake_ast_type_minus) {
            return Handle_sub(jd, n);
        }

        if (n->kind == Ake_ast_type_call) {
            return Handle_call(jd, n);
        }

        if (n->kind == Ake_ast_type_dot) {
            return Handle_dot(jd, n);
        }

        if (n->kind == AKE_AST_ID) {
            return Handle_identifier(jd, n);
        }

        if (n->kind == Ake_ast_type_struct) {
            return Handle_struct(jd, n);
        }

        if (n->kind == Ake_ast_type_struct_literal) {
            return Handle_struct_literal(jd, n);
        }

        if (n->kind == Ake_ast_type_array_literal) {
            return Handle_array_literal(jd, n);
        }

        if (n->kind == Ake_ast_type_array_subscript) {
            return Handle_subscript(jd, n);
        }

        if (n->kind == AKE_AST_SIGN) {
            return Handle_sign(jd, n);
        }

        if (n->kind == AKE_AST_NUMBER) {
            return Handle_number(jd, n);
        }

        if (n->kind == Ake_ast_type_boolean) {
            return Handle_boolean(jd, n);
        }

        if (n->kind == Ake_ast_type_string) {
            return Handle_string(jd, n);
        }

        if (n->kind == Ake_ast_type_and) {
            return Handle_and(jd, n);
        }

        printf("code gen: unhandled ast node type: %s\n", Ast_type_name(n->kind));
        exit(1);
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    void Array_copy(
            Jit_data* jd,
            Ake_Type* lhs_type,
            Ake_Type* rhs_type,
            Value* lhs_ptr,
            Value* rhs_ptr)
    {
        size_t size = 0;
        Ake_Type* lhs_type2 = NULL;
        Ake_Type* rhs_type2 = NULL;
        assert(lhs_type->kind == AKE_TYPE_ARRAY);
        assert(rhs_type->kind == AKE_TYPE_ARRAY);
        size = 0;
        if (lhs_type->data.array.dim < rhs_type->data.array.dim) {
            size = lhs_type->data.array.dim;
        } else {
            size = rhs_type->data.array.dim;
        }
        lhs_type2 = lhs_type->data.array.type;
        rhs_type2 = rhs_type->data.array.type;

        for (size_t i = 0; i < size; i++) {
            Type* t = Get_type(jd, lhs_type2);
            std::vector<Value*> list = std::vector<Value*>();
            Value* i_value = ConstantInt::get(*jd->TheContext, APInt(64, i, false));
            list.push_back(i_value);
            Value* lhs_ptr2 = jd->Builder->CreateInBoundsGEP(t, lhs_ptr, list, "lhstmp");
            Value* rhs_ptr2 = jd->Builder->CreateInBoundsGEP(t, rhs_ptr, list, "rhstmp");

            if (IsArray(lhs_type2->kind)) {
                Array_copy(jd, lhs_type2, rhs_type2, lhs_ptr2, rhs_ptr2);
            } else {
                Value* value = jd->Builder->CreateLoad(t, rhs_ptr2, "rhs");
                jd->Builder->CreateStore(value, lhs_ptr2);
            }
        }
    }
}