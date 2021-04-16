/*
 * The answer to the old shuffle a deck of cards problem
 * (c) 2020 C. Liebling
 * charles.liebling@gmail.com
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DECKLENGTH 52U

void swap(uint8_t *a, uint8_t *b)
{
	static uint8_t tmp;
	if((a == NULL) || (b == NULL)){
		return;
	}
	tmp = *a;
	*a = *b;
	*b = tmp;
}

#define SWAP(a, b) if(a != b){\
	a = a ^ b;\
	b = b ^ a;\
	a = a ^ b;}

void print_card(uint8_t card)
{
	uint8_t rank;
	uint8_t suit;
	rank = card % 13;
	suit = card / 13;
	char *suit_label;
	char *rank_label;
	switch(rank) {
		case 0:
			rank_label = "Two";
		break;
		case 1:
			rank_label = "Three";
		break;
		case 2:
			rank_label = "Four";
		break;
		case 3:
			rank_label = "Five";
		break;
		case 4:
			rank_label = "Six";
		break;
		case 5:
			rank_label = "Seven";
		break;
		case 6:
			rank_label = "Eight";
		break;
		case 7:
			rank_label = "Nine";
		break;
		case 8:
			rank_label = "Ten";
		break;
		case 9:
			rank_label = "Jack";
		break;
		case 10:
			rank_label = "Queen";
		break;
		case 11:
			rank_label = "King";
		break;
		case 12:
			rank_label = "Ace";
		break;
	}
	switch(suit) {
		case 0:
			suit_label = "Spades";
		break;
		case 1:
			suit_label = "Clubs";
		break;
		case 2:
			suit_label = "Diamonds";
		break;
		case 3:
			suit_label = "Hearts";
		break;
	}
	printf("%s of %s\n", rank_label, suit_label);
}
	

int main(void)
{
	uint8_t cards[DECKLENGTH];
	uint8_t i;
	uint8_t selected;
	time_t t;

	srand((unsigned) time(&t));


	for(i = 0; i < DECKLENGTH; i++)
	{
		cards[i] = i;
	}
	/* shuffle */
	for(i = 0; i < DECKLENGTH; i++)
	{
		selected = rand() % (DECKLENGTH - i);
		if(selected == i)
		{
			continue;
		} else {
			swap(&cards[selected], &cards[DECKLENGTH - i - 1]);
			//SWAP(cards[selected], cards[DECKLENGTH - i - 1]);
		}
	}
	for(i = 0; i < DECKLENGTH; i++)
	{
		printf("Card %hhu is %hhu ", i, cards[i]);
		print_card(cards[i]);
	}
	return 0;
}
