/*
          _____   ____  _____            _____
    /\   |  __ \ / __ \|  __ \     /\   |  __ \
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#ifndef ADORAD_AST_H
#define ADORAD_AST_H

#include <adorad/core/types.h>
#include <adorad/core/buffer.h>
#include <adorad/core/vector.h>
#include <adorad/compiler/location.h>
#include <adorad/compiler/tokens.h>

typedef struct AstNode AstNode;
typedef enum AstNodeKind AstNodeKind;
typedef struct AstNodeExpression AstNodeExpression;
typedef struct AstNodeStatement AstNodeStatement;
typedef struct AstNodeCompileTimeValue AstNodeCompileTimeValue;
typedef struct AstNodeDecl AstNodeDecl;
typedef struct AstNodeTypeDecl AstNodeTypeDecl;
typedef struct AstNodeIdentifier AstNodeIdentifier;

enum AstNodeKind {
    AstNodeKindIdentifier = 0,
    AstNodeKindBlock,        // `{ ... }

    // Functions
    AstNodeKindFuncPrototype,
    AstNodeKindFuncDef,      // `func hello()`

    // Literals
    AstNodeKindIntLiteral,    // `2`
    AstNodeKindFloatLiteral,  // `3.4224829`
    AstNodeKindCharLiteral,   // 'a', 'b'...
    AstNodeKindStringLiteral, // 'str` or "str"
    AstNodeKindBoolLiteral,   // `bool`
    AstNodeKindNilLiteral,    // `nil`

    // Declarations
    AstNodeKindEnumDecl,      // `type Fruits enum { ... }`
    AstNodeKindUnionDecl,     // `type Fruits union { ... }`
    AstNodeKindVarDecl,       // `some_type var_name = ...`

    // Expressions
    AstNodeKindFuncCallExpr,   // `sayHello('Hello!')`
    AstNodeKindIfExpr,         // `if cond { ...}`
    AstNodeKindLoopWhileExpr,  // `loop {}`
    AstNodeKindLoopCExpr,      // `loop i=0; i<10; i++ {}`
    AstNodeKindLoopInExpr,     // `loop i in 0..10 {}`
    AstNodeKindMatchExpr,      // `match expr { ... }`
    AstNodeKindCatchExpr,      // `catch Error`
    AstNodeKindBinaryOpExpr,   // a binary expression like `&&` or `||
    AstNodeKindPrefixOpExpr,
    AstNodeKindFieldAccessExpr,
    
    AstNodeKindInitExpr,
    AstNodeKindSliceExpr,
    AstNodeKindArrayAccessExpr,
    AstNodeKindArrayType,
    AstNodeKindInferredArrayType,

    // Fields
    AstNodeKindTypeDecl,       // `type name T { ... }`  where T is one of {enum/struct}

    AstNodeKindBreak,          // `break`
    AstNodeKindContinue,       // `continue`

    // Misc
    AstNodeKindParamDecl,
    AstNodeKindDefer,
    AstNodeKindReturn,
    AstNodeKindUnreachable,
    AstNodeKindMatchBranch,
    AstNodeKindMatchRange,
};

typedef enum VisibilityMode {
    VisibilityModePrivate, // default
    VisibilityModePublic   // if `export` is used
} VisibilityMode;

typedef enum AstAddressingMode {
    AstAddressingModeInvalid,   // invalid addressing mode
    AstAddressingModeCompile_time_value,  // value known at compile time
    AstAddressingModeVariable, // addressable variable (known at runtime)
    AstAddressingModeConstant, // constant
    AstAddressingModeType,     // type  
} AstAddressingMode;

typedef enum AstLanguage {
    AstLanguageAdorad,
    AstLanguageC,
    AstLanguageAmd64,
    AstLanguageI386,
    AstLanguageArm64,
    AstLanguageArm32,
    AstLanguageRv64,
    AstLanguageRv32,
} AstLanguage;

typedef struct AstNodeScope {
    Buff* scope;
} AstNodeScope;

typedef enum IdentifierKind {
    IdentifierKindUnresolved,
    IdentifierKindBlankident, // `_`
    IdentifierKindVariable,
    IdentifierKindConst,
    IdentifierKindFunction,
    IdentifierKindGlobal,     // if declared within a `global` scope
} IdentifierKind;

typedef enum AttributeKind {
    AttributeKindPlain,      // [name]
    AttributeKindString,     // ['name']
    AttributeKindNumber,     // [123]
    AttributeKindCompileTime // [compiletime]
} AttributeKind;

// The `[]` before a function/variable
// Eg: [inline], [compiletime]
typedef struct AstNodeAttribute {
    Buff* name;   // [inline]
} AstNodeAttribute;

typedef struct AstNodeIdentifier {
    AstNode* type;
    bool is_const;
    bool is_export;
    bool is_mutable;  // This is false unless explicitly mentioned by the user
} AstNodeIdentifier;

typedef struct AstNodeArguments {
    Vec* positional_args;   // Vec<AstNodeArg*>
    Vec* args;              // Vec<AstNodeArg*>
    Vec* variadic_args;     // Vec<AstNodeArg*>
    Vec* kwd_args;          // Vec<AstNodeArg*>
} AstNodeArguments;

typedef struct AstNodeArgData {
    AstNode* arg;
    AstNode* annotation;
    Buff* type_comment;
} AstNodeArgData;

typedef struct AstNodeKwdData {
    AstNode* ident;
    AstNode* value;
} AstNodeKwdData;

typedef struct AstNodeAsCast {
    AstNode* expr;
} AstNodeAsCast;

typedef struct AstNodeCastExpr {
    AstNode* arg;
    AstNode* expr;
    Buff* type;
    bool has_arg;
} AstNodeCastExpr;

typedef struct AstNodeIfExpr {
    AstNode* condition;
    AstNode* then_block;
    AstNode* else_node;
    bool is_compiletime;
    bool is_expr;
    bool has_else;
} AstNodeIfExpr;

typedef struct AstNodeInitExpr {
    AstNode* type;
    Vec* entries;
    enum {
        InitExprKindStruct,
        InitExprKindArray,
    } kind;
} AstNodeInitExpr;

typedef struct AstNodeLoopWhileExpr {
    AstNode* cond;
    Vec* statements;
    AstNodeScope* scope;
    bool is_inline;
} AstNodeLoopWhileExpr;

typedef struct AstNodeLoopCExpr {
    Buff* label;
    AstNode* init;
    bool has_init;
    AstNode* cond;
    bool has_cond;
    AstNode* updation;  // increment/decrement
    bool has_updation;
    Vec* statements;
    AstNodeScope* scope;
    bool is_inline;
} AstNodeLoopCExpr;

typedef struct AstNodeLoopInExpr {
    Buff* label;
    Buff* key_var;
    Buff* val_var;
    bool is_val_var_mutable;
    AstNode* cond;
    bool is_range;
    Vec* statements;
    TokenKind tokenkind;
    AstNodeScope* scope;
    bool is_inline;
} AstNodeLoopInExpr;

typedef struct AstNodeLoopExpr {
    Buff* label;
    union {
        AstNodeLoopWhileExpr* loop_while_expr;
        AstNodeLoopCExpr* loop_c_expr;
        AstNodeLoopInExpr* loop_in_expr;
    };
} AstNodeLoopExpr;

typedef enum FuncCallModifier {
    FuncCallModifierNone,
    FuncCallModifierAsync,
    FuncCallModifierNeverInline,
    FuncCallModifierAlwaysInline,
    FuncCallModifierCompileTime
} FuncCallModifier;

typedef struct AstNodeFuncCallExpr {
    AstNode* func_call_expr;
    Vec* params;
    FuncCallModifier modifier;
} AstNodeFuncCallExpr;

typedef struct AstNodeMatchExpr {
    AstNode* expr;
    Vec* branches;
} AstNodeMatchExpr;

typedef struct AstNodeMatchBranchExpr {
    AstNode* expr;
    Vec* branches;
    bool any_branches_are_ranges; // if any branch is a range-based match
} AstNodeMatchBranchExpr;

typedef struct AstNodeMatchRangeExpr {
    AstNode* begin;
    AstNode* end;
} AstNodeMatchRangeExpr;

typedef struct AstNodeCatchExpr {
    AstNode* op1;
    AstNode* symbol; // can be nullptr
    AstNode* op2;
} AstNodeCatchExpr;

typedef struct AstNodeTryExpr {
    Buff* symbol;
    AstNode* target_node;
    AstNode* then_node;
    AstNode* else_node;
    Buff* err_symbol;
} AstNodeTryExpr;

typedef enum BinaryOpKind {
    BinaryOpKindInvalid,
    BinaryOpKindAssignmentPlus,    // +=
    BinaryOpKindAssignmentMinus,   // -=
    BinaryOpKindAssignmentMult,    // *=
    BinaryOpKindAssignmentDiv,     // /=
    BinaryOpKindAssignmentMod,     // %=
    BinaryOpKindAssignmentBitshiftLeft,  // <<=
    BinaryOpKindAssignmentBitshiftRight, // >>=
    BinaryOpKindAssignmentBitAnd, // &=
    BinaryOpKindAssignmentBitOr,  // |=
    BinaryOpKindAssignmentBitXor, // ^=
    BinaryOpKindAssignmentEquals, // =

    BinaryOpKindCmpEqual,                 // ==
    BinaryOpKindCmpNotEqual,              // !=
    BinaryOpKindCmpLessThan,              // <
    BinaryOpKindCmpGreaterThan,           // >
    BinaryOpKindCmpLessThanorEqualTo,     // <=
    BinaryOpKindCmpGreaterThanorEqualTo,  // >=

    BinaryOpKindBoolAnd,                  // `and`
    BinaryOpKindBoolOr,                   // `or`
    BinaryOpKindBoolNot,                  // `not`

    BinaryOpKindBitAnd,  // &
    BinaryOpKindBitOr,   // |
    BinaryOpKindBitXor,  // ^
    BinaryOpKindBitOrOr, // ||
    BinaryOpKindBitNegation, // !

    BinaryOpKindBitshitLeft,   // <<
    BinaryOpKindBitshitRight,  // >>

    BinaryOpKindAdd,      // +
    BinaryOpKindSubtract, // -
    BinaryOpKindMult,     // *
    BinaryOpKindDiv,      // /
    BinaryOpKindMod,      // %
} BinaryOpKind;

typedef struct AstNodeBinaryOpExpr {
    AstNode* lhs;
    BinaryOpKind op;
    AstNode* rhs;
} AstNodeBinaryOpExpr;

typedef struct AstNodeTypeOfExpr {
    AstNode* expr;
} AstNodeTypeOfExpr;

typedef struct AstNodeSetExpr {
    Vec* exprs;     // Vec<AstNode*>
} AstNodeSetExpr;

typedef struct AstNodeLambdaExpr {
    AstNodeArguments args;
    AstNode* expr;
} AstNodeLambdaExpr;

typedef struct AstNodeAwaitExpr {
    AstNode* value;    
} AstNodeAwaitExpr;

typedef struct AstNodeSliceExpr {
    AstNode* array_ref_expr;
    AstNode* lower;
    AstNode* upper;
    AstNode* step;
    AstNode* sentinel;  // can be null
} AstNodeSliceExpr;

// This can be one of:
//     | AstNodeAsCast
//     | AstNodeCastExpr
//     | AstNodeIfExpr   
//     | AstNodeLoopExpr
//     | AstNodeFuncCallExpr
//     | AstNodeMatchExpr
//     | AstNodeCatchExpr
//     | AstNodeBinaryOpExpr
//     | AstNodeTypeExpr
//     | AstNodeTypeOfExpr
typedef struct AstNodeExpression {
    union {
        AstNodeAsCast*  as_cast;
        AstNodeCastExpr* cast_expr;
        AstNodeIfExpr* if_expr;
        AstNodeInitExpr* init_expr;
        AstNodeLoopExpr* loop_expr;
        AstNodeFuncCallExpr* func_call_expr;
        AstNodeMatchExpr* match_expr;
        AstNodeMatchBranchExpr* match_branch_expr;
        AstNodeMatchRangeExpr* match_range_expr;
        AstNodeCatchExpr* catch_expr;
        AstNodeTryExpr* try_expr;
        AstNodeBinaryOpExpr* binary_op_expr;
        // AstNodeTypeExpr* type_expr;
        AstNodeTypeOfExpr* typeof_expr;
        AstNodeSetExpr* set_expr;
        AstNodeLambdaExpr* lambda_expr;
        AstNodeAwaitExpr* await_expr;
        AstNodeSliceExpr* slice_expr;
    };
} AstNodeExpression;

typedef struct AstNodeAliasDecl {
    AstNode* name;
    AstNode* asname; // can be null
} AstNodeAliasDecl;

typedef struct AstNodeTypeEnumDecl {
    Buff* name;
    bool is_export;
    Location* loc;
    Vec* fields;     // typedef enum value fields
    Vec* attributes; // typedef enum attributes 
} AstNodeTypeEnumDecl;

typedef struct AstNodeTypeStructDecl {
    Buff* name;
    Location* loc;
    Vec* fields;    // variables, etc
    Vec* methods;   // methods
} AstNodeTypeStructDecl;

// This can be one of:
//     | AstNodeTypeEnumDecl
//     | AstNodeTypeUnionDecl (disabled for now)
//     | AstNodeTypeStructDecl
typedef struct AstNodeTypeDecl {
    union {
        AstNodeTypeEnumDecl* enum_decl;
        // AstNodeTypeUnionDecl* union_decl;
        AstNodeTypeStructDecl* struct_decl;
    };
} AstNodeTypeDecl;

typedef struct AstNodeConstantDecl {
    bool is_export;
    bool is_block;  // `const ( ... )`
    Location* loc;
    Vec* fields;    // various constant declarations
} AstNodeConstantDecl;

typedef struct AstNodeGlobalDecl {
    Buff* module;    // globals declared in a module, persist through that module
    bool is_block;   // `global ( ... )`
    Location* loc;
    Vec* fields;     // various global declarations
} AstNodeGlobalDecl;

typedef struct AstNodeSumTypeDecl {
    Buff* name;
    bool is_export;
    Vec* variant_types; // Vec<AstNodeType*>
    Location* loc;
} AstNodeSumTypeDecl;

// Function or Method Declaration
typedef struct AstNodeFuncDecl {
    Buff* name;
    Buff* module;      // name of the module
    Buff* parent_type; // the `type` of which the function belongs to (nullptr, if not a method)
    Buff* return_type;
    bool is_variadic;  // variadic arguments
    bool is_export;    // true for `export func abc()`
    bool is_noreturn;  // true for `[noreturn] func 
    bool is_main;      // true for `func main()`
    bool is_test;      // true for `func test_yyy()`
    bool no_body;      // true for function definitions (no function body) `func abc()`

    AstNode* parameters;
    AstNode* body;      // can be nullptr for no-body functions (just declarations)
    Location* loc;     // location of the `func` declaration
} AstNodeFuncDecl;

// This can be one of:
//     | AstNodeAliasDecl
//     | AstNodeTypeDecl (enum/struct)
//     | AstNodeFuncDecl
//     | AstNodeDictDecl
//     | AstNodeListDecl
//     | AstNodeTupleDecl
//     | AstNodeConstantDecl
//     | AstNodeGlobalDecl
//     | AstNodeSumTypeDecl 
typedef struct AstNodeDecl {
    union {
        AstNodeAliasDecl* alias_decl;
        AstNodeTypeDecl* type_decl;
        AstNodeFuncDecl* func_decl;
        AstNodeConstantDecl* const_decl;
        AstNodeGlobalDecl* global_decl;
        AstNodeSumTypeDecl* sumtype_decl;
    };
    Buff* name;
    bool is_export;
    Location* loc;
} AstNodeDecl;

typedef struct AstNodeAssignmentStatement {
    AstNodeKind op;
    Location* loc;
    Vec* right;        // Vec<AstNodeExpression*>
    Vec* left;         // Vec<AstNodeExpression*>
    Vec* right_types;  // Vec<AstNodeExpression*>
    Vec* left_types;   // Vec<AstNodeExpression*>
    bool is_compound;
} AstNodeAssignmentStatement;

// `{ ... }`
typedef struct AstNodeBlock {
    Buff* name; // for labeled block statements
    Vec* statements;
} AstNodeBlock;

// break/continue
typedef struct AstNodeBranchStatement {
    Buff* name;
    AstNode* expr;  // can be nullptr (`break`). always nullptr for `continue`
    enum {
        AstNodeBranchStatementBreak,
        AstNodeBranchStatementContinue
    } type;
} AstNodeBranchStatement;

typedef struct AstNodeDeferStatement {
    AstNode* expr;
} AstNodeDeferStatement;

typedef struct AstNodeEmptyStatement {
    Buff* fixme;
} AstNodeEmptyStatement;

typedef enum FuncInline {
    FuncInlineAuto,
    FuncInlineInline,
    FuncInlineNoinline
} FuncInline;

typedef struct AstNodeFuncPrototype {
    Buff* name;
    Vec* params;  // Vec<AstNode*>
    AstNode* return_type;
    AstNode* func_def;

    FuncInline func_inline;
    bool is_export;
    bool is_extern;
    bool is_generic;
    bool is_var_args;  // variable arguments used?
} AstNodeFuncPrototype;

typedef struct AstNodeImportStatement {
    Buff* module;
    Buff* alias;           // can be null
} AstNodeImportStatement;

typedef struct AstNodeModuleStatement {
    Buff* name;
    Buff* short_name;
    bool is_skip;
    bool is_export;
} AstNodeModuleStatement;

typedef struct AstNodeReturnStatement {
    AstNode* expr;
    enum {
        ReturnKindOk,
        ReturnKindError,
    } kind;
} AstNodeReturnStatement;

typedef struct AstNodeVarDecl {
    Buff* name;
    AstNode* type;    // can be null
    AstNode* expr;

    bool is_const;
    bool is_export;
    bool is_mutable;  // This is false unless explicitly mentioned by the user
} AstNodeVarDecl;

// This can be one of:
//     | AstNodeAssignmentStatement
//     | AstNodeBlock
//     | AstNodeBranchStatement
//     | AstNodeDeferStatement
//     | AstNodeEmptyStatement
//     | AstNodeExpression
//     | AstNodeFuncDecl (prototype only)
//     | AstNodeImportStatement
//     | AstNodeModuleStatement
//     | AstNodeReturnStatement
//     | AstNodeVarDecl
typedef struct AstNodeStatement {
    union {
        AstNodeAssignmentStatement* assign_stmt;
        AstNodeBlock* block_stmt;
        AstNodeBranchStatement* branch_stmt;
        AstNodeDeferStatement* defer_stmt;
        AstNodeEmptyStatement* empty_stmt;
        AstNodeExpression* expr_stmt;
        AstNodeFuncPrototype* func_proto_decl;
        AstNodeImportStatement* import_stmt;
        AstNodeModuleStatement* module_stmt;
        AstNodeReturnStatement* return_stmt;
        AstNodeVarDecl* var_decl;
    };
} AstNodeStatement;

typedef struct AstNodeEmptyExpression {
    int type; // TODO(remove)
} AstNodeEmptyExpression;

typedef struct AstNodeBoolLiteral {
    bool value;
} AstNodeBoolLiteral;

typedef struct AstNodeByteLiteral {
    Buff* value;
} AstNodeByteLiteral;

typedef struct AstNodeFloatLiteral {
    Buff* value;
    // TODO (jasmcaus) - Come up with a workaround for this
    enum {
        AstNodeFloatLiteral32,    // default
        AstNodeFloatLiteral64,
        // AstNodeFloatLiteral128 // will be supported later
    } type;
} AstNodeFloatLiteral;

typedef struct AstNodeIntegerLiteral {
    Buff* value;
    // TODO (jasmcaus) - Come up with a workaround for this
    enum {
        AstNodeIntegerLiteral8,
        AstNodeIntegerLiteral16,
        AstNodeIntegerLiteral32, // default
        AstNodeIntegerLiteral64,
        // AstNodeIntegerLiteral128 // will be supported later
        AstNodeUIntLiteral8,
        AstNodeUIntLiteral16,
        AstNodeUIntLiteral32, // default
        AstNodeUIntLiteral64,
        // AstNodeUIntLiteral128 // will be supported later
    } type;
} AstNodeIntegerLiteral;

typedef struct AstNodeCharLiteral {
    Buff* value;
    Location* loc;
} AstNodeCharLiteral;

typedef struct AstNodeStringLiteral {
    Buff* value;
    bool is_special;   // format / raw string
    enum {
        AstNodeStringLiteralNone,   // if `is_special` is false
        AstNodeStringLiteralRaw,    // `r"abc"`
        AstNodeStringLiteralFormat  // `f"name = {name}"`
    } type;
} AstNodeStringLiteral;

// This can be one of:
//     | AstNodeEmptyExpression
//     | AstNodeBoolLiteral
//     | AstNodeByteLiteral
//     | AstNodeFloatLiteral (32/64)
//     | AstNodeIntegerLiteral (8/16/32/64 + unsigned versions) 
//     | AstNodeCharLiteral
//     | AstNodeStringLiteral
typedef struct AstNodeCompileTimeValue {
    union {
        AstNodeEmptyExpression* empty_expr;
        AstNodeBoolLiteral* bool_value;
        AstNodeByteLiteral* byte_value;
        AstNodeFloatLiteral* float_value;
        AstNodeIntegerLiteral* int_value;
        AstNodeCharLiteral* char_value;
        AstNodeStringLiteral* str_value;
    };
} AstNodeCompileTimeValue;

typedef struct AstNodeConstField {
    Buff* module;
    Buff* name;
    AstNodeExpression* expr;
    AstNodeCompileTimeValue* comptime_value;
    bool is_export;
} AstNodeConstField;

typedef struct AstNodeGlobalField {
    Buff* name;
    Location* loc;
    Location* type_loc;
    AstNodeExpression* expr;

    bool has_expr;
} AstNodeGlobalField;

typedef struct AstNodeVariable {
    Buff* name;
    AstNodeExpression* expr;
    bool is_export;
    bool is_mutable;
    bool is_argument;
    bool is_used;
    bool is_tmp;
    bool is_heap_obj;
    Location* loc;
} AstNodeVariable;

// This can be one of:
//     | AstNodeConstField
//     | AstNodeGlobalField
//     | AstNodeVariable
typedef struct AstNodeScopeObject {
    union {
        AstNodeConstField* const_field;
        AstNodeGlobalField* global_field;
        AstNodeVariable* var;
    };
    Buff* name;
    // AstNodeType* type;
} AstNodeScopeObject;

// // There are only 4 possible versions of a Type info you will use:
// //    1. Enum
// //    2. Union
// //    3. Sum Type
// //    4. Interface (can be generic)
// struct {
//     union {
//         AstNode
//     };
// };

typedef struct AstNodeParamDecl {
    Buff* name;
    AstNode* type;
    bool is_alias;
    bool is_var_args;
} AstNodeParamDecl;

typedef struct AstNodeReturnExpr {
    AstNode* expr;
} AstNodeReturnExpr;

typedef struct AstNodeTestDecl {
    Buff* name;   // can be nullptr if no name
    AstNode* body;
} AstNodeTestDecl;

typedef struct AstNodeTestExpr {
    Buff* symbol;
    AstNode* target_node;
    AstNode* then_node;
    AstNode* else_node;  // null, block node, or an `if expr` node
} AstNodeTestExpr;

// TODO(jasmcaus): Work on this
typedef enum UnaryOpKind {
    UnaryOpKindNot,
    UnaryOpKindInvert,
} UnaryOpKind;

typedef struct AstNodeUnaryOpExpr {
    AstNode* lhs;
    UnaryOpKind op;
    AstNode* rhs;
} AstNodeUnaryOpExpr;

typedef enum ComparisonOpKind {
    ComparisonOpKindEqual,
    ComparisonOpKindNotEqual,
    ComparisonOpKindLessThan,
    ComparisonOpKindLessThanOrEqualTo,
    ComparisonOpKindGreaterThan,
    ComparisonOpKindGreaterThanOrEqualTo,
    ComparisonOpKindIs,
    ComparisonOpKindIsNot,
    ComparisonOpKindIn,
    ComparisonOpKindNotIn,
} ComparisonOpKind;

typedef struct AstNodeComparisonOpExpr {
    AstNode* op1;
    BinaryOpKind op;
    AstNode* op2;
} AstNodeComparisonOpExpr;

// TODO(jasmcaus): Do we need an `OperatorKind` and `AstNodeOperator`?
typedef enum OperatorKind {
    OperatorKindAdd,
    OperatorKindSub,
    OperatorKindMult,
    OperatorKindMatMult,
    OperatorKindDiv,
    OperatorKindMod,
    OperatorKindPow,
    OperatorKindLShift,
    OperatorKindRShift,
    OperatorKindBitWiseOr,
    OperatorKindBitWiseXor,
    OperatorKindBitWiseAnd,
    OperatorKindFloorDiv,
} OperatorKind;

typedef struct AstNodeOperator {
    Buff* delete_me;
} AstNodeOperator;

typedef enum PrefixOpKind {
    PrefixOpKindInvalid,
    PrefixOpKindBoolNot,   // KEYWORD(not)
    PrefixOpKindNegation,  // !var
    PrefixOpKindAddrOf,    // &var
    PrefixOpKindTry,       // KEYWORD(try)
    PrefixOpKindOptional,  // ?
} PrefixOpKind;

typedef struct AstNodePrefixOpExpr {
    PrefixOpKind op;
    AstNode* expr;
} AstNodePrefixOpExpr;

typedef struct AstNodeCompileTime {
    AstNode* expr;
} AstNodeCompileTime;

typedef struct AstNodeArrayAccessExpr {
    AstNode* array_ref_expr;
    AstNode* subscript;
} AstNodeArrayAccessExpr;

typedef struct AstNodeInferredArrayType {
    AstNode* sentinel; // can be null
    AstNode* child_type;
} AstNodeInferredArrayType;

typedef struct AstNodeArrayType {
    AstNode* size;
    AstNode* sentinel;
    AstNode* child_type;
    AstNode* align_expr;
    Token* allow_zero_token;
    bool is_const;
    bool is_volatile;
} AstNodeArrayType;

typedef struct AstNodeFieldAccessExpr {
    AstNode* struct_expr;
    Buff* field_name;
} AstNodeFieldAccessExpr;

struct AstNode {
    AstNodeKind kind; // type of AST Node
    Location* loc;

    union {
        AstNodeIdentifier* identifier;
        AstNodeTypeDecl* type_decl;
        AstNodeDecl* decl;
        AstNodeExpression* expr;
        AstNodeStatement* stmt;
        AstNodeCompileTimeValue* comptime_value;
        AstNodeScopeObject* scope_obj;
        AstNodeTestDecl* test_decl;
        AstNodeTestExpr* test_expr;
        AstNodePrefixOpExpr* prefix_op_expr;
        AstNodeParamDecl* param_decl;
        AstNodeArrayAccessExpr* array_access_expr;
        AstNodeInferredArrayType* inferred_array_type;
        AstNodeArrayType* array_type;
        AstNodeFieldAccessExpr* field_access_expr;
    } data;
};

// Each Adorad source file can be represented by one AstFile structure.
typedef struct AstFile {
    Buff* path;     // full path of the source file - `/path/to/file.ad`
    Buff* basepath; // file name - `file.ad` (useful for tracing)
    int num_lines;  // number of source code lines in the file (including comments)
    int num_bytes;  // number of processed source code bytes
    // TODO (jasmcaus) Change the type of `module` to an `AstNodeModule`
    Buff* module;   // name of the module
    bool is_test;   // true for test_*.ad files
} AstFile;

#endif // ADORAD_AST_H
