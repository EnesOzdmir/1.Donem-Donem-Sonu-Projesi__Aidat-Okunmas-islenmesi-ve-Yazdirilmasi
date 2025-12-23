// Include'lar
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Constlar

//Adresler
const char AIDAT_ADRESI[] = "C:\\Users\\Cihan\\OneDrive\\Desktop\\aidat.txt";
const char TOPLAM_ADRESI[] = "C:\\Users\\Cihan\\OneDrive\\Desktop\\toplam.txt";

const char ISLEM_SAYISI = 15; //Hepsi Farklıysa Önlem Olması için doğru yazın. Proje 15 sınırı olduğundan çok önem verilmemiştir

// Her bir daire sakini için isim, soyisim ve aidat bilgisini tutan yapı
struct daire {
    char ad[128];
    char soyad[128];
    float aidat;
};

//Fonksiyonlar
/*
 * Ekle Fonksiyonu:
 * ekle fonksiyonu temel olarak eğer bir kullanıcı eklenmemişse onu ekler,
 * kullanıcı varsa aidat parasını transfer eder.
 *
 * Kesme Fonksiyonu:
 * kesme fonksiyonu temel olarak verilen String veriyi düzenler ve
 * onu 'daire' tipindeki veri kümesi olarak döndürür.
 *
 * Yazdir Fonksiyonu:
 * yazdir fonksiyonu temel olarak 'daire' tipindeki diziyi düzenler ve
 * 'TOPLAM_ADRESI' diye tanımlanan yere çıktısını yazar.
 */

//Fonksiyonları Çağır
void ekle(struct daire* kayitlar, int* kullanici_sayisi);
void kesme(struct daire *isleme, char veri[128]);
void yazdir(struct daire bilgiler[], int kullanici_sayisi);

//Ana Fonksiyon
int main(void) {
    struct daire kayitlar[ISLEM_SAYISI];  // Proje sınırı olan 15 kayıtlık veri kümesi oluşturulur
    int kullanici_sayisi = 0; //Daire sayısını 0'dan başlat

    //Ekle fonksiyonunu çağır
    ekle(kayitlar, &kullanici_sayisi);
    
    //Yıllık aidatları toplam.txt dosyasına yazdır
    yazdir(kayitlar, kullanici_sayisi);

    return 0;
}

//Ekle Fonksiyonu
void ekle(struct daire* kayitlar,int* kullanici_sayisi) {
    char satir_verisi[128]; //Tüm satırları okusun diye konuldu
    struct daire gecici_kayit; //Veri structin kopyasini olustur

    //Dosyayı kontrol et
    FILE *dosya = fopen(AIDAT_ADRESI, "r");

    //Önlem olsun diye konuldu Bknz: Bellek rastgele tam sayı atama
    for (int i = 0; i < ISLEM_SAYISI; i++) {
        kayitlar[i].ad[0] = '\0';
        kayitlar[i].soyad[0] = '\0';
        kayitlar[i].aidat = 0.0;
    }

    // fgets fonksiyonu satır satır çektiğinden kaynaklı while döngüsünde kontrollü döngü yapılmıştır
    while (fgets(satir_verisi, sizeof(satir_verisi), dosya) != NULL) {
        kesme(&gecici_kayit, satir_verisi);

        // Proje ödevi dahilinde 15 istek sınırlandırılması için yapılmıştır.
        for (int i = 0; i < ISLEM_SAYISI+1; i++) {
            // Yine proje ödevi yüzünden max 15 kişi olabilceğinden Index 15(Kişi sayisi - 1) olamacağından 16 da otomatik kişi eklemesi için yapılmıştır.
            if (i == ISLEM_SAYISI) {
                kayitlar[(*kullanici_sayisi)] = gecici_kayit;
                *kullanici_sayisi += 1;
            }
            // Uygun(aynı) hesap var aidatı toplama işlemi gerçekleştirilir.
            else if (strcmp(kayitlar[i].ad, gecici_kayit.ad) == 0 &&strcmp(kayitlar[i].soyad, gecici_kayit.soyad) == 0)            {
                kayitlar[i].aidat += gecici_kayit.aidat;
                break;
            }
        }
    }

    //Dosyayı Kapat
    fclose(dosya);
}

void kesme(struct daire *isleme, char veri[128]) {

    // struchIndex: en son kaçıncı tokeni aldığımızı anlamamızı sağlar
    int struchIndex = 0;
    // sinir: en son ki karakterin kaçıncı Indexi tutar
    int sinir       = 0;


    /*
     * Gelen satırdaki(veri) onun 'i'ninci karekteri 'null' veya 'enter' olana kadar 'i'ninci karekteri artırıp devam etmesini sağlar
     */
    for (int i = 0; veri[i] != '\0' && veri[i] != '\n'; i++) {
        if (veri[i] == ' ' && struchIndex == 0) {
            strncpy(isleme->ad, veri, i);
            isleme->ad[i-sinir] = '\0';
            sinir = i;
            struchIndex = 1;
        }

        else if (veri[i] == ' ' && struchIndex == 1) {
            strncpy(isleme->soyad, veri + sinir +1, i-sinir-1);
            isleme->soyad[i-sinir-1] = '\0';


            sinir = i;
            struchIndex = 2;
        }
        else if (struchIndex == 2 && veri[i+1] == '\n') {
            char geciciDeger[32];
            strncpy(geciciDeger, veri + sinir + 1 , i-sinir);
            isleme->aidat = strtof(geciciDeger, NULL);
            sinir = i;

            struchIndex = 3;
        }
        else if (struchIndex == 3) {
            break;
        }
    }

}

//Yazdır Fonksiyonu
void yazdir(struct daire bilgiler[], int kullanici_sayisi) {
    FILE *dosya = fopen(TOPLAM_ADRESI, "w");

    if (dosya == NULL) {
        printf("Dosya açılamadı!\n");
        return;
    }

    /*
     * Her birini tek tek yazma işlemi.
     */
    for (int i = 0; i < kullanici_sayisi; i++) {
        fprintf(dosya, "%s %s %.2f\n",
                bilgiler[i].ad,
                bilgiler[i].soyad,
                bilgiler[i].aidat);
    }

    //Dosyayı Kapat
    fclose(dosya);
}
