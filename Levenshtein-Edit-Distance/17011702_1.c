#include <stdio.h>
#include <stdlib.h>


double olasilik(double kazanmaOlasiligi,int yeterliGalibiyet){  // A takýmýnýn tüm seriyi kazanma olasýðýný döndüren fonksiyon
	
	int i,j; // matris satýr ve sütun indislerini dolaþabilmek için kullanacaðýmýz deðiþkenler.
	double matris[yeterliGalibiyet+1][yeterliGalibiyet+1];  // Her iki takýmýn geriye kalan maçlarýný tuttuðumuz için 0..4 arasý maç sayýsý kalabilir
	
	for(i=1;i<=yeterliGalibiyet;i++){
		matris[0][i] = 1;  // A takýmýnýn kazanmasý için kalan maçý 0 olduðunda, B'nin kazanmasý gereken maçlar var ise o zaman tüm seriyi A kazanmýþtýr.
	}
	for(i=0;i<=yeterliGalibiyet;i++){
		matris[i][0] = 0;  	 // B takýmýnýn kazanmasý için kalan maçý 0 olduðunda, A'nýn kazanmasý gereken maçlar var ise o zaman tüm seriyi B kazanmýþtýr.
	}
	
	for(i=1;i<=yeterliGalibiyet;i++){
		for(j=1;j<=yeterliGalibiyet;j++){
			// Mevcut indisimizin hemen üstündeki indisi kazanma olasýlýðý ile çarpýp ve mevcut indisimizin hemen solundaki indis ile B takýmýnýn kazanma olasýðýný çarpýp bu iki deðeri topluyoruz.
			matris[i][j] = kazanmaOlasiligi * matris[i-1][j] + matris[i][j-1] * (1-kazanmaOlasiligi);  
		}
	}
	
	return matris[yeterliGalibiyet][yeterliGalibiyet];  // Matrisin en alt ve en sað köþesindeki deðer olan A'nýn seriyi kazanma olasýlýðý döner.
	
}


int main(){
	int macSayisi;  // Oynanacak maç sayýsý
	double kazanmaOlasiligi;   // A takýmýnýn bir maçý kazanma olasýlýðý
	int yeterliGalibiyet; 	// Bir takýmýn tüm seriyi kazanmasý için gereken galibiyet sayýsý
	printf("Oynanacak mac sayisini giriniz : ");
	scanf("%d",&macSayisi);
	printf("\nA takiminin bir maci kazanma olasiligini giriniz : ");
	scanf("%lf",&kazanmaOlasiligi);
	yeterliGalibiyet = macSayisi / 2 + 1;  // Örneðin 7 maç girilince 7/2 = 3,5 olacak fakat int olduðu için 3 yazýlacak o yüzden 1 arttýrýyoruz.
	double sonuc = olasilik(kazanmaOlasiligi,yeterliGalibiyet);
	printf("\n7 maclik seride A'nin kazanma olasiligi : %.2lf \n",sonuc);
	return 0;
}
