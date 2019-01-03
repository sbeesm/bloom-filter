#include <stdio.h>
#include <string.h>

// all the hash functions are taken from:
// http://www.partow.net/programming/hashfunctions/index.html
   
unsigned int RSHash  (unsigned char *);
unsigned int DJBHash (unsigned char *);
unsigned int FNVHash (unsigned char *);
unsigned int JSHash (unsigned char *);
unsigned int PJWHash (unsigned char *);
unsigned int SDBMHash (unsigned char *);
unsigned int DEKHash (unsigned char *);

void get_hashes(unsigned int [], char *);
void bloom_insert(unsigned char *);
void bloom_query(unsigned char *);
void set_bit(int);
int test_bit(int);

int bit_arr[100];

main(){	
	
	bloom_insert("abc");
	bloom_insert("Scooby_Doo");
	bloom_insert("Shagg");
	
	bloom_query("Scooby");
	bloom_query("shaggy132");
	bloom_query("ABC");
	bloom_query("Scooby_Doo");
}

void bloom_insert(unsigned char *str){
	
	unsigned int hash_arr[7];
	get_hashes(hash_arr, str);
	
	int i;
	for(i=0; i<7; ++i){
		set_bit(hash_arr[i]);
	}
	
}

void bloom_query(unsigned char *str){	
	
	unsigned int hash_arr[7];
	get_hashes(hash_arr, str);
	
	int i, check = 0;
	for(i=0; i<7; ++i){
		check += test_bit(hash_arr[i]);
	}
	
	if(check == 7){
		printf("xx Username already taken: %s\n", str);
	}	
	else{
		printf("Username available: %s\n", str);
	}
	
}

void set_bit(int k){

// for implementation details, refer to:
// http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html

	int i = k/32;
	int pos = k%32;
	
	unsigned int bit = 1;
	bit = bit << pos;
	bit_arr[i] = bit_arr[i] | bit;	
	
}

int test_bit(int k){
	
	int i = k/32;
	int pos = k%32;
	unsigned int bit = 1;
	bit = bit << pos;
	
	if(bit_arr[i] & bit){
		return 1;
	}
	else
		return -1;
		
}

void get_hashes(unsigned int hash[], char *in)
{
	unsigned char *str = (unsigned char *)in;
	int pos = strlen(in);
	hash[0] = RSHash  (str);
	hash[1] = DJBHash (str);
	hash[2] = FNVHash (str);
	hash[3] = JSHash  (str);
	hash[4] = PJWHash (str);
	hash[5] = SDBMHash(str);
	hash[6] = DEKHash (str);
}


//HASH FUNCTIONS

unsigned int RSHash(unsigned char *str)
{
   unsigned int len  = strlen(str);	
   unsigned int b    = 378551;
   unsigned int a    = 63689;
   unsigned int hash = 0;
   unsigned int i    = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash = hash * a + (*str);
      a    = a * b;
   }

   return hash%100;
}

unsigned int JSHash(unsigned char *str)
{
   unsigned int len  = strlen(str);	
   unsigned int hash = 1315423911;
   unsigned int i    = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash ^= ((hash << 5) + (*str) + (hash >> 2));
   }

   return hash%100;
}

unsigned int PJWHash(unsigned char *str)
{
   unsigned int len  = strlen(str);	
   const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
   const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
   const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
   const unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
   unsigned int hash              = 0;
   unsigned int test              = 0;
   unsigned int i                 = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash = (hash << OneEighth) + (*str);

      if((test = hash & HighBits)  != 0)
      {
         hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
      }
   }

   return hash%100;
}

unsigned int SDBMHash(unsigned char *str)
{
   unsigned int len  = strlen(str);	
   unsigned int hash = 0;
   unsigned int i    = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash = (*str) + (hash << 6) + (hash << 16) - hash;
   }

   return hash%100;
}

unsigned int DJBHash(unsigned char *str)
{
   unsigned int len  = strlen(str);	
   unsigned int hash = 5381;
   unsigned int i    = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash = ((hash << 5) + hash) + (*str);
   }

   return hash%100;
}

unsigned int DEKHash(unsigned char *str)
{
   unsigned int len  = strlen(str);	
   unsigned int hash = len;
   unsigned int i    = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
   }
   return hash%100;
}

unsigned int FNVHash(unsigned char *str)
{
   unsigned int len  = strlen(str);	
   const unsigned int fnv_prime = 0x811C9DC5;
   unsigned int hash      = 0;
   unsigned int i         = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash *= fnv_prime;
      hash ^= (*str);
   }

   return hash%100;
}
