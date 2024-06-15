#include "akela_llvm_tools.h"
#include "akela/type_def.h"

using namespace llvm;

namespace Akela_llvm {
    Value* Handle_struct(Jit_data* jd, Ast_node* n)
    {
        std::vector<Type*> type_list;
        buffer_finish(&n->value);
        struct Ast_node* element_dec = n->head;
        while (element_dec) {
            Ast_node* element_tu = Ast_node_get(element_dec, 1);
            Type* element_type = Get_type(jd, element_tu);
            type_list.push_back(element_type);
            element_dec = element_dec->next;
        }
        StructType* struct_type = StructType::create(*jd->TheContext, type_list, n->value.buf);
        assert(n->sym);
        assert(n->sym->td);
        n->sym->td->composite_type = struct_type;
        return nullptr;
    }

    Value* Handle_dot(Jit_data* jd, Ast_node* n)
    {
        Ast_node* left = Ast_node_get(n, 0);
        Ast_node* left_tu = left->tu;
        assert(left_tu);
        struct type_def* left_td = left_tu->td;
        assert(left_td);
        struct symbol* sym = left->sym;
        assert(sym);
        Value* struct_value = Dispatch(jd, left);

        Ast_node* right = Ast_node_get(n, 1);
        Ast_node* dec = left_td->composite->head;
        auto struct_type = (StructType*)left_td->composite_type;
        size_t i = 0;
        bool found = false;
        while (dec) {
            Ast_node* dec_id = Ast_node_get(dec, 0);
            if (buffer_compare(&dec_id->value, &right->value)) {
                found = true;
                break;
            }
            dec = dec->next;
            i++;
        }
        assert(found);
        Value* gep_value = jd->Builder->CreateStructGEP(struct_type, struct_value, i);

        if (jd->context.in_lhs) {
            return gep_value;
        } else {
            Type* dot_type = Get_type(jd, n->tu);
            buffer_finish(&right->value);
            return jd->Builder->CreateLoad(dot_type, gep_value, right->value.buf);
        }
    }
}