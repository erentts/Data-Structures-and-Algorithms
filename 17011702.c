#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct link{
	char adres[30];
	int sayac;
	struct link *next; // Doubly Linked List yapýsýna uyarlamak için ->next ve ->prev oluþturuyorum.
	struct link *prev;
}node;

node *head = NULL;

node *olustur(char adres[30]){  // Her düðümü node *olustur fonksiyonu icinde oluþturup BasaEkle() fonksiyonuna yolluyorum..
	node *yenidugum = (node *) malloc(sizeof(node));  // yollanan char icin bellekte yer acýyorum..
	yenidugum->sayac = 1;  // ilk defa olusturuldugu icin sayac=1 olarak baþlatýyorum..
	strcpy(yenidugum->adres,adres);  
	yenidugum->next = NULL; // Her deger aldýgýmda ilk deger gibi aldýgýmdan dolayý ->next = NULL ve ->prev = NULL atamasý yapýyorum.
	yenidugum->prev = NULL;
	return yenidugum;
}

void BasaEkle(char adres[30],int kapasite,int esik){
	node *basaekle = olustur(adres);
	if(head==NULL) // head nodu NULL ise boþ demektir o zaman node *olustur() fonksiyonundan gelen bilgiyi head noduna ekliyorum.
	{
		head = basaekle;
		return;
	}
	
	int kontrol=0; // kontrol deðiþkeni koyma nedenim dosyadan gelen string bilginin linked list icinde olup olmadýðýný sorgulamak icin kullanacaðým.
	node *current = head;
	while(current!=NULL)
	{
		if (strcmp(current->adres,adres) == 0) // Dosyadan gelen deger linked list icinde varmý ? eðer var ise if bloðuna giriþ yapacaðýz.
		{
			if (current->sayac <esik || current->sayac >esik) // Gelen string'in sahip oldugu sayac eþik deðerden düþük veya eþik deðerden büyükse sadece sayacýnda arttýrma olacak.
			{
				current->sayac++;
				kontrol++;
				return;
			}
			else // else durumunda ise geriye tek koþul kalýyo oda string'in gelen adres'e eþit olmasý.
			{
				
						if (current->prev == NULL){ // Önceki deðer NULL ise o en baþtaki elemandayýz bu nedenle sayacýný sadece arttýrmamýz yeterlidir.
							current->sayac++;
							return;
						}
						if (current->next == NULL)  // Sondaki degerin next'i NULL ise o zaman sondaki elemandayýz ve bunu baþa alacaðýz...
						{
							current->sayac++;
							node *onceki_kisim = current->prev;
							onceki_kisim->next = NULL;
							node *item2 = (node*)malloc(sizeof(node));
							strcpy(item2->adres,current->adres);
							item2->sayac = current->sayac;
							item2->prev = NULL;
							free(current);
							node *eski = head;
							head = item2;
							head->next = eski;
							eski->prev = head;	
							return;
						}
						
						
						// Önceki iki if komutunda ilk baþtaki node yada son durumdaki node'un olup olmadýgýný sorguladýk ve baþta ise zaten baþta kalacak sonda ise baþa alacaktýk.
						//Eðer node baþta ve sonda deðilse tek seçenek geriye kalýyor oda node'un ortada olmasý.
						current->sayac++;
						node *onceki = current->prev;
						node *sonraki = current->next;
						onceki->next = sonraki;
						sonraki->prev = onceki;
						node *item = (node*)malloc(sizeof(node));
						strcpy(item->adres,current->adres);
						item->sayac = current->sayac;
						item->prev = NULL;
						free(current);
						node *eski = head;
						head = item;
						head->next = eski;
						eski->prev = head;
						return;
					}
					
				}
					current = current->next; 
			}
	

	if (kontrol == 0) // kontrol degiskeninin hala sýfýr olmasý demek yani linked list icinde dosyadan gelen string ile eþleþen bir veri bulamadýðý anlamýna geliyor.
	{
		// KAPASÝTE KONTROLÜ
		node *say = head;
		int sayici=0;
		while(say->next != NULL) // Listemizde ne kadar kutu var onu öðrenmek icin bir sayici degiskeni kullandýk bunu kapasite kontrolünde kullanacaðýz.
		{
			say = say->next;
			sayici++;
		}
		if (sayici == kapasite){  // Listemizdeki kutular sayýldýktan sonra artýk kullanýcýnýn girdiði kapasite ile eþit olup olmadýðýný sorgulayabiliriz.
			node *tempp = head;
			while(tempp->next != NULL)
			{
				tempp = tempp->next;
			}
			node *oncekii = tempp->prev;
			oncekii->next = NULL;
			free(tempp);
			
			
			node *eski = head;
			head = basaekle;
			head->next = eski;
			eski->prev = head;
		}
		else { 
			node *eski = head;
			head = basaekle;
			head->next = eski;
			eski->prev = head;
		}
	}
	else
	{
		head = current;	
	}
	
}

void yazdir(node *baslangic) // Ekrana yazma fonksiyonumuz..
{
	node *temp = baslangic;
	printf("\n");
	while(temp!=NULL)
	{
		printf("%s,%d\t",temp->adres,temp->sayac);
		temp = temp->next;	
	}
}

int main(){
	char adres[30];
	int secim,secim2,n,i=0;
	int kapasite,esik;
	printf("Kapasite kac olsun : ");
	scanf("%d",&kapasite);
	printf("Esik deger kac olsun : ");
	scanf("%d",&esik);
	printf("Klavyeden Okumak icin ' 1 ' tusuna basiniz.. \nDosyadan Okumak icin ' 2 ' tusuna basiniz.  ");
	scanf("%d",&secim2);
	int sayac=0;
	FILE *fp; 
	char buff[255];
	switch(secim2){   // Burada switch kullanmamýn sebebi; dosyadan mý adresleri çekeceðiz yoksa klavyeden adres mi gireceðiz tercihini kullanýcýya yaptýrmak için.
		case 1: // CASE 1: Klavyeden girilecek.
			printf("Kac string gireceksiniz : ");
			scanf("%d",&n);
			while(i<n){
			printf("\n Eklemek istediginiz metni girin .. ");
			scanf("%s",&adres);
			BasaEkle(adres,kapasite-1,esik);
			yazdir(head);
			printf("\n");
			i++;
	}
		break;
		
		case 2: // CASE 2: Dosyadan okunacak.
   				fp = fopen("input.txt", "r");  
   				while(fscanf(fp, "%s", buff)!=EOF){  
   				strcpy(adres,buff);
				BasaEkle(adres,kapasite-1,esik);
				yazdir(head); 
				sayac++;
   				}
   				fclose(fp);  
				break;
		default: break;
	
	}
	printf("\n\n Cache Buffer'i Temizlemek istiyormusunuz ?\n \t(1-EVET)\t(2-HAYIR) \n\n");
	scanf("%d",&secim);
	switch (secim) // Kullanýcýnýn Cache Buffer'i silebilmesine olanak tanýmak için yeniden bir seçim yaptýrdým.
	{              
		case 1: printf("\n\n ******SILINIYOR*****\n\n");
				int j;
				for (j=0;j<kapasite;j++)
				{
					if (head==NULL){
						return;
					}
					if (head->next == NULL)
					{
						head = NULL;
						printf("\n Silme Islemi TAMAMLANDI..");
						return;
					}
					node *tut = head->next;
					tut->prev = NULL;
					free(head);
					head = tut;
					yazdir(head);
					printf("\t\tSilindi !\n");
				}
				break;
		case 2: 
				printf("Silmemeyi tercih ettiniz.Listenin son hali\n");
				yazdir(head);
				break;
		default: break;
	}
	return 0;
}
