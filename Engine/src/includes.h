/*! \mainpage PolyMT Engine
 * \section intro Wstêp
 
 Dokumentacja opisuj¹ca silnik stanowi¹cy podstawê pracy "Tworzenie gier wideo z wykorzystaniem rzeczywistoœci mieszanej" autorstwa Micha³a Mizery, promotor: dr in¿. Tomasz Martyn.

 Dodatkowo kod silnika jest obiektem pracy zaliczeniowej dla przedmiotu "Programowanie Gier Komputerowych".
 
 * \section features Modu³y

 Silnik sk³ada siê z nastêpuj¹cych modu³ów:

 \li  Menad¿er plików konfiguracyjncyh
 \li  Menad¿er czcionek
 \li  Menad¿er tekstur
 \li  Menad¿er dŸwiêków
 \li Mena¿er systemów efektów cz¹steczkowych
 \li Obs³uga stosu stanów gry ( wraz z przechodzeniem pomiêdzy nimi )

 Dodatkowo silnik implementuje szerek klas pomocniczych, takich jak:

 \li Sprite2D
 \li Czcionki
 \li System efektów cz¹steczkowych
 \li Kolor
 \li Licznik zaawansowania animacji
 \li Tekstura
 \li Wektor
 \li Singleton

 itd.

 * \section libs Wykorzystywane biblioteki
 
 Silnik wykorzystuje natêpuj¹ce biblioteki:

 \li FTGL
 \li SDL
 \li SDL_Image
 \li JSONCpp
 \li FMOD
 
 * \section author Autor

 Autorem silnika, oraz niniejszej dokumentacji jest student Wydzia³u Elektroniki i Technik Informacyjncyh Politechniki Warszawskiej Micha³ Mizera.

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





