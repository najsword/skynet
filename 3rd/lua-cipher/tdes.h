#ifndef _T_DES_H
#define _T_DES_H

#define MAX_CI_LEN 1024
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */
    int cipher2(const unsigned char* key,unsigned char* plain_text, unsigned char* crypted_text,int length);
    int decipher2(const unsigned char* key,unsigned char* plain_text,unsigned char* crypted_text,int length);
    int cipher3(const unsigned char* key,unsigned char* plain_text,unsigned char* crypted_text,int length);
    int decipher3(const unsigned char* key,unsigned char* plain_text,unsigned char* crypted_text,int length);
    void TDes(unsigned char orientation,unsigned char *PlainText,unsigned char *key, unsigned char *ucEncipher);
	char asc_bcd(unsigned char *what);
    void tohex(const unsigned char* ins, unsigned char* outs, int len);
#ifdef __cplusplus
}
#endif /* __cplusplus  */
#endif /* _T_DES_H */
