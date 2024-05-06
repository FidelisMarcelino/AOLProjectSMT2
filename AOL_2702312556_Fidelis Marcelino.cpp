#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHAR_SIZE 256		// mendefinisikan ukuran karakter, 256 karakter menandakan karakter ASCII
int count = 1;				// Variabel untuk menghitung jumlah kata slang

//Struktur node dalam trie unutk menyimpan deskripsi kata slang
struct descNode{			
	char description[100];	//array untuk menyimpan deskripsi
	struct descNode *next;	//pointer descNode untuk menunjuk ke struktur berikutnya dan membentuk link list
							//untuk menyimpan deskripsi dari kata slang
};

//Struktur untuk trie yang menyimpan kata slang
struct trieNode{
	struct trieNode *children[CHAR_SIZE];	//array untuk menunjuk node
	bool isEndOfWord;						//menandakan akhir dari sebuah kata slang
	struct descNode *descHead;				//deskripsi dari kata slang
};

//Fungsi untuk membuat dan menginisialisasi node trie kata slang
struct trieNode *createNode(){
	struct trieNode *node = (struct trieNode*)malloc(sizeof(struct trieNode));	//alokasi memori untuk new node
	
	if(node){
		node->isEndOfWord = false;			//set awal untuk isEndOfWord
		node->descHead = NULL;
		
		//Inisialisasi semua node child menjadi NULL
		for(int i = 0; i < CHAR_SIZE; i++){
			node->children[i] = NULL;
		}
	}
	
	return node;							//mengembalikan node ke node baru
}

//Fungsi untuk membuat dan menginisialisasi node trie deskripsi kata slang
struct descNode *createDescNode(char *desc){
	struct descNode *node = (struct descNode*)malloc(sizeof(struct descNode)); //alokasi memori untuk new node
	
	
	if(node){
		strcpy(node->description, desc);	//menyalin deskripsi dalam node ke variabel deskripsi yang baru
		node->next = NULL;					//set node next ke NULL
	}
	
	return node;							//mengembalikan pointer ke node baru
}

//Fungsi untuk memasukkan kata slang ke dalam trie
void insert(struct trieNode *root, char *key, char *desc){
	struct trieNode *current = root;		//alokasi memori untuk new node
	int length = strlen(key);				//mencari panjang kata slang
	
	for(int i = 0; i < length; i++){
		int index = key[i] - 'A';			//menghitung indeks berdasarkan karakter
		
		if(!current->children[index]){		//jika child node belum ada
			current->children[index] = createNode();	//buat new node
		}
		
		current = current->children[index];	//pindahkan ke node selanjutnya
	}
	
	current->isEndOfWord = true;			//menandakan akhir kata dari slang
	
	struct descNode *newDescNode = createDescNode(desc);	//aolikasi memori untuk new node deskripsi
	
	//Menambahkan deskripsi ke dalam linked list deskripsi jika tidak ada deskripsi yang tersimpan sebelumnya
	if(!current->descHead){
		current->descHead = newDescNode;	//menambahkan deskripsi baru sebagai head
	} else {
		//jika sudah ada deskripsi sebelumnya, cari node terakhir dari linked link deskripsi
		struct descNode *temp = current->descHead;
		
		//selama node bukan yang terakhir, maka terus cari
		while(temp->next != NULL){
			temp = temp->next;
		}
		
		//setelah menemukan node terakhir, tambah deskripsi sebagai node berikutnya
		temp->next = newDescNode;
	}
}

//Funsgi untuk mencari deskripsi dari sebuah kata slang
char *searchDesc(struct trieNode *root, char *key){
	struct trieNode *current = root;			//node ke new root
	int length = strlen(key);					//mencari panjang kata slang yang dicari
	
	//iterasi setiap kata dalam slang
	for(int i = 0; i < length; i++){
		int index = key[i] - 'A';				//hitung indeks berdasarkan karakter
		
		//jika node child tidak ditemukan, maka kembalikan nilai NULL
		if(!current->children[index]){
			return NULL;
		}
		
		current = current->children[index];		//pindahkan node yang sekarang ke node child berikutnya
	}
	
	//Jika node ditemukan dan merupakan akhir dari kata slang
	if(current != NULL && current->isEndOfWord){
		//jika ada deskripsi
		if(current->descHead != NULL){
			return current->descHead->description;	//mengembalikan nilai deskripsi tersebut
		} else {
			return NULL;
		}
	}
}

//Fungsi untuk mencetak semua kata slang dalam trie
void printTrie(struct trieNode *node, char *buffer, int depth){
	bool hasWords = false;					//Variabel untuk menunjukkan apakah ada kata slang atau tidak
	
	if(node->isEndOfWord){					//jika sudah di akhir kata slang	
		hasWords = true;					//berarti ada kata slang
		
		if(count == 1){						//jika merupakan kata slang pertama yang ditemukan
			printf("List of all slang words in the dictionary:\n");	//keluarkan output ini
		}
		
		buffer[depth] = '\0';				//menambahkan NULL terminator untuk mengakhiri string yang disimpan dalam buffer
		printf("%d. %s\n",count, buffer);	//mencetak nomor urut dan kata slang yang disimpan di buffer
		count++;							//menambah nilai dari variabel count untuk menandai nomer urut berikutnya
	}
	
	for(int i = 0; i < CHAR_SIZE; i++){		//iterasi semua node child
		if(node->children[i] != NULL){		//mengecek apakah ada node child
			buffer[depth] = i + 'A';		//jika ada, maka tambahkan karakter ke buffer
			printTrie(node->children[i], buffer, depth + 1);	//rekursif ke node child
			hasWords = true;				//menandakan ada kata slang
		}
	}
	
	//jika tidak ada kata slang	
	if(!hasWords){
		printf("There is no slang words yet in the dictionary.\n");
	}
}

//Fungsi untuk mencetak semua kata slang yang dimulai dengan awalan tertentu
void printTrieWithPrefix(struct trieNode *root, char *prefix, char *buffer, int depth){
	bool hasWords = false;					//variabel untuk menunjukkan apakah ada kata slang dengan awalan tertentu
	
	if(root->isEndOfWord){					//jika sampai pada akhir kata sebuah slang
		hasWords = true;					//berarti ada kata slang
		
		if(count == 1){						//jika adalah kata pertama slang yang ditemukan
			printf("List of all slang words in the dictionary:\n");	//mengeluarkan output ini
		}
		
		buffer[depth] = '\0';				//menambahkan NULL-terminator untuk mengakhiri string yang disimpan dalam buffer
		printf("%d. %s%s\n", count, prefix, buffer);	//mencetak nomor urut dan kata slang yang disimpan dalam buffer
		count++;							//menambah nilai dari variabel count untuk menandai nomor urut berikutnya
	}
	
	for(int i = 0; i < CHAR_SIZE; i++){		//iterasi semua node child
		if(root->children[i] != NULL){		//jika ada node child, maka
			buffer[depth] = i + 'A';		//menambahkan karakter ke buffer
			printTrieWithPrefix(root->children[i], prefix, buffer, depth + 1);	//rekursif ke node child
			hasWords = true;				//menandakan ada kata slang
		}
	}
	
	//jika tidak ada kata slang
	if(!hasWords){
		printf("There is no prefix \"%s\" in the dictionary.\n", prefix);
	}
}

//Fungsi untuk mencari dan mencetak semua kata slang yang dimulai dengan awalan tertentu
void searchPrefix(struct trieNode *root, char *prefix){
	struct trieNode *current = root;		//alokasi memori untuk new node
	int length = strlen(prefix);			//mencari panjang kata slang
	char buffer[100];						//variabel buffer untuk menyimpan kata slang yang ditemukan
	
	count = 1;								//inisialisasi nilai count ke 1
	
	//iterasi semua karakter dengan awalan tertentu
	for(int i = 0; i < length; i++){
		int index = prefix[i] - 'A';		//menghitung indeks berdasarkan karakter
		
		//Jika node child tidak ada, maka keluarkan output
		if(current->children[index] == NULL){
			printf("There is no prefix \"%s\" in the dictionary.\n", prefix);
			return;							//keluar dari fungsi
		}
		current = current->children[index];	//jika ada node child, maka pindahkan ke node child selanjutnya
	}
	
	printf("Words starting with \"%s\": \n", prefix);	//mencetak pesan
	printTrieWithPrefix(current, prefix, buffer, 0);	//mencetak semua kata slang dengan awalan tertentu
}

//Fungsi untuk menampilkan menu
void menu(){
	system("cls");										//menghapus layar konsol sebelum menampilkan menu
	puts("1. Release a new slang word");
	puts("2. Search a slang word");
	puts("3. View all slang words starting with a certain prefix word");
	puts("4. View all slang words");
	puts("5. Exit");
	printf(">> ");
}

//Fungsi untuk menghitung jumlah spasi dalam sebuah string
int wordReq(char *str){
	int length = strlen(str);				//mencari panjang string
	int counter = 0;						//inisialisasi counter untuk menghitung jumlah spasi dalam string
	
	//iterasi untuk menghitung spasi melalui setiap karakter dalam string
	for(int i = 0; i < length; i++){
		if(str[i] == ' '){					//jika karakter adalah spasi
			counter++;						//maka tambah nilai counter
		}
	}
	
	return counter;							//mengembalikan jumlah spasi dalam string
}

int main(){
	struct trieNode *root = createNode();				//membuat trie untuk kata slang
	
	int choice;											//variabel untuk menyimpan pilihan menu
	char word[100], desc[100], searchWord[100], searchPrefix1[100];	//variabel untuk deskripsi, kata yang dicari, dan awalan yang dicari
	
	//looping menu utama
	while(true){
		menu();											//menampilkan pilihan menu
		scanf("%d", &choice);							//menerima pilihan menu
		
		switch(choice){
			//menu untuk menambah kata slang baru
			case 1:{
			
				printf("Input a new slang word [Must be more than 1 characters and contains no space]: ");
				scanf("%s", word); getchar();			//menerima input kata slang yang baru					
				
				//validasi menambah kata slang dengan syarat minimal kata 2, dan tidak ada spasi dalam kata tersebut
				while(strlen(word) <= 1 && strchr(word, ' ') == 0){
					printf("Input a new slang word [Must be more than 1 characters and contains no space]: ");
					scanf("%s", word); getchar();		//meminta input ulang apabila validasi tidak terpenuhi
				} 
				
				printf("Input a new slang word description [Must be more than 2 words ]: ");
				scanf("%[^\]", desc); getchar();		//menerima input deskripsi kata slang yang baru
				
				//validasi menambah deskripsi dengan syarat harus memiliki lebih dari 2 kata
				while(wordReq(desc) < 1){									
					printf("Input a new slang word description [Must be more than 2 words ]: ");
					scanf("%[^\n]", desc); getchar();	//meminta input ulang jika validasi tidak terpenuhi
				}
				
				insert(root, word, desc);				//memasukkan kata slang dan deskripsi ke dalam trie
				
				printf("Succesfully released new slang word.\n");	//output bahwa kata slang dan deskripsi duah berhasil masuk ke trie
				system("pause");						//menunggu input sebelum melanjutkan aksi
				break;
			}
			
			//menu untuk mencari kata slang yang sudah ada
			case 2:{
				printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]: ");
				scanf("%s", searchWord); getchar();		//menerima input kata slang yang dicari
				
				while(strlen(searchWord) < 2 && strchr(searchWord, ' ') == 0){
					printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]: ");
					scanf("%s", searchWord); getchar();		//menerima input kata slang yang dicari
				}
				
				if(!searchDesc(root, searchWord)){			//jika kata slang tidak ditemukan
					printf("There is no word '%s' in the dictionary.\n", searchWord);
				} else {									//jika ditemukan
					printf("Slang word: %s\n", searchWord);	//mencetak kata slang
					
					char *wordDesc = searchDesc(root, searchWord);	//mencari deskripsi dari kata slang yang diinginkan
					
					printf("Description: %s\n", wordDesc);		//mencetak deskripsi yang ditemukan
				}
				system("pause");						//menunggu input sebelum melanjutkan aksi
				break;
			}
			
			//menu untuk mencari kata slang dengan awalan tertentu
			case 3:{
				printf("Input a prefix to be searched: ");
				scanf("%s", searchPrefix1); getchar();	//menerima input awalan kata slang
				
				searchPrefix(root, searchPrefix1);		//fungsi untuk mencari dan mencetak kata slang dengan awalan tertentu
				system("pause");						//menunggu input sebelum melanjutkan aksi
				break;
			}
			
			//Menu untuk menampilkan semua kata slang
			case 4:{
				count = 1;								//inisialisasi nilai count ke 1
				char buffer[100];						//variabel buffer untuk menyimpan kata slang
				printTrie(root, buffer, 0);				//fungsi untuk mencetak semua kata slang
				system("pause");						//menunggu input sebelum melanjutkan aksi
				break;
			}
			
			//Menu untuk keluar dari program
			case 5:{
				printf("Thank you... Have a nice day :)\n");
				return 0;								//keluar dari program
				break;
			}
		}
	}
	
	return 0;
}
