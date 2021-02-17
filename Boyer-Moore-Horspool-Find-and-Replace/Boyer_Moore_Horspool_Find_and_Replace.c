#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define BOYUT 25000   // Çok uzun bir metinde girilebileceðinden dolayý rastgele çok yüksek verdim.



void tablo_olustur(char aranankelime[50], int aranan_uzunluk, int tablo[BOYUT], int kontrol) // Badmatch tablosu oluþturuyoruz..
{
	int i;
	for(i=0;i<BOYUT;++i)
	{
		tablo[i] = aranan_uzunluk;  // Öncelikle aradýðýmýz string'in uzunlugunu tablomuzun indislerine atadýk.
	}
	for(i=0;i<aranan_uzunluk-1;++i){
		
		if (kontrol != 0){  // kontrol degiskenini almamýn nedeni not case sensitive yada case sensitive kullanýlýp kullanýlmayacagýný sorgulamak icin.
			if (aranankelime[i]>'a'){
				tablo[aranankelime[i]+'B'-'b'] = aranan_uzunluk-i-1;	// Not Case sensitive durumunda;
			}                                   // aradýðýmýz stringdeki karakterlerin kayma miktarlarýný belirledik.
			else if(aranankelime[i]<='a'){               
				tablo[aranankelime[i]-'B'+'b'] = aranan_uzunluk-i-1;  
			}		
		}	
		tablo[aranankelime[i]] = aranan_uzunluk-i-1;  // Aradýðýmýz stringdeki karakterlerin kayma miktarlarýný belirledik.
	}

}


int en_buyuk (int x,int y) {  // En büyük deger fonksiyonu
	if (x>y){
		return x;
	}
	else{
		return y;
	}
}


// Boyer Moore Horspool
int boyer_moore_horspool(char aranankelime[50], int aranan_uzunluk, char metin[BOYUT], int metin_uzunluk,char yenikelime[50],int yeni_uzunluk,int kontrol)  
{
	int i,a,k,tablo[BOYUT];
	tablo_olustur(aranankelime, aranan_uzunluk, tablo, kontrol); 
	
	// String icinde arama islemleri
	int islem_sayisi=0;  // found and replace iþlemlerinin kaç adet yapýldýgýný bu degisken icinde tutuyorum.
	int j = 0;
	while (j <= (metin_uzunluk - aranan_uzunluk)) {
		int k = aranan_uzunluk-1;
        while(((aranankelime[k] == metin[j+k]) && (k >= 0) || (kontrol == 1 && (fabs(aranankelime[k]-metin[j+k])==fabs('A'-'a'))))){  
			k--;  // case sensitive kontrolü ve aranan kelimenin metin içinde eþleþip eþleþmeme durumunu sorguluyoruz.
		} // eþleþtirme yaparken tek tek karakterlere bakýlýyor ve k deðeri azaltýlýyor eðer k<0 olursa tüm karakterlerimiz eþleþti ve;
        // kelime bulundu anlamýna geliyor.
        if (k < 0)
        {
				// REPLACE ÝSLEMLERÝ
				if (aranan_uzunluk==yeni_uzunluk){ // Eðer aradýðýmýz kelimenin karakter sayýsý , yeni girdiðimiz kelimenin karakter sayýsý ile eþitse;
					for(i=0;i<aranan_uzunluk;i++){ // shift yapmadan direk karakterlerin yerlerini deðiþtiriyorum.
						metin[j] = yenikelime[i];
						j++;
					}	
				}
				else if (yeni_uzunluk>aranan_uzunluk){  // Eðer yeni kelimenin karakter sayýsý , aradýðýmýz kelimenin karakter sayýsýndan büyük ise;	
					for (i=0;i<(yeni_uzunluk-aranan_uzunluk);i++){ ; // Saða shift yapmam gerekiyor ve bu shift miktarý (yeni_kelime - aranan_kelime) kadar olacak
						for(a=metin_uzunluk;a>j;a--){ // Örnek : 70.indis son indisim ise, 1 kere saða shift yapacaksam artýk son indisim 71.indis olacak.
							metin[a+1] = metin[a];
							
						}
					metin_uzunluk = metin_uzunluk + (yeni_uzunluk-aranan_uzunluk);
					}
					for(i=0;i<yeni_uzunluk;i++){ // Yukarýda yaptýðým saða shift iþleminden sonra artýk kelimeleri eklemek kaldý ve onlarýda eklemiþ oldum.
						metin[j] = yenikelime[i];
						j++;
					}
				}
				else {    // Burasý son durum.Yani aranan kelimenin karakter sayýsýnýn, yeni gelen kelimenin karakter sayýsýndan büyük olmasý durumunda;
					for (i=0;i<(aranan_uzunluk-yeni_uzunluk);i++){ // Sola shift iþlemi yapmamýz gerekiyor ve bu shift miktarý (aranan_kelime - yeni_kelime) kadar olacak.
						for(a=(j+yeni_uzunluk);a<metin_uzunluk;a++){ // Örneðin ilk eþleþen kelimenin indisi 21 olsun; ilk 20 karakteri sabit tutup kelimenin saðýnda kalan
							metin[a] = metin[a+1]; // karakterleri shift miktarý kadar sola kaydýrýyorum.
							
						}
						
					}
					for(i=0;i<yeni_uzunluk;i++){ // shift islemi bittikten sonra kelimeleri ekliyorum.
						metin[j] = yenikelime[i];
						j++;
					}
				
				}

			if ((j+aranan_uzunluk)<metin_uzunluk){      	
            	j = j + aranan_uzunluk-tablo[metin[j+aranan_uzunluk]];
			}
			else{
				j = j+1;
			}
			islem_sayisi++; // Found and replace iþlemi kaç kez oldugunu kontrol ettiðim deðiþken.  
        }
        else{
        	j = j + en_buyuk(1,tablo[metin[j+aranan_uzunluk-1]]);
		}  
		
    }
    return islem_sayisi;
}

int main(){
	struct timespec baslangic;
	struct timespec bitis;
	char metin[BOYUT];
	char aranankelime[50];
	char yenikelime[50];
	int kontrol;
	
	int islem_sayisi;
	
	printf("Aramak istediginiz kelimeyi giriniz : ");
	gets(aranankelime);
	printf("\nYeni kelimeyi giriniz : ");
	gets(yenikelime);
	
	char dosya_isim[50];
	printf("Dosya ismi girin (Lutfen .txt uzantisi eklemeyi unutmayin !) : ");
	scanf( "%s",dosya_isim);
	
	FILE *fp;
	if ((fp = fopen(dosya_isim, "r")) == NULL) {
	    printf("Dosya acilmadi..");
	    exit(1);
	}
	
	printf("\n\nDosya icindeki metin : \n\n");  // Dosyadaki metni dizimize aktarýp ayný zamanda console'da kullanýcý ekranýna çýkmasýný saðladýk.
	fread(metin,sizeof(char),BOYUT,fp);
	
	fclose(fp);
	int count=0;
	while (metin[count] != '\0'){
		printf("%c",metin[count]);
		count++;
	}
	
	printf("\n\nTercih : \n\nNot Case Sensitive icin 1'e basin. \nCase Sensitive icin 0'a basin. \n");
	scanf("%d",&kontrol);
	
	printf("\nAranan Kelime : %s \t Yeni Kelime : %s\n",aranankelime,yenikelime);
	
	clock_gettime(CLOCK_MONOTONIC, &baslangic); // Arama ve yerdeðiþtirme iþlemlerinin ne kadar sürede gerçekleþtiðini  göstermek için kullandýk.
	islem_sayisi = boyer_moore_horspool(aranankelime, strlen(aranankelime),metin, strlen(metin),yenikelime,strlen(yenikelime),kontrol); // found and replace sayýsý
	clock_gettime(CLOCK_MONOTONIC, &bitis);
	
	FILE *fp2 = fopen(dosya_isim,"w");
	int sayac=0;
	while (metin[sayac]!='\0'){  // Found and Replace iþlemlerinin ardýndan gelen yeni text'i dosyaya yazýyoruz.
		fputc(metin[sayac],fp2);
		sayac++;
	}

	fclose(fp2);

	
	printf("\n\nYeni metin : \n\n%s\n\nFound and Replace : %d",metin,islem_sayisi);
	
	
	printf( "\n\nIslem suresi : %.9lf milisaniye \n\n", (double)((long int)(bitis.tv_sec-baslangic.tv_sec)*1000000000 + (bitis.tv_nsec-baslangic.tv_nsec)) / 1000000 );
	return 0;
}
	
   		
	
	
	

