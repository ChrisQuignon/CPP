

typedef struct _PRIMEBITS {
    int start;
    int length;
    int retaddr;
    int noop;
} PRIMEBITS __attribute__((aligned(16)));

//TODO: Change size
typedef struct _PRIMFIELD  {
     unsigned bits:8;
} PRIMFIELD __attribute__((aligned(16)));

typedef struct _MATRIXADDRESS {
    int dira; int dirb; int dirc; int size;
} MATRIXADDRESS;

typedef struct _COLOR {
    int r; int g; int b; int noop;
} COLOR;

typedef struct _POINT {
    int x; int y; int z; int noop;
} POINT;
