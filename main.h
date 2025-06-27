#pragma once
#define GLASS 0
#define SAW 1
#define BEER 2
#define CIGAR 3
#define TYPES_OF_ITEMS 4	// �������� ���� ��
#define NAME_SIZE 20

#define MAX_BULLET 5		// ���� ������ �ִ� �Ѿ� ����
#define MAX_ITEM 5			// �й� ������ �ִ� ������ ��
#define MAX_HP 5			// ���� ������ �ִ� ����

#define REAL_BULLET 1		// ��ź
#define IMAGINAL_BULLET 0	// �ҹ�ź


typedef struct player {
	char name[NAME_SIZE];		// player name
	int hp;						// player hp
	int item[TYPES_OF_ITEMS];	// item list
	int max_hp;					// player�� ���� �� �ִ� �ִ� hp
	unsigned int money;			// player�� ����� ��
}player_t;

typedef struct bullet {
	int bullet[MAX_BULLET];	// ź�� list, �� ���Ҵ� ������ (1�̸� ��ź, 0�̸� �ҹ�ź)
	int real_bullet;		// ��ź�� ����
	int imaginal_bullet;	// �ҹ�ź�� ����
	int index;				// ���� �Ѿ� ��ġ
	int MAX_index;			// �Ѿ��� ����
}bullet_t;

typedef struct data {		// ��ŷ ǥ�ø� ���� ����ü
	char name[20];			// �̸�
	unsigned int money;		// ��
	struct data* link;		// ���� ��带 ����ų ������
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


// �߰� ��ǥ
int get_heart();			// ���ѵ� MAX �ȿ���, �̹� ���ӿ��� ����� hp
int get_num_of_items();		// ���ѵ� MAX �ȿ���, �̹� ���忡 ����� �������� ��
int get_num_of_bullet();	// ���ѵ� MAX �ȿ���, �̹� ���忡 ����� �Ѿ��� ��
void save_into_database(player_t* user);
void user_win(player_t* user, player_t* dealer, bullet_t* bullet);
void next_game(player_t* user, player_t* dealer, bullet_t* bullet);
void print_ranking();
data_t* insert_node(data_t* head, char* name, unsigned int money);
void print_all_node(data_t* head);
void delete_all_node(data_t* head);
