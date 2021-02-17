#include <stdio.h>
#include <stdlib.h>

int kontrol(int satir,int sutun,int matrisBoyut,int **matris){   // Sütunlarda ayný renk varmý bunu kontrol ettiðimiz fonksiyon
	int i;
	for(i=0;i<matrisBoyut;i++){
		if((matris[i][sutun] == matris[satir][sutun]) && (i != satir) && i<satir){
			// Gelen satýr ve sütun deðerine göre matrisin o indisindeki deðeri,
			// ayný sütunda ve üstünde yer alan diðer renkler ile karþýlaþtýrýlýr.
			// þartlar saðlanýrsa ayný renk vardýr ve 1 deðeri döndürürüz.  
			return 1;
		}
	}
	return 0; // Aksi halde 0 döner.
}

void matrisYazdir(int **matris,int matrisBoyut){   // Matris Yazdirma Fonksiyonu
	int i,j;  // Matriste gezinmek için deðiþkenler
	for(i=0;i<matrisBoyut;i++){
		for(j=0;j<matrisBoyut;j++){
			if(matris[i][j] == 1){
				printf("Kirmizi (%d) \t",matris[i][j]);    // 1 - Kirmizi
			}
			else if(matris[i][j] == 2){
				printf("Sari    (%d) \t",matris[i][j]);    //  2 - Sari
			}
			else if(matris[i][j] == 3){
				printf("Beyaz   (%d) \t",matris[i][j]);	   //  3 - Beyaz
			}
			else if(matris[i][j] == 4){
				printf("Mavi    (%d) \t",matris[i][j]);    //  4 - Mavi
			}
			else if(matris[i][j] == 5){
				printf("Mor     (%d) \t",matris[i][j]);    //  5 - Mor
			}
			else if(matris[i][j] == 6){
				printf("Turuncu (%d) \t",matris[i][j]);    //  6 - Turuncu
			}
			else if(matris[i][j] == 7){
				printf("Yesil   (%d) \t",matris[i][j]);    //  7 - Yesil
			}
			else if(matris[i][j] == 8){
				printf("Siyah   (%d) \t",matris[i][j]);    //  8 - Siyah
			}	
		}
		printf("\n");
	}
	printf("\n\n");
}

void sagaKaydir(int **matris,int matrisBoyut,int satir){  // Saða kaydýrma iþleminin yapýldýðý fonksiyon
	int i, temp;
	temp = matris[satir][matrisBoyut-1]; // Saða kaydýralacak olan satýrda son sütun deðerini temp deðiþkeninde geçici olarak tutuyoruz.
	i = matrisBoyut-1;  // Son indis hariç diðerlerini saða kaydýrabilmek için döngü sayýmýz.
	while (i > 0){
		matris[satir][i] = matris[satir][i-1]; // Son elemaný direk ilk sütuna koyacaðýz bu yüzden onu geçici deðiþkende tutup diðerlerini bir indis saða kaydýrdýk.
		i--;
	}
	matris[satir][0] = temp;  // temp deðiþkeninde tuttuðumuz son indiside ilk sütuna koyduk.
}

int backtracking(int matrisBoyut,int **matris,int sabitMatrisBoyut){   // Backtracking Fonksiyonu
	int i,j;
	int sayac=0;
	
	for(i=0;i<matrisBoyut;i++){
		for(j=0;j<matrisBoyut;j++){   // Matrisi geziyoruz.
			if(kontrol(i,j,matrisBoyut,matris) == 1){   // Eðer kontrol == 1 ise o zaman sütunlara ayný deðer yerleþmiþtir.
				sagaKaydir(matris,sabitMatrisBoyut,i);  // Saða kaydýrma iþlemi yapýyoruz.
				if(kontrol(i,j,matrisBoyut,matris) != 1){  // Saða kaydýrma iþlemi bittiðinde adým adým yazdýrýyoruz.
					printf("*********** SAGA KAYDIRILDI ***********\n",i);
					matrisYazdir(matris,sabitMatrisBoyut);
				}
			}
			else {
				backtracking(matrisBoyut-1,matris,sabitMatrisBoyut);   // Rekursif olarak geri izlemeye devam edilir.
			}
		}
	}
}


int main(){
	int i,j;  // Matriste gezinmek için deðiþkenler
	int **matris;  // Renk Matrisi
	int n;  // Matris için boyut
	printf("N x N lik bir matris icin n giriniz : ");
	scanf("%d",&n);
	if(n<3 || n>8){    // 3 ile 8 arasýnda matris boyut kontrolü
		printf("Lutfen 3 ile 8 arasinda bir matris boyutu giriniz !");
		return 0;
	}	
	
	matris = (int**)calloc(sizeof(int*),n);  // Matris Allocate Satýr ve Sütunlar için
	for(i=0;i<n;i++){
		matris[i] = (int*)calloc(sizeof(int),n); 
	}
	printf("\n1-Kirmizi\n2-Sari\n3-Beyaz\n4-Mavi\n5-Mor\n6-Turuncu\n7-Yesil\n8-Siyah\n");
	printf("\nRenk matrisini yukarida verilen her renk icin sayi karsiliklarini giriniz : \n\n");
	for(i=0;i<n;i++){
		printf("%d. satirin renklerini giriniz\n",i+1);
		for(j=0;j<n;j++){
			scanf("%d",&matris[i][j]);    // Matrisin indislerine renkleri sayýsal olarak yerleþtiriyoruz.
		}
		printf("\n");
	}
	
	printf("***** Matrisin ilk hali *******\n");
	matrisYazdir(matris,n);
	backtracking(n,matris,n);   // Backtracking iþlemleri için fonksiyonu çaðýrýyoruz.
	
	
	
	int test = 0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){    
			if(kontrol(i,j,n,matris) == 1){   // Matrisde ayný sütunlarda ayný renkler yerleþmiþ mi son olarak kontrol ediyoruz.
				test = 1;
			}
		}
	}
	
	if(test == 1){
		printf("\n\nBU MATRIS ICIN SONUC URETILEMEDI !!!!!!!!!!\n\n");
	}
	
	printf("***** Matrisin son hali *******\n");
	matrisYazdir(matris,n);
	
	
	
	
	

	for( i = 0; i < n; i++ ) {   // Matris ile iþimiz bitince satýrlarýn içini boþaltýyoruz.
		free( matris[i] );
	}
	free( matris );    // Satýrlar silindikten sonra en son matrisi boþ olarak iþaretliyoruz.

	
	return 0;
}
