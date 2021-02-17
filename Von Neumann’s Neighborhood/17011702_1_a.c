#include <stdio.h>
#include <stdlib.h>

int *enYakinDegerlerBul(int dizi[],int n){  // Olusturdugumuz dizi ve onun eleman sayisini parametre olarak alýyoruz.
	if (n<2){         //  En yakin 2 nokta bulmamýz gerektigi icin minimum 2 elemanli bir dizi olmali !
		printf("\nEn yakin 2 deger icin bu dizi en az 2 degere sahip olmali !");
		return;
	}
	else {           // Kod, else bloðuna girdiðinde en az 2 elemanlý bir diziye sahip olduðumuzu anlýyoruz.
		int i;  // Dýþ döngü için sayaç deðiþkeni
		int j;  // Ýç döngü için sayaç deðiþkeni
		int enYakinFark = abs(dizi[0]-dizi[1]);  // Baþlangýç olarak fonksiyona parametre olarak gelen dizinin ilk 2 elemaný arasýndaki mutlak deðerce farký enYakinFark deðiþkenine atýyoruz.
		int *enYakinDegerDizi = (int*)calloc(2,sizeof(int)); // Bulacaðýmýz en yakýn 2 deðerin tutulacaðý dizi
		for (i=0;i<n-1;i++){    
			for(j=i+1;j<n;j++){
				if(abs(dizi[i] - dizi[j]) <= enYakinFark){   // Dizinin i. ve j(i+1). elemanlarýnýn mutlak deðerce farký enYakinFark'tan küçükse;
					enYakinFark = abs(dizi[i] - dizi[j]);   // enYakinFark deðiþkeni güncellenecek ve yeni indislerdeki sayýlarýn mutlak deðerce farký olacak.
					enYakinDegerDizi[0] = dizi[i];  // Ayrýca, en yakýn iki deðeri enYakinDegerDizi dizimize atýp,
					enYakinDegerDizi[1] = dizi[j];  // dizimizdeki en yakýn iki deðeri güncellemiþ olduk.
				}
			}
		}
		return enYakinDegerDizi;        // Son olarak en yakýn iki noktayý döndürdük.
	}
}

int main(){
	
	int n;     // Dizinin eleman sayýsý
	int i;	   // Diziye eleman atmak için kullanacaðýmýz sayaç deðiþkeni
	printf("Dizinin eleman sayisi giriniz: ");
	scanf("%d",&n);
	int dizi[n];       // Kullanýcýdan n elemanlý bir dizi oluþturmasýný istedik.
	
	for(i=0;i<n;i++){
		printf("\n%d. elemani giriniz : ",i);
		scanf("%d",&dizi[i]);          // Dizimize deðerleri girdik.
	}
	
	printf("\nOlusturdugumuz dizi : ");
	for(i=0;i<n;i++){
		printf("\t%d",dizi[i]);        // Oluþturan dizimizi ekrana yazdýrdýk.
	}
	
	// ve son olarak kullanýcýya en yakýn 2 noktayý belirttik.
	int *enYakinDegerDizi = enYakinDegerlerBul(dizi,n);
	printf("\n\nBirbirine en yakin iki eleman : %d ve %d",enYakinDegerDizi[0],enYakinDegerDizi[1]);   
	
	return 0;
}
