
#include <stdio.h>
#include <stdlib.h>

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
typedef enum VmTokenType {
    VM_TOKEN_TYPE_COMMENT,
    VM_TOKEN_TYPE_VARIABLE_DEFINITION,
    VM_TOKEN_TYPE_FUNCTION_DEFINITION,
    VM_TOKEN_TYPE_CLASS_DEFINITION,
    VM_TOKEN_TYPE_FUNCTION_CALL,
    VM_TOKEN_TYPE_VM_FLAG,
    VM_TOKEN_TYPE_IF_STATEMENT,
    VM_TOKEN_TYPE_ELIF_STATEMENT,
    VM_TOKEN_TYPE_ELSE_STATEMENT,
    VM_TOKEN_TYPE_WHILE_LOOP,
    VM_TOKEN_TYPE_FOR_LOOP,
    VM_TOKEN_TYPE_END_SCOPE,
    VM_TOKEN_TYPE_SWITCH,
    VM_TOKEN_TYPE_CASE
} VmTokenType;

typedef enum VmPrimitiveType {
    VM_VALUE_TYPE_VOID,
    VM_VALUE_TYPE_INT,
    VM_VALUE_TYPE_FLOAT,
    VM_VALUE_TYPE_STRING
} VmPrimitiveType;

typedef struct InstructionToken {
    VmTokenType type;
    char *data;
} InstructionToken;

typedef struct ExpressionToken {
    VmPrimitiveType returnType;
    char *data;
} ExpressionToken;

typedef struct VirtualMachine {
    InstructionToken *instructions;
    size_t instructionSize;
    size_t instructionPointer;
} VirtualMachine;

typedef struct VmCommentToken {
    size_t size;
} VmCommentToken;

typedef struct VmVarDefToken {
    size_t size;
} VmVarDefToken;

typedef struct VmFuncDefToken {
    size_t size;
} VmFuncDefToken;

typedef struct VmClassDefToken {
    size_t size;
} VmClassDefToken;

typedef struct VmFuncCallToken {
    size_t size;
} VmFuncCallToken;

typedef struct VmVmFagToken {
    size_t size;
} VmVmFagToken;

typedef struct VmIfStatementToken {
    size_t size;
} VmIfStatementToken;

typedef struct VmElifStatementToken {
    size_t size;
} VmElifStatementToken;

typedef struct VmElseStatementToken {
    size_t size;
} VmElseStatementToken;

typedef struct VmWhileLoopToken {
    size_t size;
} VmWhileLoopToken;

typedef struct VmForLoopToken {
    size_t size;
} VmForLoopToken;

typedef struct VmEndScopeToken {
    size_t size;
} VmEndScopeToken;

typedef struct VmSwitchToken {
    size_t size;
} VmSwitchToken;

typedef struct VmEndToken {
    size_t size;
} VmEndToken;

static VmPrimitiveType invoke_func(VirtualMachine *vm, const char *name, void *resultDest) {
    return VM_VALUE_TYPE_VOID;
}

static VmPrimitiveType eval_expression(VirtualMachine *vm, const char *expression, void *resultDest) {
    return VM_VALUE_TYPE_VOID;
}

static void proc_instruction(VirtualMachine *vm, InstructionToken *token) {

}
#ifdef __cplusplus
}
#endif