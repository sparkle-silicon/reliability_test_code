/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-12-24 15:18:25
 * @Description:
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 *
 * Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2025龙晶石半导体科技（苏州）有限公司
 */

/*是长度为65字节的OCTET STRING，其中第一个字节代表ECPoint是否经过压缩，
如果为0x04，代表没有压缩。剩下的64个字节，
前32个字节，表示ECPoint的X坐标，后32个字节表示ECPoint的Y坐标。
OCTET STRING类型的ECPoint在转换为BIT STRING类型的subjectPublicKey时，按照大端字节序转换*/
//gcc ./sign.c ./sign.h -I/usr/include/openssl/ -L /usr/lib/x86_64-linux-gnu/ -lcrypto -lssl -D_DEBUG -o sign

/*
* echo "-----BEGIN PRIVATE KEY-----" >private_key.pem
echo "MIIBUwIBADANBgkqhkiG9w0BAQEFAASCAT0wggE5AgEAAkEAujOsIxHyO22MvDxA" >>private_key.pem
echo "uxCpReM2jDFgXDJd7yMTo+M6UvGHtgU4h5XtUn0D/VqMDa8Uf5dyJUki5E4fZUht" >>private_key.pem
echo "xCcL5wIDAQABAkAf8jWaXr3cjFMVCJFiht1D7FOzgdUyfEG5gZ8hAx9Ck3r2TfdS" >>private_key.pem
echo "qRyxHrSAEJkrg163GhcMhW/F/5be+bhS4cjhAiEA2r92ckjCyOiF17gwOvd+cHYc" >>private_key.pem
echo "GRgmaRL9nDiaui9EgWUCIQDZ6VTQ+j0Sg2NUAUrkWHos3DOI/HVpUuw5QfnC9XaJ" >>private_key.pem
echo "WwIgSaviAGtw7qkhKp3l4HYqJ0YgAHOZvBmhQFPwCHSS5eECIAK5IsE1z1uS3sPm" >>private_key.pem
echo "/DucS3KSEebNWN5HFgKlK2zfSmrvAiAfj0q84UHF+tgP8wDTdCLfa+VBr7pKRfJ0" >>private_key.pem
echo "Fl92RzRrQQ==" >>private_key.pem
echo "-----END PRIVATE KEY-----" >>private_key.pem
* */
/*
*  <SEQUENCE>
  <INTEGER/>
  <SEQUENCE>
   <OBJECT_IDENTIFIER Comment="PKCS #1" Description="rsaEncryption">1.2.840.113549.1.1.1</OBJECT_IDENTIFIER>
   <NULL/>
  </SEQUENCE>
  <OCTET_STRING>
   <SEQUENCE>
	<INTEGER/>
	<INTEGER>0x00BA33AC2311F23B6D8CBC3C40BB10A945E3368C31605C325DEF2313A3E33A52F187B605388795ED527D03FD5A8C0DAF147F9772254922E44E1F65486DC4270BE7</INTEGER>
	<INTEGER>65537</INTEGER>
	<INTEGER>0x1FF2359A5EBDDC8C531508916286DD43EC53B381D5327C41B9819F21031F42937AF64DF752A91CB11EB48010992B835EB71A170C856FC5FF96DEF9B852E1C8E1</INTEGER>
	<INTEGER>0x00DABF767248C2C8E885D7B8303AF77E70761C1918266912FD9C389ABA2F448165</INTEGER>
	<INTEGER>0x00D9E954D0FA3D12836354014AE4587A2CDC3388FC756952EC3941F9C2F576895B</INTEGER>
	<INTEGER>0x49ABE2006B70EEA9212A9DE5E0762A274620007399BC19A14053F0087492E5E1</INTEGER>
	<INTEGER>0x02B922C135CF5B92DEC3E6FC3B9C4B729211E6CD58DE471602A52B6CDF4A6AEF</INTEGER>
	<INTEGER>0x1F8F4ABCE141C5FAD80FF300D37422DF6BE541AFBA4A45F274165F7647346B41</INTEGER>
   </SEQUENCE>
  </OCTET_STRING>
 </SEQUENCE>
 * */
#include "sign.h"
#define HASH_SIZE 64 // 最大支持 SHA-512
#define AES_BLOCK_SIZE 16
// sFixedFlashInfo Fix_flash_info;
const  char *file_name = "./ec_main.bin";
const char *hash_alg = "SHA256";
const char *sign_algo = "RSA"; // 或 "ECDSA"

const char *aes_algo = NULL; // 或 "AES"
const char *private_key_file = "private_key.pem";
const  char *aes_key = "this_is_a_256_bit_key!";
const  char *aes_iv = "this_is_a_iv_123";

#if !GLE01
const char FIX_FIRMWARE_ID[] = "SPK32AE103\0";
const char DYN_FIRMWARE_ID[] = "SPK32 AE103\036EC\003\0";
#else
const char FIX_FIRMWARE_ID[] = "SPK32GLE01\0";
const char DYN_FIRMWARE_ID[] = "SPK32 GLE01\036EC\003\0";
#endif
sFixedFlashInfo Fix_flash_info = {
// #if GLE01
// 	.Firmware_ID = "SPK32GLE01\0",
// #else
// 	.Firmware_ID = "SPK32AE103\0", // 特殊字符
// #endif
};
sDynamicFlashInfo Dy_flash_info = {
#if GLE01
	.Firmware_ID = "SPK32 GLE01\036EC\003\0",
#else
	.Firmware_ID = "SPK32 AE103\036EC\003\0", // 特殊字符
#endif
	.Info.FixedFlashInfo_Addr = UINT32_MAX,
	.Info.EXTFlash_ID = UINT32_MAX,
};
uint32_t Fix_flash_info_addr = 0x100;
uint32_t Dy_flash_info_addr = 0;
uint32_t sector_4ksize = 0;//默认4k
long bin_size = 0;
uint32_t bin_seek = 4096;//256;//bin一次偏移默认大小，即按该大小填充
uint32_t *padding = NULL;
ssize_t ret = 0;
ssize_t i = 0;
ssize_t efuse_secver = 0;//默认关闭安全
// srom_data flash_data;

//sflash_info flash_info;
SM2_SIGNATURE_STRUCT sm2_sig;
struct stat file_stat;


/*********************************************************/
/*算法	分组长度（bytes）	输出长度（hash value）(bytes）	是否安全
SM3	    64	                32	                            安全
SHA0	64	                20	                            不安全
SHA1	64	                20	                            不安全
SHA-2:
SHA224	64	                28	                            安全
SHA256	64	                32	                            安全
SHA384	128	                48	                            安全
SHA512	128	                64	                            安全
* */

// int SHA0_Init(SHA_CTX *c)
// {
// 	memset(c, 0, sizeof(*c));
// 	c->h0 = 0x67452301;
// 	c->h1 = 0xEFCDAB89;
// 	c->h2 = 0x98BADCFE;
// 	c->h3 = 0x10325476;
// 	c->h4 = 0xC3D2E1F0;
// 	return 1;
// }
// void SHA0_Transform(SHA_CTX *c)
// {
// 	uint32_t A, B, C, D, E, TEMP;
// 	uint32_t W[80];
// 	//将消息块分为16个32位的词
// 	for(int i = 0; i < 16; i++)
// 	{
// 		W[i] = c->data[i];
// 	}
// 	//扩展消息块
// 	for(int i = 16; i < 80; i++)
// 	{
// 		W[i] = (W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1;
// 	}
// 	//初始化哈希值
// 	A = c->h0;
// 	B = c->h1;
// 	C = c->h2;
// 	D = c->h3;
// 	E = c->h4;
// 	//主循环
// 	for(int i = 0; i < 80; i++)
// 	{
// 		uint32_t ft = 0;
// 		uint32_t kt = 0;
// 		if(i < 20) { ft = (B & C) | (~B & D); kt = 0x5A827999; }
// 		else if(i < 40) { ft = B ^ C ^ D; kt = 0x6ED9EBA1; }
// 		else if(i < 60) { ft = (B & C) | (B & D) | (C & D); kt = 0x8F1BBCDC; }
// 		else if(i < 80) { ft = B ^ C ^ D; kt = 0xCA62C1D6; }
// 		TEMP = ((A << 5) | (A >> (32 - 5))) + ft + E + W[i] + kt;
// 		E = D;
// 		D = C;
// 		C = ((B << 30) | (B >> (32 - 30)));
// 		B = A;
// 		A = TEMP;
// 	}
// 	//添加到哈希值
// 	c->h0 += A;
// 	c->h1 += B;
// 	c->h2 += C;
// 	c->h3 += D;
// 	c->h4 += E;
// }
// int SHA0_Update(SHA_CTX *c, const unsigned char *data, size_t len)
// {
// 	size_t i;
// 	for(i = 0; i < len; i++)
// 	{
// 		c->data[c->num] = data[i];
// 		c->num++;
// 		if(c->num & 0x3f == 0)
// 		{
// 			SHA0_Transform(c);
// 		}
// 	}
// 	return 1;
// }
// int SHA0_Final(unsigned char *md, SHA_CTX *c)
// {
// 	size_t i;
// 	unsigned char final_block[64];
// 	unsigned char padding[64];
// 	unsigned char *p = final_block;
// 	//填充
// 	for(i = 0; i < c->num; i++)
// 	{
// 		*p++ = c->data[i];
// 	}
// 	while(p < final_block + 64)
// 	{
// 		*p++ = 0x80;
// 	}
// 	*(uint64_t *)(final_block + 56) = ToggleEndian(c->total_len * 8);
// 	SHA0_Transform(c, final_block);
// 	//输出
// 	for(i = 0; i < 5; i++)
// 	{
// 		*(uint32_t *)(md + i * 4) = ToggleEndian(c->h[i]);
// 	}
// 	return 1;
// }
// unsigned char *SHA0(const unsigned char *d, size_t n, unsigned char *md)
// {
// 	SHA_CTX c;
// 	static unsigned char m[SHA_DIGEST_LENGTH];
// 	if(md == NULL) md = m;
// 	SHA0_Init(&c);
// 	SHA0_Update(&c, d, n);
// 	SHA0_Final(md, &c);
// 	return md;
// }
// unsigned char *SM3(const unsigned char *d, size_t n, unsigned char *md)
// {
// 	return NULL;
// }
void handle_openssl_error()
{
	ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
}
void fill_bigint_ecc(ECC_STU_BIGINT32 *bigint, const BIGNUM *bn)
{
	memset(bigint, 0, sizeof(ECC_STU_BIGINT32));
	int num_bytes = BN_num_bytes(bn);
	bigint->uLen = (num_bytes + 3) / 4; // 计算字长度
	BN_bn2bin(bn, (unsigned char *)bigint->auValue); // 转为二进制
		// 将字节序从大端转换为小端
	for (int i = 0; i < (bigint->uLen >> 1); i++)
	{
		bigint->auValue[bigint->uLen - i - 1] ^= bigint->auValue[i];
		bigint->auValue[i] ^= bigint->auValue[bigint->uLen - i - 1];
		bigint->auValue[bigint->uLen - i - 1] ^= bigint->auValue[i];
	}
	if (bigint->uLen & 1)bigint->auValue[bigint->uLen >> 1] = __builtin_bswap32(bigint->auValue[bigint->uLen >> 1]);

}
void fill_bigint_ecc_noswap(ECC_STU_BIGINT32 *bigint, const BIGNUM *bn)
{
	memset(bigint, 0, sizeof(ECC_STU_BIGINT32));
	int num_bytes = BN_num_bytes(bn);
	bigint->uLen = (num_bytes + 3) / 4; // 计算字长度
	BN_bn2bin(bn, (unsigned char *)bigint->auValue); // 转为二进制
		// 将字节序从大端转换为小端
}

void fill_bigint_rsa(uint32_t *dest, const BIGNUM *bn, uint32_t max_len)
{
	memset(dest, 0, max_len * sizeof(uint32_t));
	int num_bytes = BN_num_bytes(bn);
	BN_bn2bin(bn, (unsigned char *)dest); // 转为二进制
	// 将字节序从大端转换为小端
	uint32_t ulen = (num_bytes + 3) / 4; // 计算字长度
	for (int i = 0; i < (ulen >> 1); i++)
	{
		dest[ulen - i - 1] ^= __builtin_bswap32(dest[i]);
		dest[i] ^= __builtin_bswap32(dest[ulen - i - 1]);
		dest[ulen - i - 1] ^= __builtin_bswap32(dest[i]);
	}
	if (ulen & 1)dest[ulen >> 1] = __builtin_bswap32(dest[ulen >> 1]);
}
void fill_bigint_rsa_noswap(uint32_t *dest, const BIGNUM *bn, uint32_t max_len)
{
	memset(dest, 0, max_len * sizeof(uint32_t));
	int num_bytes = BN_num_bytes(bn);
	BN_bn2bin(bn, (unsigned char *)dest); // 转为二进制
}
// 生成 SHA 哈希
int hash_data(const unsigned char *data, size_t data_len, unsigned char *hash, const char *hash_alg)
{
#if 0
	if (strcmp(hash_alg, "SM3") == 0)
	{
		// SM3(data, data_len, hash);
		// return SHA256_DIGEST_LENGTH;
		return hash_data(data, data_len, hash, "SHA256");//使用sha256加密
	}
	else if (strcmp(hash_alg, "SHA0") == 0)
	{
		// SHA0(data, data_len, hash);
		// return 20;
		return hash_data(data, data_len, hash, "SHA1");//使用sha1加密
	}
	else if (strcmp(hash_alg, "SHA1") == 0)
	{
		SHA1(data, data_len, hash);
		return 20;
	}
	else if (strcmp(hash_alg, "SHA224") == 0)
	{
		SHA224(data, data_len, hash);
		return SHA224_DIGEST_LENGTH;
	}
	else if (strcmp(hash_alg, "SHA256") == 0)
	{
		SHA256(data, data_len, hash);
		return SHA256_DIGEST_LENGTH;
	}
	else if (strcmp(hash_alg, "SHA384") == 0)
	{
		SHA384(data, data_len, hash);
		return SHA384_DIGEST_LENGTH;
	}
	else if (strcmp(hash_alg, "SHA512") == 0)
	{
		SHA512(data, data_len, hash);
		return SHA512_DIGEST_LENGTH;
	}
	else
	{
		fprintf(stderr, "Unsupported hash algorithm: %s\n", hash_alg);
		return -1;
	}
#else
	EVP_MD *md = EVP_MD_fetch(NULL, hash_alg, NULL);
	if (!md)
	{
		fprintf(stderr, "Unsupported hash algorithm: %s\n", hash_alg);
		return -1;
	}

	EVP_MD_CTX *ctx = EVP_MD_CTX_new();
	if (!ctx)
	{
		fprintf(stderr, "Failed to create hash context\n");
		EVP_MD_free(md);
		return -1;
	}

	if (EVP_DigestInit_ex(ctx, md, NULL) != 1)
	{
		fprintf(stderr, "Failed to initialize hash\n");
		EVP_MD_CTX_free(ctx);
		EVP_MD_free(md);
		return -1;
	}

	if (EVP_DigestUpdate(ctx, data, data_len) != 1)
	{
		fprintf(stderr, "Failed to update hash\n");
		EVP_MD_CTX_free(ctx);
		EVP_MD_free(md);
		return -1;
	}

	if (EVP_DigestFinal_ex(ctx, hash, NULL) != 1)
	{
		fprintf(stderr, "Failed to finalize hash\n");
		EVP_MD_CTX_free(ctx);
		EVP_MD_free(md);
		return -1;
	}

	EVP_MD_CTX_free(ctx);
	EVP_MD_free(md);
	return EVP_MD_size(md);

#endif
}
/*********************************************************/
// 签名（RSA 或 ECDSA）
int sign_hash(const unsigned char *hash, size_t hash_len, unsigned char **signature, size_t *sig_len, const char *private_key_file, const char *algo)
{
	FILE *key_file = fopen(private_key_file, "r");
	if (!key_file)
	{
		perror("Failed to open private key file");
		return -1;
	}
	uint8_t hash2endian[256] = { 0 };//根据情况可能要使用翻转的结果
	for (size_t i = 0; i < hash_len; i++)
	{
		hash2endian[hash_len - i - 1] = hash[i];
	};
	int result = -1; // 初始化返回值

	if (strcmp(algo, "RSA") == 0)
	{
		RSA *rsa = PEM_read_RSAPrivateKey(key_file, NULL, NULL, NULL);
		fclose(key_file);
		if (!rsa)
		{
			fprintf(stderr, "Failed to read RSA private key: %s\n", ERR_error_string(ERR_get_error(), NULL));
			return -1;
		}

		const BIGNUM *n = RSA_get0_n(rsa);
		const BIGNUM *e = RSA_get0_e(rsa);
		if (!n || !e)
		{
			fprintf(stderr, "Failed to get RSA public key components.\n");
			RSA_free(rsa);
			return -1;
		}

		// 填充结构体（如果需要）
		Dy_flash_info.publickey.rsa.bits = BN_num_bits(n);
		fill_bigint_rsa(Dy_flash_info.publickey.rsa.modulus, n, RSA_MODULUS_LEN_MAX);
		fill_bigint_rsa_noswap(Dy_flash_info.publickey.rsa.exponent, e, RSA_MODULUS_LEN_MAX);

		*sig_len = RSA_size(rsa);
		*signature = malloc(*sig_len);
		if (!*signature)
		{
			perror("Failed to allocate memory for signature");
			RSA_free(rsa);
			return -1;
		}

	#if 1
		BIGNUM *r = BN_new();
		BIGNUM *a = BN_new();
		const BIGNUM *p = RSA_get0_d(rsa);
		if (!r || !a || !p)
		{
			fprintf(stderr, "Failed to allocate memory for RSA signature components.\n");
			BN_free(r);
			BN_free(a);
			RSA_free(rsa);
			return -1;
		}
		BN_CTX *ctx = BN_CTX_new();
		if (!ctx)
		{
			fprintf(stderr, "Failed to allocate memory for BN context.\n");
			BN_free(r);
			BN_free(a);
			RSA_free(rsa);
			return -1;
		}

	#if 1
		BN_bin2bn(hash2endian, hash_len, a);
	#else
		BN_bin2bn(hash, hash_len, a);
	#endif
		BN_mod_exp(r, a, p, n, ctx);
		// BN_mod_exp(r, r, e, n, ctx);
		BN_bn2bin(r, (unsigned char *)*signature);
		BN_free(r);
		BN_free(a);
	#elif 0
	#if 1
		if (RSA_private_encrypt(64, hash2endian, *signature, rsa, RSA_NO_PADDING) == -1)//RSA_NO_PADDING
		{
			fprintf(stderr, "Failed to sign with RSA: %s\n", ERR_error_string(ERR_get_error(), NULL));
			free(*signature);
			RSA_free(rsa);
			return -1;
		}
	#else
		if (RSA_private_encrypt(64, hash, *signature, rsa, RSA_NO_PADDING) == -1)//RSA_NO_PADDING
		{
			fprintf(stderr, "Failed to sign with RSA: %s\n", ERR_error_string(ERR_get_error(), NULL));
			free(*signature);
			RSA_free(rsa);
			return -1;
		}
	#endif
	#else
	#if 1//要用该内容，需要硬件能支持补丁机制，
		if (RSA_sign(NID_sha256, hash2endian, hash_len, *signature, (unsigned int *)sig_len, rsa) != 1)
		{
			fprintf(stderr, "Failed to sign with RSA: %s\n", ERR_error_string(ERR_get_error(), NULL));
			free(*signature);
			RSA_free(rsa);
			return -1;
		}
	#else
		if (RSA_sign(NID_sha256, hash, hash_len, *signature, (unsigned int *)sig_len, rsa) != 1)
		{
			fprintf(stderr, "Failed to sign with RSA: %s\n", ERR_error_string(ERR_get_error(), NULL));
			free(*signature);
			RSA_free(rsa);
			return -1;
		}
	#endif
	#endif
		RSA_free(rsa);
		result = 0;
	}
	// else if(strcmp(algo, "ECDSA") == 0)
	// {
	// 	EC_KEY *ec_key = PEM_read_ECPrivateKey(key_file, NULL, NULL, NULL);
	// 	fclose(key_file);
	// 	if(!ec_key)
	// 	{
	// 		fprintf(stderr, "Failed to read ECDSA private key: %s\n", ERR_error_string(ERR_get_error(), NULL));
	// 		return -1;
	// 	}

	// 	// 获取公钥点
	// 	const EC_POINT *pubkey = EC_KEY_get0_public_key(ec_key);
	// 	if(!pubkey)
	// 	{
	// 		fprintf(stderr, "Failed to get ECC public key.\n");
	// 		EC_KEY_free(ec_key);
	// 		return -1;
	// 	}

	// 	const EC_GROUP *group = EC_KEY_get0_group(ec_key);
	// 	BIGNUM *x = BN_new();
	// 	BIGNUM *y = BN_new();
	// 	if(!EC_POINT_get_affine_coordinates(group, pubkey, x, y, NULL))
	// 	{
	// 		fprintf(stderr, "Failed to get ECC public key coordinates.\n");
	// 		BN_free(x);
	// 		BN_free(y);
	// 		EC_KEY_free(ec_key);
	// 		return -1;
	// 	}

	// 	// 填充结构体（如果需要）
	// 	fill_bigint_ecc(&Dy_flash_info.publickey.ecc.stuQx, x);
	// 	fill_bigint_ecc(&Dy_flash_info.publickey.ecc.stuQy, y);

	// 	// 释放 BN
	// 	BN_free(x);
	// 	BN_free(y);

	// 	*sig_len = ECDSA_size(ec_key);
	// 	*signature = malloc(*sig_len);
	// 	if(!*signature)
	// 	{
	// 		perror("Failed to allocate memory for signature");
	// 		EC_KEY_free(ec_key);
	// 		return -1;
	// 	}

	// 	if(ECDSA_sign(NID_sha256, hash, hash_len, *signature, (unsigned int *)sig_len, ec_key) != 1)
	// 	{
	// 		fprintf(stderr, "Failed to sign with ECDSA: %s\n", ERR_error_string(ERR_get_error(), NULL));
	// 		free(*signature);
	// 		EC_KEY_free(ec_key);
	// 		return -1;
	// 	}

	// 	EC_KEY_free(ec_key);
	// 	result = 0;
	// }
	else
	{
		fprintf(stderr, "Unsupported signing algorithm: %s\n", algo);
		fclose(key_file);
		return -1;
	}

	return result;
}

// AES 加密
int aes_encrypt(const unsigned char *data, size_t data_len, unsigned char **encrypted_data, size_t *encrypted_len, const unsigned char *key, const unsigned char *iv, int key_bits)
{
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	if (!ctx)
	{
		fprintf(stderr, "Failed to create AES context\n");
		return -1;
	}

	const EVP_CIPHER *cipher;
	if (key_bits == 128)
	{
		cipher = EVP_aes_128_ecb();
	}
	else if (key_bits == 192)
	{
		cipher = EVP_aes_192_ecb();
	}
	else if (key_bits == 256)
	{
		cipher = EVP_aes_256_ecb();
	}
	else
	{
		fprintf(stderr, "Unsupported AES key size: %d\n", key_bits);
		EVP_CIPHER_CTX_free(ctx);
		return -1;
	}

	if (EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv) != 1)
	{
		fprintf(stderr, "Failed to initialize AES encryption\n");
		EVP_CIPHER_CTX_free(ctx);
		return -1;
	}

	*encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
	if (!*encrypted_data)
	{
		perror("Failed to allocate memory for encrypted data");
		EVP_CIPHER_CTX_free(ctx);
		return -1;
	}

	int len, total_len = 0;
	if (EVP_EncryptUpdate(ctx, *encrypted_data, &len, data, data_len) != 1)
	{
		fprintf(stderr, "Failed to encrypt data\n");
		free(*encrypted_data);
		EVP_CIPHER_CTX_free(ctx);
		return -1;
	}
	total_len += len;

	if (EVP_EncryptFinal_ex(ctx, *encrypted_data + total_len, &len) != 1)
	{
		fprintf(stderr, "Failed to finalize AES encryption\n");
		free(*encrypted_data);
		EVP_CIPHER_CTX_free(ctx);
		return -1;
	}
	total_len += len;

	*encrypted_len = total_len;
	EVP_CIPHER_CTX_free(ctx);
	return 0;
}

int main(int argc, char **argv)
{

	//system("clear");
	  //从argv[1]开始查找参数
	for (i = 1; i < argc; i++)
	{
		//输入输出的文件名 //file
		if ((strcmp(argv[i], "-f") == 0))
		{
			if (argv[i][2] != '\0')
			{
				if (argv[i][2] == '=')
					file_name = &argv[i][3];
				else
					file_name = &argv[i][2];
			}
			else if (i + 1 < argc)
			{
				i++;
				file_name = argv[i];
			}

		}
		//给出bin的对齐信息，输出后填充按该对齐信息进行放置//seek
		else if ((strcmp(argv[i], "-s") == 0))
		{
			if (argv[i][2] != '\0')
			{
				if (argv[i][2] == '=')
					bin_seek = atoi(&argv[i][3]);
				else
					bin_seek = atoi(&argv[i][2]);
			}
			else if (i + 1 < argc)
			{
				i++;
				bin_seek = atoi(argv[i]);
			}

		}
		else if ((strcmp(argv[i], "-efuse") == 0))
		{
			efuse_secver = 1;
		}
		else if ((strcmp(argv[i], "-no_efuse") == 0))
		{
			efuse_secver = 0;
		}
		//输入hash的计算量，尾部信息要超过该区域 //hash 4k sector
		else if ((strcmp(argv[i], "-h") == 0))
		{
			if (argv[i][2] != '\0')
			{
				if (argv[i][2] == '=')
					sector_4ksize = atoi(&argv[i][3]);
				else
					sector_4ksize = atoi(&argv[i][2]);
			}
			else if (i + 1 < argc)
			{
				i++;
				sector_4ksize = atoi(argv[i]);
			}

		}
		//使用的密钥

		else if ((strcmp(argv[i], "-key") == 0))
		{
			if (argv[i][4] != '\0')
			{
				if (argv[i][4] == '=')
					aes_key = (&argv[i][5]);
				else
					aes_key = (&argv[i][4]);
			}
			else if (i + 1 < argc)
			{
				i++;
				aes_key = (argv[i]);
			}
		}
		else if ((strcmp(argv[i], "-iv") == 0))
		{
			if (argv[i][3] != '\0')
			{
				if (argv[i][3] == '=')
					aes_iv = (&argv[i][4]);
				else
					aes_iv = (&argv[i][3]);
			}
			else if (i + 1 < argc)
			{
				i++;
				aes_iv = (argv[i]);
			}
		}
		else if ((strcmp(argv[i], "-sign_key") == 0))
		{
			if (argv[i][9] != '\0')
			{
				if (argv[i][9] == '=')
					private_key_file = &argv[i][10];
				else
					private_key_file = &argv[i][9];

			}
			else if (i + 1 < argc)
			{
				i++;
				private_key_file = argv[i];
			}
		}
		else if ((strcmp(argv[i], "-addr") == 0))
		{



			if (argv[i][5] != '\0')
			{

				if (argv[i][5] == '=')
					Dy_flash_info.Info.FixedFlashInfo_Addr = strtol(&argv[i][6], NULL, 16);
				else
					Dy_flash_info.Info.FixedFlashInfo_Addr = strtol(&argv[i][5], NULL, 16);

			}
			else if (i + 1 < argc)
			{
				i++;
				Dy_flash_info.Info.FixedFlashInfo_Addr = strtol(argv[i], NULL, 16);
			}
		}
	}
		//	保护机制
	if (bin_seek < 256)bin_seek = 256;//最低需要256字节对齐

		//预处理
	{
		//	打开文件
		int elf_fd = open(file_name, O_RDWR);
		if (elf_fd <= 0) { perror("file open error"); exit(-1); }

		{//进行文件填充对齐，对齐后方可进行加密操作
			off_t elf_seek = lseek(elf_fd, 0, SEEK_END);

			bin_size = (elf_seek > ((sector_4ksize + 1) * 4096)) ? elf_seek : (sector_4ksize + 1) * 4096;//计算需要补充的大小
			bin_size = (bin_size + ((bin_size % bin_seek) ? bin_seek : 0) - (bin_size % bin_seek));//对bin进行对齐处理
			uint32_t temp = bin_size - elf_seek;
			if (temp < 0)//bin所需文件大小小于实际文件大小这判断异常
			{
				printf("file size padding error\n");
				return -1;
			}
			else if (temp > 0)//位数不够的地方进行填充
			{
				padding = malloc(temp);
				memset(padding, 0xff, temp);
				write(elf_fd, padding, temp);
				free(padding);
			}
			Dy_flash_info_addr = bin_size;//对齐以后的结果
		}
		lseek(elf_fd, 0, SEEK_SET);//从头开始算

		{//查找头部文件信息
			// while(Fix_flash_info_addr < 16 * 1024 * 1024)//最大16M的位置
			// {
			lseek(elf_fd, (Fix_flash_info_addr), SEEK_SET);
			int j = read(elf_fd, (void *)&Fix_flash_info, sizeof(sFixedFlashInfo));
			if (j < 0) { perror("Find Fix flash info error"); exit(-1); }
			if (!strcmp(FIX_FIRMWARE_ID, Fix_flash_info.Firmware_ID))
			{
				//填入Dy_flash_info_addr对应Fix_flash_info的偏移值
				Fix_flash_info.DynamicFlashInfo = Dy_flash_info_addr;
				//将新结果写入对应位置
				lseek(elf_fd, (Fix_flash_info_addr), SEEK_SET);
				int j = write(elf_fd, (void *)&Fix_flash_info, sizeof(sFixedFlashInfo));
				if (j < 0) { perror("Find Fix flash info error"); exit(-1); }
				printf("FIRMWARE ID: %s\n", Fix_flash_info.Firmware_ID);
				printf("Compiler Version: %s\n", Fix_flash_info.Compiler_Version);
				printf("Interrupt Vector Table : %#08x\n", Fix_flash_info.IVT);
				printf("Reset Start : %#08x\n", Fix_flash_info.Restart);
				if (Fix_flash_info.BACKUP_Enable || 1)//因为无效，默认都有备份区
					printf("Backup OFFSET  : %#08x\n", (Fix_flash_info.BACKUP_LOCACTION_Switch ? 1 : -1) * (Fix_flash_info.Backup_OFFSET));

				printf("APPEND DYNAMIC FLASH INFO POSITION: %#08x\n", Dy_flash_info_addr);
				if (!Fix_flash_info.PATCH_Disable)
				{
					printf("IF EFUSE ROM PATCH ENABLE,SOFTWARE ROM PATCH ENABLE\n");
					for (uint32_t i = 0; i < 8; i++)
					{
						printf("PATCH *0x1%02x%02x == %#08x\n", (Fix_flash_info.PATCH[i].addrh << 2), (Fix_flash_info.PATCH[i].addrl << 2), Fix_flash_info.PATCH[i].data.dword);
						if (!Fix_flash_info.PATCH[i].last)break;
					}
				}
				printf(" %s USED EXTERNAL FLASH TO FIND FIRMWARE\n", Fix_flash_info.EXTERNAL_FLASH_CTRL.ENABLE_USED ? "ENABLE" : "DISABLE");
				printf(" FIND EXTERNAL FLASH START %#x\n", Fix_flash_info.EXTERNAL_FLASH_CTRL.LAST8M_DISABLE ? 0 : 8 * 1024 * 1024);
				printf(" FIND EXTERNAL FLASH INTERVAL %#x\n", (256 * 1024) >> (Fix_flash_info.EXTERNAL_FLASH_CTRL.LowAddr_OFFSET << 1));
				if (Fix_flash_info.EXTERNAL_FLASH_CTRL.HighAddr_SPACE == 7)printf(" FIND EXTERNAL FLASH END %#x\n", 16 * 1024 * 1024);
				else
					printf(" FIND EXTERNAL FLASH END %#x\n", (Fix_flash_info.EXTERNAL_FLASH_CTRL.LAST8M_DISABLE ? 0 : 8 * 1024 * 1024) + (1 << (17 + Fix_flash_info.EXTERNAL_FLASH_CTRL.HighAddr_SPACE)));
				if (!Fix_flash_info.EXTERNAL_FLASH_CTRL.SPI_Switch)
				{
					printf("USED EXTERNAL QUAD FLASH");
					switch (Fix_flash_info.EXTERNAL_FLASH_CTRL.WP_Switch)
					{
						case 0:
							printf("WP SELECT GPIOB17\n");
							break;
						case 1:
						case 3:
							printf(" WP SELECT GPIOB29\n");
							break;
						case 2:
							printf("WP SELECT GPIOD8\n");
							break;
						// default:
						// 	printf(" WP SELECT NO KNOWN\n");
						// 	break;
					}


				}
				if (Fix_flash_info.EXTERNAL_FLASH_CTRL.CS_Switch)printf("SPIF CS SELECT GPIOA16\n");
				else printf("SPIF CS SELECT GPIOB22\n");
				printf(" %s\n", Fix_flash_info.EXTERNAL_FLASH_CTRL.MUST_MIRROR_DISABLE ? "ENABLE MIRROR" : "IF USED LAST 512K,CAN USED EXTERNAL FLASH");


				if (Fix_flash_info.EXTERNAL_FLASH_CTRL.PWM_Enable)
					printf(" MIRROR USED PWM%d\n", Fix_flash_info.EXTERNAL_FLASH_CTRL.PWMn_Switch);

				// const  char *file_name = "./ec_main.bin";

				const char *aes_algo = NULL; // 或 "AES"
				printf(" MIRROR SIZE %dk\n", (Fix_flash_info.EXTERNAL_FLASH_CTRL.Firmware_4KSector + 1) * 4);
				printf(" MAIN FREQ %dHz\n", (96000000 / Fix_flash_info.MainFrequency));
				printf("SECVER FUNCTION %s\n", (Fix_flash_info.SECVER_Enable || efuse_secver) ? "ENABLE" : "DISABLE");
				if (Fix_flash_info.SECVER_Enable || efuse_secver)
				{
					int a = 0;
					Fix_flash_info.SECVER_VERIFY_Switch = 1;//强制为RSA
					printf("SECVER USED %s", (sign_algo = (Fix_flash_info.SECVER_VERIFY_Switch) ? "RSA" : "ECDSA"));
					switch (Fix_flash_info.SECVER_BIT_Switch)
					{
						case 0:
							a = Fix_flash_info.SECVER_VERIFY_Switch ? 1024 : 192;
							break;
						case 1:
							a = Fix_flash_info.SECVER_VERIFY_Switch ? 2048 : 256;
							break;
						case 2:
							a = Fix_flash_info.SECVER_VERIFY_Switch ? 3072 : 384;
							break;
						case 3:
							a = Fix_flash_info.SECVER_VERIFY_Switch ? 4096 : 512;
						default:
							break;
					}
					printf("%d\n", a);

				}
				else printf("ONLY USED HASH\n");
				// printf("SECVER AES USED %s\n", (aes_algo = (Fix_flash_info.SECVER_AES_Enable) ? "AES" : "Disable"));
				// if(Fix_flash_info.SECVER_AES_Enable)
				// 	printf("SECVER USED %s\n", ((Fix_flash_info.SECVER_AES_MODE_Switch == 0) ? "OFB" : (Fix_flash_info.SECVER_AES_MODE_Switch == 1) ? "CFB" : (Fix_flash_info.SECVER_AES_MODE_Switch == 2) ? "CBC" : "ECB"));
				printf("SECVER USED %s\n", (hash_alg = (Fix_flash_info.SECVER_HASH_Switch == 0) ? "SHA224" : (Fix_flash_info.SECVER_HASH_Switch == 1) ? "SHA256" : (Fix_flash_info.SECVER_HASH_Switch == 2) ? "SHA384" : "SHA512"));

				printf("HOST INTERFACE %s\n", Fix_flash_info.LPC_Enable ? "LPC" : "ESPI");
				// break;
			}
			else
			{
				// Fix_flash_info_addr += 4096;//按4k查找
				printf("NOT FIND %s FIX FIRMWARE ID", file_name);
				close(elf_fd);
				return 0;

			}
		}
		close(elf_fd);
	}

	int elf_fd = open(file_name, O_RDONLY);

	if (elf_fd <= 0) { perror("file open error"); exit(-1); }

	ret = fstat(elf_fd, &file_stat);
	uint32_t msg1_len = (sector_4ksize + 1) * 4 * 1024;
	unsigned char *msg1 = mmap(NULL, (msg1_len), PROT_READ, MAP_SHARED, elf_fd, 0);
	if (msg1 == (void *)-1)
	{
		perror("msg1 mmap error");
		exit(-1);
	}
#ifdef _DEBUG
	printf("\nelf file size = %ld\n", file_stat.st_size);
#endif

#ifdef _DEBUG
	printf("msg1 start :0x%08x,len %.2lfK\n", *(unsigned int *)msg1, (double)msg1_len / 1024.0);
#endif
	size_t sig_len;

	unsigned char hash[512 >> 3];//hash[HASH_SIZE];
	memset(hash, 0, 512 >> 3);
	/*
	* */
	int hash_len = hash_data(msg1, msg1_len, hash, hash_alg);
	if (hash_len < 0)
	{
		close(elf_fd);
		return EXIT_FAILURE;
	}
	printf("Hash (%s): ", hash_alg);

	for (int i = 0; i < hash_len; i++)
	{
		printf("%#02x,", hash[i]);
	}
	printf("\n");
	if (Fix_flash_info.SECVER_Enable || efuse_secver)
	{
		unsigned char *signature = NULL;
		if (sign_hash(hash, hash_len, &signature, &sig_len, private_key_file, sign_algo) != 0)
		{
			close(elf_fd);
			return EXIT_FAILURE;
		}
		// if((Fix_flash_info.SECVER_VERIFY_Switch))
		{
			printf("SIGN (%s): ", sign_algo);
			for (size_t i = 0; i < sig_len; i++)
			{
				printf("%#02x,", signature[sig_len - i - 1]);
				Dy_flash_info.sign.rsa_byte[i] = signature[sig_len - i - 1];
			}
			printf("\n");
			printf("modules (%s): ", sign_algo);
			uint32_t public_bytelen = Dy_flash_info.publickey.rsa.bits >> 3;
			for (size_t i = 0; i < public_bytelen; i++)
			{
				printf("%#02x,", Dy_flash_info.publickey.rsa_byte.modulus[i]);
			}
			printf("\n");
			printf("exponent (%s): ", sign_algo);
			for (size_t i = 0; i < public_bytelen; i++)
			{
				printf("%#02x,", Dy_flash_info.publickey.rsa_byte.exponent[i]);
			}
			printf("\n");
			unsigned char public_hash[256 >> 3];
			hash_data((uint8_t *)&Dy_flash_info.publickey, sizeof(upublickey), public_hash, "SHA256");
			printf("PUBLIC KEY HASH (SHA256): ");
			for (size_t i = 0; i < (256 >> 3); i++)
			{
				printf("%#02x,", public_hash[i]);
			}
			printf("\n");
			AES_KEY key;
			uint32_t aes_key_efuse[4] = { 0x5137b6ad,0x9239ca4c,0xb7d82c24,0xd50abd5d };
			printf("AES KEY (AES-128): ");
			for (size_t i = 0; i < 16; i++)
			{
				printf("%#02x,", ((uint8_t *)aes_key_efuse)[i]);
			}
			printf("\n");
			unsigned char public_hash_aes_key[256 >> 3];
			AES_set_encrypt_key((uint8_t *)aes_key_efuse, 128, &key);

			AES_encrypt(&public_hash[0], &public_hash_aes_key[0], &key);
			AES_encrypt(&public_hash[16], &public_hash_aes_key[16], &key);
			printf("PUBLIC KEY HASH AES (AES-128): ");
			for (size_t i = 0; i < (256 >> 3); i++)
			{
				printf("%#02x,", public_hash_aes_key[i]);
			}
			printf("\n");

			AES_ecb_encrypt(&public_hash[0], &public_hash_aes_key[0], &key, AES_ENCRYPT);
			AES_ecb_encrypt(&public_hash[16], &public_hash_aes_key[16], &key, AES_ENCRYPT);
			printf("PUBLIC KEY HASH AES (AES-128-ECB): ");
			for (size_t i = 0; i < (256 >> 3); i++)
			{
				printf("%#02x,", public_hash_aes_key[i]);
			}
			printf("\n");
		}
		// else
		// {
		// 	printf("R (%s): ", sign_algo);
		// 	for(size_t i = 0; i < sig_len; i++)
		// 	{
		// 		printf("%#02x,", signature[sig_len - i - 1]);
		// 		Dy_flash_info.sign.ecc_byte.R.auValue[i] = signature[sig_len - i - 1];
		// 	}
		// 	printf("\n");
		// 	printf("S (%s): ", sign_algo);
		// 	for(size_t i = 0; i < sig_len; i++)
		// 	{
		// 		printf("%#02x,", signature[sig_len - i - 1]);
		// 		Dy_flash_info.sign.ecc_byte.S.auValue[i] = signature[sig_len - i - 1];
		// 	}
		// 	printf("\n");
		// 	printf("X (%s): ", sign_algo);
		// 	for(size_t i = 0; i < Dy_flash_info.publickey.ecc_byte.stuQx.uLen; i++)
		// 	{
		// 		printf("%#02x,", Dy_flash_info.publickey.ecc_byte.stuQx.auValue[i]);
		// 	}
		// 	printf("\n");
		// 	printf("X (%s): ", sign_algo);
		// 	for(size_t i = 0; i < Dy_flash_info.publickey.ecc_byte.stuQy.uLen; i++)
		// 	{
		// 		printf("%#02x,", Dy_flash_info.publickey.ecc_byte.stuQy.auValue[i]);
		// 	}
		// 	printf("\n");
		// }
	}
	else
	{
		printf("SIGN (%s): ", hash_alg);

		for (int i = 0; i < hash_len; i++)
		{
			printf("%#02x,", hash[i]);
			Dy_flash_info.sign.hash_byte[i] = hash[i];
		}
		printf("\n");
	}
   // if(strcmp(aes_algo, "AES")==0)//使用AES加密
   // {
   // 	unsigned char *encrypted_data = NULL;
   // 	if(aes_encrypt(hash, hash_len, &encrypted_data, &encrypted_len, aes_key, aes_iv, 256) != 0)
   // 	{
   // 		free(file_data);
   // 		free(signature);
   // 		return EXIT_FAILURE;
   // 	}
   // 	if(aes_encrypt(signature, sig_len, &encrypted_data, &encrypted_len, aes_key, aes_iv, 256) != 0)
   // 	{
   // 		free(file_data);
   // 		free(signature);
   // 		return EXIT_FAILURE;
   // 	}
   // }


	munmap(msg1, sizeof(msg1_len));
	close(elf_fd);

	printf("\n");

	// printf("Encrypted data size: %zu bytes\n", encrypted_len);

	elf_fd = open(file_name, O_WRONLY);
	lseek(elf_fd, 0, SEEK_END);
	i = write(elf_fd, (void *)&Dy_flash_info, sizeof(sDynamicFlashInfo));
	if (i == 0) { perror("flash_data write error"); exit(-1); }
	off_t elf_seek = lseek(elf_fd, 0, SEEK_END);

	bin_size = (elf_seek + ((elf_seek % bin_seek) ? bin_seek : 0) - (elf_seek % bin_seek));//对bin进行对齐处理
	uint32_t temp = bin_size - elf_seek;
	if (temp < 0)//bin所需文件大小小于实际文件大小这判断异常
	{
		printf("file size padding error\n");
		return -1;
	}
	else if (temp > 0)//位数不够的地方进行填充
	{
		padding = malloc(temp);
		memset(padding, 0xff, temp);
		write(elf_fd, padding, temp);
		free(padding);
	}
	close(elf_fd);
	printf("GLE01 OKAY\n");


	return EXIT_SUCCESS;
}
