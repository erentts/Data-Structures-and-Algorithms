#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TABLE_SIZE 999983  // 1000000'den küçük en büyük asal sayýyý tablo boyutu olarak aldýk.

typedef struct link{
	unsigned long long int key; // Key deðeri int'in alan boyunu geçtiði için unsigned long long int tanýmladým.
	char word[50];  // Kelimeleri tuttuðum yer.
}hashTable;

typedef struct link2{
	unsigned long long int key;
	char hataliKelime[50];   // Hatalý kelime tuttuðum yer
	char onerilenKelime[50]; // Önerilen kelimeyi hatalý kelimenin yanýna eklersek bir sonraki hatalý kelime arayýþýmýzda önceden hata aldýðýmýz deðer gelirse yanýndaki önerilen kelimeyi direk çýktý olarak verecek.
}hataliKelimeTablosu;

hashTable hash[TABLE_SIZE];    // Hash Tablosu
hataliKelimeTablosu hash2[TABLE_SIZE];  // Hatalý Kelime Tablosu

void baslangicKeyDegeri(){   // Tabloda key deðerlerini ilk baþta -1 olarak iþaretleme yapacaðým fonksiyon.
	int i;
	for(i=0;i<TABLE_SIZE;i++){ 
		hash[i].key = -1;  //  Böylelikle içinde deðer olmayan satýrlar -1 olarak gösterilmiþ oldu.
		hash2[i].key = -1;
	}
}

void ekleHashTable(int key,char word[]){  // Tabloya yerleþtirilecek olan bilgilerin iþlendiði fonksiyon.
	int i = 0;
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kez hash'den geçirmemiz gerekiyor key deðerini
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // Ýkinci hash'den geçirdik.
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE;  //  i = 0 deðerine göre key için gerekli indis bulundu.
	
	if (hash[calculatedHash].key == -1){  // Eðer gelen key deðeri için belirlenen indiste -1 mevcutsa o zaman ilk defa o indise key atanacaktýr.
		hash[calculatedHash].key = key;
		strcpy(hash[calculatedHash].word,word);
	}
	else{// else durumunda ise hash tablosunda belirlenen indiste -1 yok yani bu durum bize daha önce o indise bir key deðeri yerleþmiþ olduðunu belirtiyor.
		while(i<TABLE_SIZE){ // i deðeri tablo boyutunu açmayacak þekilde;
		  //  gelen key deðeri -1'den farklý mý , gelen key deðeri için daha önce ayný indiste bir key yerleþmiþ mi  kontrollerini yaparýz.
			if(strcmp(hash[calculatedHash].word, word) != 0){
				calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // böylelikle kendisine boþ indis bulana dek bu döngü i sayacýmýz sayesinde devam eder.
				if (hash[calculatedHash].key == -1 ) 
			    {
					hash[calculatedHash].key = key;  // tabloya yerleþtiriliyor.
					strcpy(hash[calculatedHash].word,word);
				}
			}
			i++;
		}
	}
}

void dokumanOku(){  // Dökümandan gelen kelimeleri okuyup kelimeler için horner metoduna göre key deðeri oluþturup bunu ekleHashTable fonksiyonuna yollayýp hashtable üzerinde gerekli indise yerleþtiriyoruz.
	FILE *fp;
	if((fp = fopen("smallDictionary.txt","r"))== NULL){   // Dosya okuma kontrolü
		printf("Dosya acma hatasi");
	}
	int i=0,j;
	unsigned long long int key=0;  // key deðeri int deðerini aþacaðý için unsigned long long int yaptým.
	char temp[50];   // Dökümandaki tüm karakterleri kontrol olarak kullanmak için alacaðým geçici dizi
	char temp2[50];  // Dökümandaki tüm kelimeleri alacaðým geçici dizi
	while(!feof(fp)){
		temp[i] = fgetc(fp);
		if(temp[i] == ' ' || temp[i] == '\n'){   // Dökümanda kelimeler arasýnda boþluk (' ') olacaðý için kontrol yapýyorum her boþlukta kelimeyi alýp hash tablosuna aktarýyorum.
			key = hornerMetodu(temp2);
			key = abs(key);
			if (key != 0){
				ekleHashTable(key,temp2);  // Gelen key deðeri,kelime ve dosyaAdina göre Hash tablosuna eklenmek üzere ekleHashTable fonksiyonuna yollanýyor.
			}
				
			for(j=0;j<50;j++){  // Gecici temp2 degiskeninin içini boþaltýyoruz.
				temp2[j] = '\0';
			}	
			i=0;
		}
		else{
			temp2[i] = temp[i];  // temp ile son iþlemde her zaman özel karakter geleceði için kelimeyi temp2'de tutuyorum.
			i++;
		}
	}
	fclose(fp);
}

int kelimeAra(char word[]){  // Bu fonksiyon sayesinde aranan kelime hash tablosunda var mý ? eðer var ise hangi dökümanlarda yer aldýðýný kullanýcýya çýktý olarak verecek.
	int i=0,j;
	unsigned long long int key;  // key deðeri int deðerini aþacaðý için unsigned long long int yaptým.
	key = hornerMetodu(word);
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kere hash'den geçirip indis deðeri üreteceðimiz için
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // Ýkinci hash iþlemi
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // Ve indis deðeri oluþtu.
	int mesafeBirVarmi=0;
	int mesafeIkiVarmi=0;
	if(hash[calculatedHash].key == key && strcmp(hash[calculatedHash].word, word) != 1){   // Ýlgili indiste bulunan key deðeri ile kullanýcýnýn göndermiþ olduðu kelimenin key deðeri birbirine eþit mi ? 
		printf("\nKelime hash tablosunda zaten mevcut : %s",hash[calculatedHash].word);
		return 1;
	}
	else{
		printf("\n");
		int distance; // LevenshteinEditDistance fonksiyonundan dönecek olan Distance(Mesafe) deðeri.
		for(i=0;i<TABLE_SIZE;i++){
			if(hash[i].key != -1){  
				distance = LevenshteinEditDistance(word,hash[i].word); // Tablodaki tüm kelimeler, klavyeden girilen kelime ile mesafesi bulunmak üzere LevenshteinEditDistance fonksiyonuna gönderilir.
				if(distance == 1){  // Mesafesi 1 olanlar
					printf("\n'%s' icin bu kelimeyi mi aradiniz (Mesafe %d) : %s",word,distance,hash[i].word);
					mesafeBirVarmi = 1; // Mesafe 1 olan kelimeler var ise ekranda onlar gösterilmeli bu yüzden kontrol iþareti koyuyoruz.
				}
			}
		}
		if(mesafeBirVarmi != 1){ // Eðer mesafeBirVarmi != 1 ise o zaman mesafe 1 olan kelime hiç yoktur.
			for(i=0;i<TABLE_SIZE;i++){
				if(hash[i].key != -1){
					distance = LevenshteinEditDistance(word,hash[i].word);  // Yine tablodaki tüm kelimeler ile klavyeden girilen kelimelerin mesafesi hesaplanýr.
					if(distance == 2){  // Mesafesi 2 olanlar
						printf("\n'%s' icin bu kelimeyi mi aradiniz (Mesafe %d) : %s",word,distance,hash[i].word);
						mesafeIkiVarmi = 1; // Mesafe 1 olmayýp, Mesafe 2 olan kelimeler var ise ekranda onlar gösterilmeli bu yüzden kontrol iþareti koyuyoruz.
					}
				}
			}			
		}
		if(mesafeBirVarmi == 0 && mesafeIkiVarmi == 0){ // Eðer Mesafe 1 ve Mesafe 2 için kelime bulunamazsa o halde aranan kelime hem hash tablosunda yok hem de mesafesi 2den büyüktür.
			return 2;
		}
	}
		
}

int dogruKelimeyiAra(char word[]){  // Bu fonksiyon sayesinde aranan kelime hash tablosunda var mý ? eðer var ise hangi dökümanlarda yer aldýðýný kullanýcýya çýktý olarak verecek.
	int i=0,j;
	unsigned long long int key;  // key deðeri int deðerini aþacaðý için unsigned long long int yaptým.
	key = hornerMetodu(word); // kelimeye ait key deðeri bulmak için horner metodundan geçirdik.
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kere hash'den geçirip indis deðeri üreteceðimiz için
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // Ýkinci hash iþlemi
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // Ve indis deðeri oluþtu.
	if(hash[calculatedHash].key == key && strcmp(hash[calculatedHash].word, word) != 1){   // Ýlgili indiste bulunan key deðeri ile kullanýcýnýn göndermiþ olduðu kelimenin key deðeri birbirine eþit mi ? 
		return 1; //  1 : Kelime var
	}
}

int hornerMetodu(char word[]){
	int i=0,j,k=0;
	unsigned long long int key=0;  // key deðeri int deðerini aþacaðý için unsigned long long int yaptým.
	int length = strlen(word); 
	int power = length-1; //  Horner metodunda R^(Length-1) alacaðý için üs deðerini belirliyoruz ve döngüde 0 a kadar azalacak þekilde iþleme sokuyoruz.
	char gecici;
	int fark = 'a' - 'A';  //ASCII tablosunda küçük harfler ve büyük harfler arasýndaki fark 32.
	while (word[k] != '\0') {
        if (word[k] >= 'A' && word[k] <= 'Z') { //harf büyük ise küçültüyoruz..
            word[k] += fark;  // fark kadar eklersek bu harfin ASCII degeri büyür ve ayný harfin küçüðüne sahip olmuþ olur.
        }
        k++;
    }
	for(j=0;j<length;j++){
		gecici = word[j];
		if (gecici >= 'a'){  // Araba ve araba ayný kelimeler gibi düþüneceðiz bu nedenle kontrolü gerçekleþtirdik.
			gecici = 'A' - 'a' + gecici;
		}	
		key += abs((pow(31,power)*(gecici-'A'+1)));  // Horner metodunda gelen kelimenin karakter sayýsýný kullanarak döngüye sokuyoruz iþlem bittiðinde kelime için key deðeri oluþacaktýr.
		power--;
	}
	key = abs(key);
	return key;  // key deðerini döndürüyoruz.
}

int hataliTablodaArama(int key,char kelime[]){ // Hatalý tabloda arama fonksiyonu
	int i=0;
	key = abs(key);
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kere hash'den geçirip indis deðeri üreteceðimiz için
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // Ýkinci hash iþlemi
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // Ve indis deðeri oluþtu.
	if(hash2[calculatedHash].key == key && strcmp(hash2[calculatedHash].hataliKelime, kelime) != 1){   // Ýlgili indiste bulunan key deðeri ile kullanýcýnýn göndermiþ olduðu kelimenin key deðeri birbirine eþit mi ? 
		printf("\n\n'%s' kelimesi HATALI KELIMELER TABLOSUnda mevcut. Bu kelime icin daha once kabul ettiginiz kelime : '%s'",hash2[calculatedHash].hataliKelime,hash2[calculatedHash].onerilenKelime);
		return 1; // Daha önce kelimenin hatalý kelime tablosunda var olduðunu belirten kontrol deðeri.
	}
	else{
		return 0;
	}
}

void ekleHataliTablo(int key,char hataliKelime[],char onerilenKelime[]){
	int i = 0;
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kez hash'den geçirmemiz gerekiyor key deðerini
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // Ýkinci hash'den geçirdik.
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE;  //  i = 0 deðerine göre key için gerekli indis bulundu.
	
	if (hash2[calculatedHash].key == -1){  // Eðer gelen key deðeri için belirlenen indiste -1 mevcutsa o zaman ilk defa o indise key atanacaktýr.
		hash2[calculatedHash].key = key;
		strcpy(hash2[calculatedHash].hataliKelime,hataliKelime);
		strcpy(hash2[calculatedHash].onerilenKelime,onerilenKelime);
	}
	else{// else durumunda ise hash tablosunda belirlenen indiste -1 yok yani bu durum bize daha önce o indise bir key deðeri yerleþmiþ olduðunu belirtiyor.
		while(i<TABLE_SIZE){ // i deðeri tablo boyutunu açmayacak þekilde;
		//  gelen key deðeri -1'den farklý mý , gelen key deðeri için daha önce ayný indiste bir key yerleþmiþ mi  kontrollerini yaparýz.
			if(strcmp(hash2[calculatedHash].hataliKelime, hataliKelime) != 0){
				calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // böylelikle kendisine boþ indis bulana dek bu döngü i sayacýmýz sayesinde devam eder.
				if (hash2[calculatedHash].key == -1 ) 
			    {
					hash2[calculatedHash].key = key;
					strcpy(hash2[calculatedHash].hataliKelime,hataliKelime);
					strcpy(hash2[calculatedHash].onerilenKelime,onerilenKelime);
				}
			}
			i++;
		}
		
	}
}

int enKucukBul(int x,int y, int z){ // Insert , Delete ve Change iþlemleri için en küçüðü bulan fonksiyon
	if(x<=y && x<=z){
		return x;
	}
	else if(y<=x && y<=z){
		return y;
	}
	else{
		return z;
	}
}


int LevenshteinEditDistance(char ilkKelime[],char ikinciKelime[]){  // Mesafe bulduðumuz fonksiyon
	
	int i,j;
	int ilkKelimeLength = strlen(ilkKelime); // 1.kelime karakter sayýsý
	int ikinciKelimeLength = strlen(ikinciKelime); // 2.kelime karakter sayýsý
	int insert,delete,change,copy;
	char temp,temp2;
	
	int matris[ilkKelimeLength+1][ikinciKelimeLength+1]; // kelimelerin 1 fazlasýnýn olma sebebi en baþta (0x0) boþ karakter olarak almamýz.
	
	for(i=0;i<=ikinciKelimeLength;i++){
		matris[0][i] = i;	// Ýlk Satýrý initialize ettik boþ karakterlere olan uzaklýklarýný kullanarak.
	}
	for(i=0;i<=ilkKelimeLength;i++){
		matris[i][0] = i;	// Ýlk Sütunu initialize ettik boþ karakterlere olan uzaklýklarýný kullanarak.
	}
	
	for(i=1;i<=ilkKelimeLength;i++){  // Matriste ilgili indislere Mesafe'leri eklemek için geziniyoruz.
		temp = ilkKelime[i-1];  // i=1'den baþladýðýmýz için i-1 yaptýk.
		for(j=1;j<=ikinciKelimeLength;j++){
			temp2 = ikinciKelime[j-1]; // j=1'den baþladýðýmýz için j-1 yaptýk.
			if(temp == temp2){
				copy = matris[i-1][j-1];  // Ayný harf gelirse sol çapraz üstündeki Mesafe'yi alabiliriz. Copy iþleminde ayný harf olduðu için COST olmaz.
				matris[i][j] = copy;
			}
			else{
				insert = matris[i-1][j]+1; // Insert için Mesafe iþlemi
				delete = matris[i][j-1]+1; // Delete için Mesafe iþlemi
				change = matris[i-1][j-1]+1; // Change için Mesafe iþlemi
				matris[i][j] = enKucukBul(insert,delete,change);  // Insert, Delete ve Change'den dönen Mesafe'lerden en küçüðünü matris[i][j] ye ekleyeceðiz.
				if(matris[i][j] > 2 && (( ilkKelimeLength-ikinciKelimeLength + j) == i)){  // ****** BONUS ********  Alt satýrlarda açýkladým ama raporda daha detaylý açýkladým.
					// enKucukBul fonksiyonundan dönen deðer 2'den büyük VE 
					// mevcut indis yani iki kelimenin farkýnýn j ile toplamý i'ye eþit ise o zaman matris[ilkKelimeLength][ikinciKelimeLength] indisine diyagonal olan indislerde 2'den büyük bir mesafe deðeri yerleþti.
					return -1; 
				}
			}
			temp2 = '\0'; // geçici karakteri sýfýrladýk.
		}
		temp = '\0'; // geçici karakteri sýfýrladýk.
	}
	return matris[ilkKelimeLength][ikinciKelimeLength]; // Matrisin en sað ve en altta olan indisi bize iki kelimenin birbirine Distance'ýný verecek.
}


int main(){
	baslangicKeyDegeri();   // Hash tablosunda baþlangýçta tüm KEY deðerlerini -1 olarak iþaretliyoruz.
	dokumanOku();  // smallDictionary sözlüðündeki kelimeler hash tablosuna çekilir.
	while(1){
		char cumle[200];
		printf("\nCumle giriniz : ");
		fflush(stdin); // bilgisayarýmda gets düzgün çalýþmýyordu internetten arattýðýmda bunun eklenmesi ile sorun çözülüyor yazýyordu ve ekledim çalýþtý.
		gets(cumle); // Girilecek cümle
		char temp[80]; // Cümleleri kelimelere ayýrýp burada tutup bu dizi üzerinden iþlem yapacaðýz.
		
		int len = strlen(cumle);
		int i = 0;
		while(i < len){  // Cümle karakter sayýsý kadar döngü kurulacak.
			int j = 0;
			while(i < len && cumle[i] != ' '){ // Her kelime arasýnda boþluk var bu yüzden boþluk kontrolü yaptýk.
				temp[j] = cumle[i];  // Cümledeki her karakteri temp'e atýyoruz.
				j++;
				i++;
			}
			i++;
			temp[j] = '\0';
			int key; // hatali kelimenin key deðeri
			key = hornerMetodu(temp); // kelime icin key deðeri üretiyoruz horner fonksiyonu ile
			if(hataliTablodaArama(key,temp) == 1){ // Daha önceden hatalý tabloda varmý ?
				// print islemini 	hataliTablodaArama() icinde yaptýgým icin buradan iþlem yapmama gerek kalmadý.
			}
			else{
				char dogruKelime[50]; // onerilen kelimeler için gireceðimiz dogru kelime
				switch(kelimeAra(temp)){  // kelime arama fonksiyonuyla gerçekleþecek olan iþlem üzerinden dönen return deðerine göre switch kontrolü yapýyoruz.
					case 1: break;  // Kelime hash tablosunda mevcut çýktýsý verir.
					case 2: printf("\n'%s' kelimesi HASH TABLOSUnda yok ayrica onerilen kelimede bulunamadi !\n",temp); break;
					default :
							printf("\n\nDogru kelimeyi secin : ");
							scanf("%s",dogruKelime);
							if(dogruKelimeyiAra(dogruKelime) == 1){  // Onerilenlere göre kullanýcýnýn girdiði dogru kelimeyi yeniden hash tablosunda arýyoruz.
								ekleHataliTablo(key,temp,dogruKelime); // varsa onerilenlerden doðru kelime girmiþtir ve hatali tabloya ekleme yapýyoruz.
							}
							else{
								printf("\nOnerilen kelimelerin disinda bir secim yaptiniz !(Cunku hash tablosunda girdiginiz kelime yok !)\n");
							}
							break;
				}		
			}
			for(j=0;j<80;j++){  // kelimeyi tuttugum temp'i bir sonraki kelimeyi alacaðýmdan dolayý temizliyorum.
				temp[j] = '\0';
			}		
		}
	}
	
	return 0;
}
