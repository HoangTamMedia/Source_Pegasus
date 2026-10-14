#pragma once

class MD5 {

public:

bool MD5_EncodeKeyVal	(
	  char * lpszInputStr,
	  char * lpszOutputKeyVal,
	  int iKeyIndex
	  );

bool MD5_EncodeString	(
	  char * lpszInputStr,
	  char * lpszOutputStr,
	  int iKeyIndex
	  );

bool MD5_CheckValue	(

	  char * lpszInputStr,
	  char * szKeyVal,
	  int iKeyIndex
	  );

  MD5              ();
  void  update     (unsigned char *input, unsigned int input_length);
  void  update     (std::istream& stream);
  void  update     (FILE *file);
  void  update     (std::ifstream& stream);
  void  finalize   ();

  MD5              (unsigned char *string);
  MD5              (std::istream& stream);
  MD5              (FILE *file);
  MD5              (std::ifstream& stream);

  unsigned char    *raw_digest ();
  char *            hex_digest ();
  friend std::ostream&   operator<< (std::ostream&, MD5 context);

private:

  void setmagicnum (int keyindex);

	unsigned char	m_cRaw_digest[16];
	char			m_cHex_digest[33];

  typedef unsigned       int uint4;
  typedef unsigned short int uint2;
  typedef unsigned      char uint1;

  uint4 state[4];
  uint4 count[2];
  uint1 buffer[64];
  uint1 digest[16];
  uint1 finalized;

  void init             ();
  void transform        (uint1 *buffer);

  static void encode    (uint1 *dest, uint4 *src, uint4 length);
  static void decode    (uint4 *dest, uint1 *src, uint4 length);
  static void memcpy    (uint1 *dest, uint1 *src, uint4 length);
  static void memset    (uint1 *start, uint1 val, uint4 length);

  static inline uint4  rotate_left (uint4 x, uint4 n);
  static inline uint4  F           (uint4 x, uint4 y, uint4 z);
  static inline uint4  G           (uint4 x, uint4 y, uint4 z);
  static inline uint4  H           (uint4 x, uint4 y, uint4 z);
  static inline uint4  I           (uint4 x, uint4 y, uint4 z);
  static inline void   FF  (uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,
			    uint4 s, uint4 ac);
  static inline void   GG  (uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,
			    uint4 s, uint4 ac);
  static inline void   HH  (uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,
			    uint4 s, uint4 ac);
  static inline void   II  (uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,
			    uint4 s, uint4 ac);

};

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21
