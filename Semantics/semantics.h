
#ifndef semantics_h
#define semantics_h

extern void semanticAnalyze(ASTNode * root, const char * file);
extern void throwSemanticError(ASTNode * node, char * content);

extern const char * fileChecking;
extern Scope * scope;

#endif /* semantics_h */
