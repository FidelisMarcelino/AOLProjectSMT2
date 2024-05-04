#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHAR_SIZE 256
int count = 1;
char word[101];

struct trieNode{
	struct trieNode *children[CHAR_SIZE];
	bool isEndOfWorld;
	char desc[100];
};

struct descTrieNode{
	struct descTrieNode *children[CHAR_SIZE];
	bool isEndOfDesc;
};

struct trieNode *createNode(){
	struct trieNode *node = (struct trieNode*)malloc(sizeof(struct trieNode));
	
	if(node){
		node->isEndOfWorld = false;
		
		for(int i = 0; i < CHAR_SIZE; i++){
			node->children[i] = NULL;
		}
	}
	
	return node;
}

struct descTrieNode *createDescNode(){
	struct descTrieNode *node = (struct descTrieNode*)malloc(sizeof(struct descTrieNode));
	
	if(node){
		node->isEndOfDesc = false;
		
		for(int i = 0; i < CHAR_SIZE; i++){
			node->children[i] = NULL;
		}
	}
	
	return node;
}

void insert(struct trieNode *root, char *key){
	struct trieNode *current = root;
	
	int length = strlen(key);
	
	for(int i = 0; i < length; i++){
		int index = key[i] - 'A';
		
		if(!current->children[index]){
			current->children[index] = createNode();
		}
		
		current = current->children[index];
	}
	
	current->isEndOfWorld = true;
}

void insertDesc(struct descTrieNode *root, char *desc){
	struct descTrieNode *current = root;
	
	int length = strlen(desc);
	
	for(int i = 0; i < length; i++){
		int index = desc[i] - 'A';
		
		if(!current->children[index]){
			current->children[index] = createDescNode();
		}
		
		current = current->children[index];
	}
	
	current->isEndOfDesc = true;
}

bool search(struct trieNode *root, char *key){
	struct trieNode *current = root;
	int length = strlen(key);
	
	for(int i = 0; i < length; i++){
		int index = key[i] - 'A';
		
		if(!current->children[index]){
			return false;
		}
		
		current = current->children[index];
	}
	
	return (current != NULL && current->isEndOfWorld);
}

void printTrie(struct trieNode *node, char *buffer, int depth){
	bool hasWords = false;
	
	if(node->isEndOfWorld){
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
	
	if(root->isEndOfWorld){
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

int main(){
	struct trieNode *root = createNode();
	struct descTrieNode *descRoot = createDescNode();
	
	int choice;
	char desc[100], searchWord[100], searchPrefix1[100];
	
	while(true){
		menu();
		scanf("%d", &choice);
		
		switch(choice){
			case 1:{
				do{
					printf("Input a new slang word [Must be more than 1 characters and contains no space]: ");
					scanf("%s", word); getchar();
				} while(strlen(word) <= 1 && strchr(word, ' ') == 0);
				
				do{
					printf("Input a new slang word description [Must be more than 2 words ]: ");
					scanf("%s", desc); getchar();
				} while(strlen(desc) <= 2);
				
				insertDesc(descRoot, desc);
				insert(root, word);
				
				printf("Succesfully released new slang word.\n");
				system("pause");
				break;
			}
			
			case 2:{
				printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]: ");
				scanf("%s", searchWord); getchar();
				
				if(!search(root, searchWord)){
					printf("There is no word '%s' in the dictionary.\n", searchWord);
				} else {
					printf("Slang word: %s\n", searchWord);
					printf("Description: %s\n", desc);
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
