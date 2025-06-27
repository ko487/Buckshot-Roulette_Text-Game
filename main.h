#pragma once
#define GLASS 0
#define SAW 1
#define BEER 2
#define CIGAR 3
#define TYPES_OF_ITEMS 4	// 아이템의 종류 수
#define NAME_SIZE 20

#define MAX_BULLET 5		// 장전 가능한 최대 총알 개수
#define MAX_ITEM 5			// 분배 가능한 최대 아이템 수
#define MAX_HP 5			// 배정 가능한 최대 생명

#define REAL_BULLET 1		// 실탄
#define IMAGINAL_BULLET 0	// 불발탄


typedef struct player {
	char name[NAME_SIZE];		// player name
	int hp;						// player hp
	int item[TYPES_OF_ITEMS];	// item list
	int max_hp;					// player가 얻을 수 있는 최대 hp
	unsigned int money;			// player가 취득한 돈
}player_t;

typedef struct bullet {
	int bullet[MAX_BULLET];	// 탄의 list, 각 원소는 데미지 (1이면 실탄, 0이면 불발탄)
	int real_bullet;		// 실탄의 개수
	int imaginal_bullet;	// 불발탄의 개수
	int index;				// 현재 총알 위치
	int MAX_index;			// 총알의 개수
}bullet_t;

typedef struct data {		// 랭킹 표시를 위한 구조체
	char name[20];			// 이름
	unsigned int money;		// 돈
	struct data* link;		// 다음 노드를 가르킬 포인터
}data_t;


void start_game(char *name);
void print_initial_interface();
void print_rule();
void print_interface(player_t* user, player_t* dealer, bullet_t* bullet);
void init_bullet(bullet_t* bullet);
void user_turn(player_t* user, player_t* dealer, bullet_t* bullet);
void dealer_turn(player_t* user, player_t* dealer, bullet_t* bullet);
void init_player(player_t* player, int heart, int num_of_items);
void game(player_t* user, player_t* dealer, bullet_t* bullet);
void phase(player_t* user, player_t* dealer, bullet_t* bullet);
void end_game(player_t* user, player_t* dealer, bullet_t* bullet);
player_t* get_item(player_t* player, int num_of_items);
player_t* use_item(player_t* player, bullet_t* bullet, int item);
int fire(player_t* target, bullet_t* bullet);


// 추가 목표
int get_heart();			// 제한된 MAX 안에서, 이번 게임에서 사용할 hp
int get_num_of_items();		// 제한된 MAX 안에서, 이번 라운드에 사용할 아이템의 수
int get_num_of_bullet();	// 제한된 MAX 안에서, 이번 라운드에 사용할 총알의 수
void save_into_database(player_t* user);
void user_win(player_t* user, player_t* dealer, bullet_t* bullet);
void next_game(player_t* user, player_t* dealer, bullet_t* bullet);
void print_ranking();
data_t* insert_node(data_t* head, char* name, unsigned int money);
void print_all_node(data_t* head);
void delete_all_node(data_t* head);
