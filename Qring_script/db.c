
#include	"global.h"
#include	"util.h"
#include	"db.h"



// --------------------------------------------------------------------------
// variable
static MYSQL			D_mysql[MAX_RECVT_CNT];
static pthread_mutex_t		D_lock;
static struct server		D_server[MAX_RECVT_CNT];



// --------------------------------------------------------------------------
// function
static int	D_check_map			( void );
static int	D_check_typemob			( void );
static int	D_check_mob			( void );
static int	D_check_level			( void );
static int	D_check_char_skill		( void );
static int	D_check_mob_skill		( void );
static int	D_check_item			( void );
static int	D_check_drop			( void );
static int	D_check_warp			( void );
static int	D_check_store			( void );

static int	D_set_map			( void );
static int	D_set_typemob			( void );
static int	D_set_mob			( void );
static int	D_set_level			( void );
static int	D_set_char_skill		( void );
static int	D_set_mob_skill			( void );
static int	D_set_item			( void );
static int	D_set_drop			( void );
static int	D_set_warp			( void );
static int	D_set_store			( void );





// --------------------------------------------------------------------------
// script
int	D_set_version( void )
{

	if( D_Mysql_Init(0) )
	{
		dprintf(M_LOG, "[ DB ]Error D_Mysql_init() \n");
		return FAIL;
	}

	if( D_Mysql_version(0, G_version) )
	{
		dprintf(M_LOG, "[ DB ]Error D_Mysql_version() \n");

		D_Mysql_Close(0);
		return FAIL;
	}

	D_Mysql_Close(0);


	dprintf(M_LOG, "\t[ DB ]Success D_set_version(%s) \n", G_version );
	return SUCC;
}


int	D_check_script( void )
{

	if( D_check_map() )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_map() \n");
		return FAIL;
	}

	if( D_check_typemob() )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_typemob() \n");
		return FAIL;
	}

	if( D_check_mob() )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_mob() \n");
		return FAIL;
	}

	if( D_check_level() )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_level() \n");
		return FAIL;
	}

	if( D_check_char_skill() )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_char_skill() \n");
		return FAIL;
	}

	if( D_check_mob_skill() )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_mob_skill() \n");
		return FAIL;
	}

	if( D_check_item() )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_item() \n");
		return FAIL;
	}

	if( D_check_drop() )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_drop() \n");
		return FAIL;
	}

	if( D_check_warp() )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_warp() \n");
		return FAIL;
	}

	if( D_check_store() )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_store() \n");
		return FAIL;
	}

	dprintf(M_LOG, "\t[ DB ]Success D_check_script() \n");
	return SUCC;
}


int	D_set_script( void )
{

	if( D_set_map() )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_map() \n");
		return FAIL;
	}

	if( D_set_typemob() )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_typemob() \n");
		return FAIL;
	}

	if( D_set_mob() )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_mob() \n");
		return FAIL;
	}

	if( D_set_level() )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_level() \n");
		return FAIL;
	}

	if( D_set_char_skill() )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_char_skill() \n");
		return FAIL;
	}

	if( D_set_mob_skill() )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_mob_skill() \n");
		return FAIL;
	}

	if( D_set_item() )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_item() \n");
		return FAIL;
	}

	if( D_set_drop() )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_drop() \n");
		return FAIL;
	}

	if( D_set_warp() )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_warp() \n");
		return FAIL;
	}

	if( D_set_store() )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_store() \n");
		return FAIL;
	}

	dprintf(M_LOG, "\t[ DB ]Success D_load_script()\n"); 
	return SUCC;
}


static int	D_check_map( void )
{

	FILE	*fp;
	int	i, tmpTotalFileCnt;
	char	tmpStr[256], tmpRealFileName[256];


	// Read Max Cnt
	if( U_get_value("script_serv.ini", "basic", "map_file", tmpStr) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_map() read map_file\n");
		return FAIL;
	}
	tmpTotalFileCnt		= atoi(tmpStr);


	for( i=0; i<tmpTotalFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/map/%04d.gcm", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Checking......... %s \n", tmpRealFileName);


		if( (fp = fopen(tmpRealFileName, "r")) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_check_map() open file %s \n", tmpRealFileName );
			return FAIL;
		}
	
		fclose(fp);


	}// end for() !!


	G_MapFileCnt = tmpTotalFileCnt;


	return SUCC;
}


static int	D_check_typemob( void )
{

	FILE	*fp;
	int	i, tmpTotalFileCnt;
	char	tmpStr[256], tmpRealFileName[256];


	// Read Max Cnt
	if( U_get_value("script_serv.ini", "basic", "typemob_file", tmpStr) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_typemob() read typemob_file\n");
		return FAIL;
	}
	tmpTotalFileCnt		= atoi(tmpStr);


	for( i=0; i<tmpTotalFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/mob%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Checking......... %s \n", tmpRealFileName);


		if( (fp = fopen(tmpRealFileName, "r")) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_check_typemob() open file %s \n", tmpRealFileName );
			return FAIL;
		}
	
		fclose(fp);


	}// end for() !!


	G_TypeMobFileCnt = tmpTotalFileCnt;


	return SUCC;
}


static int	D_check_mob( void )
{

	int	i;
	FILE	*fp;
	char	tmpRealFileName[256];


	for( i=0; i<G_MapFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/MobRegen%04d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Checking......... %s \n", tmpRealFileName);


		if( (fp = fopen(tmpRealFileName, "r")) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_check_mob() open file %s \n", tmpRealFileName );
			return FAIL;
		}

		fclose(fp);


	}// end for() !!


	return SUCC;
}


static int	D_check_level( void )
{

	FILE	*fp;
	int	i, tmpTotalFileCnt;
	char	tmpStr[256], tmpRealFileName[256];


	// Read Max Cnt
	if( U_get_value("script_serv.ini", "basic", "level_file", tmpStr) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_level() read level_file\n");
		return FAIL;
	}
	tmpTotalFileCnt		= atoi(tmpStr);


	for( i=0; i<tmpTotalFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/charlevel%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Checking......... %s \n", tmpRealFileName);


		if( (fp = fopen(tmpRealFileName, "r")) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_check_level() open file %s \n", tmpRealFileName );
			return FAIL;
		}

		fclose(fp);
	}


	G_LevelFileCnt = tmpTotalFileCnt;


	return SUCC;
}


static int	D_check_char_skill( void )
{

	FILE	*fp;
	int	i, tmpTotalFileCnt;
	char	tmpStr[256], tmpRealFileName[256];


	// Read Max Cnt
	if( U_get_value("script_serv.ini", "basic", "char_skill_file", tmpStr) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_char_skill() read char_skill_filen");
		return FAIL;
	}
	tmpTotalFileCnt		= atoi(tmpStr);


	for( i=0; i<tmpTotalFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/charskill%02d.txt", (i+1) );
		//dprintf(M_LOG< "\t[ DB ]Checking......... %s \n", tmpRealFileName);


		if( (fp = fopen(tmpRealFileName, "r")) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_check_char_skill() open file %s \n", tmpRealFileName );
			return FAIL;
		}

		fclose(fp);
	}


	G_CharSkillFileCnt = tmpTotalFileCnt;


	return SUCC;
}


static int	D_check_mob_skill( void )
{

	FILE	*fp;
	int	i, tmpTotalFileCnt;
	char	tmpStr[256], tmpRealFileName[256];


	// Read Max Cnt
	if( U_get_value("script_serv.ini", "basic", "mob_skill_file", tmpStr) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_mob_skill() read mob_skill_file\n");
		return FAIL;
	}
	tmpTotalFileCnt		= atoi(tmpStr);


	for( i=0; i<tmpTotalFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/mobskill%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Checking......... %s \n", tmpRealFileName);


		if( (fp = fopen(tmpRealFileName, "r")) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_check_mob_skill() open file %s \n", tmpRealFileName );
			return FAIL;
		}

		fclose(fp);
	}


	G_MobSkillFileCnt = tmpTotalFileCnt;


	return SUCC;
}


static int	D_check_item( void )
{

	FILE	*fp;
	int	i, tmpTotalFileCnt;
	char	tmpStr[256], tmpRealFileName[256];


	// Read Max Cnt
	if( U_get_value("script_serv.ini", "basic", "item_file", tmpStr) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_item() read item_file\n");
		return FAIL;
	}
	tmpTotalFileCnt		= atoi(tmpStr);


	for( i=0; i<tmpTotalFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/item%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Checking......... %s \n", tmpRealFileName);


		if( (fp = fopen(tmpRealFileName, "r")) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_check_item() open file %s \n", tmpRealFileName );
			return FAIL;
		}

		fclose(fp);
	}


	G_ItemFileCnt = tmpTotalFileCnt;


	return SUCC;
}


static int	D_check_drop( void )
{

	FILE	*fp;
	int	i, tmpTotalFileCnt;
	char	tmpStr[256], tmpRealFileName[256];


	// Read Max Cnt
	if( U_get_value("script_serv.ini", "basic", "drop_file", tmpStr) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_drop() read drop_file\n");
		return FAIL;
	}
	tmpTotalFileCnt		= atoi(tmpStr);


	for( i=0; i<tmpTotalFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/itemdrop%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Checking......... %s \n", tmpRealFileName);


		if( (fp = fopen(tmpRealFileName, "r")) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_check_drop() open file %s \n", tmpRealFileName );
			return FAIL;
		}

		fclose(fp);
	}


	G_DropFileCnt = tmpTotalFileCnt;


	return SUCC;
}


static int	D_check_warp( void )
{

	FILE	*fp;
	int	i, tmpTotalFileCnt;
	char	tmpStr[256], tmpRealFileName[256];


	// Read Max Cnt
	if( U_get_value("script_serv.ini", "basic", "warp_file", tmpStr) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_warp() read warp_file\n");
		return FAIL;
	}
	tmpTotalFileCnt		= atoi(tmpStr);


	for( i=0; i<tmpTotalFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/warp%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Checking......... %s \n", tmpRealFileName);


		if( (fp = fopen(tmpRealFileName, "r")) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_check_warp() open file %s \n", tmpRealFileName );
			return FAIL;
		}

		fclose(fp);
	}


	G_WarpFileCnt = tmpTotalFileCnt;


	return SUCC;
}


static int	D_check_store( void )
{

	FILE	*fp;
	int	i, tmpTotalFileCnt;
	char	tmpStr[256], tmpRealFileName[256];


	// Read Max Cnt
	if( U_get_value("script_serv.ini", "basic", "store_file", tmpStr) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_store() read store_file\n");
		return FAIL;
	}
	tmpTotalFileCnt		= atoi(tmpStr);


	for( i=0; i<tmpTotalFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/store%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Checking......... %s \n", tmpRealFileName);


		if( (fp = fopen(tmpRealFileName, "r")) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_check_store() open file %s \n", tmpRealFileName );
			return FAIL;
		}

		fclose(fp);
	}


	G_StoreFileCnt = tmpTotalFileCnt;


	return SUCC;
}


static int	D_set_map( void )
{

	int	i;
	char	tmpRealMapName[256];


	// Allocation Map Data
	if( !(G_MapList	= (struct map_info *)calloc(G_MapFileCnt, sizeof(struct map_info))) )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_map() calloc() : %s \n", strerror(errno) );
		return FAIL;
	}
	

	for( i=0; i<G_MapFileCnt; i ++ )
	{

		sprintf( tmpRealMapName, "./script/map/%04d.gcm", (i+1) );
		//dprintf(M_LOG< "\t[ DB ]Loading.......... %s \n", tmpRealMapName);


		if( U_get_length(tmpRealMapName, "r", &G_MapList[i].size) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_map() read file_length(%s) \n", tmpRealMapName);
			return FAIL;
		}

		if( U_get_data(tmpRealMapName, "r", G_MapList[i].size, G_MapList[i].data) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_map() read file_data(%s) \n", tmpRealMapName);
			return FAIL;
		}
	}

	return SUCC;
}


static int	D_set_typemob( void )
{
	int	i;
	char	tmpRealFileName[256];


	// alloc()
	if( (G_MobTypeList = (struct script_file *)calloc(G_TypeMobFileCnt, sizeof(struct script_file))) == NULL )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_typemob() calloc() : %s \n", strerror(errno) );
		return FAIL;
	}


	for( i=0; i<G_TypeMobFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/mob%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Loading.......... %s \n", tmpRealFileName);


		if( U_get_length(tmpRealFileName, "r", &G_MobTypeList[i].size) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_typemob() read file_length \n");
			return FAIL;
		}

		if( (G_MobTypeList[i].data = (char *)calloc(G_MobTypeList[i].size, SIZEOF_CHAR)) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_typemob() calloc() : %s \n", strerror(errno) );
			return FAIL;
		}

		if( U_get_data(tmpRealFileName, "r", G_MobTypeList[i].size, G_MobTypeList[i].data) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_typemob() read file_data \n");
			return FAIL;
		}

		if( (G_MobTypeList[i].line_cnt = U_get_line(tmpRealFileName, "r")) < 1 )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_typemob() read line_cnt\n");
			return FAIL;
		}
	}

	return SUCC;
}


static int	D_set_mob( void )
{
	int	i;
	char	tmpRealFileName[256];


	// alloc()
	if( (G_MobList = (struct script_file *)calloc(G_MapFileCnt, sizeof(struct script_file))) == NULL )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_mob() calloc() : %s \n", strerror(errno) );
		return FAIL;
	}


	for( i=0; i<G_MapFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/MobRegen%04d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Loading.......... %s \n", tmpRealFileName);


		if( U_get_length(tmpRealFileName, "r", &G_MobList[i].size) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_mob() read file_length \n");
			return FAIL;
		}

		if( (G_MobList[i].data = (char *)calloc(G_MobList[i].size, SIZEOF_CHAR)) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_mob() calloc() : %s \n", strerror(errno) );
			return FAIL;
		}

		if( U_get_data(tmpRealFileName, "r", G_MobList[i].size, G_MobList[i].data) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_mob() read file_data \n");
			return FAIL;
		}

		if( (G_MobList[i].line_cnt = U_get_line(tmpRealFileName, "r")) < 1 )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_mob() read file_line_cnt\n");
			return FAIL;
		}
	}

	return SUCC;
}


static int	D_set_level( void )
{
	int	i;
	char	tmpRealFileName[256];


	// alloc()
	if( (G_LevelList = (struct script_file *)calloc(G_LevelFileCnt, sizeof(struct script_file))) == NULL )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_level() calloc() : %s \n", strerror(errno) );
		return FAIL;
	}


	for( i=0; i<G_LevelFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/charlevel%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Loading.......... %s \n", tmpRealFileName);


		if( U_get_length(tmpRealFileName, "r", &G_LevelList[i].size) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_level() read file_length \n");
			return FAIL;
		}

		if( (G_LevelList[i].data = (char *)calloc(G_LevelList[i].size, SIZEOF_CHAR)) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_level() calloc() : %s \n", strerror(errno) );
			return FAIL;
		}

		if( U_get_data(tmpRealFileName, "r", G_LevelList[i].size, G_LevelList[i].data) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_level() read file_data \n");
			return FAIL;
		}

		if( (G_LevelList[i].line_cnt = U_get_line(tmpRealFileName, "r")) < 1 )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_level() read file_line_cnt\n");
			return FAIL;
		}
	}

	return SUCC;
}


static int	D_set_char_skill( void )
{
	int	i;
	char	tmpRealFileName[256];


	// alloc()
	if( (G_CharSkillList = (struct script_file *)calloc(G_CharSkillFileCnt, sizeof(struct script_file))) == NULL )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_char_skill() calloc() : %s \n", strerror(errno) );
		return FAIL;
	}


	for( i=0; i<G_CharSkillFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/charskill%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Loading.......... %s \n", tmpRealFileName);


		if( U_get_length(tmpRealFileName, "r", &G_CharSkillList[i].size) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_char_skill() read file_length \n");
			return FAIL;
		}

		if( (G_CharSkillList[i].data = (char *)calloc(G_CharSkillList[i].size, SIZEOF_CHAR)) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_char_skill() calloc() : %s \n", strerror(errno) );
			return FAIL;
		}

		if( U_get_data(tmpRealFileName, "r", G_CharSkillList[i].size, G_CharSkillList[i].data) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_char_skill() read file_data \n");
			return FAIL;
		}

		if( (G_CharSkillList[i].line_cnt = U_get_line(tmpRealFileName, "r")) < 1 )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_char_skill() read file_line_cnt\n");
			return FAIL;
		}
	}

	return SUCC;
}


static int	D_set_mob_skill( void )
{
	int	i;
	char	tmpRealFileName[256];


	// alloc()
	if( (G_MobSkillList = (struct script_file *)calloc(G_MobSkillFileCnt, sizeof(struct script_file))) == NULL )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_mob_skill() calloc() : %s \n", strerror(errno) );
		return FAIL;
	}


	for( i=0; i<G_MobSkillFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/mobskill%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Loading.......... %s \n", tmpRealFileName);


		if( U_get_length(tmpRealFileName, "r", &G_MobSkillList[i].size) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_mob_skill() read file_length \n");
			return FAIL;
		}

		if( (G_MobSkillList[i].data = (char *)calloc(G_MobSkillList[i].size, SIZEOF_CHAR)) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_mob_skill() calloc() : %s \n", strerror(errno) );
			return FAIL;
		}

		if( U_get_data(tmpRealFileName, "r", G_MobSkillList[i].size, G_MobSkillList[i].data) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_mob_skill() read file_data \n");
			return FAIL;
		}

		if( (G_MobSkillList[i].line_cnt = U_get_line(tmpRealFileName, "r")) < 1 )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_mob_skill() read file_line_cnt\n");
			return FAIL;
		}
	}

	return SUCC;
}


static int	D_set_item( void )
{
	int	i;
	char	tmpRealFileName[256];


	// alloc()
	if( (G_ItemList = (struct script_file *)calloc(G_ItemFileCnt, sizeof(struct script_file))) == NULL )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_item() calloc() : %s \n", strerror(errno) );
		return FAIL;
	}


	for( i=0; i<G_ItemFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/item%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Loading.......... %s \n", tmpRealFileName);


		if( U_get_length(tmpRealFileName, "r", &G_ItemList[i].size) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_item() read file_length \n");
			return FAIL;
		}

		if( (G_ItemList[i].data = (char *)calloc(G_ItemList[i].size, SIZEOF_CHAR)) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_item() calloc() : %s \n", strerror(errno) );
			return FAIL;
		}

		if( U_get_data(tmpRealFileName, "r", G_ItemList[i].size, G_ItemList[i].data) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_item() read file_data \n");
			return FAIL;
		}

		if( (G_ItemList[i].line_cnt = U_get_line(tmpRealFileName, "r")) < 1 )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_item() read file_line_cnt\n");
			return FAIL;
		}
	}

	return SUCC;
}


static int	D_set_drop( void )
{
	int	i;
	char	tmpRealFileName[256];


	// alloc()
	if( (G_DropList = (struct script_file *)calloc(G_DropFileCnt, sizeof(struct script_file))) == NULL )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_drop() calloc() : %s \n", strerror(errno) );
		return FAIL;
	}


	for( i=0; i<G_DropFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/itemdrop%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Loading.......... %s \n", tmpRealFileName);


		if( U_get_length(tmpRealFileName, "r", &G_DropList[i].size) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_drop() read file_length \n");
			return FAIL;
		}

		if( (G_DropList[i].data = (char *)calloc(G_DropList[i].size, SIZEOF_CHAR)) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_drop() calloc() : %s \n", strerror(errno) );
			return FAIL;
		}

		if( U_get_data(tmpRealFileName, "r", G_DropList[i].size, G_DropList[i].data) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_drop() read file_data \n");
			return FAIL;
		}

		if( (G_DropList[i].line_cnt = U_get_line(tmpRealFileName, "r")) < 1 )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_drop() read file_line_cnt\n");
			return FAIL;
		}
	}

	return SUCC;
}


static int	D_set_warp( void )
{
	int	i;
	char	tmpRealFileName[256];


	// alloc()
	if( (G_WarpList = (struct script_file *)calloc(G_WarpFileCnt, sizeof(struct script_file))) == NULL )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_warp() calloc() : %s \n", strerror(errno) );
		return FAIL;
	}


	for( i=0; i<G_WarpFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/warp%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Loading.......... %s \n", tmpRealFileName);


		if( U_get_length(tmpRealFileName, "r", &G_WarpList[i].size) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_warp() read file_length \n");
			return FAIL;
		}

		if( (G_WarpList[i].data = (char *)calloc(G_WarpList[i].size, SIZEOF_CHAR)) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_warp() calloc() : %s \n", strerror(errno) );
			return FAIL;
		}

		if( U_get_data(tmpRealFileName, "r", G_WarpList[i].size, G_WarpList[i].data) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_warp() read file_data \n");
			return FAIL;
		}

		if( (G_WarpList[i].line_cnt = U_get_line(tmpRealFileName, "r")) < 1 )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_warp() read file_line_cnt\n");
			return FAIL;
		}
	}

	return SUCC;
}


static int	D_set_store( void )
{
	int	i;
	char	tmpRealFileName[256];


	// alloc()
	if( (G_StoreList = (struct script_file *)calloc(G_StoreFileCnt, sizeof(struct script_file))) == NULL )
	{
		dprintf(M_LOG, "[ DB ]Error D_set_store() calloc() : %s \n", strerror(errno) );
		return FAIL;
	}


	for( i=0; i<G_StoreFileCnt; i ++ )
	{

		sprintf( tmpRealFileName, "./script/store%02d.txt", (i+1) );
		//dprintf(M_LOG, "\t[ DB ]Loading.......... %s \n", tmpRealFileName);


		if( U_get_length(tmpRealFileName, "r", &G_StoreList[i].size) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_store() read file_length \n");
			return FAIL;
		}

		if( (G_StoreList[i].data = (char *)calloc(G_StoreList[i].size, SIZEOF_CHAR)) == NULL )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_store() calloc() : %s \n", strerror(errno) );
			return FAIL;
		}

		if( U_get_data(tmpRealFileName, "r", G_StoreList[i].size, G_StoreList[i].data) )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_store() read file_data \n");
			return FAIL;
		}

		if( (G_StoreList[i].line_cnt = U_get_line(tmpRealFileName, "r")) < 1 )
		{
			dprintf(M_LOG, "[ DB ]Error D_set_store() read file_line_cnt\n");
			return FAIL;
		}
	}

	return SUCC;
}





// --------------------------------------------------------------------------
//Server List 
int	D_ProSL_init( void )
{
	int	i;

	pthread_mutex_init( &D_lock, NULL );

	for( i=0; i<MAX_RECVT_CNT; i++ )
	{
		D_SetSL_login( i, 0 );
		D_SetSL_fd( i, FD_IS_EMPTY );
	}

	return SUCC;
}


void	D_SetSL_fd( const int TIDX, const int FD )
{
	D_server[TIDX].fd	= FD;
}


int	D_GetSL_fd( const int TIDX )
{
	return (D_server[TIDX].fd);
}


void	D_SetSL_ipaddress( const int TIDX, const char *ipaddress )
{
	strncpy( D_server[TIDX].ipaddress, ipaddress, SIZEOF_IP );
}


char *	D_GetSL_ipaddress( const int TIDX )
{
	return (D_server[TIDX].ipaddress);
}


void	D_SetSL_login( const int TIDX, const int value )
{
	D_server[TIDX].login	= value;
}


int	D_GetSL_login( const int TIDX )
{
	return (D_server[TIDX].login);
}


int	D_SLMysql_Init( const int TIDX )
{
	D_SLMysql_Close( TIDX );

	if( D_Mysql_Init(TIDX) )
		return FAIL;

	return SUCC;
}


int	D_SLMysql_Close( const int TIDX )
{
	D_Mysql_Close( TIDX );

	return SUCC;
}





// --------------------------------------------------------------------------
//Mysql
int	D_Mysql_CheckIDX( const int TIDX )
{
	if( (TIDX < 0) || (TIDX > (MAX_RECVT_CNT-1)) )
	{
		dprintf(TIDX, "[ DB ]Error argument tidx(%d) \n", TIDX );
		return FAIL;
	}

	if( mysql_ping(&D_mysql[TIDX]) )
	{
		D_Mysql_Close( TIDX );

		if( D_Mysql_Init(TIDX) )
			return FAIL;
	}

	return SUCC;
}


int	D_Mysql_Init( const int TIDX )
{
	unsigned int	timeout;


	if( (TIDX < 0) || (TIDX > (MAX_RECVT_CNT-1)) )
	{
		dprintf(TIDX, "[ DB ]Error argument idx(%d) \n", TIDX );
		return FAIL;
	}

	pthread_mutex_lock(&D_lock);

		mysql_init(&D_mysql[TIDX]);

		timeout	= 3;
		mysql_options(&D_mysql[TIDX], MYSQL_OPT_CONNECT_TIMEOUT, (char *)&timeout);
		mysql_options(&D_mysql[TIDX], MYSQL_SET_CHARSET_NAME, "euckr");

		if( !mysql_real_connect(&D_mysql[TIDX], G_script_db.ipaddress, G_script_db.id
			, G_script_db.pwd, G_script_db.name, 0, MYSQL_UNIX_ADDR, 0) )
		{
			dprintf(TIDX, "[ DB ]Error mysql_real_connect() %s \n ", mysql_error(&D_mysql[TIDX]) );
			pthread_mutex_unlock(&D_lock);
			return FAIL;
		}

	pthread_mutex_unlock(&D_lock);

	return SUCC;
}


int	D_Mysql_Close( const int TIDX )
{
	if( (TIDX < 0) || (TIDX > (MAX_RECVT_CNT - 1)) )
	{
		dprintf(TIDX, "[ DB ]Error argument idx=%d \n", TIDX );
		return FAIL;
	}

	pthread_mutex_lock(&D_lock);

		mysql_close(&D_mysql[TIDX]);

	pthread_mutex_unlock(&D_lock);

	return SUCC;
}


int	D_Mysql_version( const int TIDX, char *p_game_version )
{
	MYSQL_ROW	row;
	MYSQL_RES	*res;
	char		str_query[1024];
	int		n, version_count;


	if( D_Mysql_CheckIDX(TIDX) )
		return FAIL;


	n = snprintf( str_query, 1024, "select version from %s", DB_TBL_VERSION );
	//dprintf(TIDX, "[ DB ]query = %s \n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX], str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s\n", mysql_error(&D_mysql[TIDX]) );
		return FAIL;
	}

	if( !(res = mysql_store_result(&D_mysql[TIDX])) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_store_result() : %s\n", mysql_error(&D_mysql[TIDX]) );
		return FAIL;
	}

	if( (version_count = mysql_num_rows(res)) != 1 )
	{
		dprintf(TIDX, "[ DB ]Error version count(%d)\n", version_count );
		mysql_free_result(res);
		return FAIL;
	}

	if( !(row = mysql_fetch_row(res)) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_fetch_row() : %s \n", mysql_error(&D_mysql[TIDX]) );
		mysql_free_result(res);
		return FAIL;
	}

	strncpy( p_game_version, row[0], (VERSION_LEN + 1) );

	mysql_free_result(res);
	
	return SUCC;
}


int	D_WS_MYINFO( const int TIDX, const char *p_ipaddress, int *p_game, int *p_world )
{
	MYSQL_ROW	row;
	MYSQL_RES	*res;
	int		n, info_cnt;
	char		str_query[1024];


	if( D_Mysql_CheckIDX(TIDX) )
		return FAIL;


	n = snprintf( str_query, 1024, "select gamecode, world from %s where ip = '%s'", DB_TBL_SERVER_IDX, p_ipaddress );
	dprintf(TIDX, "[ DB ]query = %s \n", str_query); 
	
	if( mysql_real_query(&D_mysql[TIDX], str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s\n", mysql_error(&D_mysql[TIDX]) );
		return FAIL;
	}

	if( !(res = mysql_store_result(&D_mysql[TIDX])) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_store_result() : %s\n", mysql_error(&D_mysql[TIDX]) );
		return FAIL;
	}

	if( (info_cnt = mysql_num_rows(res)) != 1 )
	{
		dprintf(TIDX, "[ DB ]Error info_cnt(%d)\n", info_cnt );
		mysql_free_result(res);
		return FAIL;
	}

	if( !(row = mysql_fetch_row(res)) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_fetch_row() : %s \n", mysql_error(&D_mysql[TIDX]) );
		mysql_free_result(res);
		return FAIL;
	}


	*p_game		= atoi( row[0] );
	*p_world	= atoi( row[1] );

	mysql_free_result(res);

	//dprintf(TIDX, "[ DB ]Success D_WS_MYINFO() : ipaddress(%s), game(%d), world(%d) \n", p_ipaddress, *p_game, *p_world );
	return SUCC;
}


int	D_WS_WORLD_INFO( const int TIDX, const int game, int *p_info_cnt, char *p_buf )
{
	MYSQL_ROW	row;
	MYSQL_RES	*res;
	char		str_query[1024];
	int		n, info_cnt, i;
	int		tmpLen, tmpInt;


	*p_info_cnt	= 0;


	if( D_Mysql_CheckIDX(TIDX) )
		return FAIL;


	n = snprintf( str_query, 1024, "select world, world_name from %s where gamecode=%d ", DB_TBL_WORLD, game);
	dprintf(TIDX, "[ DB ]query = %s \n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX], str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX]) );
		return FAIL;
	}

	if( !(res = mysql_store_result(&D_mysql[TIDX])) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_store_result() : %s\n", mysql_error(&D_mysql[TIDX]) );
		return FAIL;
	}

	if( ((info_cnt = mysql_num_rows(res)) < 1) || (info_cnt > 256) )
	{
		dprintf(TIDX, "[ DB ]Error info_cnt(%d)\n", info_cnt );
		mysql_free_result(res);
		return FAIL;
	}

	for( i=0, tmpLen=0; i<info_cnt; i ++ )
	{
		if( !(row = mysql_fetch_row(res)) )
		{
			dprintf(TIDX, "[ DB ]Error mysql_fetch_row() \n");
			mysql_free_result(res);
			return FAIL;
		}

		// world
		tmpInt		= atoi(row[0]);
		memcpy( (p_buf + tmpLen), &tmpInt, SIZEOF_INT );
		tmpLen		+= SIZEOF_INT;

		// world_name
		strncpy( (p_buf + tmpLen), row[1], (WORLD_NAME_LEN + 1) );
		tmpLen		+= (WORLD_NAME_LEN + 1);

		//dprintf(TIDX, "\t-> world(%d), world_name(%s) \n", atoi(row[0]), row[1] );
	}

	mysql_free_result(res);

	*p_info_cnt	= info_cnt;

	//dprintf(TIDX, "[ DB ]Success D_WS_WORLD_INFO() game(%d), cnt(%d) \n", game, *p_info_cnt );
	return SUCC;
}


int	D_WS_GAMESERV_INFO( const int TIDX, const int game, const int world, int *p_info_cnt, char *p_buf )
{
	MYSQL_ROW	row;
	MYSQL_RES	*res;
	char		str_query[1024];
	int		n, info_cnt, i;
	unsigned short	tmpLen;


	*p_info_cnt	= 0;


	if( D_Mysql_CheckIDX(TIDX) )
		return FAIL;


	n = snprintf( str_query, 1024, "select ip, outer_ip from %s where gamecode=%d and world=%d ", DB_TBL_SERVER_IDX, game, world );
	dprintf(TIDX, "[ DB ]query = %s \n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX], str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX]) );
		return FAIL;
	}

	if( !(res = mysql_store_result(&D_mysql[TIDX])) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_store_result() : %s\n", mysql_error(&D_mysql[TIDX]) );
		return FAIL;
	}

	if( ((info_cnt = mysql_num_rows(res)) < 1) || (info_cnt > 256) )
	{
		dprintf(TIDX, "[ DB ]Error info_cnt(%d)\n", info_cnt );
		mysql_free_result(res);
		return FAIL;
	}

	for( i=0, tmpLen=0; i<info_cnt; i ++ )
	{
		if( !(row = mysql_fetch_row(res)) )
		{
			dprintf(TIDX, "[ DB ]Error mysql_fetch_row() \n");
			mysql_free_result(res);
			return FAIL;
		}

		strncpy( (p_buf + tmpLen), row[0], (SIZEOF_IP + 1) );
		tmpLen		+= (SIZEOF_IP + 1);

		strncpy( (p_buf + tmpLen), row[1], (SIZEOF_IP + 1) );
		tmpLen		+= (SIZEOF_IP + 1);

		//dprintf(TIDX, "\t-> inner_ip(%s), outer_ip(%s) \n", row[0], row[1] );
	}

	mysql_free_result(res);

	*p_info_cnt	= info_cnt;

	//dprintf(TIDX, "[ DB ]Success D_WS_GAMESERV_INFO() game(%d), world(%d), cnt(%d) \n", game, world, *p_info_cnt );
	return SUCC;
}


int	D_WS_DBAGENT_INFO( const int TIDX, const int game, const int world, int *p_info_cnt, char *p_buf )
{
	MYSQL_ROW	row;
	MYSQL_RES	*res;
	char		str_query[1024];
	int		n, info_cnt, i;
	unsigned short	tmpLen;


	*p_info_cnt	= 0;


	if( D_Mysql_CheckIDX(TIDX) )
		return FAIL;


	n = snprintf( str_query, 1024, "select ip, world from %s where gamecode=%d and world=%d", DB_TBL_DBAGENT, game, world );
	dprintf(TIDX, "[ DB ]query = %s \n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX], str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX]) );
		return FAIL;
	}

	if( !(res = mysql_store_result(&D_mysql[TIDX])) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_store_result() : %s\n", mysql_error(&D_mysql[TIDX]) );
		return FAIL;
	}

	if( ((info_cnt = mysql_num_rows(res)) < 1) || (info_cnt > 256) )
	{
		dprintf(TIDX, "[ DB ]Error info_cnt(%d)\n", info_cnt );
		mysql_free_result(res);
		return FAIL;
	}

	for( i=0, tmpLen=0; i<info_cnt; i ++ )
	{
		if( !(row = mysql_fetch_row(res)) )
		{
			dprintf(TIDX, "[ DB ]Error mysql_fetch_row() \n");
			mysql_free_result(res);
			return FAIL;
		}

		strncpy( (p_buf + tmpLen), row[0], (SIZEOF_IP + 1) );
		tmpLen		+= (SIZEOF_IP + 1);

		//dprintf(TIDX, "\t-> agentserv_ip(%s) \n", row[0] );
	}

	mysql_free_result(res);

	*p_info_cnt	= info_cnt;

	//dprintf(TIDX, "[ DB ]Success D_WS_DBAGENT_INFO() game(%d), world(%d), cnt(%d) \n", game, world, *p_info_cnt );
	return SUCC;
}


int	D_WS_MOBSERV_INFO( const int TIDX, const int game, const int world, int *p_info_cnt, char *p_buf )
{
	MYSQL_ROW	row;
	MYSQL_RES	*res;
	char		str_query[1024];
	int		n, info_cnt, i;
	unsigned short	tmpLen;


	*p_info_cnt	= 0;


	if( D_Mysql_CheckIDX(TIDX) )
		return FAIL;


	n = snprintf( str_query, 1024, "select ip, world from %s where gamecode=%d and world=%d", DB_TBL_MOBSERV, game, world );
	dprintf(TIDX, "[ DB ]query = %s \n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX], str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX]) );
		return FAIL;
	}

	if( !(res = mysql_store_result(&D_mysql[TIDX])) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_store_result() : %s\n", mysql_error(&D_mysql[TIDX]) );
		return FAIL;
	}

	if( ((info_cnt = mysql_num_rows(res)) < 1) || (info_cnt > 256) )
	{
		dprintf(TIDX, "[ DB ]Error info_cnt(%d)\n", info_cnt );
		mysql_free_result(res);
		return FAIL;
	}

	for( i=0, tmpLen=0; i<info_cnt; i ++ )
	{
		if( !(row = mysql_fetch_row(res)) )
		{
			dprintf(TIDX, "[ DB ]Error mysql_fetch_row() \n");
			mysql_free_result(res);
			return FAIL;
		}

		strncpy( (p_buf + tmpLen), row[0], (SIZEOF_IP + 1) );
		tmpLen		+= (SIZEOF_IP + 1);

		//dprintf(TIDX, "\t-> mobserv_ip(%s) \n", row[0] );
	}

	mysql_free_result(res);

	*p_info_cnt	= info_cnt;

	//dprintf(TIDX, "[ DB ]Success D_WS_MOBSERV_INFO() game(%d), world(%d), cnt(%d) \n", game, world, *p_info_cnt );
	return SUCC;
}
