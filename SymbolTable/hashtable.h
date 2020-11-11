
#ifndef hashtable_h
#define hashtable_h

#define TableArraySize 1024

struct Value {
    void * target;
    char * key;
    struct Value * next;
};

struct Hashtable {
    struct Value tableArray[TableArraySize];
    void (*put)(struct Hashtable * table, char * key, void * value);
    void * (*get)(struct Hashtable * table, char * key);
};

typedef struct Value Value;
typedef struct Hashtable Hashtable;

extern Hashtable * HashtableConstructor(void);

#endif /* hashtable_h */
