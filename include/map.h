#pragma once

#include <SFML/Graphics.hpp>
inline constexpr int HEIGHT_MAP = 25;// Map height in tiles
inline constexpr int WIDTH_MAP = 80;// Map width in tiles


inline const sf::String InitialTileMap[HEIGHT_MAP] = {



         "                                                                                 ",
         "                                                                                 ",
         "                                                                                 ",
         "                                                                                 ",
         "                                                  cdddddddddddddddddddddddddddddd",
         "                                                  c34        Y   Y           12c",
         "                                                  c          y   y             c",
         "                                                  c                  zx        c",
         "                                                  c                  nv        c",
         "                                                  [                            c",
         "dddddddddddddddddddddddddddddddddb            u  !]                 l     H    c",
         "b34                            12b    u       u aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
         "b                                b    u       u          R1234 U              c ",
         "b       zx              zx       b    u       u          U     R              c ",
         "b       nv              nv       b    u       u          R     U              c ",
         "b                                {    u       u          U     R              c ",
         "b H r     MLW    MLW   MLW       };   u   k   p          R     U              c ",
         "aaaaaaaaaiiiiiiiiiiiiiiiiiiaaaaaaaaaa u  MLLW            U     R              c ",
         "b34      aaaaaaaaaaaaaaaaaa           u                  R     U              c ",
         "b             Y      Y                u              zx  U  zx R  zx          c ",
         "b             y      y                u              nv  R  nv T  nv          c ",
         "b                                     u                  U                    c ",
         "b                                     u                  T                    ( ",
         "b                 >                   p              <<<<<<<<<<<<<<<         :)s",
         "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",


};

inline sf::String TileMap[HEIGHT_MAP];

inline void resetTileMap() {
    for (int i = 0; i < HEIGHT_MAP; ++i) {
        TileMap[i] = InitialTileMap[i];
    }
}



