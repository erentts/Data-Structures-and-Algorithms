#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct link{
	char harf;
	int frequency;
	struct link *next,*left,*right;
}node;

node *head = NULL;

node *Olustur(char harf){  // Her yeni dugumu olusturdugum fonksiyon.
	node *yenidugum = (node*)malloc(sizeof(node));
	yenidugum->frequency = 1; 
	yenidugum->harf = harf;
	yenidugum->next = NULL;
	yenidugum->left = NULL;
	yenidugum->right = NULL;
	return yenidugum;
}
void SonaEkle(char harf){ 
	
	node *sonaeklenecek = Olustur(harf);  //Olustur() fonksiyonumuzdan gelen bilgileri yeni bir node'a atadým.
	if (head == NULL){  // Eðer linkli listemiz boþ ise ilk deðer atamasý yapýlacak.
		head = sonaeklenecek;
		return;
	}
	int kontrol=0; // kontrol degiskenimizi eðer yeni gelen harfin daha önce listede olup olmadýðýný sorgulamak için kullandýk.
	node *current = head;
	while(current != NULL){
		if (current->harf == harf){ // Gelen yeni harfin linkli listede olup olmadýðýný sorguluyoruz.
			current->frequency++; // Eðer gelen harf listemizde mevcutsa frekansýný arttýrýyoruz.
			kontrol++; 
			return;
		}
		current = current->next;
	}
	if (kontrol == 0){  // kontrol-> 0 ise; gelen yeni harf listemizde yoktu ve yeni düðüm oluþturduk.
		node *temp = head;
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = sonaeklenecek;
	}
	else{
		head = current; // kontrol-> 1 olmasý halinde yukarýdaki frekans arttýrma iþlemi yapýldý ve head'e atýldý.
	}
}

node *insertion_sort(node* head2)   // Insertion sort iþlemleri..
{
    node *temp1, *temp2, *temp3;
    temp1 = head2;
    head2 = NULL;
 
    while(temp1 != NULL)
    {
        temp3 = temp1;
        temp1 = temp1->next;
        if (head2 != NULL)
        {
            if(temp3->frequency > head2->frequency)
            {
                temp2 = head2;
                while ((temp2->next != NULL) && (temp3->frequency> temp2->next->frequency))
                {
                    temp2 = temp2->next;
                }
                temp3->next = temp2->next;
                temp2->next = temp3;
            }
            else
            {
                temp3->next = head2;
                head2 = temp3;
            }
        }
        else
        {
            temp3->next = NULL;
            head2 = temp3;
        }
    }
    return head2;
}

void huffman(){
	
	int sayici=0;
	node *sayac = head;
	while(sayac != NULL){ // huffman tree'deki en tepeki iþlem yaptýðýmýz elemanlarýn sayýsýný alýyorum.
		sayici++; // bu sayiyi almamýn nedeni tepede yalnýzca 1 sayý kaldýðýnda iþlemi sonlandýrmak.
		sayac = sayac->next;
	}
	while(sayici!=1){ // sayici deðiþkeni her iþlem yapýldýgýnda 1 azaltýlýyor.Eðer sayici = 1 ise iþlemimiz bitti ve huffman aðacý oluþtu demektir.
		node *tut = head; // öncelikle mevcut head'imizi tut isimli yeni bir node'a aktarýyoruz.
		node *temp = (node*)malloc(sizeof(node));
		temp->left = NULL;
		temp->right = NULL;
		temp->next=NULL;
		temp->frequency = tut->frequency+tut->next->frequency; // temp node'u için frekans deðeri atamasý.
		temp->harf = ' '; // harf boþ olacaðý için boþ karakter atýyoruz.
		temp->left = tut;
		temp->right = tut->next;
		head = tut->next->next;


		node *current;
		current = temp; // oluþturdugumuz temp'i head'imize ekleme iþlemleri ve ekledikten sonra insertion_sort fonksiyonumuzla sýralamayý güncelliyoruz.
		current->next = head;
		head = current;
		head = insertion_sort(head);
		
		sayici--; 
	}
}

void yazdir(){ // Liste yazdirma fonksiyonu
	node *temp = head;
	while(temp != NULL){
		printf("%d%c  ",temp->frequency,temp->harf);
		temp = temp->next;
	}
}

void yazdir_level(node* tree, int level) // Aðaçtaki her seviyeyi ayrý ayrý olarak main fonksiyonumuza çekip orada for ile tüm seviyeleri yazdýrýyoruz.
{ 
    if (tree == NULL){
    	return;
	}  
    if (level == 1){
    	printf("%d%c  ",tree->frequency,tree->harf); 
	}
    else if (level > 1) 
    { 
        yazdir_level(tree->left,level-1);  // level-1 olarak almamýzýn sebebi; Örneðin:level=3 olsun; 
        yazdir_level(tree->right,level-1); // 3 kere left veya right iþlemleri ile 3.seviyeye inmemiz gerekiyor.Bu nedenle;
      								   	   // level'i recursion iþlemi ile 3 den 1 e getiriyoruz böylikle,
      								   	   // right ve left iþlemleri ile 3.seviyeye inmiþ oluyoruz.
    } 
} 

int tree_height(node *tree) // aðaç yüksekliðini bulmamýz gerekiyor yazdirma islemi icin.
{ 
    if (tree==NULL){
    	return 0;
	}  
    else{ 
		int left_height = tree_height(tree->left); 
        int right_height = tree_height(tree->right);
        
        if (left_height > right_height){ // Burada en uzun hangisi onu sorguladýk çünkü bize en uzunu lazým.
        	return (left_height+1);
		} 
        else {
        	return (right_height+1);	
		} 
    } 
} 

int main(){
	char text[200];
	printf("\n Bir yazi giriniz : ");
	gets(text);
	printf("\n");
	int i=0;
	while(text[i] != '\0'){	
		SonaEkle(text[i]); 
		i++;
	}
	yazdir();
	printf("\n\n ******Siralanmis liste ****\n\n");
	head = insertion_sort(head);
	yazdir();
	
	printf("\n\n ****** HUFFMAN TREE ****\n\n");
	huffman();
	
	int a,b,treeheight;
	treeheight = tree_height(head); //aðacýn yüksekligini treeheight degiskenine atadýk ve i=1 den baþlayýp treeheight'a kadar levelleri yazdýracagýz.
	for (a=1;a<=treeheight;a++){
		yazdir_level(head,a);
		printf("\n");
	}	
	
	return 0;
}
	

