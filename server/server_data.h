#ifndef __SERVERDATA_H__
#define __SERVERDATA_H__

#define SREFUSE 0
#define SRLINK	1
#define SGIVE_TABLE	2
#define SCREAT_GAME 3
#define SASK_PLAYER 4
#define SRETURN_BATTLE 5
#define SRETURN_WINNER 6
#define SCHAT	7

// a game's result
#define WIN 1
#define FAIL 0
#define EQUAL 2

// a client's state
#define NOT_ONLINE 0
#define ONLINE 1
#define FIGHTING 2    // playing with someone

#define TIME_LIMIT 5 // 10 seconds

#pragma pack(1)
struct return_data{
	char lifetime;     //Ê£ÓàÉúÃüÊý
	char pk_stuff[STUFF_LEN];
	char win_station;
};

struct player_data{
	char station;     // NOT_ONLINE Or ONLINE Or FIGHTING
	char id[ID_LEN];
};

struct server_data{
	char station;            //0,½ûÖ¹·ÃÎÊ£¬1£¬ÑéÖ¤Í¨¹ý£¬2£¬Ë¢ÐÂÓÃ»§ÁÐ±í£¬3£¬pkÁ¬½Ó½¨Á¢£¬4£¬pk·´À¡£¬5£¬ÎÊÑ¯£¬6£¬Í¨ÖªÊ¤¸º£¬7£¬ÁÄÌì×ª·¢
	char another_id[ID_LEN];
	return_data returndata;
	
	char game_over[PROMPT_LEN];      // when the game is over, the client should print this string
	char game_station;       // WIN Or FAIL Or Equal
	
	player_data players[MAX_PLAYERS_NUM];  
	char saying[SAYING_LEN];
};
#pragma pack()

typedef struct server_data server_data;
typedef struct return_data return_data;
typedef struct player_data player_data;

static inline void set_player(player_data *p, uint8_t s, char *id) {
	if(p == NULL) return;

	p->station = s;
	if(id == NULL)
		(p->id)[0] = '\0';
	else
		strncpy(p->id, id, ID_LEN);
}
/*
 * Set the station field in a server_data
 */
inline void set_sd_station(server_data *p, uint8_t station) {
	if(p == NULL) return;

	p->station = station;
}

/*
 * Set the another_id field in a server_data
 */
inline void set_sd_rid(server_data *p, char *rival_id) {
	if(p == NULL) return;

    if(rival_id == NULL)
		p->another_id[0] = '\0';
	else
	    strncpy(p->another_id, rival_id, ID_LEN);
}

/*
 * Set the returndata field in a server_data
 */
inline void set_sd_rd(server_data *p, uint8_t lifetime, char *stuff, uint8_t win) {
	if(p == NULL) return;

	return_data *t = &(p->returndata);

	t->lifetime = lifetime;
	if( stuff == NULL )
		t->pk_stuff[0] = '\0';
	else
		strncpy(t->pk_stuff, stuff, STUFF_LEN);
}

/*
 * Set the game_over field in a server_data
 */
inline void set_sd_over(server_data *p, uint8_t s, char *prompt) {
	if( p == NULL ) return;

	p->game_station = s;

	if(prompt == NULL)
		p->game_over[0] = '\0';
	else
		strncpy(p->game_over, prompt, PROMPT_LEN);
}

/*
 * Set the saying field in a server_data
 */
inline void set_sd_saying(server_data *p, char *say) {
	if(p == NULL) return;

	if( say == NULL )
		p->saying[0] = '\0';
	else 
		strncpy(p->saying, say, SAYING_LEN);
}

#endif
