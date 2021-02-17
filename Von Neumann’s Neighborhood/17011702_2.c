#include <stdio.h>
#include <stdlib.h>


int *VonNeumann(int **matris, int matrisBoyutu,int n){   // main'den gelen matris,matris boyutu ve kullanýcýdan alýnan n deðeri parametre olarak gelmiþtir.
	int *sayici; // Her satýrda siyah renk olan karelerin sayýsýný dizimizin indislerine atabilmek için "sayici" isimli bir dizi oluþturuyoruz.
	int i; // Döngü için sayac degiskeni
	int j; // Döngü için sayaç deðiþkeni
	sayici = (int *)malloc(sizeof(int)*matrisBoyutu);   // her satýrýn siyah kare sayýsýný atabilmek için bellekte yer açýyoruz.
	for (i=0;i<matrisBoyutu;i++){    
			sayici[i] = 0;		// Matris boyutu kadar içerisini 0 deðeriyle dolduruyoruz bunun nedeni tuhaf deðerler gelmesini önlemek.
	}
	for (i=0;i<matrisBoyutu;i++){	// {(x,y) : |x-x0| + |y-y0| <= n} koþulunu gerçekledik.
		for (j=0;j<matrisBoyutu;j++){ // Bu kural doðrultusunda; matristeki siyah karelerin satýr numarasý ile matrisin ortadaki elemanýn satýr numarasý,
			if (abs(i-n-1)+abs(j-n-1)<=n){  // arasýndaki farkýn mutlak deðeri ile sütun numaralarý arasýndaki farkýn mutlak deðerinin toplamý N deðerinden küçük veya eþit olmalý. 
				matris[i][j] = 1;   // Matrisin i. satýrýný ve j.sütununu 1 olarak iþaretledik.
				sayici[i]++;   // ve ilgili satýrda kaç tane 1(siyah kare) varsa bunlarýn toplamýný aldýk.
			}
		}			
	}
	return sayici;   // Her satýrdaki siyah kare sayýsýnýn yer aldýðý diziyi döndürüyoruz.
}

int main() {

	int n;  // Kullanýcýdan alýnacak deðer 
	printf("N degerini giriniz : ");
	scanf("%d", &n);  // Kullanýcýdan alýnan deðer n deðiþkenine atanýyor.
	int matrisBoyutu;  // AxA ' lýk bir matris için n deðeri ile þekildeki örüntü arasýnda bað kuruyoruz.
	matrisBoyutu = 2*n+3;  // n = 0 için; 3x3 matris , n = 1 için; 5x5 matris , n = 2 için; 7x7 matris ...
	int i;  // döngü için kullanýlacak sayaç deðiþkeni
	int j;  // döngü için kullanýlacak sayaç deðiþkeni
	int **matris;  // çok boyutlu matrisimizi oluþturuyoruz.
    int *sayici; //Her satýrda siyah renk olan karelerin sayýsýný dizimizin indislerine atabilmek için "sayici" isimli bir dizi oluþturuyoruz.
	
	matris = (int **) malloc(sizeof(int*)*matrisBoyutu); // Her satýr bellekte yer açýyoruz.
	if(matris == NULL){
		printf("Yetersiz bellek !");
	}
	
	// Her satýr için gerekli olan sütun sayýsý için bellekte yer açýyoruz.
	for(i=0;i<matrisBoyutu;i++){
		matris[i] = (int *)malloc(sizeof(int)*matrisBoyutu );
		if(matris[i] == NULL){
			printf("Yetersiz bellek !");
		}
	}
	
	// Matris içindeki tüm deðerleri 0 yapýyoruz. Çünkü biz matrisde gerekli alanlarý 1 yapacaðýz ve kalanlar 0 olacak.	
	for (i=0;i<matrisBoyutu;i++){
		for (j=0;j<matrisBoyutu;j++){
			matris[i][j] = 0;
		}
	}	
	
	printf("\n");
	
	sayici = VonNeumann(matris,matrisBoyutu,n);   // Her satýrdaki siyah kare sayýsýnýn yer aldýðý diziyi döndürdük ve sayac dizimize attýk.
		
	for (i=0;i<matrisBoyutu;i++){
		for (j=0;j<matrisBoyutu;j++){
			printf("%d ",matris[i][j]);   // AxA ' lýk matrisimizde 1(siyah kare) olan kýsýmlarý göstermek için matrisi ekran yazdýrdýk.
		}
		printf("\n");
	}
	
	printf("\n");
	
	for (i=0;i<matrisBoyutu;i++){   // Her satýrda kaç tane 1(siyah kare) varsa bunlarýn sayýsý ekrana yazdýrýyoruz.
		printf("%d.Satir siyah kare sayisi : %d\n",i,sayici[i]);
	}
	
	// Ve matrisdeki toplam siyah karesini ekrana yazdýrýyoruz.
	printf("\nToplam siyah kare sayisi : %d",(2*n*(n+1))+1);
	
	// Burada matrisi hafýzadan silmemiz gerektiði için satýrlarý boþaltýyoruz. 
	for(i=0;i<matrisBoyutu;i++) {
		free(matris[i]);
	}
	
	// Satýrlarý boþalan matrisi boþ olarak ifade ediyoruz.
	free(matris);
	
	return 0; 	
}

