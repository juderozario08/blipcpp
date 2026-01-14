enum class BufType { ORIGINAL, ADD };
typedef struct PieceTable {
    BufType buffer;
    int start;
    int length;
} PieceTable;

