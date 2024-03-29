//   Librairie de fontes de caract�res 8x8 "Standard vga 8x8 font" tir�e de aalib  
//               Created by Acco Labs from project aalib  "ascii art"
// 
// la ligne x (x de 20 � 275) du fichier contient les 8 Octets du caract�re de code asci x-20
//===> font8data contient les 8 octets de fonte des 256 caract�res de la tables ASCII �tendue
//  '_' de code ascii 0x5F = 95       et  'P'  de code ascii 0x50 = 80
//  font8data[95*8+0]=0x00= ........	font8data[80*8+0]=0xfc= 111111..
//  font8data[95*8+1]=0x00= ........	font8data[80*8+0]=0x66= .11..11.
//  font8data[95*8+2]=0x00= ........	font8data[80*8+0]=0x66= .11..11.		 . � la place de 0
//  font8data[95*8+3]=0x00= ........	font8data[80*8+0]=0x7c= .11111..		   pour visibilit�
//  font8data[95*8+4]=0x00= ........	font8data[80*8+0]=0x60= .11.....
//  font8data[95*8+5]=0x00= ........	font8data[80*8+0]=0x60= .11.....
//  font8data[95*8+6]=0x00= ........	font8data[80*8+0]=0xf0= 1111....
//  font8data[95*8+7]=0xFF= 11111111	font8data[80*8+0]=0x00= ........
//===> char * Rotation (char *)  permet de "tourner" la fonte d'un caract�re 
//	  Rotation est d�finie et comment�e apr�s font8data ligne 280

unsigned char font8data[8*256] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7e, 0x81, 0xa5, 0x81, 0xbd, 0x99, 0x81, 0x7e,
    0x7e, 0xff, 0xdb, 0xff, 0xc3, 0xe7, 0xff, 0x7e,
    0x6c, 0xfe, 0xfe, 0xfe, 0x7c, 0x38, 0x10, 0x00,
    0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x10, 0x00,
    0x38, 0x7c, 0x38, 0xfe, 0xfe, 0x92, 0x10, 0x7c,
    0x00, 0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x7c,
    0x00, 0x00, 0x18, 0x3c, 0x3c, 0x18, 0x00, 0x00,
    0xff, 0xff, 0xe7, 0xc3, 0xc3, 0xe7, 0xff, 0xff,
    0x00, 0x3c, 0x66, 0x42, 0x42, 0x66, 0x3c, 0x00,
    0xff, 0xc3, 0x99, 0xbd, 0xbd, 0x99, 0xc3, 0xff,
    0x0f, 0x07, 0x0f, 0x7d, 0xcc, 0xcc, 0xcc, 0x78,
    0x3c, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x7e, 0x18,
    0x3f, 0x33, 0x3f, 0x30, 0x30, 0x70, 0xf0, 0xe0,
    0x7f, 0x63, 0x7f, 0x63, 0x63, 0x67, 0xe6, 0xc0,
    0x99, 0x5a, 0x3c, 0xe7, 0xe7, 0x3c, 0x5a, 0x99,
    0x80, 0xe0, 0xf8, 0xfe, 0xf8, 0xe0, 0x80, 0x00,
    0x02, 0x0e, 0x3e, 0xfe, 0x3e, 0x0e, 0x02, 0x00,
    0x18, 0x3c, 0x7e, 0x18, 0x18, 0x7e, 0x3c, 0x18,
    0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00,
    0x7f, 0xdb, 0xdb, 0x7b, 0x1b, 0x1b, 0x1b, 0x00,
    0x3e, 0x63, 0x38, 0x6c, 0x6c, 0x38, 0x86, 0xfc,
    0x00, 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x7e, 0x00,
    0x18, 0x3c, 0x7e, 0x18, 0x7e, 0x3c, 0x18, 0xff,
    0x18, 0x3c, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x00,
    0x18, 0x18, 0x18, 0x18, 0x7e, 0x3c, 0x18, 0x00,
    0x00, 0x18, 0x0c, 0xfe, 0x0c, 0x18, 0x00, 0x00,
    0x00, 0x30, 0x60, 0xfe, 0x60, 0x30, 0x00, 0x00,
    0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xfe, 0x00, 0x00,
    0x00, 0x24, 0x66, 0xff, 0x66, 0x24, 0x00, 0x00,
    0x00, 0x18, 0x3c, 0x7e, 0xff, 0xff, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x7e, 0x3c, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x3c, 0x3c, 0x18, 0x18, 0x00, 0x18, 0x00,	//'!'
    0x6c, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x6c, 0x6c, 0xfe, 0x6c, 0xfe, 0x6c, 0x6c, 0x00,
    0x18, 0x7e, 0xc0, 0x7c, 0x06, 0xfc, 0x18, 0x00,
    0x00, 0xc6, 0xcc, 0x18, 0x30, 0x66, 0xc6, 0x00,
    0x38, 0x6c, 0x38, 0x76, 0xdc, 0xcc, 0x76, 0x00,
    0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00,
    0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00,
    0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00,
    0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30,
    0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,
    0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00,
    0x7c, 0xce, 0xde, 0xf6, 0xe6, 0xc6, 0x7c, 0x00,
    0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x00,
    0x78, 0xcc, 0x0c, 0x38, 0x60, 0xcc, 0xfc, 0x00,
    0x78, 0xcc, 0x0c, 0x38, 0x0c, 0xcc, 0x78, 0x00,
    0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x1e, 0x00,
    0xfc, 0xc0, 0xf8, 0x0c, 0x0c, 0xcc, 0x78, 0x00,
    0x38, 0x60, 0xc0, 0xf8, 0xcc, 0xcc, 0x78, 0x00,
    0xfc, 0xcc, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x00,
    0x78, 0xcc, 0xcc, 0x78, 0xcc, 0xcc, 0x78, 0x00,
    0x78, 0xcc, 0xcc, 0x7c, 0x0c, 0x18, 0x70, 0x00,
    0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00,
    0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30,
    0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x00,
    0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00, 0x00, //'='
    0x60, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x60, 0x00,
    0x3c, 0x66, 0x0c, 0x18, 0x18, 0x00, 0x18, 0x00,
    0x7c, 0xc6, 0xde, 0xde, 0xdc, 0xc0, 0x7c, 0x00,
    0x30, 0x78, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0x00,	//'A'
    0xfc, 0x66, 0x66, 0x7c, 0x66, 0x66, 0xfc, 0x00,
    0x3c, 0x66, 0xc0, 0xc0, 0xc0, 0x66, 0x3c, 0x00,
    0xf8, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00,
    0xfe, 0x62, 0x68, 0x78, 0x68, 0x62, 0xfe, 0x00,
    0xfe, 0x62, 0x68, 0x78, 0x68, 0x60, 0xf0, 0x00,
    0x3c, 0x66, 0xc0, 0xc0, 0xce, 0x66, 0x3a, 0x00,
    0xcc, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0xcc, 0x00,
    0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x1e, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78, 0x00,
    0xe6, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0xe6, 0x00,
    0xf0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xfe, 0x00,
    0xc6, 0xee, 0xfe, 0xfe, 0xd6, 0xc6, 0xc6, 0x00,
    0xc6, 0xe6, 0xf6, 0xde, 0xce, 0xc6, 0xc6, 0x00,
    0x38, 0x6c, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x00,
    0xfc, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0, 0x00, //'P'
    0x7c, 0xc6, 0xc6, 0xc6, 0xd6, 0x7c, 0x0e, 0x00,
    0xfc, 0x66, 0x66, 0x7c, 0x6c, 0x66, 0xe6, 0x00,
    0x7c, 0xc6, 0xe0, 0x78, 0x0e, 0xc6, 0x7c, 0x00,
    0xfc, 0xb4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
    0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfc, 0x00,
    0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00,
    0xc6, 0xc6, 0xc6, 0xc6, 0xd6, 0xfe, 0x6c, 0x00,
    0xc6, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0xc6, 0x00,
    0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x30, 0x78, 0x00,
    0xfe, 0xc6, 0x8c, 0x18, 0x32, 0x66, 0xfe, 0x00,
    0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00,
    0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x02, 0x00,
    0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00,
    0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,	//'_'
    0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00,	//'a'
    0xe0, 0x60, 0x60, 0x7c, 0x66, 0x66, 0xdc, 0x00,
    0x00, 0x00, 0x78, 0xcc, 0xc0, 0xcc, 0x78, 0x00,
    0x1c, 0x0c, 0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00,
    0x00, 0x00, 0x78, 0xcc, 0xfc, 0xc0, 0x78, 0x00,
    0x38, 0x6c, 0x64, 0xf0, 0x60, 0x60, 0xf0, 0x00,
    0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8,
    0xe0, 0x60, 0x6c, 0x76, 0x66, 0x66, 0xe6, 0x00,
    0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x0c, 0x00, 0x1c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78,
    0xe0, 0x60, 0x66, 0x6c, 0x78, 0x6c, 0xe6, 0x00,
    0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x00, 0x00, 0xcc, 0xfe, 0xfe, 0xd6, 0xd6, 0x00,
    0x00, 0x00, 0xb8, 0xcc, 0xcc, 0xcc, 0xcc, 0x00,
    0x00, 0x00, 0x78, 0xcc, 0xcc, 0xcc, 0x78, 0x00,
    0x00, 0x00, 0xdc, 0x66, 0x66, 0x7c, 0x60, 0xf0,
    0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0x1e,
    0x00, 0x00, 0xdc, 0x76, 0x62, 0x60, 0xf0, 0x00,
    0x00, 0x00, 0x7c, 0xc0, 0x70, 0x1c, 0xf8, 0x00,
    0x10, 0x30, 0xfc, 0x30, 0x30, 0x34, 0x18, 0x00,
    0x00, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00,
    0x00, 0x00, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00,
    0x00, 0x00, 0xc6, 0xc6, 0xd6, 0xfe, 0x6c, 0x00,
    0x00, 0x00, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0x00,
    0x00, 0x00, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8,
    0x00, 0x00, 0xfc, 0x98, 0x30, 0x64, 0xfc, 0x00,	 //'z'
    0x1c, 0x30, 0x30, 0xe0, 0x30, 0x30, 0x1c, 0x00,
    0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00,
    0xe0, 0x30, 0x30, 0x1c, 0x30, 0x30, 0xe0, 0x00,
    0x76, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe, 0x00,
    0x7c, 0xc6, 0xc0, 0xc6, 0x7c, 0x0c, 0x06, 0x7c,
    0x00, 0xcc, 0x00, 0xcc, 0xcc, 0xcc, 0x76, 0x00,
    0x1c, 0x00, 0x78, 0xcc, 0xfc, 0xc0, 0x78, 0x00,
    0x7e, 0x81, 0x3c, 0x06, 0x3e, 0x66, 0x3b, 0x00,
    0xcc, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00,
    0xe0, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00,
    0x30, 0x30, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00,
    0x00, 0x00, 0x7c, 0xc6, 0xc0, 0x78, 0x0c, 0x38,
    0x7e, 0x81, 0x3c, 0x66, 0x7e, 0x60, 0x3c, 0x00,
    0xcc, 0x00, 0x78, 0xcc, 0xfc, 0xc0, 0x78, 0x00,
    0xe0, 0x00, 0x78, 0xcc, 0xfc, 0xc0, 0x78, 0x00,
    0xcc, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x7c, 0x82, 0x38, 0x18, 0x18, 0x18, 0x3c, 0x00,
    0xe0, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
    0xc6, 0x10, 0x7c, 0xc6, 0xfe, 0xc6, 0xc6, 0x00,
    0x30, 0x30, 0x00, 0x78, 0xcc, 0xfc, 0xcc, 0x00,
    0x1c, 0x00, 0xfc, 0x60, 0x78, 0x60, 0xfc, 0x00,
    0x00, 0x00, 0x7f, 0x0c, 0x7f, 0xcc, 0x7f, 0x00,
    0x3e, 0x6c, 0xcc, 0xfe, 0xcc, 0xcc, 0xce, 0x00,
    0x78, 0x84, 0x00, 0x78, 0xcc, 0xcc, 0x78, 0x00,
    0x00, 0xcc, 0x00, 0x78, 0xcc, 0xcc, 0x78, 0x00,
    0x00, 0xe0, 0x00, 0x78, 0xcc, 0xcc, 0x78, 0x00,
    0x78, 0x84, 0x00, 0xcc, 0xcc, 0xcc, 0x76, 0x00,
    0x00, 0xe0, 0x00, 0xcc, 0xcc, 0xcc, 0x76, 0x00,
    0x00, 0xcc, 0x00, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8,
    0xc3, 0x18, 0x3c, 0x66, 0x66, 0x3c, 0x18, 0x00,
    0xcc, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0x78, 0x00,
    0x18, 0x18, 0x7e, 0xc0, 0xc0, 0x7e, 0x18, 0x18,
    0x38, 0x6c, 0x64, 0xf0, 0x60, 0xe6, 0xfc, 0x00,
    0xcc, 0xcc, 0x78, 0x30, 0xfc, 0x30, 0xfc, 0x30,
    0xf8, 0xcc, 0xcc, 0xfa, 0xc6, 0xcf, 0xc6, 0xc3,
    0x0e, 0x1b, 0x18, 0x3c, 0x18, 0x18, 0xd8, 0x70,
    0x1c, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00,
    0x38, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x00, 0x1c, 0x00, 0x78, 0xcc, 0xcc, 0x78, 0x00,
    0x00, 0x1c, 0x00, 0xcc, 0xcc, 0xcc, 0x76, 0x00,
    0x00, 0xf8, 0x00, 0xb8, 0xcc, 0xcc, 0xcc, 0x00,
    0xfc, 0x00, 0xcc, 0xec, 0xfc, 0xdc, 0xcc, 0x00,
    0x3c, 0x6c, 0x6c, 0x3e, 0x00, 0x7e, 0x00, 0x00,
    0x38, 0x6c, 0x6c, 0x38, 0x00, 0x7c, 0x00, 0x00,
    0x18, 0x00, 0x18, 0x18, 0x30, 0x66, 0x3c, 0x00,
    0x00, 0x00, 0x00, 0xfc, 0xc0, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xfc, 0x0c, 0x0c, 0x00, 0x00,
    0xc6, 0xcc, 0xd8, 0x36, 0x6b, 0xc2, 0x84, 0x0f,
    0xc3, 0xc6, 0xcc, 0xdb, 0x37, 0x6d, 0xcf, 0x03,
    0x18, 0x00, 0x18, 0x18, 0x3c, 0x3c, 0x18, 0x00,
    0x00, 0x33, 0x66, 0xcc, 0x66, 0x33, 0x00, 0x00,
    0x00, 0xcc, 0x66, 0x33, 0x66, 0xcc, 0x00, 0x00,
    0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88,
    0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa,
    0xdb, 0xf6, 0xdb, 0x6f, 0xdb, 0x7e, 0xd7, 0xed,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0xf8, 0x18, 0x18, 0x18,
    0x18, 0x18, 0xf8, 0x18, 0xf8, 0x18, 0x18, 0x18,
    0x36, 0x36, 0x36, 0x36, 0xf6, 0x36, 0x36, 0x36,
    0x00, 0x00, 0x00, 0x00, 0xfe, 0x36, 0x36, 0x36,
    0x00, 0x00, 0xf8, 0x18, 0xf8, 0x18, 0x18, 0x18,
    0x36, 0x36, 0xf6, 0x06, 0xf6, 0x36, 0x36, 0x36,
    0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
    0x00, 0x00, 0xfe, 0x06, 0xf6, 0x36, 0x36, 0x36,
    0x36, 0x36, 0xf6, 0x06, 0xfe, 0x00, 0x00, 0x00,
    0x36, 0x36, 0x36, 0x36, 0xfe, 0x00, 0x00, 0x00,
    0x18, 0x18, 0xf8, 0x18, 0xf8, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf8, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x1f, 0x00, 0x00, 0x00,
    0x18, 0x18, 0x18, 0x18, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x1f, 0x18, 0x18, 0x18,
    0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00,
    0x18, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x1f, 0x18, 0x1f, 0x18, 0x18, 0x18,
    0x36, 0x36, 0x36, 0x36, 0x37, 0x36, 0x36, 0x36,
    0x36, 0x36, 0x37, 0x30, 0x3f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3f, 0x30, 0x37, 0x36, 0x36, 0x36,
    0x36, 0x36, 0xf7, 0x00, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0x00, 0xf7, 0x36, 0x36, 0x36,
    0x36, 0x36, 0x37, 0x30, 0x37, 0x36, 0x36, 0x36,
    0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00,
    0x36, 0x36, 0xf7, 0x00, 0xf7, 0x36, 0x36, 0x36,
    0x18, 0x18, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00,
    0x36, 0x36, 0x36, 0x36, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0x00, 0xff, 0x18, 0x18, 0x18,
    0x00, 0x00, 0x00, 0x00, 0xff, 0x36, 0x36, 0x36,
    0x36, 0x36, 0x36, 0x36, 0x3f, 0x00, 0x00, 0x00,
    0x18, 0x18, 0x1f, 0x18, 0x1f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1f, 0x18, 0x1f, 0x18, 0x18, 0x18,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x36, 0x36, 0x36,
    0x36, 0x36, 0x36, 0x36, 0xff, 0x36, 0x36, 0x36,
    0x18, 0x18, 0xff, 0x18, 0xff, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0xf8, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x1f, 0x18, 0x18, 0x18,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0,
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x76, 0xdc, 0xc8, 0xdc, 0x76, 0x00,
    0x00, 0x78, 0xcc, 0xf8, 0xcc, 0xf8, 0xc0, 0xc0,
    0x00, 0xfc, 0xcc, 0xc0, 0xc0, 0xc0, 0xc0, 0x00,
    0x00, 0x00, 0xfe, 0x6c, 0x6c, 0x6c, 0x6c, 0x00,
    0xfc, 0xcc, 0x60, 0x30, 0x60, 0xcc, 0xfc, 0x00,
    0x00, 0x00, 0x7e, 0xd8, 0xd8, 0xd8, 0x70, 0x00,
    0x00, 0x66, 0x66, 0x66, 0x66, 0x7c, 0x60, 0xc0,
    0x00, 0x76, 0xdc, 0x18, 0x18, 0x18, 0x18, 0x00,
    0xfc, 0x30, 0x78, 0xcc, 0xcc, 0x78, 0x30, 0xfc,
    0x38, 0x6c, 0xc6, 0xfe, 0xc6, 0x6c, 0x38, 0x00,
    0x38, 0x6c, 0xc6, 0xc6, 0x6c, 0x6c, 0xee, 0x00,
    0x1c, 0x30, 0x18, 0x7c, 0xcc, 0xcc, 0x78, 0x00,
    0x00, 0x00, 0x7e, 0xdb, 0xdb, 0x7e, 0x00, 0x00,
    0x06, 0x0c, 0x7e, 0xdb, 0xdb, 0x7e, 0x60, 0xc0,
    0x38, 0x60, 0xc0, 0xf8, 0xc0, 0x60, 0x38, 0x00,
    0x78, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x00,
    0x00, 0x7e, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00,
    0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x7e, 0x00,
    0x60, 0x30, 0x18, 0x30, 0x60, 0x00, 0xfc, 0x00,
    0x18, 0x30, 0x60, 0x30, 0x18, 0x00, 0xfc, 0x00,
    0x0e, 0x1b, 0x1b, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0xd8, 0xd8, 0x70,
    0x18, 0x18, 0x00, 0x7e, 0x00, 0x18, 0x18, 0x00,
    0x00, 0x76, 0xdc, 0x00, 0x76, 0xdc, 0x00, 0x00,
    0x38, 0x6c, 0x6c, 0x38, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
    0x0f, 0x0c, 0x0c, 0x0c, 0xec, 0x6c, 0x3c, 0x1c,
    0x58, 0x6c, 0x6c, 0x6c, 0x6c, 0x00, 0x00, 0x00,
    0x70, 0x98, 0x30, 0x60, 0xf8, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};



// char * Rotation (char * in)
//________
//	Permet de tourner les octest de fonte d'un caract�re
//  par exemple les caract�re '_' et 'P' ont  pour fonte
//   0x00 	........	        0xfc   111111..
//   0x00 	........			0x66   .11..11.
//   0x00 	........			0x66   .11..11.		 . � la place de 0
//   0x00 	........			0x7c   .11111..		   pour visibilit�
//   0x00 	........			0x60   .11.....
//   0x00 	........			0x60   .11.....
//   0x00 	........			0xf0   1111....
//   0xFF 	11111111			0x00   ........
// 
//  Une fois tourn� les fontes deviennent
//   
//   0x80 	1.......	        0x41   .1.....1
//   0x80 	1.......	        0x7f   .1111111
//   0x80 	1.......	        0x7f   .1111111
//   0x80 	1.......	        0x49   .1..1..1
//   0x80 	1.......	        0x09   ....1..1
//   0x80 	1.......	        0x0f   ....1111
//   0x80 	1.......	        0x06   .....11.
//   0x80 	1.......	        0x00   ........

//_______
//  <==  in : pointe au d�but des 8 octets de fontes d'un caract�re
//  ==> renvoie un pointeur sur 8 octets d'espace m�moire contenant 
//       la fonte du caract�re "tourn�"
//_______

static char riote[8] = "T vierge";	// Espace m�moire r�serv� pour la fonte tourn�e.
									// Static permet ici de ne pas rendre public l'�tiquette
									// "riote"	=> variable persistante, globale au fichier mais de type priv�e
char *  Rotation (char * in)
{
	char i,j,val;

	for (i=0;i<8;i++)
	{ // i indice de l'octet  
		val = 0;
		for (j=0;j<8;j++)
		{
			if ((*(in+j))&(1<<i)) val |= (1<<(j));
		}
	 	riote[7-i] = val;
	}
  return riote;	
}
