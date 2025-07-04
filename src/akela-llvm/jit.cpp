#include "tools.h"

using namespace llvm;
using namespace llvm::orc;

namespace Akela_llvm {
    void Declare_abort(Jit_data* jd);
    void Declare_printf(Jit_data* jd);
    void Declare_exit(Jit_data* jd);

    Jit_data* Init()
    {
        InitializeNativeTarget();
        InitializeNativeTargetAsmPrinter();
        InitializeNativeTargetAsmParser();
        auto jd = new Jit_data();
        Zinc_error_list* errors = NULL;
        Zinc_error_list_create(&errors);
        Jit_data_init(jd, errors);
        return jd;
    }

    void Destroy(Jit_data* jd)
    {
        free(jd->el);
        delete jd;
    }

    bool Jit(Akela_llvm_cg* cg, Jit_data* jd, Ake_Ast* n, Ake_code_gen_result* result)
    {
        bool valid = true;

        // InitializeNativeTarget();
        // InitializeNativeTargetAsmPrinter();
        // InitializeNativeTargetAsmParser();
        // Jit_data jd;
        // Jit_data_init(&jd, cg->el);

        if (!Zinc_string_list_contains_str(cg->extern_list, "abort")) {
            Declare_abort(jd);
        }
        if (!Zinc_string_list_contains_str(cg->extern_list, "printf")) {
            Declare_printf(jd);
        }
        if (!Zinc_string_list_contains_str(cg->extern_list, "exit")) {
            Declare_exit(jd);
        }

        std::vector<Type*> param_types = std::vector<Type*>();
        Type* ret_type = Get_type_pointer(jd, n->type);
        FunctionType *func_type = FunctionType::get(ret_type, param_types, false);
        Function *toplevel = Function::Create(
                func_type,
                Function::ExternalLinkage,
                TOP_LEVEL_NAME,
                *jd->TheModule);
        jd->toplevel = toplevel;

        std::string error_str;
        raw_string_ostream error_os(error_str);
        if (verifyFunction(*toplevel, &error_os)) {
            struct Zinc_location loc = {};
            Zinc_location_init(&loc);
            Zinc_error_list_set(cg->el, &loc, "%s", error_str.c_str());
        }

        jd->current_function.push_back(toplevel);
        BasicBlock* entry = BasicBlock::Create(*jd->TheContext, "entry", toplevel);
        jd->Builder->SetInsertPoint(entry);
        Value* value = Dispatch(jd, n);

        BasicBlock* last_block = nullptr;
        Function::iterator blocks = jd->toplevel->end();
        if (blocks != jd->toplevel->begin()) {
            last_block = &*--blocks;
        }
        jd->Builder->SetInsertPoint(last_block);

        if (n->type) {
            jd->Builder->CreateRet(value);
        } else {
            jd->Builder->CreateRetVoid();
        }

        jd->current_function.pop_back();
        if (cg->el->head) {
            valid = false;
        }

        std::string str;
        raw_string_ostream os(str);
        if  (verifyModule(*jd->TheModule, &os)) {
            struct Zinc_location loc = {};
            Zinc_location_init(&loc);
            Zinc_error_list_set(cg->el, &loc, "%s", str.c_str());
            valid = false;
        }

        std::string str2;
        raw_string_ostream os2(str2);
        jd->TheModule->print(os2, nullptr);
        Zinc_string_add_format(&result->module_text, "%s", str2.c_str());
        Zinc_string_finish(&result->module_text);

        std::string str4;
        raw_string_ostream os4(str4);
        jd->toplevel->print(os4, nullptr);
        Zinc_string_add_str(&result->function_text, str4.c_str());
        Zinc_string_finish(&result->function_text);

        if (result->debug) {
            fprintf(stderr, "\n%s", result->module_text.buf);
        }

        if (valid && !result->dry_run) {
            auto rt = jd->TheJIT->getMainJITDylib().createResourceTracker();
            auto tsm = ThreadSafeModule(std::move(jd->TheModule), std::move(jd->TheContext));
            jd->ExitOnErr(jd->TheJIT->addModule(std::move(tsm), rt));
            Run(jd, n, result);
            jd->ExitOnErr(rt->remove());
            Zinc_string_finish(&result->value);
        }

        return true;
    }

    void Declare_abort(Jit_data* jd)
    {
        std::vector<Type*> param_types = std::vector<Type*>();
        Type* ret_type = Type::getVoidTy(*jd->TheContext);
        FunctionType *func_type = FunctionType::get(ret_type, param_types, false);
        Function* f = Function::Create(func_type, Function::ExternalLinkage, "abort", *jd->TheModule);
        jd->abort_function = f;
    }

    void Declare_printf(Jit_data* jd)
    {
        std::vector<Type*> param_types = std::vector<Type*>();
        Type* char_type = Type::getInt8Ty(*jd->TheContext);
        //Type* string_type = char_type->getPointerTo();
        Type* string_type = PointerType::get(char_type, 0);
        param_types.push_back(string_type);
        Type* ret_type = Type::getVoidTy(*jd->TheContext);
        FunctionType *func_type = FunctionType::get(ret_type, param_types, true);
        Function* f = Function::Create(func_type, Function::ExternalLinkage, "printf", *jd->TheModule);
        jd->printf_function = f;
    }

    void Declare_exit(Jit_data* jd)
    {
        std::vector<Type*> param_types = std::vector<Type*>();
        Type* status_type = Type::getInt32Ty(*jd->TheContext);
        param_types.push_back(status_type);
        Type* ret_type = Type::getVoidTy(*jd->TheContext);
        FunctionType *func_type = FunctionType::get(ret_type, param_types, false);
        Function* f = Function::Create(func_type, Function::ExternalLinkage, "exit", *jd->TheModule);
        jd->exit_function = f;
    }
}
