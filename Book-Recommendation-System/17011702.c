#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node {   // Dosyadan cekilmis verilerin tutuldugu struct
	char userName[50];      // Kullanici adi : U1...NU5
	char books[50][50];		// Kitaplar
	int givenPoint[8];      // Kitaplara verilen puanlar
	struct node *next;	
}bookRecommendation;

bookRecommendation *front = NULL;
bookRecommendation *rear = NULL;

typedef struct nodeTwo{   // Hesaplanan similarity degerlerinin tutuldugu struct yapisi
	char userName[50];  	// Kullanici adi : NU1...NU5
	char who[50][50];		// Kullanici adi : U1...U20
	float similarity[20];   // Her NU1...NU5 kullanicilari icin U1....U20 arasindaki kullanicilar ile arasindaki similarity degerleri
	struct nodeTwo *next;
}calculatedSimilarities;

calculatedSimilarities *frontTwo = NULL;
calculatedSimilarities *rearTwo = NULL;

bookRecommendation *createNode(){     // Her kullanici ve onlarin okudugu kitaplara verdigi puanlar icin bellekte yer aciyorum.
	bookRecommendation *newInformation = (bookRecommendation*)calloc(sizeof(bookRecommendation),1);
	strcpy(newInformation->books[0],"TRUE BELIEVER");        // Kitap isimleri ile islem yapmayacagim icin en basta direk buradan ekledim.Bunun disinda hersey dosyadan cekiliyor.
	strcpy(newInformation->books[1],"THE DA VINCI CODE");
	strcpy(newInformation->books[2],"THE WORLD IS FLAT");
	strcpy(newInformation->books[3],"MY LIFE SO FAR");
	strcpy(newInformation->books[4],"THE TAKING");
	strcpy(newInformation->books[5],"THE KITE RUNNER");
	strcpy(newInformation->books[6],"RUNNY BABBIT");
	strcpy(newInformation->books[7],"HARRY POTTER");
	newInformation->next = NULL;
	return newInformation;
}

calculatedSimilarities *createNodeTwo(){    //  Similarity degerlerinin tutuldugu struct yapisina gelecek olan her node icin burada yer aciyorum.
	calculatedSimilarities *newInformation = (calculatedSimilarities*)calloc(sizeof(calculatedSimilarities),1);
	newInformation->next = NULL;
	return newInformation;
}

void enqueue(bookRecommendation *newInformation){   // Queue yapisi kullandigim icin dosyadan cektigim her satiri kuyruga ekliyorum.
	if (front == NULL){        // Eger kuyruk bos ise front ve rear gelecek olan ilk nodu gosterecek.
		front = newInformation;
		rear = newInformation;
	}
	else{                     // Onun haricinde eklenecek her node bir sonraki siraya yerlesecek ve rear orayi gosterecek.
		rear->next = newInformation;
		rear = newInformation;
	}
}

void enqueue2(calculatedSimilarities *newInformation){      // Similarity degerlerinin tutulduðu yerde de Queue yapisi kullandim.
	if (frontTwo == NULL){		// Eger kuyruk bos ise front ve rear gelecek olan ilk nodu gosterecek.
		frontTwo = newInformation;
		rearTwo = newInformation;
	}
	else{						// Onun haricinde eklenecek her node bir sonraki siraya yerlesecek ve rear orayi gosterecek.
		rearTwo->next = newInformation;
		rearTwo = newInformation;
	}
}

void readTheFile(){    // Dosyadan veri okumak icin kullandigimiz fonksiyon
	FILE *file = fopen("RecomendationDataSet.csv","r");
	if(file == NULL){
		printf("Dosya okunamadi !");
	}
	else{
		printf("Dosya okuma basarili ! \n");
		char buff[1024];           // Dosyadaki verileri tutacaðým dizi
		int rowCount = 0;      	   // Kitap isimlerinin olduðu satýrý atlayaðým kontrol deðiþkeni
		int fieldCount;			   //  Okuma yapacaðým sütun için tuttuðum sayaç
		int columnCount = 0;	   // Kitap ismi icin kontrol degiskeni
		char bookNames[50][50];	   // Kitap isimleri icin dizi
		char book[50];			   // kitap isimleri gecici degisken
		bookRecommendation *newInformation;		// Struct yapýsýna aktarmak üzere oluþturduðum geçiçi yapý
		while(fgets(buff,1024,file)){	// satýr satýr okuyorum dosyayý
			
			fieldCount = 0;
			rowCount++;
			int c=0;  // kitap isimleri icin sayac degiskeni
			int d=0;  //	kitap isimlerini alirken kullanilacak sayac degiskeni
			int e=0;  // 	kitap isimlerini alirken kullanilacak sayac degiskeni
			if(rowCount == 1){     // Ilk satiri atliyor yani kitap isimlerini almiyorum.
				while(buff[c] != '\0'){
					if(buff[c] != ';' && buff[c] != '\n' && buff[c] != '\0'){    
						book[d]	= buff[c];		// Her filmi book dizisine karakter karakter dolduruyorum.
						d++;
					}
					else{
						columnCount++;
						if(columnCount != 1){     // ilk satir ilk sutundaki USERS metnini es geciyorum.
							strcpy(bookNames[e],book);
							e++;
						}
						int x;
						for(x=0;x<sizeof(book)/sizeof(char);x++){   // gecici diziyi temizliyorum.
							book[x] = '\0';
						}
						book[d] = '\0';
						d=0;	
					}
					c++;
				}		
				continue;		// ilk satiri aldiktan sonra artik bu if yapisina girmeyecek.
			}
			int i=0;
			int j=0;
			int z;
			
			char field[50];   // Kullanýcý ismi ve verilen puanlarý tutacaðým deðiþken
			int b = -1;		  // Verilen puanlar üzerinde dolaþabilmek için sayaç deðiþkeni. -1'den baþlama sebebi ilk baþta Kullanýcý Adý geldiði için orda da bir kere artýyo. Puana gelince sýfýrdan baþlýyor.	
			newInformation = createNode();    // Bellekte yer acýyorum.
			
			while(buff[i] != '\0'){        // Okunan satirin sonuna kadar karakter karakter bakiyorum
				if(buff[i] != ';' && buff[i] != '\n' && buff[i] != '\0'){   // Noktalý virgüllerle ayrýlýyor bilgiler o yüzden 
					field[j] = buff[i];            							// ';' gelene kadar karakterleri alýyoruz.
					j++;
				}
				else{
					int newField;
				
					if(fieldCount == 0){		// Ýlk sütunda kullanýcý adý var onu alýyoruz
						strcpy(newInformation->userName,field);
					}
					if(fieldCount == 1){		// 1.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){    // noktalý virgülden sonra yine noktalý virgül,null ve bosluk gelirse
							newField = 0;		// O halde o sütundaki deðer sýfýr olacak.
						}
						else{
							newField = atoi(field);			// Aksi halde puaný int cast edip struct a aktarýyoruz.
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 2){	 	// 2.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 3){		// 3.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 4){		// 4.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 5){		// 5.puan 
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 6){		// 6.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 7){		// 7.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 8){		//	8.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
				
					for(z=0;z<j;z++){    // gecici degisken olan field ý temizliyoruz.
						field[z] = '\0';
					}
					field[j] = '\0';
					j=0;
					fieldCount++;		// bir sonraki sütuna geçebilmek için fieldCount u arttýrýyoruz.
					b++;
				}
				i++;
				field[j] = '\0';
				
			}
			i = 0;
			enqueue(newInformation);		// Ýlk satýrý struct'a enqueue ediyoruz.
		}
		fclose(file);
	}
}

void calculateSimilarity(char userName[70]){		// Similarity hesaplama fonksiyonu
	if (front == NULL){      // Kuyruk boþ ise Queue is clear mesajý döndür.
		printf("\n Queue is clear");
		return;
	}
	bookRecommendation * temp = front;
	bookRecommendation * temp2 = front;
	while(temp!=NULL){     // Struct icerisinde NULL gelene kadar dönüyoruz.
		if(strcmp(temp->userName,userName) == 0){	// Parametre olarak gelen userName, struct icindeki userName ile eþleþtiðinde iþleme baþlýyoruz.
			calculatedSimilarities *toUsers;  // gecici toUsers olustuyoruz.
			toUsers = createNodeTwo();     // 2.structta yer açýyoruz gelecek olan similarity deðerine.
			strcpy(toUsers->userName,temp->userName);
			int simCount = 0;
			while(strcmp(temp2->userName,"NU1") != 0){		// U1...U20 arasýndaki kullanýcýlar ile similarity deðerleri oluþturuyoruz.
				int control = 0;	// kitaplara verilen puanlarý alabilmek için indis deðeri.
				int count=0;  // kitap sayýsý kadar döngü kuracak deðer.
				float averageU = 0;		// U Kullanýcýsý için ortalama deðiþkeni
				float averageN = 0;		// NU Kullanýcýsý için ortalama deðiþkeni
				int commonCount=0;		// ortak okunan kitap sayýsý
				float totalU=0;			// U kullanýcýsýnýn verdiði toplam puan
				float totalN=0;			// NU kullanýcýsýnýn verdiði toplam puan
				int pointsU[sizeof(temp2->givenPoint) / sizeof(int)];	// U1..U20 arasý kullanýcýlarýn, NU kullanýcýsý ile ortak okuduðu kitaplara verdiði puanlarýn tutulduðu dizi
				int pointsN[sizeof(temp->givenPoint) / sizeof(int)];	// NU1..NU5 arasý kullanýcýlarýn, U kullanýcýsý ile ortak okuduðu kitaplara verdiði puanlarýn tutulduðu dizi
				
				while(count < sizeof(temp2->givenPoint) / sizeof(int)){
					if(temp->givenPoint[control] != 0 && temp2->givenPoint[control] != 0 ){   // Her ikisinin verdiði puan sýfýrdan farklý ise ikiside o kitabý okumuþtur.
						totalU += temp2->givenPoint[control];		// U kullanýcýsýnýn puanlarý toplanýyor.
						totalN += temp->givenPoint[control];		// NU kullanýcýsýnýn puanlarý toplanýyor.
						pointsU[commonCount] = temp2->givenPoint[control];	// U kullanýcýsýnýn puanlarý diziye aktarýlýyor.
						pointsN[commonCount] = temp->givenPoint[control];	// NU kullanýcýsýnýn puanlarý diziye aktarýlýyor.
						commonCount++;
					}
					control++;
					count++;	
				}
				
				averageU = totalU / commonCount;     // U kullanýcýsýnýn puanlarýnýn ortalamasý hesaplanýyor.
				averageN = totalN / commonCount;	// NU kullanýcýsýnýn puanlarýnýn ortalamasý hesaplanýyor.
				int i=0;
				float pay = 0;
				float payda = 0;
				
				while(i<commonCount){
					pay += (pointsN[i]-averageN) * (pointsU[i]-averageU);    // Formüldeki pay kýsmý hesaplanýyor.
					i++;
				}
				i=0;
				
				float x=0,y=0;
				while(i<commonCount){
					x += (pointsN[i]-averageN)*(pointsN[i]-averageN);
					y += (pointsU[i]-averageU)*(pointsU[i]-averageU);
					i++;
				}
				payda = sqrt(x*y);   // formüldeki payda kýsmý hesaplanýyor.
				float result;
				result = pay / payda;  // sonuç bulunuyor.
				strcpy(toUsers->who[simCount],temp2->userName);
				toUsers->similarity[simCount] = result;
				simCount++;
				temp2 = temp2->next;
			}
			enqueue2(toUsers);    // similarity deðerleri ile NU ve U kullanýcýlarýda enqueue ediliyor.
			temp2 = front;
		}
		temp = temp->next;
	}
}


calculatedSimilarities *findKUsers(char userName[50]){	// Bir okuyucunun en benzer olduðu k kiþinin belirlendiði fonksiyon
	calculatedSimilarities *temp = frontTwo;
	while(strcmp(temp->userName,userName) != 0){   // parametre olarak gelen userName i struct içinde bulana kadar dönüyoruz.
		temp = temp->next;
	}
	if(strcmp(temp->userName,userName) == 0){
		printf("\n ******  %s icin similarity degerleri ****** \t\n", temp->userName);
		int i=0,j;
		float a;
		char user[50];
		while(i<sizeof(temp->similarity)/sizeof(float)){			// Similarityleri sýraladýk ve en büyükleri döndürüyoruz.
			for (j=i+1;j<sizeof(temp->similarity)/sizeof(float);++j)
            {
                if (temp->similarity[i] < temp->similarity[j]) 
                {
                    a =  temp->similarity[i];
            		strcpy(user,temp->who[i]);
                    temp->similarity[i] = temp->similarity[j];
                    strcpy(temp->who[i],temp->who[j]);
                    temp->similarity[j] = a;
                    strcpy(temp->who[j],user);
                }
            }
			i++;
		}	
		return temp;
	}
}

int predictBook(calculatedSimilarities *sim,int k,char userName[50]){    // Hangi kitap önerilecek bunun belirlendiði fonksiyon
	bookRecommendation *userN = front;
	while(strcmp(userN->userName,userName) != 0){	// parametre olarak gelen userName i struct içinde bulana kadar dönüyoruz.
		userN = userN->next;
	}
	int i=0;
	float averageN = 0;    // NU Kullanýcýsý için ortalama deðiþkeni
	float totalN=0;		  // NU kullanýcýsýnýn verdiði toplam puan 
	int notReadBooks = 0;  // NU kullanýcýsýnýn okumaðý kitap sayýsýnýn deðiþkeni
	int placedZero[sizeof(userN->givenPoint)/sizeof(int)];  // NU kullanýcýsýnda hangi indislerde 0 var yani okunmamýþ kitap hangi indiste bunu tuttuk. U1..U20 kullanýcýlarýnda iþimize yarayacak.
	while(i<sizeof(userN->givenPoint)/sizeof(int)){  
		if(userN->givenPoint[i] == 0){
			placedZero[notReadBooks] = i;	// Okumadýðý kitaplarýn indislerini buluyoruz.
			notReadBooks++;	               // Okumadigi kitap sayisi
		}  
		totalN += userN->givenPoint[i];
		i++;
	}
	averageN = totalN / (sizeof(userN->givenPoint)/sizeof(int) - notReadBooks);   // ra ortalama : 0 dan farkli olanlarin sayisina bolduk yani okunmus kitap sayisini bulduk	
	
	int a=0;
	
	printf("\n");
	
	bookRecommendation *userU = front;
	float pay=0;	// Formüldeki pay kýsmý
	float payda = 0;  // Formüldeki payda kýsmý
	float *result = (float*)calloc(sizeof(float),notReadBooks);  // Puan tahmin deðerlerinin tutulduðu dizi
	char bookNames[notReadBooks][150];	// Kitap isimleri
	
	
	i=0;
	
	while(i<notReadBooks){			// Okunmayan kitap sayýsý kadar dönüyoruz
		int j=0;	
		while(j<k){					// En benzer k kiþi sayýsý kadar dönüyoruz.
			while(strcmp(userU->userName,"NU1") != 0){		// Kullanýcýlarý U1...U20 arasýnda olacak þekilde bir döngü daha kuruyoruz.
				int count = 0;
				float totalU = 0;		// U kullanýcýsýnýn verdiði toplam puan
				float averageU;			// U Kullanýcýsý için ortalama deðiþkeni
				int countZero = 0;		// U kullanýcýsýnýn okumadýðý kitap sayýsý deðiþkeni
				while(count < sizeof(userU->givenPoint) / sizeof(int)){   // kitap sayýsý kadar dönüp puanlarý inceliyoruz.
					totalU += userU->givenPoint[count];
					if(userU->givenPoint[count] == 0){
						countZero++;
					}
					count++;
					
				}
				averageU = totalU / (sizeof(userU->givenPoint) / sizeof(int) - countZero);    // rb ortalama : 0 dan farkli olanlarin sayisina bolduk yani okunmus kitap sayisini bulduk
				if(strcmp(userU->userName,sim->who[j]) == 0){         // Puanlarý alabilmek için ilk struct yapýmýza eriþtik kullanýcý ismi sorgusu yaparak.
					pay += sim->similarity[j] * (userU->givenPoint[placedZero[i]] - averageU);   // formüldeki pay hesaplanýyor..
					payda += sim->similarity[j];		// formüldeki payda hesaplanýyor..
					strcpy(bookNames[i],userU->books[placedZero[i]]);
				}
				userU= userU->next;
			}
			userU = front;
			j++;
		}
		result[i] = averageN + (pay/payda);  // sonuç hesaplandý ve sonuç dizisine aktarýldý.
		printf("\n'%s'---> %.3f",bookNames[i],result[i]);
		pay = 0;
		payda = 0;
		i++;
	}
	i=0;
	int j=0;
	float temporary;
	char bookName[70];
	while(i<notReadBooks){
		for (j=i+1;j<notReadBooks;++j){          // BURADA PUANI EN YÜKSEK OLANI BELÝRLÝYORUZ ÇÜNKÜ ONU KULLANICIYA ÖNERECEÐÝZ.
            if (result[i] < result[j]) 
            {
                temporary =  result[i];
            	strcpy(bookName,bookNames[i]);
                result[i] = result[j];
                strcpy(bookNames[i],bookNames[j]);
                result[j] = temporary;
                strcpy(bookNames[j],bookName);
            }
        }
		i++;
	}
	printf("\n\n Onerilecek Kitap : %s",bookNames[0]);    // En yüksek puaný olan kitap önerildi.
}


int main(){
	readTheFile();    // Dosya okuduk.
	char userName[70];		// Kullanýcýnýn gireceði kullanýcý adý : NU1...NU5
	printf("Kitap onerisi yapilacak kullanici adi : ");
	scanf("%s",userName);
	calculateSimilarity(userName);  // Similarity deðerlerini hesapladýðýmýz fonksiyon
	int k;  // Bir okuyucunun en benzer olduðu k kiþinin belirlenmesi girilecek olan k deðeri
	printf("\n\nEn benzer k kisi icin k degeri giriniz : ");
	scanf("%d",&k);
	
	calculatedSimilarities *temp = findKUsers(userName);    // Bir okuyucunun en benzer olduðu k kiþinin belirlendiði fonksiyon
	int i=0;
	while(i<k){
		printf("\n%s : %.3f",temp->who[i],temp->similarity[i]);    // En benzer k kiþiyi yazdýrýyoruz similarity deðerleri ile birlikte.
		i++;
	}
	
	predictBook(temp,k,userName);   // Son olarak hangi kitap önerilecek bunun belirlendiði fonksiyon
	
	return 0;

}
