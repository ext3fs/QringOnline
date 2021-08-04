#include 	"global.h"
#include	"util.h"
#include	"db.h"
#include	"astar.h"


struct NODE 
{
	long 		f,h;
	int 		g;
	int 		x, y;
	int 		NodeNum;
	struct NODE *	prev;
	struct NODE *	next;
	struct NODE *	Child[8];       // a node may have upto 8+(NULL) children
};

struct STACK
{
	struct NODE *	NodePtr;
	struct STACK *	NextStackPtr;
};

struct astar
{
	struct NODE *	OPEN;
	struct NODE *	CLOSED;
	struct NODE *	BESTNODE;

	struct STACK *	STK;
};

struct node_pool
{
	struct NODE	node[MAX_NODE_CNT];
	int		node_idx;
};

struct stack_pool
{
	struct STACK	stack[MAX_STACK_CNT];
	int		stack_idx;
};

struct astar		AS_node		[MAX_MAP_CNT];
struct node_pool	AS_node_pool	[MAX_MAP_CNT];
struct stack_pool	AS_stack_pool	[MAX_MAP_CNT];





int		AS_Find_Main			( const int map_num, long sx, long sy, long dx, long dy, int start_tile, int dest_tile );
int		AS_Path_Sort			( const int map_num );
struct NODE *	AS_ReturnBestNode		( const int map_num );

int		AS_GenerateSuccessors		( const int map_num, struct NODE *BestNode, const long dx, const long dy );
int		AS_GenerateSucc			( const int map_num, struct NODE *BestNode
						, const long x, const long y, const long dx, const long dy );
struct NODE *	AS_CheckOPEN			( const int map_num, const int tilenum );
struct NODE *	AS_CheckCLOSED			( const int map_num, const int tilenum );
void		AS_Insert			( const int map_num, struct NODE *Successor );
int		AS_PropagateDown		( const int map_num, struct NODE *Old );


void		AS_NodePool_Init		( struct NODE *p_node );
struct NODE * 	AS_NodePool_Alloc		( const int map_num );
void		AS_NodePool_Free		( const int map_num );

void		AS_StackPool_Init		( struct STACK *p_stack );
struct STACK * 	AS_StackPool_Alloc		( const int map_num );
void		AS_StackPool_Free		( const int map_num );

int		AS_Push				( const int map_num, struct NODE *Node );
struct NODE *	AS_Pop				( const int map_num );





int	AS_Init( void )
{
	int	i, j;

	for( i=D_GetMap_proc_start_map(); i<=D_GetMap_proc_end_map(); i++ )
	{
		AS_node[i].BESTNODE	= NULL;

		// node pool init()
		AS_node_pool[i].node_idx = 0;

		for( j=0; j<MAX_NODE_CNT; j++ )
			AS_NodePool_Init( &(AS_node_pool[i].node[j]) );

		if( (AS_node[i].OPEN = (struct NODE *)AS_NodePool_Alloc(i)) == NULL )
		{
			dprintf("[ AS ]Error AS_NodePool_Alloc() \n");
			return FAIL;
		}

		if( (AS_node[i].CLOSED = (struct NODE *)AS_NodePool_Alloc(i)) == NULL )
		{
			dprintf("[ AS ]Error AS_NodePool_Alloc() \n");
			return FAIL;
		}

		// stack pool init()
		AS_stack_pool[i].stack_idx = 0;

		for( j=0; j<MAX_STACK_CNT; j++ )
			AS_StackPool_Init( &(AS_stack_pool[i].stack[j]) );

		if( (AS_node[i].STK = (struct STACK *)AS_StackPool_Alloc(i)) == NULL )
		{
			dprintf("[ AS ]Error AS_StackPool_Alloc() \n");
			return FAIL;
		}
	}

	return SUCC;
}


int	AS_Find_Path( const int map_num, const int sx, const int sy, const int dx, const int dy )
{
	int 	start_tile	= D_GetMap_tilenumber(map_num, sx, sy);
	int	dest_tile	= D_GetMap_tilenumber(map_num, dx, dy);

	if( !D_GetMap_collission(map_num, dx, dy) && !D_GetMap_collission(map_num, sx, sy) 
		&& (start_tile != dest_tile) )
	{
		AS_NodePool_Free(map_num);
		AS_StackPool_Free(map_num);

		if( AS_Find_Main(map_num, sx, sy, dx, dy, start_tile, dest_tile) )
		{
			AS_NodePool_Free(map_num);
			AS_StackPool_Free(map_num);
			return FAIL;
		}

		AS_Path_Sort(map_num);
	}
	else 
	{
		return FAIL;
	}

	return SUCC;
}


int	AS_Path_Sort( const int map_num )
{
	return SUCC;
}


int	AS_ReachedGoal( const int map_num )
{
	return (AS_node[map_num].BESTNODE->prev == NULL) ? SUCC : FAIL;
}


void	AS_PathNextNode( const int map_num ) 
{ 
	AS_node[map_num].BESTNODE = AS_node[map_num].BESTNODE->prev; 
}


int	AS_NodeGetX( const int map_num )     
{ 
	return AS_node[map_num].BESTNODE->x; 
}


int	AS_NodeGetY( const int map_num )
{ 
	return AS_node[map_num].BESTNODE->y; 
}


int	AS_Find_Main( const int map_num, long sx, long sy, long dx, long dy, int start_tile, int dest_tile )
{
	struct NODE	*start_node, *BestNode;


	if( (start_node = (struct NODE *)AS_NodePool_Alloc(map_num)) == NULL )
	{
		dprintf("[ AS ]Error AS_NodePool_Alloc() \n");
		return FAIL;
	}

	start_node->g		= 0;
	start_node->h		= ((sx - dx) * (sx - dx)) + ((sy - dy) * (sy - dy)); 
	start_node->f		= start_node->g + start_node->h;
	start_node->NodeNum	= start_tile;
	start_node->x		= sx;
	start_node->y		= sy;


	AS_node[map_num].OPEN->next	= start_node; 


	while( 1 )
	{
		if( (BestNode = (struct NODE *)AS_ReturnBestNode(map_num)) == NULL ) 
		{
			AS_node[map_num].BESTNODE	= AS_node[map_num].OPEN;
			AS_node[map_num].BESTNODE->x	= dx;
			AS_node[map_num].BESTNODE->y	= dy;
			return FAIL;
		}

		if( BestNode->NodeNum == dest_tile )	
			break;

		if( AS_GenerateSuccessors(map_num, BestNode, dx, dy) )
			return FAIL;
	}

	AS_node[map_num].BESTNODE 	= BestNode;

	return SUCC;
}


struct NODE *AS_ReturnBestNode( const int map_num )
{
	struct NODE 	*tmp;

	if( AS_node[map_num].OPEN->next == NULL )
	{
		dprintf( "[ AS ]Error AS_ReturnBestNode() : destination not reachable!\n");
		return NULL;
	}

	tmp				= AS_node[map_num].OPEN->next;	
	AS_node[map_num].OPEN->next 	= tmp->next;

	tmp->next			= AS_node[map_num].CLOSED->next;
	AS_node[map_num].CLOSED->next	= tmp;

	return tmp;
}


int	AS_GenerateSuccessors( const int map_num, struct NODE *BestNode, const long dx, const long dy )
{
	long	x, y;

	if( !D_GetMap_collission(map_num, (x=(BestNode->x - TILESIZE)), (y=(BestNode->y - TILESIZE))) )
	{
		if( AS_GenerateSucc(map_num, BestNode , x, y, dx, dy) )
			return FAIL;
	}

	if( !D_GetMap_collission(map_num, (x=(BestNode->x)), (y=(BestNode->y - TILESIZE))) )
	{
		if( AS_GenerateSucc(map_num, BestNode, x, y, dx, dy) )
			return FAIL;
	}

	if( !D_GetMap_collission(map_num, (x=(BestNode->x + TILESIZE)), (y=(BestNode->y - TILESIZE))) )
	{
		if( AS_GenerateSucc(map_num, BestNode, x, y, dx, dy) )
			return FAIL;
	}

	if( !D_GetMap_collission(map_num, (x=(BestNode->x + TILESIZE)), (y=BestNode->y)) )
	{
		if( AS_GenerateSucc(map_num, BestNode, x, y, dx, dy) )
			return FAIL;
	}

	if( !D_GetMap_collission(map_num, (x=(BestNode->x + TILESIZE)), (y=BestNode->y + TILESIZE)) )
	{
		if( AS_GenerateSucc(map_num, BestNode, x, y, dx, dy) )
			return FAIL;
	}

	if( !D_GetMap_collission(map_num, (x=BestNode->x), (y=(BestNode->y + TILESIZE))) )
	{
		if( AS_GenerateSucc(map_num, BestNode, x, y, dx, dy) )
			return FAIL;
	}

	if( !D_GetMap_collission(map_num, (x=(BestNode->x - TILESIZE)), (y=(BestNode->y + TILESIZE))) )
	{
		if( AS_GenerateSucc(map_num, BestNode, x, y, x, dy) )
			return FAIL;
	}

	if( !D_GetMap_collission(map_num, (x=(BestNode->x - TILESIZE)), (y=BestNode->y)) )
	{
		if( AS_GenerateSucc(map_num, BestNode, x, y, dx, dy) )
			return FAIL;
	}

	return SUCC;
}


int	AS_GenerateSucc( const int map_num, struct NODE *BestNode, const long x, const long y, const long dx, const long dy )
{
	int		g, TileNumS, c=0;
	struct NODE	*Old, *Successor;


	g		= BestNode->g + 1;				
	TileNumS	= D_GetMap_tilenumber(map_num, (int)x, (int)y);		

	if( (Old = AS_CheckOPEN(map_num, TileNumS)) != NULL )
	{
		for( c=0; c<8; c++ )
		{
			if( BestNode->Child[c] == NULL )
			{
				BestNode->Child[c] = Old;
				break;
			}
		}

		if( g < Old->g )		
		{
			Old->prev 	= BestNode;
			Old->g	  	= g;
			Old->f	  	= g+Old->h;
		}
	}
	else if( (Old = AS_CheckCLOSED(map_num, TileNumS)) != NULL ) 
	{
		for( c=0; c<8; c++ )
		{
			if( BestNode->Child[c] == NULL )
			{
				BestNode->Child[c] = Old;
				break;
			}
		}		

		if( g < Old->g )		
		{
			Old->prev	= BestNode;
			Old->g		= g;
			Old->f		= g+Old->h;

			if( AS_PropagateDown(map_num, Old) )
				return FAIL;		
		}
    	}
	else
	{
		if( (Successor = (struct NODE *)AS_NodePool_Alloc(map_num)) == NULL )
		{
			dprintf("[ AS ]Error AS_NodePool_Alloc() \n");
			return FAIL;
		}

		Successor->prev		= BestNode;
		Successor->g		= g;
		Successor->h		= (x-dx)*(x-dx)+(y-dy)*(y-dy);
		Successor->f		= g + Successor->h;	
		Successor->x		= x;		
		Successor->y		= y;
		Successor->NodeNum	= TileNumS;
		AS_Insert(map_num, Successor);	

		for(c=0;c<8;c++)
			if(BestNode->Child[c] == NULL)
				break;

		BestNode->Child[c]	= Successor;
	}

	return SUCC;
}



struct NODE *AS_CheckOPEN( const int map_num, const int tilenum )
{
	struct NODE	*tmp;

	tmp	= AS_node[map_num].OPEN->next;

	while( tmp != NULL )
	{
		if( tmp->NodeNum == tilenum )
			return tmp;
		else
			tmp = tmp->next;
	}
	return NULL;
}


struct NODE *AS_CheckCLOSED( const int map_num, const int tilenum )
{
	struct NODE	*tmp;

	tmp	= AS_node[map_num].CLOSED->next;

	while( tmp != NULL )
	{
		if( tmp->NodeNum == tilenum )
			return tmp;
		else
			tmp=tmp->next;
	}
	return NULL;
}


void	AS_Insert( const int map_num, struct NODE *Successor )
{
	struct NODE	*tmp1,*tmp2;
	long		f;

	if( AS_node[map_num].OPEN->next == NULL )
	{
		AS_node[map_num].OPEN->next = Successor;
		return;
	}

	f	= Successor->f;
	tmp1	= AS_node[map_num].OPEN;
	tmp2	= AS_node[map_num].OPEN->next;

	while( (tmp2 != NULL) && (tmp2->f < f) )
	{
		tmp1 = tmp2;
		tmp2 = tmp2->next;
	}

	Successor->next		= tmp2;
	tmp1->next		= Successor;
}


int	AS_PropagateDown( const int map_num, struct NODE *Old )
{
	int		c, g;
	struct NODE	*Child, *Father;

	g	= Old->g;			

	for( c=0; c<8; c++ )
	{
		if( (Child = Old->Child[c]) == NULL )
			break;
		if( g+1 < Child->g )
		{
			Child->g	= g + 1;
			Child->f	= Child->g+Child->h;
			Child->prev	= Old;	

			if( AS_Push(map_num, Child) )
				return FAIL;	
		}			
	}

	while( AS_node[map_num].STK->NextStackPtr != NULL )
	{
		Father = AS_Pop(map_num);

		for( c=0; c<8; c++ )
		{
			if( (Child = Father->Child[c])==NULL )
				break;
			if( Father->g+1 < Child->g )	
			{			
				Child->g	= Father->g+1;
				Child->f	= Child->g+Child->h;
				Child->prev	= Father;

				if( AS_Push(map_num, Child) )
					return FAIL;
			}
		}
	}

	return SUCC;
}




//------------------------------------------------------------------------------
// NODE / STACK FUNCTIONS
//------------------------------------------------------------------------------
void	AS_NodePool_Init( struct NODE *p_node )
{
	int	i;

	p_node->f		= 0;
	p_node->h		= 0;
	p_node->g		= 0;

	p_node->x		= 0;
	p_node->y		= 0;
	p_node->NodeNum		= 0;

	p_node->prev		= NULL;
	p_node->next		= NULL;

	for( i=0; i<8; i++ )
		p_node->Child[i]	= NULL;
}


struct NODE * 	AS_NodePool_Alloc( const int map_num )
{
	int	node_num = AS_node_pool[map_num].node_idx;

	if( (node_num < 0) || (node_num > MAX_NODE_CNT -1) )
	{
		dprintf("[ AS ]AS_NodePool_Alloc() : %d's maps node_pool full \n", map_num, node_num );
		return NULL;
	}

	//dprintf("[ AS ]AS_NodePool_Alloc() : map_num = %d, node_num = %d \n", map_num, node_num );

	AS_NodePool_Init( &(AS_node_pool[map_num].node[node_num]) );
	(AS_node_pool[map_num].node_idx) ++;

	return ((struct NODE *)&AS_node_pool[map_num].node[node_num]);
}


void	AS_NodePool_Free( const int map_num )
{
	struct NODE *Node, *OldNode;

	AS_node_pool[map_num].node_idx	= 2;//OPEN, CLOSED

	if( AS_node[map_num].OPEN != NULL )
	{
		Node = AS_node[map_num].OPEN->next;

		while( Node != NULL )
		{
			AS_NodePool_Init( Node );

			OldNode = Node;
			Node 	= Node->next;
		}

		AS_NodePool_Init( AS_node[map_num].OPEN );
	}

	if( AS_node[map_num].CLOSED != NULL )
	{
		Node = AS_node[map_num].CLOSED->next;

		while( Node != NULL )
		{
			AS_NodePool_Init( Node );

			OldNode = Node;
			Node 	= Node->next;
		}

		AS_NodePool_Init( AS_node[map_num].CLOSED );
	}

	AS_node[map_num].BESTNODE	= NULL;
}


void	AS_StackPool_Init( struct STACK *p_stack )
{
	p_stack->NodePtr	= NULL;
	p_stack->NextStackPtr	= NULL;
}


struct STACK * 	AS_StackPool_Alloc( const int map_num )
{
	int	stack_num = AS_stack_pool[map_num].stack_idx;

	if( (stack_num < 0) || (stack_num > MAX_STACK_CNT -1) )
	{
		dprintf("[ AS ]AS_StackPool_Alloc() : %d's maps stack_pool full \n", map_num, stack_num );
		return NULL;
	}

	//dprintf("[ AS ]AS_StackPool_Alloc() : map_num = %d, stack_num = %d \n", map_num, stack_num );

	AS_StackPool_Init( &(AS_stack_pool[map_num].stack[stack_num]) );
	(AS_stack_pool[map_num].stack_idx) ++;

	return ((struct STACK *)&AS_stack_pool[map_num].stack[stack_num]);
}


void	AS_StackPool_Free( const int map_num )
{
	int	i, stack_num = AS_stack_pool[map_num].stack_idx;

	AS_stack_pool[map_num].stack_idx	= 1;

	for( i=1; i<stack_num; i++ )
		AS_StackPool_Init( &(AS_stack_pool[map_num].stack[i]) );
}


int	AS_Push( const int map_num, struct NODE *Node )
{
	struct STACK	*tmp;

	if( (tmp = (struct STACK *)AS_StackPool_Alloc(map_num)) == NULL )
	{
		dprintf("[ AS ]Error AS_StackPool_Alloc() \n");
		return FAIL;
	}

	tmp->NodePtr		= Node;
	tmp->NextStackPtr	= AS_node[map_num].STK->NextStackPtr;
	AS_node[map_num].STK->NextStackPtr	= tmp;

	return SUCC;
}


struct NODE *AS_Pop( const int map_num)
{
	struct NODE	*tmp;
	struct STACK	*tmpSTK;

	tmpSTK		= AS_node[map_num].STK->NextStackPtr;
	tmp		= tmpSTK->NodePtr;
	AS_node[map_num].STK->NextStackPtr	= tmpSTK->NextStackPtr;
	AS_StackPool_Init( tmpSTK );

	return tmp;
}


