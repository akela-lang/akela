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
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    FunctionType *Get_function_type(Jit_data *jd, Ake_type_use *tu) {
        Ake_type_use *inputs = nullptr;
        Ake_type_use *outputs = nullptr;
        Ake_get_function_children(tu, &inputs, &outputs);
        bool is_variadic = false;

        std::vector<Type *> param_types = std::vector<Type *>();
        if (inputs) {
            Ake_type_use* p = inputs->head;
            while (p) {
                if (p->type == Ake_type_use_function_ellipsis) {
                    is_variadic = true;
                    p = p->next;
                    continue;
                }
                Type *dec_type = Get_type_pointer(jd, p);
                param_types.push_back(dec_type);
                p = p->next;
            }
        }

        Type *ret_type;
        if (outputs) {
            Ake_type_use *ret = outputs->head;
            ret_type = Get_type_pointer(jd, ret);
        } else {
            ret_type = Type::getVoidTy(*jd->TheContext);
        }

        return FunctionType::get(ret_type, param_types, is_variadic);
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Type *Get_scalar_type(Jit_data *jd, Ake_type_use *tu) {
        if (!tu) {
            return Type::getVoidTy(*jd->TheContext);
        }

        struct Ake_type_def *td = tu->td;

        if (td->type == Ake_type_integer) {
            Type *t;
            if (td->bit_count == 64) {
                t = Type::getInt64Ty(*jd->TheContext);
            } else if (td->bit_count == 32) {
                t = Type::getInt32Ty(*jd->TheContext);
            } else if (td->bit_count == 16) {
                t = Type::getInt16Ty(*jd->TheContext);
            } else if (td->bit_count == 8) {
                t = Type::getInt8Ty(*jd->TheContext);
            } else {
                assert(false);
            }
            return t;
        } else if (td->type == Ake_type_float) {
            if (td->bit_count == 64) {
                return Type::getDoubleTy(*jd->TheContext);
            } else if (td->bit_count == 32) {
                return Type::getFloatTy(*jd->TheContext);
            } else if (td->bit_count == 16) {
                return Type::getHalfTy(*jd->TheContext);
            } else {
                assert(false && "unsupported real bit count");
            }
        } else if (td->type == Ake_type_boolean) {
            return Type::getInt1Ty(*jd->TheContext);
        } else if (td->type == Ake_type_function) {
            return Get_function_type(jd, tu);
        } else if (td->type == Ake_type_struct) {
            return (Type *) (StructType *) td->composite_type;
        } else {
            assert(false);
        }

        return nullptr;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Type* Get_type_pointer(Jit_data *jd, Ake_type_use *tu)
    {
        Type* t = Get_type(jd, tu);
        if (tu) {
            if (tu->is_array || tu->td->type == Ake_type_function || tu->td->type == Ake_type_struct) {
                t = t->getPointerTo();
            }
        }
        return t;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Type* Get_type(Jit_data* jd, Ake_type_use* tu)
    {
        Type *t = Get_scalar_type(jd, tu);
        if (tu && tu->is_array) {
            size_t i = tu->dim.count - 1;
            while (true) {
                auto dim = (Ake_type_dimension*)ZINC_VECTOR_PTR(&tu->dim, i);
                if (tu->td->type == Ake_type_function) {
                    t = t->getPointerTo();
                }
                t = ArrayType::get(t, dim->size);
                if (i == 0) break;
                i--;
            }
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

    void Run(Jit_data* jd, Ake_ast* n, Ake_code_gen_result* result)
    {
        Zinc_string* value = &result->value;
        auto ExprSymbol = jd->ExitOnErr(jd->TheJIT->lookup(TOP_LEVEL_NAME));
        if (n->tu) {
            enum Ake_type type = n->tu->td->type;
            bool is_array = n->tu->is_array;
            int bit_count = n->tu->td->bit_count;
            if (type == Ake_type_integer) {
                if (is_array) {
                    if (bit_count == 64) {
                        long* (*fp)() = ExprSymbol.getAddress().toPtr<long*(*)()>();
                        long* p = fp();
                        Zinc_vector* dim_vector = &n->tu->dim;
                        size_t count = 1;
                        for (int i = 0; i < dim_vector->count; i++) {
                            auto dim = (Ake_type_dimension*)ZINC_VECTOR_PTR(dim_vector, i);
                            count *= dim->size;
                        }
                        Zinc_string_add_char(value, '[');
                        for (int i = 0; i < count; i++) {
                            if (i >= 1) {
                                Zinc_string_add_char(value, ',');
                            }
                            Zinc_string_add_format(value, "%ld", *p++);
                        }
                        Zinc_string_add_char(value, ']');
                        Zinc_string_add_char(value, '\n');
                    } else if (bit_count == 32) {
                        int* (*fp)() = ExprSymbol.getAddress().toPtr<int*(*)()>();
                        int* p = fp();
                        Zinc_vector* dim_vector = &n->tu->dim;
                        size_t count = 1;
                        for (int i = 0; i < dim_vector->count; i++) {
                            auto dim = (Ake_type_dimension*)ZINC_VECTOR_PTR(dim_vector, i);
                            count *= dim->size;
                        }
                        Zinc_string_add_char(value, '[');
                        for (int i = 0; i < count; i++) {
                            if (i >= 1) {
                                Zinc_string_add_char(value, ',');
                            }
                            Zinc_string_add_format(value, "%d", *p++);
                        }
                        Zinc_string_add_char(value, ']');
                        Zinc_string_add_char(value, '\n');
                    } else if (bit_count == 8) {
                        char* (*fp)() = ExprSymbol.getAddress().toPtr<char*(*)()>();
                        char* p = fp();
                        Zinc_string_add_format(value, "%s", p);
                        Zinc_string_add_char(value, '\n');
                    } else {
                        assert(false);
                    }
                } else {
                    if (n->tu->td->is_signed) {
                        if (n->tu->td->bit_count == 64) {
                            long (*fp)() = ExprSymbol.getAddress().toPtr<long(*)()>();
                            long v = fp();
                            Zinc_string_add_format(value, "%ld", v);
                            Zinc_string_add_char(value, '\n');
                            if (result->return_address) {
                                *(long*)result->return_address = v;
                            }
                        } else if (n->tu->td->bit_count == 32) {
                            int (*fp)() = ExprSymbol.getAddress().toPtr < int(*)
                            () > ();
                            int v = fp();
                            Zinc_string_add_format(value, "%d", v);
                            Zinc_string_add_char(value, '\n');
                            if (result->return_address) {
                                *(int*)result->return_address = v;
                            }
                        } else if (n->tu->td->bit_count == 16) {
                            short int (*fp)() = ExprSymbol.getAddress().toPtr < short int(*)
                            () > ();
                            short int v = fp();
                            Zinc_string_add_format(value, "%hd", v);
                            Zinc_string_add_char(value, '\n');
                            if (result->return_address) {
                                *(short int*)result->return_address = v;
                            }
                        } else if (bit_count == 8) {
                            int8_t (*fp)() = ExprSymbol.getAddress().toPtr<int8_t(*)()> ();
                            int8_t v = fp();
                            Zinc_string_add_format(value, "%hhd", (int)v);
                            Zinc_string_add_char(value, '\n');
                            if (result->return_address) {
                                *(int8_t*)result->return_address = v;
                            }
                        } else {
                            assert(false);
                        }
                    } else {
                        if (n->tu->td->bit_count == 64) {
                            unsigned long (*fp)() = ExprSymbol.getAddress().toPtr<unsigned long(*)()>();
                            unsigned long v = fp();
                            Zinc_string_add_format(value, "%lu", v);
                            Zinc_string_add_char(value, '\n');
                        } else if (n->tu->td->bit_count == 32) {
                            u_int32_t (*fp)() = ExprSymbol.getAddress().toPtr<u_int32_t(*)() > ();
                            u_int32_t v = fp();
                            Zinc_string_add_format(value, "%u", v);
                            Zinc_string_add_char(value, '\n');
                            if (result->return_address) {
                                *(u_int32_t*)result->return_address = v;
                            }
                        } else if (n->tu->td->bit_count == 16) {
                            u_int16_t (*fp)() = ExprSymbol.getAddress().toPtr < u_int16_t(*)
                            () > ();
                            u_int16_t v = fp();
                            Zinc_string_add_format(value, "%hu", v);
                            Zinc_string_add_char(value, '\n');
                            if (result->return_address) {
                                *(u_int16_t*)result->return_address = v;
                            }
                        } else if (bit_count == 8) {
                            u_int8_t (*fp)() = ExprSymbol.getAddress().toPtr<u_int8_t(*)()> ();
                            u_int8_t v = fp();
                            Zinc_string_add_format(value, "%hhu", (unsigned int)v);
                            Zinc_string_add_char(value, '\n');
                            if (result->return_address) {
                                *(u_int8_t*)result->return_address = v;
                            }
                        } else {
                            assert(false);
                        }
                    }
                }
            } else if (type == Ake_type_float) {
                if (is_array) {
                    double* (*fp)() = ExprSymbol.getAddress().toPtr <double*(*)()>();
                    double* p = fp();
                    Zinc_string_add_format(value, "%lf", *p);
                    Zinc_string_add_char(value, '\n');
                } else {
                    if (n->tu->td->bit_count == 64) {
                        double (*fp)() = ExprSymbol.getAddress().toPtr <double(*)()>();
                        double v = fp();
                        Zinc_string_add_format(value, "%lf", v);
                        Zinc_string_add_char(value, '\n');
                    } else if (n->tu->td->bit_count == 32) {
                        float (*fp)() = ExprSymbol.getAddress().toPtr <float(*)()>();
                        float v = fp();
                        Zinc_string_add_format(value, "%f", v);
                        Zinc_string_add_char(value, '\n');
                    } else if (n->tu->td->bit_count == 16) {
                        _Float16 (*fp)() = ExprSymbol.getAddress().toPtr <_Float16(*)()>();
                        _Float16 v = fp();
                        Zinc_string_add_format(value, "%f", (float)v);
                        Zinc_string_add_char(value, '\n');
                    }
                }
            } else if (type == Ake_type_boolean) {
                bool (*fp)() = ExprSymbol.getAddress().toPtr <bool(*)()>();
                bool v = fp();
                if (v) {
                    Zinc_string_add_format(value, "true", v);
                } else {
                    Zinc_string_add_format(value, "false", v);
                }
                Zinc_string_add_char(value, '\n');
            } else if (type == Ake_type_function) {
                void* (*fp)() = ExprSymbol.getAddress().toPtr<void*(*)()>();
                void* v = fp();
                Zinc_string_add_format(value, "Function");
                Zinc_string_add_char(value, '\n');
            } else {
                struct Zinc_location loc{};
                Zinc_location_init(&loc);
                Zinc_error_list_set(jd->el, &loc, "type not handled");
                Zinc_string_add_char(value, '\n');
            }
        } else {
            void (*fp)() = ExprSymbol.getAddress().toPtr <void(*)()>();
            fp();
            Zinc_string_add_char(value, '\n');
        }
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Dispatch(Jit_data* jd, Ake_ast* n)
    {
        if (n->type == Ake_ast_type_stmts) {
            return Handle_stmts(jd, n);
        } else if (n->type == Ake_ast_type_extern) {
            return Handle_extern(jd, n);
        } else if (n->type == Ake_ast_type_if) {
            return Handle_if(jd, n);
        } else if (n->type == Ake_ast_type_const || n->type == Ake_ast_type_var) {
            return Handle_variable_dec(jd, n);
        } else if (n->type == Ake_ast_type_function) {
            return Handle_function(jd, n);
        } else if (n->type == Ake_ast_type_assign) {
            return Handle_assign(jd, n);
        } else if (n->type == Ake_ast_type_mult) {
            return Handle_mult(jd, n);
        } else if (n->type == Ake_ast_type_plus) {
            return Handle_add(jd, n);
        } else if (n->type == Ake_ast_type_minus) {
            return Handle_sub(jd, n);
        } else if (n->type == Ake_ast_type_call) {
            return Handle_call(jd, n);
        } else if (n->type == Ake_ast_type_dot) {
            return Handle_dot(jd, n);
        } else if (n->type == Ake_ast_type_id) {
            return Handle_identifier(jd, n);
        } else if (n->type == Ake_ast_type_struct) {
            return Handle_struct(jd, n);
        } else if (n->type == Ake_ast_type_struct_literal) {
            return Handle_struct_literal(jd, n);
        } else if (n->type == Ake_ast_type_array_literal) {
            return Handle_array_literal(jd, n);
        } else if (n->type == Ake_ast_type_array_subscript) {
            return Handle_subscript(jd, n);
        } else if (n->type == Ake_ast_type_sign) {
            return Handle_sign(jd, n);
        } else if (n->type == Ake_ast_type_number) {
            return Handle_number(jd, n);
        } else if (n->type == Ake_ast_type_boolean) {
            return Handle_boolean(jd, n);
        } else if (n->type == Ake_ast_type_string) {
            return Handle_string(jd, n);
        } else {
            printf("code gen: unhandled ast node type: %s", Ast_type_name(n->type));
            exit(1);
        }
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    void Array_copy(
            Jit_data* jd,
            Ake_type_use* lhs_tu,
            Ake_type_use* rhs_tu,
            Value* lhs_ptr,
            Value* rhs_ptr)
    {
        size_t size = *(size_t*)ZINC_VECTOR_PTR(&lhs_tu->dim, 0);

        Ake_type_use* lhs_tu2 = Ake_type_use_clone(lhs_tu);
        Ake_type_use_reduce_dimension(lhs_tu2);

        Ake_type_use* rhs_tu2 = Ake_type_use_clone(rhs_tu);
        Ake_type_use_reduce_dimension(rhs_tu2);

        for (size_t i = 0; i < size; i++) {
            Type* t = Get_type(jd, lhs_tu2);
            std::vector<Value*> list = std::vector<Value*>();
            Value* i_value = ConstantInt::get(*jd->TheContext, APInt(64, i, false));
            list.push_back(i_value);
            Value* lhs_ptr2 = jd->Builder->CreateInBoundsGEP(t, lhs_ptr, list, "lhstmp");
            Value* rhs_ptr2 = jd->Builder->CreateInBoundsGEP(t, rhs_ptr, list, "rhstmp");

            if (lhs_tu2->is_array) {
                Array_copy(jd, lhs_tu2, rhs_tu2, lhs_ptr2, rhs_ptr2);
            } else {
                Value* value = jd->Builder->CreateLoad(t, rhs_ptr2, "rhs");
                jd->Builder->CreateStore(value, lhs_ptr2);
            }
        }

        Ake_type_use_destroy(lhs_tu2);
        Ake_type_use_destroy(rhs_tu2);
    }

}