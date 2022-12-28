#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <gmp.h>
#include <libakrypt-base.h>
#include <source/libakrypt.h>

//#define AK_HAVE_GMP_H
//#define LIBAKRYPT_HAVE_GMP_H
 
int main(void)
{


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//ВЫЧИСЛЕНИЕ ТОЧКИ K_a (1)
	printf("\nСТОРОНА А ВЫЧИСЛЯЕТ ТОЧКУ K_А:\n");
	////////////////////////
	
	ak_mpzn256 k_a;     	  
	int i;
	
	//СОЗДАЕМ ГЕНЕРАТОР
	struct random generator;
	ak_random_create_lcg( &generator );
	
	memset( k_a, 0, sizeof( k_a )); 
	
	//ВЫРАБАТЫВАЕМ ПОСЛЕДОВАТЕЛЬНОСТИ ПСЕВДО-СЛУЧАЙНЫХ ДАННЫХ	
	ak_random_ptr( &generator, k_a, 32 ); 
	
	//ЭКСПОРТ КРИВОЙ
	extern const struct wcurve id_tc26_gost_3410_2012_256_paramSetTest; /////////////Кривая B
	ak_wcurve wc = &id_tc26_gost_3410_2012_256_paramSetTest;
	
	
	//ВЫВОДИМ q, k_a
	const char *str = NULL;
	printf("str q_a   = %s\n", str = ak_mpzn_to_hexstr( wc->q, wc->size ));
	//printf("str   k_a = %s\n", str = ak_mpzn_to_hexstr( k_a, wc->size ));
	
		
	//ВЫЧИСЛЯЕМ ПО МОДУЛЮ
	ak_mpzn256 res;	
	ak_uint64 *p = wc->q;

	ak_mpzn_rem( res, k_a, p, wc->size );
	printf("mod_k_a   = %s\n", str = ak_mpzn_to_hexstr( res, wc->size ));
	
	ak_uint64 *mod_k_a = res;
	
		
	
	struct wpoint K_A;
	struct wpoint P;
	
	//ПРИСВОЕНИЕ ТОЧКИ ПРЯМОЙ
	ak_wpoint_set( &P, wc );
	//ПРОВЕРИМ ПРИНАДЛЕЖНОСТЬ
	//printf( "P >> wc  bool = %d\n", ak_wpoint_is_ok( &P, wc ));
	//ВЫЧИСЛИМ КРАТНУЮ ТОЧКУ 
	ak_wpoint_pow( &K_A, &P, mod_k_a, ak_mpzn256_size , wc );
	//ПРОВЕРИМ ПРИНАДЛЕЖНОСТЬ
	printf( "K_A >> wc  bool = %d\n", ak_wpoint_is_ok( &K_A, wc ));
	
	
	//////////////////////////
	//(6e
	printf("\nСТОРОНА В ВЫЧИСЛЯЕТ k_b:\n");
	//////////////////////////
	ak_mpzn256 k_b;     	  
	memset( k_b, 0, sizeof( k_b )); 	
	ak_random_ptr( &generator, k_b, 32 ); 
	const char *str1 = NULL;
	extern const struct wcurve id_rfc4357_gost_3410_2001_paramSetA; /////////////Кривая A
	ak_wcurve ec = &id_rfc4357_gost_3410_2001_paramSetA;
	
	//extern const struct wcurve id_tc26_gost_3410_2012_256_paramSetTest; /////////////
	//ak_wcurve ec = &id_tc26_gost_3410_2012_256_paramSetTest;
	
	
	printf("str q_b   = %s\n", str1 = ak_mpzn_to_hexstr( ec->q, ec->size ));
	//printf("str k_b   = %s\n", str1 = ak_mpzn_to_hexstr( k_b, ec->size ));
	ak_mpzn256 res1;	
	ak_uint64 *b = ec->q;
	
	ak_mpzn_rem( res1, k_b, b, ec->size );
	printf("mod_k_b   = %s\n", str1 = ak_mpzn_to_hexstr( res1, ec->size ));
	ak_uint64 *mod_k_b = res1;
	struct wpoint K_B;
	struct wpoint B;
	//ПРИСВОЕНИЕ ТОЧКИ ПРЯМОЙ
	ak_wpoint_set( &B, ec );
	//ПРОВЕРИМ ПРИНАДЛЕЖНОСТЬ
	//printf( "P >> wc  bool = %d\n", ak_wpoint_is_ok( &P, wc ));
	//ВЫЧИСЛИМ КРАТНУЮ ТОЧКУ 
	ak_wpoint_pow( &K_B, &B, mod_k_b, ak_mpzn256_size , ec );
	//ПРОВЕРИМ ПРИНАДЛЕЖНОСТЬ
	printf( "K_B >> ec  bool = %d\n", ak_wpoint_is_ok( &K_B, ec ));
	
	
	
	////////////////////////////////////
	//S_a S_b Генерируем по образу К_a, Id_a, Id_b
	printf("\nВЫЧИСЛЕНИЯ ДЛЯ КЛЮЧЕЙ S_a S_b, ГЕНЕРАЦИЯ Id_a, Id_b:\n");
	/////////////////////
	ak_mpzn256 s_a, s_b;     	  
	memset( s_a, 0, sizeof( s_a )); 
	memset( s_b, 0, sizeof( s_b ));	
	//ak_random_ptr( &generator, s_a, 32 ); 
	ak_mpzn_set(s_a,  k_b, ak_mpzn256_size);
	//ak_random_ptr( &generator, s_b, 32 );
	ak_mpzn_set(s_b,  k_a, ak_mpzn256_size);
	ak_mpzn256 res2, res3;
	ak_mpzn_rem( res2, s_a, b, ec->size );
	ak_uint64 *mod_s_a = res2;
	//printf("mod_s_a   = %s\n", str1 = ak_mpzn_to_hexstr( res2, ec->size ));
	ak_mpzn_rem( res3, s_b, p, wc->size );
	ak_uint64 *mod_s_b = res3;
	//printf("mod_s_b   = %s\n", str1 = ak_mpzn_to_hexstr( res3, wc->size ));
	
	
	struct wpoint S_A;
	struct wpoint S_B;
	//struct wpoint B;
	//ak_wpoint_set( &B, ec );
	ak_wpoint_pow( &S_A, &B, mod_s_a, ak_mpzn256_size , ec );
	ak_wpoint_pow( &S_B, &P, mod_s_b, ak_mpzn256_size , wc );
	printf( "S_a >> ec  bool = %d\n", ak_wpoint_is_ok( &S_A, ec ));
	printf( "S_b >> wc  bool = %d\n", ak_wpoint_is_ok( &S_B, wc ));
	
	// Id_a, Id_b 
	const char *Id_a = NULL;
	const char *Id_b = NULL;
	printf("strId_a   = %s\n", Id_a = "qwertyuiop123456" );
	printf("strId_b   = %s\n", Id_b = "098765asdfghjklz" );
		
	ak_random_destroy( &generator );
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//СТОРОНА В ВЫЧИСЛЯЕТ Q_AB R_AB (7)
	printf("\nСТОРОНА В ВЫЧИСЛЯЕТ Q_AB R_AB:\n");
	/////////////////////////////////////////
	
	// Q_AB
	
	// m/q = кофактор
	ak_uint64 c_a;
	//ak_uint64 *r;
	ak_mpzn256 r, re;
	//printf("cofactor   ec = %lld\n", c_a = ec->cofactor);
	
	//Умножаем вычет k_b на кофактор	
	ak_mpzn_mul_ui( r, mod_k_b, ak_mpzn256_size, ec->cofactor );	// куда. k_b. size. с_а
	//printf("str c*k_b = %s\n", str = ak_mpzn_to_hexstr( r, ec->size ));
	
	
	ak_mpzn_rem( re, r, b, ec->size );// надо ли
	//printf("mod c*k_b = %s\n", str = ak_mpzn_to_hexstr( re, ec->size ));
	
	
	struct wpoint Q_AB;
	ak_wpoint_pow( &Q_AB, &S_A, re, ak_mpzn256_size, ec );
	printf( "Q_AB >> ec  bool = %d\n", ak_wpoint_is_ok( &Q_AB, ec ));
	
	
	// R_AB = c_b * s_B * K_a
	
	ak_uint64 c_b;
	//printf("cofactor   wc = %lld\n", c_b = wc->cofactor);
	
	ak_mpzn256 r1, re1;
	ak_mpzn_mul_ui( r1, mod_s_b, ak_mpzn256_size, wc->cofactor );
	//printf("str c*s_b = %s\n", str = ak_mpzn_to_hexstr( r1, wc->size ));
	
	ak_mpzn_rem( re1, r1, p, wc->size );
	
	struct wpoint R_AB;
	ak_wpoint_pow( &R_AB, &K_A, re1, ak_mpzn256_size, wc );
	printf( "R_AB >> wc  bool = %d\n", ak_wpoint_is_ok( &R_AB, wc ));
	
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//СТОРОНА В ВЫЧИСЛЯЕТ T_AB (8)
	printf("\nСТОРОНА В ВЫЧИСЛЯЕТ T_AB:\n");
	/////////////////////////////////////////
	
	//ak_uint64 T_AB;
	
	// ----------------------------------------------------------------------------------------------- 
	ak_uint8 kin[160];
	
   	ak_uint8 iv[64] = {
   	0x40, 0x39, 0xd4, 0x68, 0xd2, 0xd9, 0x35, 0xc1, 0x4f, 0x22, 0x42, 0xb6, 0xe3, 0x9d, 0xb3, 0xb5,
   	0xd5, 0x99, 0x95, 0xc1, 0xe7, 0xa0, 0x11, 0x87, 0xf6, 0xf7, 0xa3, 0xd7, 0xe3, 0xc2, 0x6a, 0xc3,
   	0x19, 0x12, 0xf4, 0xc2, 0x4e, 0x1d, 0x64, 0xfe, 0x62, 0xec, 0x44, 0xad, 0x48, 0xd8, 0xa4, 0x6b,
   	0x7a, 0x9e, 0xf8, 0xe4, 0xab, 0x7f, 0x7b, 0x3b, 0x47, 0x95, 0x18, 0x3d, 0xf6, 0x73, 0x1c, 0x1e };

 	ak_uint8 label[77] =
  	"jhljdfhlajhadbfbjr=QQ387452nad,mfn8bkhagbdfkhadhgfafnaba98z7cz987s987csa98c6w";

	ak_uint8 seed[32] = "abcdefghijklmnopqrstuvwxyz012345";

	// ----------------------------------------------------------------------------------------------- 
	int j;         
        
        
	const char *hex = NULL;
	hex = ak_mpzn_to_hexstr( Q_AB.x, ec->size );
	//printf("Q_AB x  = %s\n", hex);
	
	for( j = 0; j < 64; j++ ) {
        	if (j<64 && j>-1) kin[j] = hex[j];
        	//printf( "%x " , kin[j]);
        	}
	
	
	
	const char *rex = NULL;
	rex = ak_mpzn_to_hexstr( R_AB.x, wc->size );
	//printf("R_AB x  = %s\n", rex);
	
	
	
	
	for( j = 64; j < 160; j++ ) {
        	//if (j<64 && j>-1) kin[j] = hex[j];
        	if (j<128 && j>63) kin[j] = rex[j-64];
        	if (j<144 && j>127) kin[j] = Id_a[j-128];
        	if (j<160 && j>143) kin[j] = Id_b[j-144];                 
        	//printf( "%x " , kin[j]);	
        }
        
        //printf("\n");
         
        struct kdf_state ks; // вычисляем kdf
	const char *ptr;
	
	ak_uint8 buffer[64];
	
	
	
	ak_kdf_state_create( &ks, kin, sizeof(kin), hmac_hmac512_kdf, NULL, 0, seed, 32, NULL, 0, 3);
	
	ak_kdf_state_next( &ks, buffer, 64 ); // ( ak_kdf_state state, ak_pointer buffer, const size_t buffer_size )	
	
  	ak_kdf_state_destroy( &ks );
  	
  	printf("key_AB: %s (%d bytes)\n", ptr = ak_ptr_to_hexstr( buffer, 64, ak_false ), 64 );
	
	//printf("\n");
	//ak_uint64 T_AB = buffer;
	
	
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//СТОРОНА В ВЫЧИСЛЯЕТ K_AB M_AB (9)
	//printf("\nСТОРОНА В ВЫЧИСЛЯЕТ K_AB M_AB:\n");
	/////////////////////////////////////////
	ak_uint8 mab_tag[64];
        
        for( j = 0; j < 64; j++ ) {  
        	
        	mab_tag[j] = ptr[j+64];  
        	
        	//printf( "%x " , mab_tag[j]);	
        }
        
        
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//СТОРОНА В ВЫЧИСЛЯЕТ tag_b (10)
	printf("\nСТОРОНА В ВЫЧИСЛЯЕТ tag_b:\n");
	///////////////////////////////////////////
	
	
	
	ak_uint8 kin_tag_b[160];
	
	const char *kbx = NULL;
	kbx = ak_mpzn_to_hexstr( K_B.x, 4 );
	//printf("K_B x  = %s\n", kbx);
	
	for( j = 0; j < 64; j++ ) {
        	if (j<64 && j>-1) kin_tag_b[j] = kbx[j];
        	//printf( "%x " , kin_tag_b[j]);
        	}
	
	const char *kax = NULL;
	kax = ak_mpzn_to_hexstr( K_A.x, 4 );
	//printf("K_A x  = %s\n", kax);
	
	
	
	for( j = 64; j < 160; j++ ) {  //K_B K_A ID_B ID_A
        	//if (j<64 && j>-1) kin[j] = hex_b[j]; //K_B
        	if (j<128 && j>63) kin_tag_b[j] = kax[j-64]; //K_A
        	if (j<144 && j>127) kin_tag_b[j] = Id_b[j-128];
        	if (j<160 && j>143) kin_tag_b[j] = Id_a[j-144];
        	//printf( "%x " , kin_tag[j]);	
        }
        //printf("\n");
        
        
       
	
	ak_oid oid;
	
   	
   	ak_hmac ctx = ak_oid_new_object( oid = ak_oid_find_by_name( "hmac-streebog512" ));
	
	ak_uint8 buffer_tag_b[128];
	
	memset( buffer_tag_b, 0, sizeof( buffer_tag_b ));
	
	ak_hmac_set_key( ctx, mab_tag, sizeof(mab_tag) );
		
   	ak_hmac_ptr( ctx, kin_tag_b, sizeof(kin_tag_b), buffer_tag_b, sizeof( buffer_tag_b ));
   	
   	printf("tab_b :%s (%lu bytes: %s, %s)\n",
                                 ak_ptr_to_hexstr( buffer_tag_b, ak_hmac_get_tag_size( ctx ), ak_false ),
						(unsigned long int)ak_hmac_get_tag_size( ctx ), ctx->key.oid->name[0], ctx->key.oid->id[0] );
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//СТОРОНА A ВЫЧИСЛЯЕТ Q_BA R_BA (15)
	printf("\nСТОРОНА A ВЫЧИСЛЯЕТ Q_BA R_BA:\n");
	/////////////////////////////////////////
	
	// Q_BA
	
	
	ak_mpzn256 r_b, re_b;
	
	
	//Умножаем вычет s_a на кофактор	
	ak_mpzn_mul_ui( r_b, mod_s_a, ak_mpzn256_size, ec->cofactor );	// куда. k_b. size. с_а
	//printf("str c*s_a = %s\n", str = ak_mpzn_to_hexstr( r_b, ec->size ));
	
	
	ak_mpzn_rem( re_b, r_b, b, ec->size );
	//printf("mod c*s_a = %s\n", str = ak_mpzn_to_hexstr( re_b, ec->size ));
	
	
	struct wpoint Q_BA;
	ak_wpoint_pow( &Q_BA, &K_B, re_b, ak_mpzn256_size, ec );
	printf( "Q_BA >> ec  bool = %d\n", ak_wpoint_is_ok( &Q_BA, ec ));
	
	
	
	
	ak_mpzn256 r1_b, re1_b;
	ak_mpzn_mul_ui( r1_b, mod_k_a, ak_mpzn256_size, wc->cofactor );
	//printf("str c*k_b = %s\n", str = ak_mpzn_to_hexstr( r1_b, ec->size ));
	
	ak_mpzn_rem( re1_b, r1_b, p, wc->size );
	
	struct wpoint R_BA;
	ak_wpoint_pow( &R_BA, &S_B, re1_b, ak_mpzn256_size, wc );
	printf( "R_BA >> wc  bool = %d\n", ak_wpoint_is_ok( &R_BA, wc ));
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//СТОРОНА A ВЫЧИСЛЯЕТ T_BA(16)
	printf("\nСТОРОНА A ВЫЧИСЛЯЕТ T_BA:\n");
	/////////////////////////////////////////
	
	const char *hex_b = NULL;	
	hex_b = ak_mpzn_to_hexstr( Q_BA.x, ec->size );
	//printf("Q_BA x  = %s\n", hex_b);
	
	for( j = 0; j < 64; j++ ) {
        	if (j<64 && j>-1) kin[j] = hex_b[j];
        	//printf( "%x " , kin[j]);
        	}
	
	const char *hex1_b = NULL;	
	hex1_b = ak_mpzn_to_hexstr( R_BA.x, wc->size );	
	//printf("R_BA x  = %s\n", hex1_b);
	
	
	//int j;
	for( j = 64; j < 160; j++ ) {
        	//if (j<64 && j>-1) kin[j] = hex_b[j];
        	if (j<128 && j>63) kin[j] = hex1_b[j-64];
        	if (j<144 && j>127) kin[j] = Id_a[j-128];
        	if (j<160 && j>143) kin[j] = Id_b[j-144];                 
        	//printf( "%x " , kin[j]);	
        }
        //printf("\n");
         
        
        
        struct kdf_state ks_b; // вычисляем kdf
	const char *ptr_b;
	
	ak_uint8 buffer_b[64];
	
	
	ak_kdf_state_create( &ks_b, kin, sizeof(kin), hmac_hmac512_kdf, NULL, 0, seed, 32, NULL, 0, 3 );
	
	ak_kdf_state_next( &ks_b, buffer_b, 64 ); // ( ak_kdf_state state, ak_pointer buffer, const size_t buffer_size )
	
  	ak_kdf_state_destroy( &ks_b );
  	
  	
  	
  	printf("key_BA: %s (%d bytes)\n", ptr_b = ak_ptr_to_hexstr( buffer_b, 64, ak_false ), 64 );
	
	
	
	//ak_uint64 T_BA = buffer_b;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//СТОРОНА A ВЫЧИСЛЯЕТ K_BA M_BA (17)
	//printf("\nСТОРОНА A ВЫЧИСЛЯЕТ K_BA M_BA:\n");
	/////////////////////////////////////////
	
	
	ak_uint8 mba_tag[64];
        
        for( j = 0; j < 64; j++ ) {  
        	
        	mba_tag[j] = ptr_b[j+64];  
        	
        	//printf( "%x " , mab_tag[j]);	
        }
	
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//СТОРОНА A ВЫЧИСЛЯЕТ tag_b' (18) + сравнение tag'ов
	printf("\nСТОРОНА A ВЫЧИСЛЯЕТ tag_b':\n");
	///////////////////////////////////////////
	
	ak_oid oid1;
   	
   	ak_hmac ctx1 = ak_oid_new_object( oid1 = ak_oid_find_by_name( "hmac-streebog512" ));
	
	ak_uint8 buffer_tag_b1[128];
	
	memset( buffer_tag_b1, 0, sizeof( buffer_tag_b1 ));
	
	ak_hmac_set_key( ctx1, mba_tag, sizeof(mba_tag) );
		
   	ak_hmac_ptr( ctx1, kin_tag_b, sizeof(kin_tag_b), buffer_tag_b1, sizeof( buffer_tag_b1 ));
   	
   	printf("tab_b' :%s (%lu bytes: %s, %s)",
                                 ak_ptr_to_hexstr( buffer_tag_b1, ak_hmac_get_tag_size( ctx1 ), ak_false ),
						(unsigned long int)ak_hmac_get_tag_size( ctx1 ), ctx1->key.oid->name[0], ctx1->key.oid->id[0] );
	
	if (ak_ptr_to_hexstr( buffer_tag_b1, ak_hmac_get_tag_size( ctx1 ), ak_false ) == ak_ptr_to_hexstr( buffer_tag_b, ak_hmac_get_tag_size( ctx ), ak_false )) {
		printf(" EQUAL\n");
		}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//СТОРОНА A ВЫЧИСЛЯЕТ tag_a (19) 
	printf("\nСТОРОНА A ВЫЧИСЛЯЕТ tag_a:\n");
	///////////////////////////////////////////
	
	ak_uint8 kin_tag_a[160];
	
	const char *k_ax = NULL;
	k_ax = ak_mpzn_to_hexstr( K_A.x, 4 );
	//printf("K_A x  = %s\n", k_ax);
	
	for( j = 0; j < 64; j++ ) {
        	if (j<64 && j>-1) kin_tag_a[j] = k_ax[j];
        	//printf( "%x " , kin_tag_b[j]);
        	}
	
	const char *k_bx = NULL;
	k_bx = ak_mpzn_to_hexstr( K_B.x, 4 );
	//printf("K_B x  = %s\n", k_bx);
	
	
	
	for( j = 64; j < 160; j++ ) {  
        	//if (j<64 && j>-1) kin[j] = hex_b[j]; //K_A
        	if (j<128 && j>63) kin_tag_b[j] = k_bx[j-64]; //K_B
        	if (j<144 && j>127) kin_tag_b[j] = Id_a[j-128];
        	if (j<160 && j>143) kin_tag_b[j] = Id_b[j-144];
        	//printf( "%x " , kin_tag[j]);	
        }
        //printf("\n");
        
        
       
	
	ak_oid oid2;
	
   	
   	ak_hmac ctx2 = ak_oid_new_object( oid2 = ak_oid_find_by_name( "hmac-streebog512" ));
	
	ak_uint8 buffer_tag_a[128];
	
	memset( buffer_tag_a, 0, sizeof( buffer_tag_a ));
	
	ak_hmac_set_key( ctx2, mba_tag, sizeof(mba_tag) );
		
   	ak_hmac_ptr( ctx2, kin_tag_a, sizeof(kin_tag_a), buffer_tag_a, sizeof( buffer_tag_a ));
   	
   	printf("tab_a :%s (%lu bytes: %s, %s)\n",
                                 ak_ptr_to_hexstr( buffer_tag_a, ak_hmac_get_tag_size( ctx2 ), ak_false ),
						(unsigned long int)ak_hmac_get_tag_size( ctx2 ), ctx2->key.oid->name[0], ctx2->key.oid->id[0] );
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//СТОРОНА В ВЫЧИСЛЯЕТ tag_a' (21) + сравнение tag'ов
	printf("\nСТОРОНА В ВЫЧИСЛЯЕТ tag_a':\n");
	///////////////////////////////////////////
	
	
	ak_oid oid3;
	
   	
   	ak_hmac ctx3 = ak_oid_new_object( oid3 = ak_oid_find_by_name( "hmac-streebog512" ));
	
	ak_uint8 buffer_tag_a1[128];
	
	memset( buffer_tag_a1, 0, sizeof( buffer_tag_a1 ));
	
	ak_hmac_set_key( ctx3, mab_tag, sizeof(mab_tag) );
		
   	ak_hmac_ptr( ctx3, kin_tag_a, sizeof(kin_tag_a), buffer_tag_a1, sizeof( buffer_tag_a1 ));
   	
   	printf("tab_a' :%s (%lu bytes: %s, %s)",
                                 ak_ptr_to_hexstr( buffer_tag_a1, ak_hmac_get_tag_size( ctx3 ), ak_false ),
						(unsigned long int)ak_hmac_get_tag_size( ctx3 ), ctx3->key.oid->name[0], ctx3->key.oid->id[0] );
	
	if (ak_ptr_to_hexstr( buffer_tag_a1, ak_hmac_get_tag_size( ctx3 ), ak_false ) == ak_ptr_to_hexstr( buffer_tag_a, ak_hmac_get_tag_size( ctx2 ), ak_false )) {
		printf(" EQUAL\n");
		}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//ПРОВЕРКА K_AB == K_BA (21)
	printf("\nПРОВЕРКА K_AB == K_BA:\n");
	///////////////////////////////////////////
	//if( ptr_b == ptr) printf("yes\n");
	
       
       
	
	printf("K_AB: %s\n", ak_ptr_to_hexstr( buffer, 32, ak_false ));
	printf("K_BA: %s\n", ak_ptr_to_hexstr( buffer_b, 32, ak_false ));
	
	if (ak_ptr_to_hexstr( buffer, 32, ak_false) == ak_ptr_to_hexstr( buffer, 32, ak_false)) {
		printf("EQUAL\n");
		}
		

	/////////////////////////////////////////////////////////////////////// TECT
	
	/*
	
	ak_mpzn256 *k_a;  
	ak_mpzn256 *k_b;
	ak_mpzn256 *s_a, *s_b;	   	  
	
	ak_mpzn256 a, b, c, d;
	
	k_a = &a;
	k_b = &b;
	s_a = &c;
	s_b = &d;
	
	
	//СОЗДАЕМ ГЕНЕРАТОР
	struct random generator;
	ak_random_create_lcg( &generator );
	
	
	 
	
	//ЭКСПОРТ КРИВОЙ
	extern const struct wcurve id_tc26_gost_3410_2012_256_paramSetTest; /////////////Кривая B
	ak_wcurve wc = &id_tc26_gost_3410_2012_256_paramSetTest;
	
	extern const struct wcurve id_rfc4357_gost_3410_2001_paramSetA; /////////////Кривая A
	ak_wcurve ec = &id_rfc4357_gost_3410_2001_paramSetA;
	
	
	ak_mpzn_set_random_modulo( a, wc->q, ak_mpzn256_size, &generator);
	ak_mpzn_set_random_modulo( b, ec->q, ak_mpzn256_size, &generator);
	ak_mpzn_set_random_modulo( c, ec->q, ak_mpzn256_size, &generator);
	ak_mpzn_set_random_modulo( d, wc->q, ak_mpzn256_size, &generator);
	
	
	//ak_mpzn_set_hexstr(k_a , ak_mpzn256_size, "1999c54e717e4ac5cd2368cbfbda3ce8f87833482d8a45f1b5a13b079587e90e");
	//ak_mpzn_set_hexstr(k_b , ak_mpzn256_size, "7abe19cddd22cba5cb66e1f6b7af15d43191e0e1a94bb78bab0d4e512332d8ff");
	//ak_mpzn_set_hexstr(s_a , ak_mpzn256_size, "6c588477bd4394df6b113ba328a7f1964557f01af2216e5d7cf4de9e8ded3269");
	//ak_mpzn_set_hexstr(s_b , ak_mpzn256_size, "77f6b3c6edb31c31259c8f1f8f72027f38e3aa4e8acb38d98f761df92edbc655");
	
	
	//ВЫВОДИМ q, k_a
	const char *str = NULL;
	printf("str  q_a  = %s\n", str = ak_mpzn_to_hexstr( wc->q, wc->size ));
	printf("str k_a   = %s\n", str = ak_mpzn_to_hexstr( a, wc->size ));
	printf("str s_b   = %s\n", str = ak_mpzn_to_hexstr( d, wc->size ));
	
	printf("str  q_b  = %s\n", str = ak_mpzn_to_hexstr( ec->q, ec->size ));
	printf("str k_b   = %s\n", str = ak_mpzn_to_hexstr( b, ec->size ));
	printf("str s_a   = %s\n", str = ak_mpzn_to_hexstr( c, ec->size ));
	
	
	
	
	
	// ТОЧКИ	
	
	struct wpoint K_A;
	struct wpoint K_B;
	struct wpoint S_A;
	struct wpoint S_B;
	
	
	struct wpoint P, B;
	struct wpoint P1, B1;
	
	//ПРИСВОЕНИЕ ТОЧКИ ПРЯМОЙ
	ak_wpoint_set( &P, wc );
	ak_wpoint_set( &B, ec );
	
	ak_wpoint_set( &P1, wc );
	ak_wpoint_set( &B1, ec );
	
	//ПРОВЕРИМ ПРИНАДЛЕЖНОСТЬ
	printf( "P >> wc  bool = %d\n", ak_wpoint_is_ok( &P, wc ));
	printf( "B >> ec  bool = %d\n", ak_wpoint_is_ok( &B, ec ));
	printf( "P1 >> wc  bool = %d\n", ak_wpoint_is_ok( &P1, wc ));
	printf( "B1 >> ec  bool = %d\n", ak_wpoint_is_ok( &B1, ec ));
	
	//ВЫЧИСЛИМ КРАТНУЮ ТОЧКУ 
	ak_wpoint_pow( &K_A, &P, a, ak_mpzn256_size , wc );
	ak_wpoint_pow( &K_B, &B, b, ak_mpzn256_size , ec );
	ak_wpoint_pow( &S_A, &B1, c, ak_mpzn256_size , ec );
	ak_wpoint_pow( &S_B, &P1, d, ak_mpzn256_size , wc );
	
	//ПРОВЕРИМ ПРИНАДЛЕЖНОСТЬ
	printf( "K_A >> wc  bool = %d\n", ak_wpoint_is_ok( &K_A, wc ));
	printf( "K_A >> wc  check = %d\n", ak_wpoint_check_order( &K_A, wc ));
	printf( "K_B >> ec  bool = %d\n", ak_wpoint_is_ok( &K_B, ec ));
	printf( "K_B >> ec  check = %d\n", ak_wpoint_check_order( &K_B, ec ));
	printf( "S_a >> ec  bool = %d\n", ak_wpoint_is_ok( &S_A, ec ));
	printf( "S_a >> ec  check = %d\n", ak_wpoint_check_order( &S_A, ec ));
	printf( "S_b >> wc  bool = %d\n", ak_wpoint_is_ok( &S_B, wc ));
	printf( "S_b >> wc  check = %d\n", ak_wpoint_check_order( &S_B, wc ));
	

	ak_random_destroy( &generator );
	
	
	/////    Q_AB Q_BA
	
	struct wpoint Q_AB;
	ak_wpoint_pow( &Q_AB, &S_A, b, ak_mpzn256_size, ec );
	printf( "Q_AB >> ec  bool = %d\n", ak_wpoint_is_ok( &Q_AB, ec ));
	printf( "Q_AB >> ec  check = %d\n", ak_wpoint_check_order( &Q_AB, ec ));
	
	struct wpoint Q_BA;
	ak_wpoint_pow( &Q_BA, &K_B, c, ak_mpzn256_size, ec );
	printf( "Q_BA >> ec  bool = %d\n", ak_wpoint_is_ok( &Q_BA, ec ));
	printf( "Q_BA >> ec  check = %d\n", ak_wpoint_check_order( &Q_BA, ec ));
	
	/////// Q_AB.x   Q_BA.x
	
	const char *hex = NULL;
	hex = ak_mpzn_to_hexstr( Q_AB.x, ec->size );
	printf("Q_AB x  = %s\n", hex);
	
	
	const char *hex_b = NULL;	
	hex_b = ak_mpzn_to_hexstr( Q_BA.x, ec->size );\
	printf("Q_BA x  = %s\n", hex_b);
	
	*/
	
	
	/////////////////////////////////////////////////////////////////////////
	return 0;
		
}
