#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> //para el manejo de directorios
#include <openssl/evp.h>
#include <openssl/aes.h>

void encrypt(FILE *ifp, FILE *ofp)
{
    //Get file size
    fseek(ifp, 0L, SEEK_END);
    int fsize = ftell(ifp);
    //set back to normal
    fseek(ifp, 0L, SEEK_SET);

    int outLen1 = 0; int outLen2 = 0;
    unsigned char *indata = malloc(fsize);
    unsigned char *outdata = malloc(fsize*2);
    unsigned char ckey[] =  "thiskeyisverybad";
    unsigned char ivec[] = "dontusethisinput";
    EVP_CIPHER_CTX ctx;

    //lee el archivo
     fread(indata,sizeof(char),fsize, ifp);
     EVP_EncryptInit(&ctx,EVP_aes_128_cbc(),ckey,ivec);
     EVP_EncryptUpdate(&ctx,outdata,&outLen1,indata,fsize);	
    //Set up encryption
    EVP_EncryptFinal(&ctx,outdata + outLen1,&outLen2);
    fwrite(outdata,sizeof(char),outLen1 + outLen2,ofp);
}

void decrypt(FILE *ifp, FILE *ofp)
{
    //Get file size
    fseek(ifp, 0L, SEEK_END);
    int fsize = ftell(ifp);
    //set back to normal
    fseek(ifp, 0L, SEEK_SET);

    int outLen1 = 0; int outLen2 = 0;
    unsigned char *indata = malloc(fsize);
    unsigned char *outdata = malloc(fsize);
    unsigned char ckey[] =  "thiskeyisverybad";
    unsigned char ivec[] = "dontusethisinput";

    //Read File
    fread(indata,sizeof(char),fsize, ifp);//Read Entire File

    //setup decryption
    EVP_CIPHER_CTX ctx;
    EVP_DecryptInit(&ctx,EVP_aes_128_cbc(),ckey,ivec);
    EVP_DecryptUpdate(&ctx,outdata,&outLen1,indata,fsize);
    EVP_DecryptFinal(&ctx,outdata + outLen1,&outLen2);
    fwrite(outdata,sizeof(char),outLen1 + outLen2,ofp);
}

void dir(char* start_path)
{
	DIR *dir;
	
}

int main(int argc, char *argv[])
{        
    if(argc!=2){
        printf("se necesita un argumento /path/to/file/exe");
        return -1;
    }
    FILE *fIN, *fOUT;
    fIN = fopen(argv[1], "rb");//File to be encrypted; plain text
    fOUT = fopen("cyphertext.txt", "wb");//File to be written; cipher text

    encrypt(fIN, fOUT);
    fclose(fIN);
    fclose(fOUT);
    //Decrypt file now
    fIN = fopen("cyphertext.txt", "rb");//File to be written; cipher text
    fOUT = fopen("decrypted.txt", "wb");//File to be written; cipher text
    decrypt(fIN,fOUT);
    fclose(fIN);
    fclose(fOUT);

    return 0;
}
