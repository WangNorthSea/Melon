
#include <stdio.h>
#include "../ASTNode/node.h"

int dumpLevel = -1;

void kindTable(int kind) {
    switch (kind) {
        case 100:
            printf("IntegerLiteral");
            break;
        case 101:
            printf("CharacterLiteral");
            break;
        case 102:
            printf("StringLiteral");
            break;
        case 103:
            printf("Identifier");
            break;
        case 104:
            printf("Cast");
            break;
        case 105:
            printf("PrefixOp");
            break;
        case 106:
            printf("UnaryOp");
            break;
        case 107:
            printf("Dereference");
            break;
        case 108:
            printf("Address");
            break;
        case 109:
            printf("SizeofType");
            break;
        case 110:
            printf("SizeofExpr");
            break;
        case 111:
            printf("SuffixOp");
            break;
        case 112:
            printf("ArrayRef");
            break;
        case 113:
            printf("Member");
            break;
        case 114:
            printf("PtrMember");
            break;
        case 115:
            printf("Funcall");
            break;
        case 116:
            printf("BinaryOp");
            break;
        case 117:
            printf("LogicAnd");
            break;
        case 118:
            printf("LogicOr");
            break;
        case 119:
            printf("CondExpr");
            break;
        case 120:
            printf("Assign");
            break;
        case 121:
            printf("OpAssign");
            break;
        case 122:
            printf("Operator");
            break;
        case 123:
            printf("If");
            break;
        case 124:
            printf("While");
            break;
        case 125:
            printf("DoWhile");
            break;
        case 126:
            printf("For");
            break;
        case 127:
            printf("Switch");
            break;
        case 128:
            printf("Break");
            break;
        case 129:
            printf("Continue");
            break;
        case 130:
            printf("Goto");
            break;
        case 131:
            printf("Return");
            break;
        case 132:
            printf("CaseClauses");
            break;
        case 133:
            printf("CaseClause");
            break;
        case 134:
            printf("DefaultClause");
            break;
        case 135:
            printf("Cases");
            break;
        case 136:
            printf("CaseBody");
            break;
        case 137:
            printf("Block");
            break;
        case 138:
            printf("Stmts");
            break;
        case 139:
            printf("EmptyStmt");
            break;
        case 140:
            printf("Label");
            break;
        case 141:
            printf("Stmt");
            break;
        case 142:
            printf("FuncPtr");
            break;
        case 143:
            printf("DefinedVariables");
            break;
        case 144:
            printf("Variable");
            break;
        case 145:
            printf("DefinedFunc");
            break;
        case 146:
            printf("VoidType");
            break;
        case 147:
            printf("CharType");
            break;
        case 148:
            printf("ShortIntType");
            break;
        case 149:
            printf("IntType");
            break;
        case 150:
            printf("LongIntType");
            break;
        case 151:
            printf("UnsignedCharType");
            break;
        case 152:
            printf("UnsignedShortIntType");
            break;
        case 153:
            printf("UnsignedIntType");
            break;
        case 154:
            printf("UnsignedLongIntType");
            break;
        case 155:
            printf("StructType");
            break;
        case 156:
            printf("UnionType");
            break;
        case 158:
            printf("ParamsNode");
            break;
        case 159:
            printf("FuncPtrParam");
            break;
        case 160:
            printf("ConstParam");
            break;
        case 161:
            printf("NormalParam");
            break;
        case 162:
            printf("DefinedConstVariables");
            break;
        case 163:
            printf("DefinedStruct");
            break;
        case 164:
            printf("DefinedUnion");
            break;
        case 165:
            printf("TypeDef");
            break;
        case 166:
            printf("Args");
            break;
        case 167:
            printf("MemberList");
            break;
        case 168:
            printf("Slot");
            break;
        case 169:
            printf("PtrRef");
            break;
        case 170:
            printf("Varname");
            break;
        case 171:
            printf("UnfixedArray");
            break;
        case 172:
            printf("FixedArray");
            break;
        case 173:
            printf("Static");
            break;
        case 174:
            printf("Name");
            break;
        case 175:
            printf("Import");
            break;
        case 176:
            printf("TopDefs");
            break;
        case 177:
            printf("Imports");
            break;
        case 178:
            printf("Root");
            break;
        case 179:
            printf("FloatType");
            break;
        case 180:
            printf("DoubleType");
            break;
        case 181:
            printf("FloatLiteral");
            break;
        case 182:
            printf("UnlimitedParams");
            break;
        case 183:
            printf("FuncStmt");
            break;
        case 184:
            printf("ExternFunc");
            break;
        case 185:
            printf("ExternVar");
            break;
        case 186:
            printf("ExternConst");
            break;
        case 187:
            printf("ConstFuncPtr");
            break;
        case 188:
            printf("ConstVariable");
            break;
        case 189:
            printf("BoolType");
            break;
        case 190:
            printf("BoolLiteral");
            break;
        case 191:
            printf("ListExpr");
            break;
        default:
            break;
    }
}

void dumpAST(ASTNode * rootNode) {
    int i;
    
    dumpLevel++;
    
    for (i = 0; i < dumpLevel; i++)
        printf("\t");
    
    printf("<<");
    kindTable(rootNode -> kind);
    printf(">>\n");
    for (i = 0; i < dumpLevel; i++)
        printf("\t");
    printf("line: %d\n", rootNode -> line);
    
    if (rootNode -> image != NULL) {
        for (i = 0; i < dumpLevel; i++)
            printf("\t");
        printf("image: %s\n", rootNode -> image);
    }
    
    for (i = 0; i < 6; i++) {
        if (rootNode -> ptrs[i] != NULL)
            dumpAST(rootNode -> ptrs[i]);
    }
    
    for (i = 0; i < rootNode -> listLen; i++)
        dumpAST(rootNode -> list + i);
    
    dumpLevel--;
}
