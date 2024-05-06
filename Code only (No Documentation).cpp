#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHAR_SIZE 256

int count = 1;
char word[101];

struct descNode{
	char description[100];
	struct descNode *next;
};

struct trieNode{
	struct trieNode *children[CHAR_SIZE];
	bool isEndOfWord;
	struct descNode *descHead;
};

struct trieNode *createNode(){
	struct trieNode *node = (struct trieNode*)malloc(sizeof(struct trieNode));
	if(node){
		node->isEndOfWord = false;
		node->descHead = NULL;
		for(int i = 0; i < CHAR_SIZE; i++){
			node->children[i] = NULL;
		}
	}
	return node;
}

struct descNode *createDescNode(char *desc){
	struct descNode *node = (struct descNode*)malloc(sizeof(struct descNode));
	if(node){
		strcpy(node->description, desc);
		node->next = NULL;
	}
	return node;
}

void insert(struct trieNode *root, char *key, char *desc){
	struct trieNode *current = root;
	int length = strlen(key);
	for(int i = 0; i < length; i++){
		int index = key[i] - 'A';
		if(!current->children[index]){
			current->children[index] = createNode();
		}
		current = current->children[index];
	}
	current->isEndOfWord = true;
	struct descNode *newDescNode = createDescNode(desc);
	if(!current->descHead){
		current->descHead = newDescNode;
	} else {
		struct descNode *temp = current->descHead;
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = newDescNode;
	}
}

char *searchDesc(struct trieNode *root, char *key){
	struct trieNode *current = root;
	int length = strlen(key);
	for(int i = 0; i < length; i++){
		int index = key[i] - 'A';
		if(!current->children[index]){
			return NULL;
		}
		current = current->children[index];
	}
	if(current != NULL && current->isEndOfWord){
		if(current->descHead != NULL){
			return current->descHead->description;
		} else {
			return NULL;
		}
	}
}

void printTrie(struct trieNode *node, char *buffer, int depth){
	bool hasWords = false;
	if(node->isEndOfWord){
		hasWords = true;
		if(count == 1){
			printf("List of all slang words in the dictionary:\n");
		}
		buffer[depth] = '\0';
		printf("%d. %s\n",count, buffer);
		count++;
	}
	for(int i = 0; i < CHAR_SIZE; i++){
		if(node->children[i] != NULL){
			buffer[depth] = i + 'A';
			printTrie(node->children[i], buffer, depth + 1);
			hasWords = true;
		}
	}
	if(!hasWords){
		printf("There is no slang words yet in the dictionary.\n");
	}
}

void printTrieWithPrefix(struct trieNode *root, char *prefix, char *buffer, int depth){
	bool hasWords = false;
	if(root->isEndOfWord){
		hasWords = true;
		if(count == 1){
			printf("List of all slang words in the dictionary:\n");
		}
		buffer[depth] = '\0';
		printf("%d. %s%s\n", count, prefix, buffer);
		count++;
	}
	for(int i = 0; i < CHAR_SIZE; i++){
		if(root->children[i] != NULL){
			buffer[depth] = i + 'A';
			printTrieWithPrefix(root->children[i], prefix, buffer, depth + 1);
			hasWords = true;
		}
	}
	if(!hasWords){
		printf("There is no prefix \"%s\" in the dictionary.\n", prefix);
	}
}

void searchPrefix(struct trieNode *root, char *prefix){
	struct trieNode *current = root;
	int length = strlen(prefix);
	char buffer[100];
	count = 1;
	for(int i = 0; i < length; i++){
		int index = prefix[i] - 'A';
		if(current->children[index] == NULL){
			printf("There is no prefix \"%s\" in the dictionary.\n", prefix);
			return;
		}
		current = current->children[index];
	}
	printf("Words starting with \"%s\": \n", prefix);
	printTrieWithPrefix(current, prefix, buffer, 0);
}

void menu(){
	system("cls");
	puts("1. Release a new slang word");
	puts("2. Search a slang word");
	puts("3. View all slang words starting with a certain prefix word");
	puts("4. View all slang words");
	puts("5. Exit");
	printf(">> ");
}

int wordReq(char *str){
	int length = strlen(str);
	int counter = 0;
	for(int i = 0; i < length; i++){
		if(str[i] == ' '){
			counter++;
		}
	}
	return counter;
}

int main(){
	struct trieNode *root = createNode();
	int choice;
	char desc[100], searchWord[100], searchPrefix1[100];
	while(true){
		menu();
		scanf("%d", &choice);
		switch(choice){
			case 1:{
				printf("Input a new slang word [Must be more than 1 characters and contains no space]: ");
				scanf("%s", word); getchar();
				while(strlen(word) <= 1 && strchr(word, ' ') == 0){
					printf("Input a new slang word [Must be more than 1 characters and contains no space]: ");
					scanf("%s", word); getchar();
				}
				printf("Input a new slang word description [Must be more than 2 words ]: ");
				scanf("%[^\n]", desc); getchar();
				while(wordReq(desc) < 1){
					printf("Input a new slang word description [Must be more than 2 words ]: ");
					scanf("%[^\n]", desc); getchar();
				}
				insert(root, word, desc);
				printf("Succesfully released new slang word.\n");
				system("pause");
				break;
			}
			case 2:{
				printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]: ");
				scanf("%s", searchWord); getchar();
				while(strlen(searchWord) < 2 && strchr(searchWord, ' ') == 0){
					printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]: ");
					scanf("%s", searchWord); getchar();
				}
				if(!searchDesc(root, searchWord)){
					printf("There is no word '%s' in the dictionary.\n", searchWord);
				} else {
					printf("Slang word: %s\n", searchWord);
					char *wordDesc = searchDesc(root, searchWord);
					printf("Description: %s\n", wordDesc);
				}
				system("pause");
				break;
			}
			case 3:{
				printf("Input a prefix to be searched: ");
				scanf("%s", searchPrefix1); getchar();
				searchPrefix(root, searchPrefix1);
				system("pause");
				break;
			}
			case 4:{
				count = 1;
				char buffer[100];
				printTrie(root, buffer, 0);
				system("pause");
				break;
			}
			case 5:{
				printf("Thank you... Have a nice day :)\n");
				return 0;
				break;
			}
		}
	}
	return 0;
}

