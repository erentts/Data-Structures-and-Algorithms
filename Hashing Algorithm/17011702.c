#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TABLE_SIZE 997  // 1000'den küçük en büyük asal sayýyý tablo boyutu olarak aldýk.

typedef struct link{
	unsigned long long int key; // Key deðeri int'in alan boyunu geçtiði için unsigned long long int tanýmladým.
	char word[50];  // Kelimeleri tuttuðum yer.
	char dosyaAdi[50][50];  // Dosya isimlerini tuttuðum yer. 
}hashTable;

hashTable hash[TABLE_SIZE];    // Hash Tablosu

void baslangicKeyDegeri(){   // Tabloda key deðerlerini ilk baþta -1 olarak iþaretleme yapacaðým fonksiyon.
	int i;
	for(i=0;i<TABLE_SIZE;i++){ 
		hash[i].key = -1;  //  Böylelikle içinde deðer olmayan satýrlar -1 olarak gösterilmiþ oldu.
	}
}

double tablodakiElemanSayisi(){   // Load Factor hesaplarken bizim için gerekli olan hash tablosundaki eleman sayýsýný(N) buluyoruz.
	int i;
	double sayac=0;
	for(i=0;i<TABLE_SIZE;i++){
		if(hash[i].key != -1){  // -1 olmayan tüm keylerde kelimeler var bu yüzden -1 key deðeri olmayanlarýn sayýsýný alabiliriz.
			sayac++;
		}
	}
	return sayac;  // Tablodaki Eleman Sayýsý Döner.
}

double calculateLoadFactor(){   // LoadFactor = Tablodaki Eleman Sayýsý / Tablo Boyutu ; L = N / M 
	int tableSize = TABLE_SIZE;  // M -> Tablo Boyutu
	double tabloElemanSayisi = (double)tablodakiElemanSayisi();  // N -> Tablodaki Eleman Sayýsý
	double loadFactor = (double)(tabloElemanSayisi / tableSize); // LoadFactor = N / M
	return loadFactor;
}

void ekleHashTable(int key,char word[],char dosyaAdi[]){  // Tabloya yerleþtirilecek olan bilgilerin iþlendiði fonksiyon.
	int i = 0;
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kez hash'den geçirmemiz gerekiyor key deðerini
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // Ýkinci hash'den geçirdik.
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE;  //  i = 0 deðerine göre key için gerekli indis bulundu.
	
	if (hash[calculatedHash].key == -1){  // Eðer gelen key deðeri için belirlenen indiste -1 mevcutsa o zaman ilk defa o indise key atanacaktýr.
		hash[calculatedHash].key = key;
		strcpy(hash[calculatedHash].word,word);
		strcpy(hash[calculatedHash].dosyaAdi[0],dosyaAdi);	
	}
	else{// else durumunda ise hash tablosunda belirlenen indiste -1 yok yani bu durum bize daha önce o indise bir key deðeri yerleþmiþ olduðunu belirtiyor.
		while((hash[calculatedHash].key != -1) && hash[calculatedHash].key != key && i<TABLE_SIZE){ // i deðeri tablo boyutunu açmayacak þekilde;
			i++;  //  gelen key deðeri -1'den farklý mý , gelen key deðeri için daha önce ayný indiste bir key yerleþmiþ mi  kontrollerini yaparýz.
			calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // böylelikle kendisine boþ indis bulana dek bu döngü i sayacýmýz sayesinde devam eder.
		}
		i=0;
		
		while(strlen(hash[calculatedHash].dosyaAdi[i]) > 0){    // Gelen key deðeri ve kelime daha önce varsa üstüne yazýlýr, fakat;
			i++;												// farklý dökümandan geldiyse dosya isimleri sýrasý ile eklenir.
		}														// Bunun için ayný dosya ismi 2.kez geldiðinde onu yazdýrmamak adýna
		int kontrol=0,j;										// bir kontrol yapmamýz gerekiyor bu nedenle önce kaç tane dosya varsa sayýsýný alýyorum.
		for(j=0;j<i;j++){  // dosya sayýsý kadar döngü gerçekleþtiriyorum.
			if(strcmp(hash[calculatedHash].dosyaAdi[j],dosyaAdi)== 0){  // Yeni gelen dosya daha önce eklenmiþ mi onun sorgusunu yapýp bir kontrol deðeri oluþturuyorum.
				kontrol = 1;
			}
		}
		if(kontrol == 0){   // kontrol = 1 ise ayný dosya daha önce kaydedilmiþ fakat kontrol = 0 ise farklý dosya gelmiþ ve kaydedebiliriz.
			hash[calculatedHash].key = key;
			strcpy(hash[calculatedHash].word,word);
			strcpy(hash[calculatedHash].dosyaAdi[i],dosyaAdi);	
		}
	}
}

void oncedenHashTablosuVarmi(){  // Ýlk dökümaný kullandýðýmýzda bir hash tablosu oluþacak fakat sonradan gelen dökümanlar için önceki hash tablosunuda kullanacaðýz.
	FILE *fp;
	if((fp = fopen("17011702.txt","r"))!= NULL){ // Daha önce hiç hash tablosu oluþmuþ mu bunun sorgusunu yapýyoruz.
		printf("\nDaha once olusturulan bir hash tablosu vardi simdi yeni dokuman ismi giriniz...");
		hashTable newData;
		char temp[50];  // Sayfadaki tüm karakterleri kontrol etmek üzere kullanacaðým dizi
		char temp2[50];  // Kelime , key ve dosya adýný hashtablosuna tekrar yollarken kullanacaðým geçici dizi
		char key[50]; // temp2 dizisinden gelen key deðerini bu key dizisine atacaðým daha sonra bunu integer formatýna dönüþtürüyor olacaðým.
		char word[50]; // temp2 dizisinden gelen word deðerini bu word dizisine atacaðým.
		char dosyaAdi[50]; // temp2 dizisinden gelen word deðerini bu dosyaAdi dizisine atacaðým.
		int sira=0;  // key , kelime ve dosya adý iþlemlerinin hangisi temp2 dizisine geldiyse kontrol sayacý olarak kullanacaðým deðiþken.
		int i=0,j;
		int newKey;  // dosyadan okurken char tipinde olan key deðerini tekrar integer formatýna dönüþtüreceðiz ve o deðeri bu deðiþkende tutacaðýz.
		while(!feof(fp)){  // Dosya sonuna kadar okuma yapýyoruz.
			temp[i] = fgetc(fp);
			if(temp[i] == '/' || temp[i] == '\n'){  // Dosyada, key/kelime/dosyaAd1 dosyaAdi2 dosyaAdi 3 þeklinde tutulduðu için '/' ve '\n' sorgusu yapýyorum.
				if(sira == 0){  // sira == 0 ise KEY deðeri gelmiþtir ve key dizisinde saklýyoruz.
					strcpy(key,temp2);
					sira++;
					for(j=0;j<50;j++){  // daha sonra temp2 deðiþkenini yine kullanacaðým için içindeki karakterleri siliyorum.
						temp2[j] = '\0';
					}
					i=0;
				}
				else if(sira == 1){   // sira == 1 ise KELÝME gelmiþtir ve word dizisinde saklýyoruz.
					strcpy(word,temp2);
					sira++;
					for(j=0;j<50;j++){ // daha sonra temp2 deðiþkenini yine kullanacaðým için içindeki karakterleri siliyorum.
						temp2[j] = '\0';
					}
					i=0;
				}
				else{      // sira = 3 olmuþtur ve son iþlem olan dosya isimlerini aktarma ve daha önce word ve key dizisinde tuttuklarýmýzýda tekrar tabloya aktaracaðýz.
					strcpy(dosyaAdi,temp2); // temp2 deki dosya adýný dosyaAdi dizisine aktardýk.
					newKey = atoi(key); // key deðerinde char olarak tutulan deðeri integer olarak düzenledik.
					ekleHashTable(newKey,word,dosyaAdi);  // ekleHashTable fonksiyonuna key deðerini belirleyip hash tablosuna aktarmak üzere yolladýk.
					sira = 0;
					for(j=0;j<50;j++){
						temp2[j] = '\0';  // her seferinde yeni deðerler için kullanacaðýmýz geçici temp2 deðiþkeninin içindekileri siliyoruz.
					}
					for(j=0;j<50;j++){
						key[j] = '\0';   // her seferinde yeni deðerler için kullanacaðýmýz geçici key deðiþkeninin içindekileri siliyoruz.
						word[j] = '\0';   // her seferinde yeni deðerler için kullanacaðýmýz geçici word deðiþkeninin içindekileri siliyoruz.
						dosyaAdi[j] = '\0';   // her seferinde yeni deðerler için kullanacaðýmýz geçici dosyaAdi deðiþkeninin içindekileri siliyoruz.
					}
					i=0;
					newKey = 0;
				}
			}
			else{
				temp2[i] = temp[i];  // temp ile son iþlemde her zaman özel karakter geleceði için key,kelime ve dosya adýný temp2'de tutuyorum.
				i++;
			}
		}
	}
}

void dosyayaYaz(double loadFactor){   // Hash Tablosunu dosyaya yazdýðýmýz fonksiyon
	FILE *fp;
	if((fp = fopen("17011702.txt","w+")) == NULL){  
		printf("Dosya acma hatasi");
	}
	
	int i,j=0;
	for(i=0;i<TABLE_SIZE;i++){
		if(hash[i].key != -1){     // -1 deðerine eþit olan key'lere bilgi gelmemiþtir ve onlarý haricinde olan tüm bilgileri döndürebiliriz.
			fprintf(fp,"%d/%s/",hash[i].key,hash[i].word);  // her gelen satýr için key, word deðerini yazdýrýrýz.
			j=0;
			while(strlen(hash[i].dosyaAdi[j]) > 0){  // dosya adýnda ise 1 den fazla dosya adý ismi olabileceði için onuda kendi içerisinde döngüye sokuyoruz.
				fprintf(fp,"%s",hash[i].dosyaAdi[j]);
				j++;
			}
			fprintf(fp,"%c\n",' ');
		}
	}
	fprintf(fp," Load Factor  = %.1f",loadFactor);
	fclose(fp);
}

void dokumanOku(){  // Dökümandan gelen kelimeleri okuyup kelimeler için horner metoduna göre key deðeri oluþturup bunu ekleHashTable fonksiyonuna yollayýp hashtable üzerinde gerekli indise yerleþtiriyoruz.
	char dosyaAdi[50];
	printf("\nAcilacak dosya adi giriniz ( Ornek : dosyaAdi.txt ) : ");
	scanf("%s",dosyaAdi);
	FILE *fp;
	if((fp = fopen(dosyaAdi,"r"))== NULL){   // Dosya okuma kontrolü
		printf("Dosya acma hatasi");
	}
	int i=0,j;
	double loadFactor; // loadFactor deðiþkeni
	unsigned long long int key=0;  // key deðeri int deðerini aþacaðý için unsigned long long int yaptým.
	char temp[50];   // Dökümandaki tüm karakterleri kontrol olarak kullanmak için alacaðým geçici dizi
	char temp2[50];  // Dökümandaki tüm kelimeleri alacaðým geçici dizi
	while(!feof(fp)){
		temp[i] = fgetc(fp);
		if(temp[i] == ' ' || temp[i] == '\n'){   // Dökümanda kelimeler arasýnda boþluk (' ') olacaðý için kontrol yapýyorum her boþlukta kelimeyi alýp hash tablosuna aktarýyorum.
			int length = strlen(temp2);          // Ayrýca satýr sonuna gelince ('\n') satýrdaki son kelimeyide alýp alt satýr için iþleme devam eder.
			int power = length-1;  //  Horner metodunda R^(Length-1) alacaðý için üs deðerini belirliyoruz ve döngüde 0 a kadar azalacak þekilde iþleme sokuyoruz.
			key = 0;
			char gecici;
			for(j=0;j<length;j++){
				gecici = temp2[j];
				if (gecici >= 'a'){     // Araba ve araba ayný kelimeler gibi düþüneceðiz bu nedenle kontrolü gerçekleþtirdik.
					gecici = 'A' - 'a' + gecici;
				}
				key += abs((pow(3,power)*(gecici-'A'+1)));   // Horner metodunda gelen kelimenin karakter sayýsýný kullanarak döngüye sokuyoruz iþlem bittiðinde kelime için key deðeri oluþacaktýr.
				power--;		
			}	
			loadFactor = calculateLoadFactor(); // calculateLoadFactor fonksiyonu ile Load Factor sayýsýný çektik.
			printf("LoadFactor : %.1f\n",loadFactor);
			if(loadFactor == 1.0 || loadFactor > 1.0){  // Eðer LoadFactor >= 1.0 ise Ekleme Ýþlemi Duracak ve Eklenemeyen Kelimeler Ekrana Yazýlacak.
				printf("Eklenemeyen Kelime : %s\n",temp2);
				for(j=0;j<50;j++){ // temp2 daha sonraki kelimelerde kullanacaðýmýz için içini boþalttýk.
					temp2[j] = '\0';
				}
				i=0;
			}
			else{
				if(loadFactor > 0.8){   // Load Factor > 0.8 'den 0.8 deðerini geçtiðimizi bildirmek için kullanýcýya uyarý mesajý verecek.
					printf("LOAD FACTOR 0.8 degerini gecti\n");
				}
			
				if (key != 0){
					ekleHashTable(key,temp2,dosyaAdi);  // Gelen key deðeri,kelime ve dosyaAdina göre Hash tablosuna eklenmek üzere ekleHashTable fonksiyonuna yollanýyor.
				}
				
				for(j=0;j<50;j++){  // Gecici temp2 degiskeninin içini boþaltýyoruz.
					temp2[j] = '\0';
				}	

				i=0;
			}
		}
		else{
			temp2[i] = temp[i];  // temp ile son iþlemde her zaman özel karakter geleceði için kelimeyi temp2'de tutuyorum.
			i++;
		}
	}
	dosyayaYaz(loadFactor);  // Daha sonra dosyaya yazdýrýyorum oluþan hash tablosunu ve loadfactor deðerinide yolluyorum.
	fclose(fp);
}

void kelimeAra(char word[]){  // Bu fonksiyon sayesinde aranan kelime hash tablosunda var mý ? eðer var ise hangi dökümanlarda yer aldýðýný kullanýcýya çýktý olarak verecek.
	int i=0,adimSayisi=1,j;
	unsigned long int key=0;  // key deðeri int deðerini aþacaðý için unsigned long long int yaptým.
	int length = strlen(word); 
	int power = length-1; //  Horner metodunda R^(Length-1) alacaðý için üs deðerini belirliyoruz ve döngüde 0 a kadar azalacak þekilde iþleme sokuyoruz.
	char gecici;
	for(j=0;j<length;j++){
		gecici = word[j];
		if (gecici >= 'a'){  // Araba ve araba ayný kelimeler gibi düþüneceðiz bu nedenle kontrolü gerçekleþtirdik.
			gecici = 'A' - 'a' + gecici;
		}	
		key += abs((pow(3,power)*(gecici-'A'+1)));  // Horner metodunda gelen kelimenin karakter sayýsýný kullanarak döngüye sokuyoruz iþlem bittiðinde kelime için key deðeri oluþacaktýr.
		power--;
	}
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kere hash'den geçirip indis deðeri üreteceðimiz için
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // Ýkinci hash iþlemi
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // Ve indis deðeri oluþtu.
	if(hash[calculatedHash].key == key){   // Ýlgili indiste bulunan key deðeri ile kullanýcýnýn göndermiþ olduðu kelimenin key deðeri birbirine eþit mi ? 
		printf("\n%d adim gerceklesti.",adimSayisi);
		i=0;
	
		while(strlen(hash[calculatedHash].dosyaAdi[i]) > 0){     // kaç tane döküman ismi varsa sayýsýný tutuyoruz.
			i++;
		}
		int dosyaSayisi = i;
		for(i=0;i<dosyaSayisi;i++){  // döküman sayýsý kadar döngü kurup kelimenin yanýna dosya isimlerini tek tek yazýdýrýyoruz.
		printf("\n%s --> %s",hash[calculatedHash].word,hash[calculatedHash].dosyaAdi[i]);     
		}
	}
	else{
		printf("\n%d adim gerceklesti ve aranan kelime tabloda mevcut degil.",adimSayisi);
	}
}


int main(){
	char word[50];   // Aranacak kelimenin tutulduðu dizi
	baslangicKeyDegeri();   // Hash tablosunda baþlangýçta tüm KEY deðerlerini -1 olarak iþaretliyoruz.
	oncedenHashTablosuVarmi();   // Daha önceden hash tablosu varmý onu kontrol ediyoruz.
	printf("\n Dokuman Eklemek icin '1' tusuna basin  /  Kelime Aramak icin '2' tusuna basin");
	int secim;
	scanf("%d",&secim);
	
	if(secim == 1){
		dokumanOku();   // // Dökümandan gelen kelimeleri okuyup kelimeler için horner metoduna göre key deðeri oluþturup bunu ekleHashTable fonksiyonuna yollayýp hashtable üzerinde gerekli indise yerleþtirmek üzere çaðýrdýðýmýz fonksiyon
	}
	else if(secim == 2){
		printf("\nArayacaginiz kelimeyi giriniz : ");
		scanf("%s",word);
		kelimeAra(word);  // Kelime arama iþlemi
	}
	else{
		printf("Yanlis Tercih Yaptiniz !");
	}


	return 0;
}
