#include <stdio.h>
#include <stdlib.h>


void birlestir(int dizi[],int firstIndex,int middleIndex,int lastIndex){  // mergeSort fonksiyonundan gelen dizi,firstIndex,middleIndex ve lastIndex parametreleri
    
	int firstSubArrayCount;  // Ýlk alt dizinin eleman sayýsý
	firstSubArrayCount = middleIndex - firstIndex + 1;
    int secondSubArrayCount;	 // Ýkinci alt dizinin eleman sayýsý
	secondSubArrayCount = lastIndex - middleIndex; 
    int i;  // Döngü için sayaç deðiþkeni
	int firstSubArray[firstSubArrayCount];   // Ýlk alt dizimizi oluþturduk. --> firstSubArray [firstIndex...middleIndex]
	int secondSubArray[secondSubArrayCount]; // Ýkinci alt dizimizi oluþturduk. --> secondSubArray [firstIndex...middleIndex]
    
	for(i=0;i<firstSubArrayCount;i++){
		firstSubArray[i] = dizi[firstIndex+i];	 // Orta noktaya kadar olan deðerleri firstSubArray dizisine aktarýyoruz.
	}
        
    for(i=0;i<secondSubArrayCount;i++){
    	secondSubArray[i] = dizi[middleIndex+i+1];   // Orta nokta + 1 'den son elemana kadar olan deðerleri ise secondSubArray dizisine aktarýyoruz.
	}
    
	    
    int first;  // Birleþtirilmiþ alt dizinin baþlangýç indeksi.
	first = firstIndex;  
	int a = 0;  // Ýlk alt dizinin baþlangýç indeksi.
	int b = 0;  // Ýkinci alt dizinin baþlangýç indeksi.
       
    while(a<firstSubArrayCount && b<secondSubArrayCount){   // Her iki alt dizinin baþlangýç index'inin kendi eleman sayýlarýndan küçük olduðu sorguyu yaparýz.
        if(firstSubArray[a]<=secondSubArray[b]){   // Eðer ilk alt dizinin baþlangýç deðeri, ikinci alt dizinin baþlangýç deðerinden küçük ise dizinin ilk elemaný olur.
            dizi[first]=firstSubArray[a];
            a++;
        }
        else{
            dizi[first]=secondSubArray[b];   // Aksi halde ise ikinci alt dizinin ilk elemaný, büyük dizinin ilk elemaný olur.
            b++;
        }
        first++;
    }
    
    while (a < firstSubArrayCount)   // Eðer varsa kalan firstSubArray elemanlarýný aktarýrýz.
    {
        dizi[first] = firstSubArray[a];
        a++;
        first++;
    }
 
    while (b < secondSubArrayCount)  // Eðer varsa kalan secondSubArray elemanlarýný aktarýrýz.
    {
        dizi[first] = secondSubArray[b];
        b++;
        first++;
    }
        
}

void mergeSort(int dizi[],int firstIndex,int lastIndex){ // Oluþturduðumuz diziyi, o dizinin ilk ve son elemanlarýný parametre olarak aldýk.
    if (lastIndex > firstIndex) 
    {
    	int middle; // Dizi için orta noktayý belirlediðimiz deðiþken
        middle = (firstIndex+lastIndex)/2; //  Merge Sort'ta diziyi sürekli ortadan ikiye ayýrdýðýmýz için her seferinde dizinin orta noktasýný alýyoruz.
        mergeSort(dizi,firstIndex,middle); // Ýlk eleman ile orta noktaya kadar olan bölümü rekursif olarak kendi içerisinde her defasýnda ortadan ikiye ayýrýp tek parça haline getiriyoruz.
        mergeSort(dizi,middle+1,lastIndex); // Orta nokta + 1 ' den baþlayarak son elemana kadar olan bölümü rekursif olarak kendi içerisinde her defasýnda ortadan ikiye ayýrýp tek parça haline getiriyoruz.
        birlestir(dizi,firstIndex,middle,lastIndex);  // Tek parça halinde kalanlarý birleþtir fonksiyonu ile yani merge mantýðýyla birleþiririz.
    }
    
}


int *enYakinDegerlerBul(int dizi[],int n){   // parametre olarak Merge Sort ile sýraladýðýmýz diziyi ve dizinin elemaný sayýsýný yolladýk.
	int i;  // Döngü sayaç deðiþkeni
	int enYakinFark;  // En yakýn iki deðer arasýndaki mutlak deðerce farký tutacaðýmýz deðiþken
	int *enYakinDegerDizi = (int*)calloc(2,sizeof(int));  // En yakýn iki deðeri tuttuðumuz dizi 
	enYakinFark = abs(dizi[0]-dizi[1]);  // Baþlangýç olarak dizinin ilk ve ikinci indislerinin mutlak deðerce farkýný aldýk. Bunu döngüde kullanacaðýz.
	for(i=0;i<n;i++){
		if(enYakinFark >= (abs(dizi[i]-dizi[i+1]))){    // Eðer bir önceki en yakýn 2 deðerin mutlak deðerce farký yeni sorguladýðýmýz sýralý iki deðerin mutlak deðerce farkýndan büyük ise;
			enYakinFark = abs(dizi[i]-dizi[i+1]);   // enYakinFark deðiþkenimizi yeni gelecek iki deðerin mutlak deðerce farkýyla güncelleþtiriyoruz.
			enYakinDegerDizi[0] = dizi[i];  
			enYakinDegerDizi[1] = dizi[i+1];  // Ayrýca, En yakýn iki noktayýda enYakinDegerDizi dizisine aktarýyoruz böylelikle enYakinDegerDizi dizisinide güncellemiþ oluyoruz. 
		}
	}
	return enYakinDegerDizi;   // Tüm iþlemler bittikten sonra en yakýn iki deðerin olduðu enYakinDegerDizi dizisini döndürüyoruz.
}


int main(){
	
	int n;  // Dizinin eleman sayýsý
	int i;  // Döngü için sayaç deðiþkeni
	printf("Dizinin eleman sayisi giriniz: ");
	scanf("%d",&n);
	int dizi[n];  // n elemanlý bir dizi oluþturduk.
	
	for(i=0;i<n;i++){
		printf("\n%d. elemani giriniz : ",i);
		scanf("%d",&dizi[i]);    // Kullanýcý diziye elemanlarý ekledi.
	}
	
	printf("\nOlusturdugumuz dizi : ");
	for(i=0;i<n;i++){
		printf("\t%d",dizi[i]);    // Oluþturduðumuz diziyi ekrana yazdýrdýk.
	}
	
	mergeSort(dizi,0,n-1);   // Merge Sort ile diziyi sýraladýk.
	
	printf("\n\nMerge Sort ile Siralanmis dizi : ");
	for(i=0;i<n;i++){
		printf("\t%d",dizi[i]);   // Sýralanmýþ diziyi ekrana yazdýrdýk.
	}
	
	
	int *yeniDizi = enYakinDegerlerBul(dizi,n);
	printf("\n\nEn yakin iki nokta : %d ve %d",yeniDizi[0],yeniDizi[1]);   // En yakýn iki noktayý kullanýcýya belirttik.
	
	
	return 0;
}
