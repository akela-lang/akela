#include "akela/parse_types.h"
#include "akela_llvm_tools.h"
#include "akela_llvm_function.h"
#include "akela_llvm_literal.h"
#include "akela_llvm_variable.h"
#include "akela_llvm_stmts.h"
#include "akela_llvm_operator.h"
#include "akela_llvm_struct.h"

using namespace llvm;
using namespace llvm::orc;

namespace Akela_llvm {
    void Jit_data_init(Jit_data* jd, struct error_list* el)
    {
        jd->el = el;
        jd->TheJIT = jd->ExitOnErr(KaleidoscopeJIT::Create());
        jd->TheContext = std::make_unique<LLVMContext>();
        jd->TheModule = std::make_unique<Module>(MODULE_NAME, *jd->TheContext);
        jd->TheModule->setDataLayout(jd->TheJIT->getDataLayout());
        jd->Builder = std::make_unique<IRBuilder<>>(*jd->TheContext);
        jd->toplevel = nullptr;
        jd->context.in_lhs = false;
        jd->abort_function = nullptr;
        jd->printf_function = nullptr;
        jd->exit_function = nullptr;
    }

/* NOLINTNEXTLINE(misc-no-recursion) */
    FunctionType* Get_function_type(Jit_data* jd, Ast_node* tu)
    {
        Ast_node *input = nullptr;
        Ast_node *output = nullptr;
        get_function_children(tu, &input, &output);

        std::vector<Type *> param_types = std::vector<Type *>();
        size_t input_count = Ast_node_count_children(input);
        if (input_count > 0) {
            for (size_t i = 0; i < input_count; i++) {
                Ast_node *dec = Ast_node_get(input, i);
                Ast_node *type_use = Ast_node_get(dec, 1);
                Type *dec_type = Get_type(jd, type_use);
                param_types.push_back(dec_type);
            }
        }

        Type *ret_type = nullptr;
        if (output) {
            Ast_node *ret = Ast_node_get(output, 0);
            ret_type = Get_return_type(jd, ret);
        } else {
            ret_type = Type::getVoidTy(*jd->TheContext);
        }

        return FunctionType::get(ret_type, param_types, false);
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Type* Get_scalar_type(Jit_data* jd, Ast_node* tu)
    {
        if (!tu) {
            return Type::getVoidTy(*jd->TheContext);
        }

        struct type_def *td = tu->td;

        if (td->type == type_integer) {
            Type* t = nullptr;
            if (td->bit_count == 64) {
                t = Type::getInt64Ty(*jd->TheContext);
            } else if (td->bit_count == 32) {
                t = Type::getInt32Ty(*jd->TheContext);
            } else if (td->bit_count == 8) {
                t = Type::getInt8Ty(*jd->TheContext);
            } else {
                assert(false);
            }
            return t;
        } else if (td->type == type_float) {
            if (td->bit_count == 64) {
                return Type::getDoubleTy(*jd->TheContext);
            } else if (td->bit_count == 32) {
                return Type::getFloatTy(*jd->TheContext);
            }
        } else if (td->type == type_boolean) {
            return Type::getInt1Ty(*jd->TheContext);
        } else if (td->type == type_function) {
            return Get_function_type(jd, tu);
        } else if (td->type == type_struct) {
            
        } else {
            assert(false);
        }

        return nullptr;
    }

/* NOLINTNEXTLINE(misc-no-recursion) */
    Type* Get_type(Jit_data* jd, Ast_node* tu)
    {
        Type *t = Get_scalar_type(jd, tu);
        if (tu && tu->to.is_array) {
            size_t i = tu->to.dim.count - 1;
            while (true) {
                auto dim = (Type_dimension*)VECTOR_PTR(&tu->to.dim, i);
                t = ArrayType::get(t, dim->size);
                if (i == 0) break;
                i--;
            }
        }

        return t;
    }

/* NOLINTNEXTLINE(misc-no-recursion) */
    Type* Get_return_type(Jit_data* jd, Ast_node* tu)
    {
        if (tu && tu->td && tu->td->type == type_function) {
            FunctionType *func_type = Get_function_type(jd, tu);
            return static_cast<Type *>(func_type->getPointerTo());
        } if (tu && tu->to.is_array) {
            return Get_type(jd, tu)->getPointerTo();
        } else {
            return Get_type(jd, tu);
        }
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

    void Run(Jit_data* jd, Ast_node* n, struct buffer* bf)
    {
        auto ExprSymbol = jd->ExitOnErr(jd->TheJIT->lookup(TOPLEVEL_NAME));
        if (n->tu) {
            enum type type = n->tu->td->type;
            bool is_array = n->tu->to.is_array;
            int bit_count = n->tu->td->bit_count;
            if (type == type_integer) {
                if (is_array) {
                    if (bit_count == 64) {
                        long* (*fp)() = ExprSymbol.getAddress().toPtr<long*(*)()>();
                        long* p = fp();
                        Vector* dim_vector = &n->tu->to.dim;
                        size_t count = 1;
                        for (int i = 0; i < dim_vector->count; i++) {
                            auto dim = (Type_dimension*)VECTOR_PTR(dim_vector, i);
                            count *= dim->size;
                        }
                        buffer_add_char(bf, '[');
                        for (int i = 0; i < count; i++) {
                            if (i >= 1) {
                                buffer_add_char(bf, ',');
                            }
                            buffer_add_format(bf, "%ld", *p++);
                        }
                        buffer_add_char(bf, ']');
                    } else if (bit_count == 32) {
                        int* (*fp)() = ExprSymbol.getAddress().toPtr<int*(*)()>();
                        int* p = fp();
                        Vector* dim_vector = &n->tu->to.dim;
                        size_t count = 1;
                        for (int i = 0; i < dim_vector->count; i++) {
                            auto dim = (Type_dimension*)VECTOR_PTR(dim_vector, i);
                            count *= dim->size;
                        }
                        buffer_add_char(bf, '[');
                        for (int i = 0; i < count; i++) {
                            if (i >= 1) {
                                buffer_add_char(bf, ',');
                            }
                            buffer_add_format(bf, "%d", *p++);
                        }
                        buffer_add_char(bf, ']');
                    } else if (bit_count == 8) {
                        char* (*fp)() = ExprSymbol.getAddress().toPtr<char*(*)()>();
                        char* p = fp();
                        buffer_add_format(bf, "%s", p);
                    } else {
                        assert(false);
                    }
                } else {
                    if (n->tu->td->bit_count == 64) {
                        long (*fp)() = ExprSymbol.getAddress().toPtr<long(*)()>();
                        long v = fp();
                        buffer_add_format(bf, "%d", v);
                    } else if (n->tu->td->bit_count == 32) {
                        int (*fp)() = ExprSymbol.getAddress().toPtr < int(*)
                        () > ();
                        int v = fp();
                        buffer_add_format(bf, "%d", v);
                    } else if (bit_count == 8) {
                        char (*fp)() = ExprSymbol.getAddress().toPtr<char(*)()> ();
                        char v = fp();
                        buffer_add_format(bf, "%d", v);
                    } else {
                        assert(false);
                    }
                }
            } else if (type == type_float) {
                if (is_array) {
                    double* (*fp)() = ExprSymbol.getAddress().toPtr <double*(*)()>();
                    double* p = fp();
                    buffer_add_format(bf, "%lf", *p);
                } else {
                    double (*fp)() = ExprSymbol.getAddress().toPtr <double(*)()>();
                    double v = fp();
                    buffer_add_format(bf, "%lf", v);
                }
            } else if (type == type_boolean) {
                bool (*fp)() = ExprSymbol.getAddress().toPtr <bool(*)()>();
                bool v = fp();
                if (v) {
                    buffer_add_format(bf, "true", v);
                } else {
                    buffer_add_format(bf, "false", v);
                }
            } else if (type == type_function) {
                void* (*fp)() = ExprSymbol.getAddress().toPtr<void*(*)()>();
                void* v = fp();
                buffer_add_format(bf, "Function");
            } else {
                struct location loc{};
                location_init(&loc);
                error_list_set(jd->el, &loc, "type not handled");
            }
        } else {
            void (*fp)() = ExprSymbol.getAddress().toPtr <void(*)()>();
            fp();
        }
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Dispatch(Jit_data* jd, Ast_node* n)
    {
        if (n->type == ast_type_stmts) {
            return Handle_stmts(jd, n);
        } else if (n->type == ast_type_extern) {
            return Handle_extern(jd, n);
        } else if (n->type == ast_type_if) {
            return Handle_if(jd, n);
        } else if (n->type == ast_type_let) {
            return Handle_let(jd, n);
        } else if (n->type == ast_type_function) {
            return Handle_function(jd, n);
        } else if (n->type == ast_type_assign) {
            return Handle_assign(jd, n);
        } else if (n->type == ast_type_plus) {
            return Handle_add(jd, n);
        } else if (n->type == ast_type_minus) {
            return Handle_sub(jd, n);
        } else if (n->type == ast_type_call) {
            return Handle_call(jd, n);
        } else if (n->type == ast_type_dot) {
            return Handle_dot(jd, n);
        } else if (n->type == ast_type_id) {
            return Handle_identifier(jd, n);
        } else if (n->type == ast_type_struct) {
            return Handle_struct(jd, n);
        } else if (n->type == ast_type_array_literal) {
            return Handle_array_literal(jd, n);
        } else if (n->type == ast_type_array_subscript) {
            return Handle_subscript(jd, n);
        } else if (n->type == ast_type_sign) {
            return Handle_sign(jd, n);
        } else if (n->type == ast_type_number) {
            return Handle_number(jd, n);
        } else if (n->type == ast_type_boolean) {
            return Handle_boolean(jd, n);
        } else if (n->type == ast_type_string) {
            return Handle_string(jd, n);
        } else {
            char* names[ast_type_count];
            Ast_set_names(names);
            printf("code gen: unhandled ast node type: %s", names[n->type]);
            exit(1);
        }
    }
}