#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "main.h"


int main() {
	char name[NAME_SIZE];
	printf("� ������. �̸��� �Է����ֽʽÿ�.\n");

	scanf("%s", name);
	getchar();

	while (1) {
		int choice = 0;

		srand((unsigned int)time(NULL));

		print_initial_interface();

		scanf("%d", &choice);
		getchar();

		switch (choice) { // 1. ���� ���� 2. �� ���� 3. ��ŷ
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
			printf("�����մϴ�.\n");
			return 0;

		default:
			printf("�߸��� �Է��Դϴ�.\n");
			break;
		}
	}
	
	return 0;
}

void delete_all_node(data_t* head) {
	data_t* prev = NULL, * curr = head;

	if (head == NULL) {
		printf("������ �����Ͱ� �����ϴ�.\n");
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
		printf("%d�� : %s	$ : %u\n", i, head->name, head->money);
		head = head->link;
		i++;
	}
}

void print_ranking() {
	FILE* fp;
	data_t* head = NULL; // �����͸� ���� ���

	char name[NAME_SIZE];
	int money = 0;

	fp = fopen("database.txt", "r");
	if (fp == NULL) {
		printf("���� ���� ����\n");
		return;
	}

	while (fscanf(fp, "%s %u", name, &money) == 2) {
		head = insert_node(head, name, money);
	}

	if (head == NULL) {
		printf("����� �����Ͱ� �����ϴ�.\n");
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

	if (head == NULL) {	// head�� ����� ��
		head = temp;
		return head;
	}

	// ������ ��������.
	// �ڽ� ���� ���� money�� ���� ��� �տ� ����
	while (curr) {
		if (temp->money > curr->money) {
			if (prev == NULL) { // curr�� �����
				head = temp;
				temp->link = curr;
				return head;
			}
			else {	// curr �տ� ���� �ʿ�
				prev->link = temp;
				temp->link = curr;
				return head;
			}
		}
		prev = curr;
		curr = curr->link;
	}
	// curr�� ���̶��
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

	// �ʱ�ȭ
	init_player(user, current_max_hp, 0);
	init_player(dealer, current_max_hp, 0);

	printf("������ �����մϴ�.\n\n");

	game(user, dealer, bullet);
}

void game(player_t* user, player_t* dealer, bullet_t* bullet) {
	int num_of_items = 0;

	while (user->hp > 0 && dealer->hp > 0) { // �� �� �Ѹ��� ��� ������ ����

		init_bullet(bullet);
		num_of_items = get_num_of_items();		// �̹� ������ ������ �������� ��			
		user = get_item(user, num_of_items);	// ���Ӱ� �������� �ο��ް� ����
		dealer = get_item(dealer, num_of_items);

		Sleep(1000);
		printf("ź�� �����մϴ�.\n");
		Sleep(1000);
		printf("�������� �й��մϴ�.\n\n");

		phase(user, dealer, bullet);

		printf("��� ź���� �Ҹ��Ͽ����ϴ�.\n");
		Sleep(3000);
	}
	end_game(user, dealer, bullet);
}

void phase(player_t* user, player_t* dealer, bullet_t* bullet) {
	while (bullet->index < bullet->MAX_index && user->hp > 0 && dealer->hp > 0) { // �� ����
		Sleep(2000);

		user_turn(user, dealer, bullet);

		Sleep(2000);

		dealer_turn(user, dealer, bullet);
	}
}

void end_game(player_t* user, player_t* dealer, bullet_t* bullet) {

	printf("������ �������ϴ�!\n\n");
	Sleep(1000);
	printf("���ڴ�.");
	Sleep(500);
	putchar('.');
	Sleep(500);
	putchar('.');
	Sleep(1000);

	if (user->hp <= 0) {
		printf("dealer�Դϴ�!\n");
		Sleep(1000);
		printf("���� ���� ��� �����̽��ϴ�!\n");
		exit(0);
	}
	else {	// ������ �¸�
		user_win(user, dealer, bullet);
	}
	exit(0);
}

void user_win(player_t* user, player_t* dealer, bullet_t* bullet) {

	int answer = 0;

	printf("%s�Դϴ�!\n", user->name);
	Sleep(1000);

	if (user->money == 0) {
		user->money = 10000;
		Sleep(1000);
		printf("%s���� 10,000$�� ȹ���ϼ̽��ϴ�.\n", user->name);
	}
	else {
		printf("�ǵ��� 2�谡 �˴ϴ�!\n\n");
		user->money = user->money * 2;
	}

	Sleep(1000);
	printf("������� %s���� ȹ���Ͻ� ���� �� %u$�Դϴ�.\n\n", user->name, user->money);
	Sleep(1000);
	printf("����Ͻðڽ��ϱ�?\n 1. ���	2. ����\n### �¸� �� �ǵ��� 2�辿 ���� �մϴ�.###\n");

	scanf("%d", &answer);

	switch (answer) {
	case 1:
		next_game(user, dealer, bullet);
		break;
	default:
		printf("������ �����մϴ�.\n");
		save_into_database(user);
		Sleep(1000);
		printf("������ �����մϴ�.\n");
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

	printf("������ �簳�մϴ�.\n");
	Sleep(1000);
	printf("����� ���ϴ�.\n\n");
	game(user, dealer, bullet);
}

void save_into_database(player_t* user) {
	FILE* fp;

	fp = fopen("database.txt", "a");
	if (fp == NULL)	// file stream ����
		return;

	fprintf(fp, "%s %u\n", user->name, user->money);
	fclose(fp);
}

int get_num_of_bullet() {	// 2~MAX_BULLET ������ ���� return
	return (rand() % (MAX_BULLET - 1)) + 2;
}

int get_num_of_items() {
	return rand() % MAX_ITEM + 1;
}

int get_heart() {
	return rand() % MAX_HP + 1;
}

player_t* use_item(player_t* player, bullet_t* bullet, int item) {

	char present_bullet[][10] = { "�ҹ�", "��", "��" };
	char item_list[][10] = { "������", "��", "����", "�ð�" };

	printf("%s ���!\n\n", item_list[item]);
	Sleep(1500);

	if (player->item[item] == 0) { // �ش� �������� ���ٸ�
		printf("%s�� �����ϴ�.\n\n", item_list[item]);
		return player;
	}

	switch (item) {
	case GLASS:
		printf("���� ������ ź���� %sź�Դϴ�.\n\n", present_bullet[bullet->bullet[bullet->index]]);
		break;
	case SAW:
		if (bullet->bullet[bullet->index] >= 2) {
			printf("�̹� ���� ���Ǿ����ϴ�.\n\n");
			break;
		}
		printf("���� ������ ź���� �������� 2�谡 �˴ϴ�!\n\n");
		bullet->bullet[bullet->index] = bullet->bullet[bullet->index] * 2;
		break;
	case BEER:
		printf("���� ź���� �Һ��մϴ�.\n");
		Sleep(1000);
		printf("���� ź���� %sź�Դϴ�.\n\n", present_bullet[bullet->bullet[bullet->index]]);
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
			printf("�̹� Full HP�Դϴ�.\n\n");
		break;
	default:
		printf("���� �������Դϴ�.\n\n");
		break;
	}

	player->item[item] = player->item[item] - 1; // item �Ҹ�
	Sleep(1500);
	return player;
}

int fire(player_t* target, bullet_t* bullet) {
	int present_bullet = 0;

	printf("%s���� ����!\n", target->name);
	Sleep(500);
	printf("fire!\n\n");
	Sleep(1000);

	if (bullet->bullet[bullet->index]) {
		printf("�̹� ź�� ��ź�Դϴ�!\n\n");
		present_bullet = REAL_BULLET;
		bullet->real_bullet = bullet->real_bullet - 1;
	}
	else {
		printf("�̹� ź�� �ҹ�ź�Դϴ�!\n\n");
		present_bullet = IMAGINAL_BULLET;
		bullet->imaginal_bullet = bullet->imaginal_bullet - 1;
	}
	Sleep(1000);
	printf("%s�� %d�� �������� �Խ��ϴ�!\n\n", target->name, bullet->bullet[bullet->index]);
	target->hp = target->hp - bullet->bullet[bullet->index];
	bullet->index = bullet->index + 1;
	Sleep(1000);

	return present_bullet;
}

void dealer_turn(player_t* user, player_t* dealer, bullet_t* bullet) {

	system("cls");
	printf("%s�� ���Դϴ�.\n", dealer->name);
	printf("==========================\n");
	Sleep(500);

	while (bullet->index < bullet->MAX_index && dealer->hp > 0 && user->hp > 0) { // �Ѿ��� ��� �Ҹ��� ������

		double percent = (double)bullet->real_bullet / (bullet->MAX_index - bullet->index);

		if (dealer->hp <= 3 && dealer->item[CIGAR] > 0) { // hp�� 3 �����̰�, �ð��� �ִٸ�. �ð� ���
			use_item(dealer, bullet, CIGAR);
			continue;
		}

		if (percent >= 0.7 && dealer->item[SAW] > 0) { // ��ź�� Ȯ���� 70% �̻��̰�, ���� �ִٸ�. �� ���
			use_item(dealer, bullet, SAW);
			Sleep(1000);
			fire(user, bullet);
			break;
		}
		else if (percent >= 0.5) { // ��ź�� Ȯ���� 50% �̻��̶�� ����
			fire(user, bullet);
			break;
		}
		else if (dealer->item[GLASS] > 0) { // ��ź�� Ȯ���� 50% �̸��̰�, �����Ⱑ �ִٸ�
			if (bullet->bullet[bullet->index] >= REAL_BULLET) { // ��ź�̶��
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
		else if (dealer->item[BEER] > 0) { // ��ź�� Ȯ���� 50% �̸��̰�, ���ְ� �ִٸ�
			use_item(dealer, bullet, BEER);
			continue;
		}
		else { // ��ź�� Ȯ���� 50% �̸��̰�, �������� �ƹ��͵� ���ٸ� �ڽ� ����. ��ź�̶�� ���� �Ѿ.
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
	printf("dealer�� ���� �������ϴ�.\n\n");
	Sleep(1000);
}

void user_turn(player_t* user, player_t* dealer, bullet_t* bullet) {

	system("cls");
	printf("%s�� ���Դϴ�.\n", user->name);
	Sleep(1500);
	while (bullet->index < bullet->MAX_index && dealer->hp > 0 && user->hp > 0) { // �Ѿ��� ��� �Ҹ��� ������
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
				return; // ��ź�̶�� ���� �Ѿ.
			break; // ��ź�̶�� �ٽ� �ѹ� ������ ��
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

	// �ʱ�ȭ
	for (int i = 0; i < MAX_BULLET; i++)
		bullet->bullet[i] = 0;

	bullet->real_bullet = 0;
	bullet->imaginal_bullet = 0;
	bullet->index = 0;
	bullet->MAX_index = num_of_bullets;

	// �Ѿ� ����
	for (int i = 0; i < num_of_bullets; i++) {
		int rand_value = rand() % 2;
		bullet->bullet[i] = rand_value;

		if (rand_value)
			bullet->real_bullet = bullet->real_bullet + 1;
		else
			bullet->imaginal_bullet = bullet->imaginal_bullet + 1;
	}

	// ���� ��ź�� �ϳ��� ���ٸ� �������� �ѹ��� ��ź���� ����. �ݴ뵵 ����
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

	for (int i = 0; i < TYPES_OF_ITEMS; i++) // ���� ���� 0����
		player->item[i] = 0;

	for (int i = 0; i < num_of_items; i++) { // ������ ����
		int j = rand() % TYPES_OF_ITEMS;
		player->item[j] = player->item[j] + 1;
	}
	return player;
}

void print_interface(player_t* user, player_t* dealer, bullet_t* bullet) {
	printf("=========================\n");
	printf("%s�� ü�� : %d\n", user->name, user->hp);
	printf("%s�� ü�� : %d\n", dealer->name, dealer->hp);
	printf("���� ź�� ���� : %d��\n", bullet->MAX_index - bullet->index);
	printf("�Ѿ� ��ź / �ҹ�ź : %d / %d\n\n", bullet->real_bullet, bullet->imaginal_bullet);
	printf("������\n");
	printf("1. �������� ����\n");
	printf("2. �ڽſ��� ����\n");
	printf("3. ������ ���(���� : %d)\n", user->item[GLASS]);
	printf("4. �� ���(���� : %d)\n", user->item[SAW]);
	printf("5. ���� ���(���� : %d)\n", user->item[BEER]);
	printf("6. �ð� ���(���� : %d)\n", user->item[CIGAR]);
	printf("=========================\n");
}

void print_initial_interface() {
	printf("=========================\n");
	printf("1. ���� ����\n");
	printf("2. �� ����\n");
	printf("3. ��ŷ Ȯ��\n");
	printf("4. ����\n");
	printf("=========================\n\n");
}

void print_rule() {
	printf("Rules\n");
	Sleep(1000);
	printf("======================================================================\n\n");
	printf("1. ������ ����� ������ ������ ��� ��븦 ��ų�, �ڽ��� ���ϴ�.\n");
	Sleep(1000);
	printf("2. ���ǿ��� �ִ� %d���� ź�� �����Ǿ� ������, ��ź�� �ҹ�ź�� ������ ������ �����˴ϴ�.\n", MAX_BULLET);
	Sleep(1000);
	printf("3. ��ź�� �߻�Ǿ��� �ÿ��� ������ ���� �Ѿ��, \n�ڱ� �ڽ��� ����� �� �ҹ�ź�̶�� ���� �Ѿ�� �ʰ� �̾�ϴ�.\n\n");
	Sleep(1000);
	printf("4. %d���� �������� �������� �־����ϴ�. �� ������ �Ʒ��� �����ϴ�.\n", TYPES_OF_ITEMS);
	Sleep(1000);
	printf("4-1 ������ : ���� ������ ź�� �� �� �ֽ��ϴ�.\n");
	printf("4-2 �� : �̹� ������ ź�� �������� 2�谡 �˴ϴ�.(1���� ��� ����)\n");
	printf("4-3 ���� : �̹� ������ ź�� ���� �ʰ� �Ҹ��մϴ�.(�� ����)\n");
	printf("4-4 �ð� : ���� 1���� ȸ���մϴ�.(�ִ�ġ �ʰ� �Ұ�)\n\n");
	printf("======================================================================\n");
	Sleep(1000);
}