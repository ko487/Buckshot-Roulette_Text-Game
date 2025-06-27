#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "main.h"


int main() {
	char name[NAME_SIZE];
	printf("어서 오세요. 이름을 입력해주십시오.\n");

	scanf("%s", name);
	getchar();

	while (1) {
		int choice = 0;

		srand((unsigned int)time(NULL));

		print_initial_interface();

		scanf("%d", &choice);
		getchar();

		switch (choice) { // 1. 게임 시작 2. 룰 설명 3. 랭킹
		case 1:
			start_game(name);
			return 0;
		case 2:
			print_rule();
			break;
		case 3:
			print_ranking();
			break;
		case 4:
			printf("종료합니다.\n");
			return 0;

		default:
			printf("잘못된 입력입니다.\n");
			break;
		}
	}
	
	return 0;
}

void delete_all_node(data_t* head) {
	data_t* prev = NULL, * curr = head;

	if (head == NULL) {
		printf("삭제할 데이터가 없습니다.\n");
		return;
	}

	while (curr) {
		prev = curr;
		curr = curr->link;
		free(prev);
	}
}

void print_all_node(data_t* head) {
	int i = 1;

	while (head) {
		printf("%d위 : %s	$ : %u\n", i, head->name, head->money);
		head = head->link;
		i++;
	}
}

void print_ranking() {
	FILE* fp;
	data_t* head = NULL; // 데이터를 담을 헤드

	char name[NAME_SIZE];
	int money = 0;

	fp = fopen("database.txt", "r");
	if (fp == NULL) {
		printf("파일 오픈 실패\n");
		return;
	}

	while (fscanf(fp, "%s %u", name, &money) == 2) {
		head = insert_node(head, name, money);
	}

	if (head == NULL) {
		printf("출력할 데이터가 없습니다.\n");
		return;
	}

	printf("==================================\n");
	printf("Ranking\n\n");

	print_all_node(head);
	delete_all_node(head);

	printf("==================================\n\n");

	fclose(fp);
	return;
}

data_t* insert_node(data_t* head, char* name, unsigned int money) {

	data_t* temp = (data_t*)malloc(sizeof(data_t));
	data_t* prev = NULL, * curr = head;

	strcpy(temp->name, name);
	temp->money = money;
	temp->link = NULL;

	if (head == NULL) {	// head가 비었을 때
		head = temp;
		return head;
	}

	// 연결은 내림차순.
	// 자신 보다 작은 money를 가진 노드 앞에 연결
	while (curr) {
		if (temp->money > curr->money) {
			if (prev == NULL) { // curr가 헤드라면
				head = temp;
				temp->link = curr;
				return head;
			}
			else {	// curr 앞에 연결 필요
				prev->link = temp;
				temp->link = curr;
				return head;
			}
		}
		prev = curr;
		curr = curr->link;
	}
	// curr가 널이라면
	prev->link = temp;

	return head;
}

void start_game(char* name) {

	player_t user_origin, dealer_origin;
	bullet_t bullet_origin;
	int current_max_hp = 0;

	strcpy(user_origin.name, name);
	strcpy(dealer_origin.name, "dealer");

	player_t* user = &user_origin, * dealer = &dealer_origin;
	bullet_t* bullet = &bullet_origin;

	current_max_hp = get_heart();

	// 초기화
	init_player(user, current_max_hp, 0);
	init_player(dealer, current_max_hp, 0);

	printf("게임을 시작합니다.\n\n");

	game(user, dealer, bullet);
}

void game(player_t* user, player_t* dealer, bullet_t* bullet) {
	int num_of_items = 0;

	while (user->hp > 0 && dealer->hp > 0) { // 둘 중 한명이 사망 때까지 루프

		init_bullet(bullet);
		num_of_items = get_num_of_items();		// 이번 장전에 배정할 아이템의 수			
		user = get_item(user, num_of_items);	// 새롭게 아이템을 부여받고 시작
		dealer = get_item(dealer, num_of_items);

		Sleep(1000);
		printf("탄을 장전합니다.\n");
		Sleep(1000);
		printf("아이템을 분배합니다.\n\n");

		phase(user, dealer, bullet);

		printf("모든 탄알을 소모하였습니다.\n");
		Sleep(3000);
	}
	end_game(user, dealer, bullet);
}

void phase(player_t* user, player_t* dealer, bullet_t* bullet) {
	while (bullet->index < bullet->MAX_index && user->hp > 0 && dealer->hp > 0) { // 한 게임
		Sleep(2000);

		user_turn(user, dealer, bullet);

		Sleep(2000);

		dealer_turn(user, dealer, bullet);
	}
}

void end_game(player_t* user, player_t* dealer, bullet_t* bullet) {

	printf("게임이 끝났습니다!\n\n");
	Sleep(1000);
	printf("승자는.");
	Sleep(500);
	putchar('.');
	Sleep(500);
	putchar('.');
	Sleep(1000);

	if (user->hp <= 0) {
		printf("dealer입니다!\n");
		Sleep(1000);
		printf("얻은 돈을 모두 잃으셨습니다!\n");
		exit(0);
	}
	else {	// 유저의 승리
		user_win(user, dealer, bullet);
	}
	exit(0);
}

void user_win(player_t* user, player_t* dealer, bullet_t* bullet) {

	int answer = 0;

	printf("%s입니다!\n", user->name);
	Sleep(1000);

	if (user->money == 0) {
		user->money = 10000;
		Sleep(1000);
		printf("%s님은 10,000$을 획득하셨습니다.\n", user->name);
	}
	else {
		printf("판돈이 2배가 됩니다!\n\n");
		user->money = user->money * 2;
	}

	Sleep(1000);
	printf("현재까지 %s님이 획득하신 돈은 총 %u$입니다.\n\n", user->name, user->money);
	Sleep(1000);
	printf("계속하시겠습니까?\n 1. 계속	2. 종료\n### 승리 시 판돈이 2배씩 증가 합니다.###\n");

	scanf("%d", &answer);

	switch (answer) {
	case 1:
		next_game(user, dealer, bullet);
		break;
	default:
		printf("전적을 저장합니다.\n");
		save_into_database(user);
		Sleep(1000);
		printf("게임을 종료합니다.\n");
		Sleep(1000);
		break;
	}
}

void next_game(player_t* user, player_t* dealer, bullet_t* bullet) {
	int money = user->money;
	int current_max_hp = get_heart();
	int num_of_items = get_num_of_items();

	init_player(user, current_max_hp, num_of_items);
	init_player(dealer, current_max_hp, num_of_items);

	user->money = money;

	printf("게임을 재개합니다.\n");
	Sleep(1000);
	printf("행운을 빕니다.\n\n");
	game(user, dealer, bullet);
}

void save_into_database(player_t* user) {
	FILE* fp;

	fp = fopen("database.txt", "a");
	if (fp == NULL)	// file stream 실패
		return;

	fprintf(fp, "%s %u\n", user->name, user->money);
	fclose(fp);
}

int get_num_of_bullet() {	// 2~MAX_BULLET 사이의 수만 return
	return (rand() % (MAX_BULLET - 1)) + 2;
}

int get_num_of_items() {
	return rand() % MAX_ITEM + 1;
}

int get_heart() {
	return rand() % MAX_HP + 1;
}

player_t* use_item(player_t* player, bullet_t* bullet, int item) {

	char present_bullet[][10] = { "불발", "실", "실" };
	char item_list[][10] = { "돋보기", "톱", "맥주", "시가" };

	printf("%s 사용!\n\n", item_list[item]);
	Sleep(1500);

	if (player->item[item] == 0) { // 해당 아이템이 없다면
		printf("%s가 없습니다.\n\n", item_list[item]);
		return player;
	}

	switch (item) {
	case GLASS:
		printf("현재 장전된 탄알은 %s탄입니다.\n\n", present_bullet[bullet->bullet[bullet->index]]);
		break;
	case SAW:
		if (bullet->bullet[bullet->index] >= 2) {
			printf("이미 톱이 사용되었습니다.\n\n");
			break;
		}
		printf("현재 장전된 탄알의 데미지가 2배가 됩니다!\n\n");
		bullet->bullet[bullet->index] = bullet->bullet[bullet->index] * 2;
		break;
	case BEER:
		printf("현재 탄알을 소비합니다.\n");
		Sleep(1000);
		printf("현재 탄알은 %s탄입니다.\n\n", present_bullet[bullet->bullet[bullet->index]]);
		if (bullet->bullet[bullet->index])
			bullet->real_bullet = bullet->real_bullet - 1;
		else
			bullet->imaginal_bullet = bullet->imaginal_bullet - 1;
		bullet->index = bullet->index + 1;
		break;
	case CIGAR:
		if (player->hp < player->max_hp)
			player->hp = player->hp + 1;
		else
			printf("이미 Full HP입니다.\n\n");
		break;
	default:
		printf("없는 아이템입니다.\n\n");
		break;
	}

	player->item[item] = player->item[item] - 1; // item 소모
	Sleep(1500);
	return player;
}

int fire(player_t* target, bullet_t* bullet) {
	int present_bullet = 0;

	printf("%s에게 발포!\n", target->name);
	Sleep(500);
	printf("fire!\n\n");
	Sleep(1000);

	if (bullet->bullet[bullet->index]) {
		printf("이번 탄은 실탄입니다!\n\n");
		present_bullet = REAL_BULLET;
		bullet->real_bullet = bullet->real_bullet - 1;
	}
	else {
		printf("이번 탄은 불발탄입니다!\n\n");
		present_bullet = IMAGINAL_BULLET;
		bullet->imaginal_bullet = bullet->imaginal_bullet - 1;
	}
	Sleep(1000);
	printf("%s은 %d의 데미지를 입습니다!\n\n", target->name, bullet->bullet[bullet->index]);
	target->hp = target->hp - bullet->bullet[bullet->index];
	bullet->index = bullet->index + 1;
	Sleep(1000);

	return present_bullet;
}

void dealer_turn(player_t* user, player_t* dealer, bullet_t* bullet) {

	system("cls");
	printf("%s의 턴입니다.\n", dealer->name);
	printf("==========================\n");
	Sleep(500);

	while (bullet->index < bullet->MAX_index && dealer->hp > 0 && user->hp > 0) { // 총알을 모두 소모할 때까지

		double percent = (double)bullet->real_bullet / (bullet->MAX_index - bullet->index);

		if (dealer->hp <= 3 && dealer->item[CIGAR] > 0) { // hp가 3 이하이고, 시가가 있다면. 시가 사용
			use_item(dealer, bullet, CIGAR);
			continue;
		}

		if (percent >= 0.7 && dealer->item[SAW] > 0) { // 실탄일 확률이 70% 이상이고, 톱이 있다면. 톱 사용
			use_item(dealer, bullet, SAW);
			Sleep(1000);
			fire(user, bullet);
			break;
		}
		else if (percent >= 0.5) { // 실탄일 확률이 50% 이상이라면 발포
			fire(user, bullet);
			break;
		}
		else if (dealer->item[GLASS] > 0) { // 실탄일 확률이 50% 미만이고, 돋보기가 있다면
			if (bullet->bullet[bullet->index] >= REAL_BULLET) { // 실탄이라면
				use_item(dealer, bullet, GLASS);
				Sleep(1000);
				fire(user, bullet);
				break;
			}
			else {
				use_item(dealer, bullet, GLASS);
				Sleep(1000);
				fire(dealer, bullet);
				continue;
			}
		}
		else if (dealer->item[BEER] > 0) { // 실탄일 확률이 50% 미만이고, 맥주가 있다면
			use_item(dealer, bullet, BEER);
			continue;
		}
		else { // 실탄일 확률이 50% 미만이고, 아이템이 아무것도 없다면 자신 발포. 실탄이라면 턴이 넘어감.
			int present_bullet = fire(dealer, bullet);

			if (present_bullet)
				break;
			else
				continue;
		}
	}
	Sleep(1000);
	printf("==========================\n");
	Sleep(1000);
	printf("dealer의 턴이 끝났습니다.\n\n");
	Sleep(1000);
}

void user_turn(player_t* user, player_t* dealer, bullet_t* bullet) {

	system("cls");
	printf("%s의 턴입니다.\n", user->name);
	Sleep(1500);
	while (bullet->index < bullet->MAX_index && dealer->hp > 0 && user->hp > 0) { // 총알을 모두 소모할 때까지
		print_interface(user, dealer, bullet);

		int choice = 0;
		int present_bullet = 0;

		scanf("%d", &choice);

		switch (choice) {
		case 1:
			fire(dealer, bullet);
			return;
		case 2:
			present_bullet = fire(user, bullet);
			if (present_bullet)
				return; // 실탄이라면 턴이 넘어감.
			break; // 허탄이라면 다시 한번 본인의 턴
		case 3:
			user = use_item(user, bullet, GLASS);
			break;
		case 4:
			user = use_item(user, bullet, SAW);
			break;
		case 5:
			user = use_item(user, bullet, BEER);
			break;
		case 6:
			user = use_item(user, bullet, CIGAR);
			break;
		default:
			break;
		}
	}
}

void init_bullet(bullet_t* bullet) {

	int num_of_bullets = get_num_of_bullet();

	// 초기화
	for (int i = 0; i < MAX_BULLET; i++)
		bullet->bullet[i] = 0;

	bullet->real_bullet = 0;
	bullet->imaginal_bullet = 0;
	bullet->index = 0;
	bullet->MAX_index = num_of_bullets;

	// 총알 장전
	for (int i = 0; i < num_of_bullets; i++) {
		int rand_value = rand() % 2;
		bullet->bullet[i] = rand_value;

		if (rand_value)
			bullet->real_bullet = bullet->real_bullet + 1;
		else
			bullet->imaginal_bullet = bullet->imaginal_bullet + 1;
	}

	// 만약 실탄이 하나도 없다면 랜덤으로 한발을 실탄으로 변경. 반대도 동일
	if (bullet->real_bullet == 0) {
		int rand_value = rand() % num_of_bullets;
		bullet->bullet[rand_value] = 1;
		bullet->real_bullet = 1;
		bullet->imaginal_bullet = bullet->imaginal_bullet - 1;
	}
	else if (bullet->imaginal_bullet == 0) {
		int rand_value = rand() % num_of_bullets;
		bullet->bullet[rand_value] = 0;
		bullet->imaginal_bullet = 1;
		bullet->real_bullet = bullet->real_bullet - 1;
	}
}

void init_player(player_t* player, int current_max_hp, int num_of_items) {
	player->hp = current_max_hp;
	player->money = 0;
	player->max_hp = current_max_hp;
	player = get_item(player, num_of_items);
}

player_t* get_item(player_t* player, int num_of_items) {

	for (int i = 0; i < TYPES_OF_ITEMS; i++) // 원소 전부 0으로
		player->item[i] = 0;

	for (int i = 0; i < num_of_items; i++) { // 아이템 배정
		int j = rand() % TYPES_OF_ITEMS;
		player->item[j] = player->item[j] + 1;
	}
	return player;
}

void print_interface(player_t* user, player_t* dealer, bullet_t* bullet) {
	printf("=========================\n");
	printf("%s의 체력 : %d\n", user->name, user->hp);
	printf("%s의 체력 : %d\n", dealer->name, dealer->hp);
	printf("남은 탄알 갯수 : %d개\n", bullet->MAX_index - bullet->index);
	printf("총알 실탄 / 불발탄 : %d / %d\n\n", bullet->real_bullet, bullet->imaginal_bullet);
	printf("선택지\n");
	printf("1. 딜러에게 발포\n");
	printf("2. 자신에게 발포\n");
	printf("3. 돋보기 사용(개수 : %d)\n", user->item[GLASS]);
	printf("4. 톱 사용(개수 : %d)\n", user->item[SAW]);
	printf("5. 맥주 사용(개수 : %d)\n", user->item[BEER]);
	printf("6. 시가 사용(개수 : %d)\n", user->item[CIGAR]);
	printf("=========================\n");
}

void print_initial_interface() {
	printf("=========================\n");
	printf("1. 게임 시작\n");
	printf("2. 룰 설명\n");
	printf("3. 랭킹 확인\n");
	printf("4. 종료\n");
	printf("=========================\n\n");
}

void print_rule() {
	printf("Rules\n");
	Sleep(1000);
	printf("======================================================================\n\n");
	printf("1. 딜러와 당신이 샷건을 번갈아 쥐며 상대를 쏘거나, 자신을 쏩니다.\n");
	Sleep(1000);
	printf("2. 샷건에는 최대 %d개의 탄이 장전되어 있으며, 실탄과 불발탄이 랜덤한 순서로 장전됩니다.\n", MAX_BULLET);
	Sleep(1000);
	printf("3. 실탄이 발사되었을 시에는 무조건 턴이 넘어가며, \n자기 자신을 쏘았을 때 불발탄이라면 턴이 넘어가지 않고 이어갑니다.\n\n");
	Sleep(1000);
	printf("4. %d가지 아이템이 랜덤으로 주어집니다. 그 종류는 아래와 같습니다.\n", TYPES_OF_ITEMS);
	Sleep(1000);
	printf("4-1 돋보기 : 현재 장전된 탄을 볼 수 있습니다.\n");
	printf("4-2 톱 : 이번 장전된 탄의 데미지가 2배가 됩니다.(1번만 사용 가능)\n");
	printf("4-3 맥주 : 이번 장전된 탄을 쏘지 않고 소모합니다.(턴 유지)\n");
	printf("4-4 시가 : 생명 1개를 회복합니다.(최대치 초과 불가)\n\n");
	printf("======================================================================\n");
	Sleep(1000);
}