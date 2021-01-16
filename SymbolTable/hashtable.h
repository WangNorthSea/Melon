
#ifndef hashtable_h
#define hashtable_h

#define TableArraySize 1024

#define GLOBAL_VAR 0
#define PARAM_VAR 1
#define LOCAL_VAR 2

struct varinfo {
    int loc_type;
    int frame_offset;
    int isArray;
    int arrlen;
};

struct Value {
    void * target;
    char * key;
    struct Value * next;
    struct varinfo info;
};

struct Hashtable {
    struct Value tableArray[TableArraySize];
    void (*put)(struct Hashtable * table, char * key, void * value, int loc_type, int isArray, int arrlen);
    void * (*get)(struct Hashtable * table, char * key);
};

typedef struct Value Value;
typedef struct Hashtable Hashtable;

extern Hashtable * HashtableConstructor(void);

extern struct varinfo * global_info_ptr;

#endif /* hashtable_h */
