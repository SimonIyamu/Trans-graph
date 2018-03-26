#define MAX_NAME_LENGTH	80 
#define MAXVERTEX 80

typedef enum {FALSE, TRUE} Boolean;

typedef int Vertex;

typedef struct edge {
   Vertex endpoint;
   struct edge *nextedge;
} Edge;

typedef struct ln{
	Vertex v;
	struct ln *next;
}ListNode;

typedef struct graph {
   int n;     /* number of vertices in the graph */
   Edge *firstedge[MAXVERTEX];
   char stations[MAXVERTEX][MAX_NAME_LENGTH];    /*Each line is a station name */
   ListNode *line[MAXVERTEX];           /*This is used to know in which line or lines every station belongs*/
} Graph;


