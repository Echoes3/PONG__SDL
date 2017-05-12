#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 780
#define RAQUE_WIDTH 10
#define RAQUE_HEIGHT 10

#define rayon 36

typedef struct game{


     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_ptexture;
     SDL_Surface *g_psurface;


}game;

typedef struct gameState{

    int g_bRunning;


}gameState;

typedef struct coordonnees{

    double x;
    double y;

}coordonnees;

//prototypes
int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame);
void destroy(game *myGame);
void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2,coordonnees *balle,coordonnees *mvt);
void delay(unsigned int frameLimit);

void drawInSurface(game *myGame);
void Terrain(game *myGame,coordonnees *dep,coordonnees *dep2,coordonnees *balle );




int main(int argc, char *argv[])
{


     game myGame;
     gameState state;

     unsigned int frameLimit = SDL_GetTicks() + 16;
     coordonnees dep;
     coordonnees dep2;
     coordonnees balle;
     coordonnees mvt;
     mvt.x=1;
     mvt.y=1;

     int choix=0;

     balle.x=SCREEN_WIDTH/2;

     balle.y=SCREEN_HEIGHT/2;



     state.g_bRunning=1;




        //Create texture for drawing in texture or load picture
        myGame.g_ptexture=SDL_CreateTexture(myGame.g_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);




        while(state.g_bRunning){



                if(choix==0){
                    printf("1 Commencer le pong\n");

                    scanf("%i",&choix);


                }
                switch (choix){


                        case 1:init("pong",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame);
                                while(state.g_bRunning){

                                    handleEvents(&state,&dep,&dep2,&balle,&mvt);
                                    Terrain(&myGame,&dep,&dep2,&balle);
                                    background(&myGame);
                                }

                                 break;
                        default:break;


                }

                 system ("cls");


             // Gestion des 60 fps (1000ms/60 = 16.6 -> 16
            delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16;



        }

        destroy(&myGame);

        SDL_Quit();


    return 0;
}


int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame){


    myGame->g_pWindow=NULL;
    myGame->g_pRenderer=NULL;
    myGame->g_psurface=NULL;
    myGame->g_ptexture=NULL;


    //initialize SDL

    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {
            //if succeeded create our window
            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
            //if succeeded create window, create our render
            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_PRESENTVSYNC);


            }


    }else{


        return 0;
    }



    return 1;



}

void background(game *myGame){

        SDL_SetRenderDrawColor(myGame->g_pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(myGame->g_pRenderer);

        SDL_SetRenderDrawColor(myGame->g_pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_Rect ligne;
        SDL_Rect ligne1;
        SDL_Rect ligne2;

        ligne.x=SCREEN_WIDTH/2;//debut x
        ligne.y=0;//debut y
        ligne.w=5; //Largeur
        ligne.h=220; //Hauteur

        ligne1.x=SCREEN_WIDTH/2;
        ligne1.y=260;
        ligne1.w=5;
        ligne1.h=220;

         ligne2.x=SCREEN_WIDTH/2;
        ligne2.y=520;
        ligne2.w=5;
        ligne2.h=220;


        //rectangle plein
        SDL_RenderFillRect(myGame->g_pRenderer,&ligne);
        SDL_RenderFillRect(myGame->g_pRenderer,&ligne1);
        SDL_RenderFillRect(myGame->g_pRenderer,&ligne2);






}


void Terrain(game *myGame,coordonnees *dep,coordonnees *dep2,coordonnees *balle ) {



    int i;
    SDL_Point cercle;

    for(i=1;i<=360;i++){

        cercle.x=rayon*cos(i);
        cercle.y=rayon*sin(i);
        SDL_SetRenderDrawColor(myGame->g_pRenderer,180,2,126,255);
         SDL_RenderDrawPoint(myGame->g_pRenderer,cercle.x+balle->x,cercle.y+balle->y);

    }





       //Définition de la raquette j 1 a dessiner
        SDL_Rect raquettej1;
        raquettej1.x=0;//debut x
        raquettej1.y=dep->y;//debut y
        raquettej1.w=10; //Largeur
        raquettej1.h=100; //Hauteur


        //Définition du rectangle 2 a dessiner
        SDL_Rect raquettej2;
        raquettej2.x=SCREEN_WIDTH-10;//debut x
        raquettej2.y=dep2->y;//debut y
        raquettej2.w=10; //Largeur
        raquettej2.h=100; //Hauteur

     /*   SDL_SetRenderDrawColor(myGame->g_pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(myGame->g_pRenderer, 400, 30, 400, 30);*/



        //Draw in texture
        SDL_SetRenderDrawColor(myGame->g_pRenderer,255,0,0,255);
        SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture); //on modifie la texture

        SDL_RenderFillRect(myGame->g_pRenderer, &raquettej1);
        SDL_RenderFillRect(myGame->g_pRenderer, &raquettej2);

        SDL_SetRenderTarget(myGame->g_pRenderer, NULL);// Dorénavent, on modifie à nouveau le renderer

        SDL_Rect position;//position de la texture dans screen
        position.x = 0;
        position.y = 0;
        SDL_QueryTexture(myGame->g_ptexture, NULL, NULL, &position.w, &position.h);
        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&position);


         SDL_RenderPresent(myGame->g_pRenderer);

         SDL_SetRenderDrawColor(myGame->g_pRenderer,0,0,0,255);
         SDL_DestroyTexture(myGame->g_ptexture);
         SDL_RenderClear(myGame->g_pRenderer);




}

void destroy(game *myGame){

      //Destroy render
     if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);

      //Destroy texture
     if(myGame->g_ptexture!=NULL)
        SDL_DestroyTexture(myGame->g_ptexture);

    //Destroy surface
     if(myGame->g_psurface!=NULL)
         SDL_FreeSurface(myGame->g_psurface);

    //Destroy window
    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

}


void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2,coordonnees *balle,coordonnees *mvt){


    SDL_Event event;

    if(balle->y-rayon<=0){
        mvt->y=-2;
    }

    if(balle->y+rayon>=SCREEN_HEIGHT){
        mvt->y=2;
    }

    if(balle->x-rayon==dep->x+RAQUE_WIDTH  && balle->y+rayon<=dep->y+RAQUE_HEIGHT && balle->y-rayon>=dep->y){
        mvt->x=-2;
    }

    if(balle->x+rayon<0){
        balle->x=SCREEN_WIDTH/2;
        balle->y=SCREEN_HEIGHT/2;

    }

    if(balle->x-rayon>SCREEN_WIDTH){
        balle->x=SCREEN_WIDTH/2;
        balle->y=SCREEN_HEIGHT/2;

    }


    if(balle->x+rayon==dep2->x && balle->y+rayon<=dep2->y+RAQUE_HEIGHT && balle->y+rayon>=dep2->y){
        mvt->x=2;
    }

    balle->y-=mvt->y;
    balle->x-=mvt->x;


    int i;
    int j;

    i=dep;
    j=dep2;



    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
              state->g_bRunning=0;break;
        case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                            {

                                case SDLK_z:    dep->y-=10 ; break;
                                case SDLK_s:  dep->y+=10;break;
                                case SDLK_UP: dep2->y-=10;break;
                                case SDLK_DOWN: dep2->y+=10; break;
                            }
                            break;

        case SDL_KEYUP:;break;

        default:break;

        }
    }


}

void delay(unsigned int frameLimit)
{
    // Gestion des 60 fps (images/seconde)
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}
