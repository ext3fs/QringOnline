

int	D_Mysql_WA_CHAR_SAVE( const int )
{
	char			str_query[MAX_QUERY];

	// basic_info
	n  = snprintf( str_query, 4096, "update char_table set load_cnt=%d, map_num=%d, x=%d, y=%d, z=%d, regen_map=%d, regen_x=%d, regen_y=%d, regen_z=%d, sex=%d, hair=%d, face=%d, level=%d, exp=%d, hp=%d, ap=%d, money=%d, skill_point=%d, skill1=%d, skill2=%d, skill3=%d, skill4=%d, skill5=%d, skill6=%d, skill7=%d, skill8=%d, skill9=%d, skill10=%d, skill11=%d, skill12=%d, skill13=%d, skill14=%d, skill15=%d, skill16=%d, skill17=%d, skill18=%d, skill19=%d, skill20=%d, skill21=%d, skill22=%d, skill23=%d, skill24=%d, skill25=%d, skill26=%d, skill27=%d, skill28=%d, skill29=%d, skill30=%d, skill31=%d, skill32=%d, skill33=%d, skill34=%d, skill35=%d, skill36=%d, skill37=%d, skill38=%d, skill39=%d, skill40=%d, skill41=%d, skill42=%d, skill43=%d, skill44=%d, skill45=%d, skill46=%d, skill47=%d, skill48=%d where user_id='%s' and char_name='"
		, req_body1->load_cnt
		, req_body1->map
		, req_body1->x
		, req_body1->y
		, req_body1->z
		, req_body1->regen_map
		, req_body1->regen_x
		, req_body1->regen_y
		, req_body1->regen_z
		, req_body1->sex
		, req_body1->hair
		, req_body1->face
		, req_body1->level
		, req_body1->exp
		, req_body1->hp
		, req_body1->ap
		, req_body1->money
		, req_body1->skill_point
		, req_body1->skill_slot[0]
		, req_body1->skill_slot[1]
		, req_body1->skill_slot[2]
		, req_body1->skill_slot[3]
		, req_body1->skill_slot[4]
		, req_body1->skill_slot[5]
		, req_body1->skill_slot[6]
		, req_body1->skill_slot[7]
		, req_body1->skill_slot[8]
		, req_body1->skill_slot[9]
		, req_body1->skill_slot[10]
		, req_body1->skill_slot[11]
		, req_body1->skill_slot[12]
		, req_body1->skill_slot[13]
		, req_body1->skill_slot[14]
		, req_body1->skill_slot[15]
		, req_body1->skill_slot[16]
		, req_body1->skill_slot[17]
		, req_body1->skill_slot[18]
		, req_body1->skill_slot[19]
		, req_body1->skill_slot[20]
		, req_body1->skill_slot[21]
		, req_body1->skill_slot[22]
		, req_body1->skill_slot[23]
		, req_body1->skill_slot[24]
		, req_body1->skill_slot[25]
		, req_body1->skill_slot[26]
		, req_body1->skill_slot[27]
		, req_body1->skill_slot[28]
		, req_body1->skill_slot[29]
		, req_body1->skill_slot[30]
		, req_body1->skill_slot[31]
		, req_body1->skill_slot[32]
		, req_body1->skill_slot[33]
		, req_body1->skill_slot[34]
		, req_body1->skill_slot[35]
		, req_body1->skill_slot[36]
		, req_body1->skill_slot[37]
		, req_body1->skill_slot[38]
		, req_body1->skill_slot[39]
		, req_body1->skill_slot[40]
		, req_body1->skill_slot[41]
		, req_body1->skill_slot[42]
		, req_body1->skill_slot[43]
		, req_body1->skill_slot[44]
		, req_body1->skill_slot[45]
		, req_body1->skill_slot[46]
		, req_body1->skill_slot[47]
		, req_body1->user_id );
	n += U_Change_escape_string( (str_query+n), req_body1->char_name, strlen(req_body1->char_name) );
	n += snprintf( (str_query+n), (MAX_QUERY-n), "' " );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	// item_insert
	for( i=0; i<req_body1->item_slot_cnt; i++ )
	{
		p_item	= (struct item_slot *)((char *)req_body2 + (i * sizeof(struct body2_WA_CHAR_SAVE)));

		if( D_Mysql_ItemInsert(TIDX, req_body1->user_id, req_body1->char_name, p_item, p_result_code) )
		{
			dprintf(TIDX, "[ DB ]Error D_Mysql_ItemInsert() user_id(%s), char_name(%s), item_slot(%d) \n"
				, req_body1->user_id, req_body1->char_name, p_item->item_slot );
		}
	}


	dprintf(TIDX, "[ DB ]Success char_save user_id(%s), char_name(%s), load_cnt(%d), item_slot_cnt(%d) \n"
		, req_body1->user_id, req_body1->char_name, req_body1->load_cnt, req_body1->item_slot_cnt );
	return SUCC;
}


static inline int	D_Mysql_ItemInsert( const int TIDX, const char *user_id, const char *char_name
			, const struct item_slot *p_item, int *p_result_code )
{
	char			str_query[MAX_QUERY];
	int			row_cnt, n;


	if( !(p_item->item1) && !(p_item->item2) && !(p_item->item3) && !(p_item->item4) )
		return SUCC;

	if( p_item->serial[0] == 0 )
	{
		n  = snprintf( str_query, MAX_QUERY, "insert into %s (char_name, user_id, position, item1, item2, item3, item4) values ('"
			, DB_TBL_ITEM );
		n += U_change_escape_string( (str_query+n), char_name, strlen(char_name) );
		n += snprintf( (str_query+n), (MAX_QUERY-n), "', '%s', %d, %d, %d, %d, %d )"
			, user_id, p_item->item_slot, p_item->item1, p_item->item2, p_item->item3, p_item->item4 );
	}
	else
	{
		n  = snprintf( str_query, MAX_QUERY, "insert into %s (char_name, user_id, position, serial, item1, item2, item3, item4) values ('"
			, DB_TBL_ITEM );
		n += U_change_escape_string( (str_query+n), char_name, strlen(char_name) );
		n += snprintf( (str_query+n), (MAX_QUERY-n), "', '%s', %d, '", user_id, p_item->item_slot );
		n += U_change_escape_string( (str_query+n), p_item->serial, ITEM_SERIAL_LEN );
		n += snprintf( (str_query+n), (MAX_QUERY-n), "', %d, %d, %d, %d)"	
			, p_item->item1, p_item->item2, p_item->item3, p_item->item4 );
	}

	dprintf(TIDX, "[ DB ] %s\n", str_query); 


	if( mysql_real_query(&D_mysql[TIDX].world, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	if( (row_cnt = mysql_affected_rows(&D_mysql[TIDX].world)) != 1 )
	{
		dprintf(TIDX, "[ DB ]Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	return SUCC;
}
