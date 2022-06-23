/*
 * Copyright (C) 2020 Charles R. Liebling
 *
 * convolve.cpp
 * Draws a Mandelbrot set that moves in response to the mouse cursor.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */ 
#include <iostream>
#include <complex>
#include <cstdint>
#include <cstdlib>
#include <random>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_image.h>
#include <string>
#define MAXWIDTH 2560
#define MAXHEIGHT 1440
#define MAXRADIUS 20.0
#define WIDTH 640 
#define HEIGHT 480

int32_t convolve2d(SDL_Surface *src,
		   SDL_Surface *dst,
		   std::vector<int32_t> kernel,
		   uint32_t kernX,
		   uint32_t kernY)
{
	int kernelXMidpt;
	int kernelYMidpt;
	int32_t accumulatorR;
	int32_t accumulatorG;
	int32_t accumulatorB;
	int32_t accumulatorA;
	uint32_t frameX;
	uint32_t frameY;
	uint8_t bytesPerPixel;
	uint32_t *frame;
	uint32_t *convolutedFrame;	
	if(src == NULL || dst == NULL){
		return -1;
	}

	bytesPerPixel = src->format->BytesPerPixel;

	frameX = src->w;
	frameY = src->h;

	kernelXMidpt = kernX / 2;
	kernelYMidpt = kernY / 2;
	
	frame = (uint32_t*)src->pixels;
	convolutedFrame = (uint32_t*)dst->pixels;

	for(int y = 0; y < frameY; y++){
		for(int x = 0; x < frameX; x++){
			accumulatorR = 0;
			accumulatorG = 0;
			accumulatorB = 0;
			accumulatorA = 0;

			for(int ky = -kernelYMidpt; ky < kernelYMidpt; ky++){
				for(int kx = -kernelXMidpt; kx < kernelXMidpt; kx++){
					if((x + kx >= 0) && (y + ky >= 0)
					   && (x + kx < frameX) && (y + ky < frameY)){
						accumulatorB += (int32_t)((frame[y * frameX + x] >> 16) & 0xFF)
							    * kernel[(ky + kernelYMidpt) * kernX + (kx + kernelXMidpt)];
						accumulatorG += (int32_t)((frame[y * frameX + x] >> 8) & 0xFF)
							    * kernel[(ky + kernelYMidpt) * kernX + (kx + kernelXMidpt)];
						accumulatorR += (int32_t)(frame[y * frameX + x] & 0xFF)
							    * kernel[(ky + kernelYMidpt) * kernX + (kx + kernelXMidpt)];
					}
				}
			}
			convolutedFrame[y * frameX + x] = (uint32_t)((accumulatorB << 16) | (accumulatorG << 8) | (accumulatorR & 0xFF));
		}
	}
	return 0;
}

int main(int argc, char **argv)
{       
        uint32_t i;
        int32_t error;
        int32_t width, height, tmpval;
        int32_t mx, my, mxp, myp;
        int32_t opt;
        bool change, quit;
        SDL_Surface *convolutedImage;
        SDL_Surface *image;
        SDL_Texture *imageTex;
        SDL_Window *window;
        SDL_Renderer *renderer;
        std::string fileName;
        std::vector<uint32_t> frame;
        std::vector<uint32_t> convolutedFrame;
        std::vector<int32_t> kernel = {1, 0, -1, 0, 0, 0, -1, 0, 1};
        //std::vector<int32_t> kernel = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
        //std::vector<int32_t> kernel = {0, 0, 0, 0, 1, 0, 0, 0, 0};
        width = WIDTH;
        height = HEIGHT;
        while((opt = getopt(argc, argv, "f:")) != -1) {
                switch (opt) {
                case 'f':
                        fileName = optarg;
                        break;
                default:
                        break;
                }
        }
        error = SDL_Init(SDL_INIT_VIDEO);
        if(error != 0){
                std::cerr<<"Could not initialize video, SDL_Init returned ";
                std::cerr<<error<<std::endl;
                return error;
        }
	error = IMG_Init(IMG_INIT_PNG);
	if(error == 0){
		std::cerr<<"Could not initialize image library.\n";
		SDL_Quit();
		return -1;
	}
	
	image = IMG_Load(fileName.c_str());
        error = SDL_CreateWindowAndRenderer(image->w, image->h, SDL_WINDOW_RESIZABLE,
                                            &window, &renderer);
	if(error != 0){
                std::cerr<<"Error: SDL_CreateWindowAndRenderer returned ";
                std::cerr<<error<<std::endl;
                SDL_Quit();
                return error;
        }

	//imageTex = SDL_CreateTextureFromSurface(renderer, image);
        SDL_RenderClear(renderer);
        //SDL_RenderCopy(renderer, imageTex, NULL, NULL);
        //SDL_RenderPresent(renderer);
	quit = false;
	std::cout<<"Width: "<<image->w<<std::endl;
	std::cout<<"Height: "<<image->h<<std::endl;
	std::cout<<"Bytes/pixel: "<<image->format->BitsPerPixel<<std::endl;
	std::cout<<"Pitch: "<<image->pitch<<std::endl;
	std::cout<<"R mask: "<<std::hex<<image->format->Rmask<<std::endl;
	std::cout<<"G mask: "<<std::hex<<image->format->Gmask<<std::endl;
	std::cout<<"B mask: "<<std::hex<<image->format->Bmask<<std::endl;

	convolutedImage = SDL_CreateRGBSurface(0, image->w, image->h, 32,
			                       0xFF, 0xFF00, 0xFF0000, 0x00);

	
	convolve2d(image, convolutedImage, kernel,3, 3);
	imageTex = SDL_CreateTextureFromSurface(renderer, convolutedImage);
        SDL_RenderCopy(renderer, imageTex, NULL, NULL);
	SDL_RenderPresent(renderer);

        while(!quit){
                SDL_PumpEvents();
                if(SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT)){
                        quit = true;
                }                    
        }
	SDL_FreeSurface(image);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}
