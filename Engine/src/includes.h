/*! \mainpage PolyMT Engine
 * \section intro Wst�p
 
 Dokumentacja opisuj�ca silnik stanowi�cy podstaw� pracy "Tworzenie gier wideo z wykorzystaniem rzeczywisto�ci mieszanej" autorstwa Micha�a Mizery, promotor: dr in�. Tomasz Martyn.

 Dodatkowo kod silnika jest obiektem pracy zaliczeniowej dla przedmiotu "Programowanie Gier Komputerowych".
 
 * \section features Modu�y

 Silnik sk�ada si� z nast�puj�cych modu��w:

 \li  Menad�er plik�w konfiguracyjncyh
 \li  Menad�er czcionek
 \li  Menad�er tekstur
 \li  Menad�er d�wi�k�w
 \li Mena�er system�w efekt�w cz�steczkowych
 \li Obs�uga stosu stan�w gry ( wraz z przechodzeniem pomi�dzy nimi )

 Dodatkowo silnik implementuje szerek klas pomocniczych, takich jak:

 \li Sprite2D
 \li Czcionki
 \li System efekt�w cz�steczkowych
 \li Kolor
 \li Licznik zaawansowania animacji
 \li Tekstura
 \li Wektor
 \li Singleton

 itd.

 * \section libs Wykorzystywane biblioteki
 
 Silnik wykorzystuje nat�puj�ce biblioteki:

 \li FTGL
 \li SDL
 \li SDL_Image
 \li JSONCpp
 \li FMOD
 
 * \section author Autor

 Autorem silnika, oraz niniejszej dokumentacji jest student Wydzia�u Elektroniki i Technik Informacyjncyh Politechniki Warszawskiej Micha� Mizera.

 */

#pragma once

#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>

#define AX_PRINT_ERROR(msg) fprintf(stderr, "%s", msg);
#define AX_PRINT_ERROR_FORMAT(format, msg) fprintf(stderr, format, msg);
#define AX_PRINT(msg) fprintf(stdout, "%s", msg);
#define AX_PRINT_FORMAT(format, msg) fprintf(stdout, format, msg);

#define SCREEN_WIDTH 177.0f
#define SCREEN_HEIGHT 100.0f

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long int s64;
typedef unsigned long long int u64;





