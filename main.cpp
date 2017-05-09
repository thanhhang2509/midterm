//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <fstream>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 690;
const int rankListMax = 10;

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTransparent = NULL;
SDL_Texture* gTexture = NULL;
SDL_Texture* gKhung = NULL;
SDL_Texture* gText = NULL;
SDL_Texture* gRankList = NULL;
SDL_Texture* gInput = NULL;
TTF_Font* gFont = NULL;
SDL_Surface* gSurface = NULL;
SDL_Texture* gBackground = NULL;
SDL_Texture* gEffectMo = NULL;
SDL_Texture* gMenu_play = NULL;
SDL_Texture* gMenu_setting = NULL;
SDL_Texture* gMenu_guide = NULL;
SDL_Texture* gMenu_rank = NULL;
SDL_Texture* gMenu_quit = NULL;
SDL_Texture* gMenu_play_choose = NULL;
SDL_Texture* gMenu_setting_choose = NULL;
SDL_Texture* gMenu_guide_choose = NULL;
SDL_Texture* gMenu_rank_choose = NULL;
SDL_Texture* gMenu_quit_choose = NULL;
SDL_Texture* gExitGame = NULL;
SDL_Texture* gGameover = NULL;
SDL_Texture* gHeart = NULL;
SDL_Texture* gPause = NULL;

//sounds
Mix_Chunk* gMenu_click = NULL;
Mix_Chunk* gEat = NULL;
Mix_Chunk* gOver = NULL;
Mix_Chunk* gDie = NULL;
Mix_Chunk* gCongratulation = NULL;

//rank
SDL_Texture* gRank_top_1 = NULL;
SDL_Texture* gRank_top_2 = NULL;
SDL_Texture* gRank_top_3 = NULL;

//Snake
SDL_Texture* gDot = NULL;
SDL_Texture* gDot_ngangtrai = NULL;
SDL_Texture* gDot_ngangphai = NULL;
SDL_Texture* gDot_doctren = NULL;
SDL_Texture* gDot_docduoi = NULL;
SDL_Texture* gDot_left_top = NULL;
SDL_Texture* gDot_left_bot = NULL;
SDL_Texture* gDot_right_top = NULL;
SDL_Texture* gDot_right_bot = NULL;
SDL_Texture* gDot_top_left = NULL;
SDL_Texture* gDot_top_right = NULL;
SDL_Texture* gDot_bot_left = NULL;
SDL_Texture* gDot_bot_right = NULL;
SDL_Texture* gDau = NULL;
SDL_Texture* gDau_right = NULL;
SDL_Texture* gDau_left = NULL;
SDL_Texture* gDau_down = NULL;
SDL_Texture* gDau_up = NULL;
SDL_Texture* gTarget = NULL;
SDL_Texture* gTarget_2 = NULL;
SDL_Texture* gDuoi = NULL;
SDL_Texture* gDuoi_ngangtrai = NULL;
SDL_Texture* gDuoi_ngangphai = NULL;
SDL_Texture* gDuoi_doctren = NULL;
SDL_Texture* gDuoi_docduoi = NULL;
fstream rank_in;
fstream rank_out;

SDL_Event e;
bool quit = false;
time_t target_1,target_2;
enum trangthai{LEFT,UP,RIGHT,DOWN};
int mouse_x, mouse_y;
bool returnmenu = false;
struct RankPerson{
    int score;
    string name;
};

RankPerson rankPerson[rankListMax];
struct Snake{
    int x;
    int y;
    int dodai;
    int kichthuoc;
    SDL_Rect dot[1000];
    trangthai tt;
    int score;
    bool die;
    int life;
};

Snake snake;
SDL_Rect target;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SNAKE C++", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

                 //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
                //Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
    rank_in.open("rank/rank.txt", ios :: in);
    if(rank_in.is_open()){
        int tmp = 0;
        while(!rank_in.eof() && tmp < rankListMax){
            rank_in >> rankPerson[tmp].score;
            getline(rank_in, rankPerson[tmp].name);
            tmp++;
        }
    }
    else{
        cout << "Can't open file rank.txt";
        success = false;
    }
    rank_in.close();
    //rank_out.close();
	//Load texture

    gPause = loadTexture( "img/pause.png");
	if(gPause == NULL){
        printf( "Failed to load texture image!\n" );
		success = false;
	}
	gGameover = loadTexture( "img/gameover.png");
	if(gGameover == NULL){
        printf( "Failed to load texture image!\n" );
		success = false;
	}
	gBackground = loadTexture( "img/background.png" );
	if( gBackground == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gTransparent = loadTexture( "img/transparent.png" );
	if( gTransparent == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gEffectMo = loadTexture( "img/effectMo.png" );
	if( gEffectMo == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gRankList = loadTexture( "img/rank.png");
	if( gRankList == NULL){
        printf( "Failed to load texture image!\n" );
		success = false;
	}
	gInput = loadTexture( "img/input.png");
	if( gInput == NULL){
        printf( "Failed to load texture image!\n" );
		success = false;
	}
	// Load menu_play
	gMenu_play = loadTexture( "img/menu_play.png" );
	if( gMenu_play == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gMenu_play_choose = loadTexture( "img/menu_play_choose.png" );
	if( gMenu_play_choose == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	//load menu_setting
	gMenu_setting = loadTexture( "img/menu_setting.png" );
	if( gMenu_setting == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gMenu_setting_choose = loadTexture( "img/menu_setting_choose.png" );
	if( gMenu_setting_choose == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	//load menu_rank
	gMenu_rank = loadTexture( "img/menu_rank.png" );
	if( gMenu_rank == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gMenu_rank_choose = loadTexture( "img/menu_rank_choose.png" );
	if( gMenu_rank_choose == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	//load menu_guide
	gMenu_guide = loadTexture( "img/menu_guide.png" );
	if( gMenu_guide == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gMenu_guide_choose = loadTexture( "img/menu_guide_choose.png" );
	if( gMenu_guide_choose == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	//load menu_quit
	gMenu_quit = loadTexture( "img/menu_quit.png" );
	if( gMenu_quit == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gMenu_quit_choose = loadTexture( "img/menu_quit_choose.png" );
	if( gMenu_quit_choose == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	//load exitGame
	gExitGame = loadTexture( "img/exitGame.png" );
	if( gExitGame == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	//load sounds
	gMenu_click = Mix_LoadWAV( "sounds/click.wav" );
	if( gMenu_click == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gEat = Mix_LoadWAV( "sounds/eat.wav" );
	if( gEat == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gDie = Mix_LoadWAV( "sounds/die.wav" );
	if( gDie == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gOver = Mix_LoadWAV( "sounds/over.wav" );
	if( gOver == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gCongratulation = Mix_LoadWAV( "sounds/congratulation.wav" );
	if( gCongratulation == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	//load snake
	gDau_up = loadTexture( "img/dau_up.png" );
	if( gDau_up == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDau_down = loadTexture( "img/dau_down.png" );
	if( gDau_down == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDau_left = loadTexture( "img/dau_left.png" );
	if( gDau_left == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDau_right = loadTexture( "img/dau_right.png" );
	if( gDau_right == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
    gKhung = loadTexture( "img/khung.png" );
	if( gKhung == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	//rank top
	gRank_top_1 = loadTexture( "img/rank_top_1.png" );
	if( gRank_top_1 == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gRank_top_2 = loadTexture( "img/rank_top_2.png" );
	if( gRank_top_2 == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gRank_top_3 = loadTexture( "img/rank_top_3.png" );
	if( gRank_top_3 == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	//Load dot
	gDot_ngangphai = loadTexture( "img/dot_ngangphai.png" );
	if( gDot_ngangphai == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDot_ngangtrai = loadTexture( "img/dot_ngangtrai.png" );
	if( gDot_ngangtrai == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDot_docduoi = loadTexture( "img/dot_docduoi.png" );
	if( gDot_docduoi == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDot_doctren = loadTexture( "img/dot_doctren.png" );
	if( gDot_doctren == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDot_left_bot = loadTexture( "img/dot_left_bot.png" );
	if( gDot_left_bot == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDot_left_top = loadTexture( "img/dot_left_top.png" );
	if( gDot_left_top == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDot_right_bot = loadTexture( "img/dot_right_bot.png" );
	if( gDot_right_bot == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDot_right_top = loadTexture( "img/dot_right_top.png" );
	if( gDot_right_top == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDot_bot_left = loadTexture( "img/dot_bot_left.png" );
	if( gDot_bot_left == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDot_bot_right = loadTexture( "img/dot_bot_right.png" );
	if( gDot_bot_right == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDot_top_left = loadTexture( "img/dot_top_left.png" );
	if( gDot_top_left == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDot_top_right = loadTexture( "img/dot_top_right.png" );
	if( gDot_top_right == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	gHeart = loadTexture( "img/heart.png" );
	if( gHeart == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	//Load target
	gTarget = loadTexture( "img/target.png" );
	if( gTarget == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gTarget_2 = loadTexture( "img/target_2.png" );
	if( gTarget_2 == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	gDuoi_docduoi = loadTexture( "img/duoi_docduoi.png");
	if( gDuoi_docduoi == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDuoi_doctren = loadTexture( "img/duoi_doctren.png");
	if( gDuoi_doctren == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDuoi_ngangphai = loadTexture( "img/duoi_ngangphai.png");
	if( gDuoi_ngangphai == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gDuoi_ngangtrai = loadTexture( "img/duoi_ngangtrai.png");
	if( gDuoi_ngangtrai == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	//Nothing to load
	return success;
}

void close()
{

	//Free loaded image
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;
	SDL_DestroyTexture( gTransparent );
	gTransparent = NULL;
    SDL_DestroyTexture( gKhung );
	gKhung = NULL;
    SDL_DestroyTexture( gText );
    gText = NULL;
    SDL_DestroyTexture( gRankList );
    gRankList = NULL;
    SDL_DestroyTexture( gGameover);
    gGameover = NULL;
    SDL_DestroyTexture( gPause);
    gPause = NULL;
    SDL_DestroyTexture( gRank_top_1 );
    gRank_top_1 = NULL;
    SDL_DestroyTexture( gRank_top_2 );
    gRank_top_2 = NULL;
    SDL_DestroyTexture( gRank_top_3 );
    gRank_top_3 = NULL;

     //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

    //free menu
    SDL_DestroyTexture( gBackground );
    gBackground = NULL;
    SDL_DestroyTexture( gEffectMo );
    gEffectMo = NULL;
    SDL_DestroyTexture( gMenu_guide );
    gMenu_guide = NULL;
    SDL_DestroyTexture( gMenu_play );
    gMenu_play = NULL;
    SDL_DestroyTexture( gMenu_setting );
    gMenu_setting = NULL;
    SDL_DestroyTexture( gMenu_quit );
    gMenu_quit = NULL;
    SDL_DestroyTexture( gMenu_rank );
    gMenu_rank = NULL;

    //free sounds
    Mix_FreeChunk( gMenu_click );
    gMenu_click = NULL;
    Mix_FreeChunk( gEat );
    gEat = NULL;
    Mix_FreeChunk( gOver);
    gOver = NULL;
    Mix_FreeChunk( gDie );
    gDie = NULL;
    Mix_FreeChunk( gCongratulation );
    gCongratulation = NULL;

    SDL_DestroyTexture( gMenu_guide_choose );
    gMenu_guide_choose = NULL ;
    SDL_DestroyTexture( gMenu_play_choose );
    gMenu_play_choose = NULL;
    SDL_DestroyTexture( gMenu_setting_choose );
    gMenu_setting_choose = NULL;
    SDL_DestroyTexture( gMenu_quit_choose );
    gMenu_quit_choose = NULL;
    SDL_DestroyTexture( gMenu_rank_choose );
    gMenu_rank_choose = NULL;
    SDL_DestroyTexture( gExitGame );
    gExitGame = NULL;
    SDL_DestroyTexture( gInput );
    gInput = NULL;
    SDL_DestroyTexture( gHeart );
    gHeart = NULL;
    //
	SDL_DestroyTexture( gDot );
	gDot = NULL;
	SDL_DestroyTexture( gDot_doctren );
	gDot_doctren = NULL;
	SDL_DestroyTexture( gDot_docduoi );
	gDot_docduoi = NULL;
	SDL_DestroyTexture( gDot_ngangtrai );
	gDot_ngangtrai = NULL;
	SDL_DestroyTexture( gDot_ngangphai );
	gDot_ngangphai = NULL;
	SDL_DestroyTexture( gDot_left_bot );
	gDot_left_bot = NULL;
	SDL_DestroyTexture( gDot_left_top );
	gDot_left_top = NULL;
	SDL_DestroyTexture( gDot_right_bot );
	gDot_right_bot = NULL;
	SDL_DestroyTexture( gDot_right_top );
	gDot_right_top = NULL;
	SDL_DestroyTexture( gDot_top_left );
	gDot_top_left = NULL;
	SDL_DestroyTexture( gDot_top_right );
	gDot_top_right = NULL;
	SDL_DestroyTexture( gDot_bot_left );
	gDot_bot_left = NULL;
	SDL_DestroyTexture( gDot_bot_right );
	gDot_bot_right = NULL;
	SDL_DestroyTexture( gDau );
	gDau = NULL;
	SDL_DestroyTexture( gDau_down );
	gDau_down = NULL;
	SDL_DestroyTexture( gDau_left );
	gDau_left = NULL;
	SDL_DestroyTexture( gDau_right );
	gDau_right = NULL;
	SDL_DestroyTexture( gDau_up );
	gDau_up = NULL;
	SDL_DestroyTexture( gDuoi );
	gDuoi = NULL;
	SDL_DestroyTexture( gDuoi_ngangtrai );
	gDuoi_ngangtrai = NULL;
	SDL_DestroyTexture( gDuoi_ngangphai );
	gDuoi_ngangphai = NULL;
	SDL_DestroyTexture( gDuoi_doctren );
	gDuoi_doctren = NULL;
	SDL_DestroyTexture( gDuoi_docduoi );
	gDuoi_docduoi = NULL;
	SDL_DestroyTexture( gTarget );
	gTarget = NULL;
	SDL_DestroyTexture( gTarget_2 );
	gTarget_2 = NULL;
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

char numToChar(int n){
    return char(48 + n);
}

int charToNum(char c){
    return c - 48;
}

string numToString(int n){
    string s = "";
    do{
        int tmp = n % 10;
        s.insert(0, 1, numToChar(tmp));
        n /= 10;
    }while(n > 0);
    return s;
}

void SizeText(int sizetext){
   gFont = TTF_OpenFont("koverwatch.ttf", sizetext);
}

void vietChu(string text, int posx, int posy, SDL_Color fg){
    gSurface = TTF_RenderText_Solid(gFont, text.c_str(), fg);
	gText = SDL_CreateTextureFromSurface(gRenderer, gSurface);
	SDL_FreeSurface(gSurface);

	SDL_Rect srcRest;
	SDL_Rect desRect;
	TTF_SizeText(gFont, text.c_str(), &srcRest.w, &srcRest.h);

	srcRest.x = 0;
	srcRest.y =  0;

	desRect.x = posx;
	desRect.y = posy;

	desRect.w = srcRest.w;
	desRect.h = srcRest.h;
    SDL_RenderCopy(gRenderer, gText, &srcRest, &desRect);
}

void veKhung(){
    //Khung
    SDL_Rect khung;
    khung.x = 0;
    khung.y = 0;
    khung.w = SCREEN_WIDTH;
    khung.h = SCREEN_HEIGHT;
    SDL_RenderSetViewport( gRenderer, &khung );

    //Render texture to screen
    SDL_RenderCopy( gRenderer, gKhung, NULL, NULL );
}
void randomTarget(){
    bool check = false;
    while(!check){
        target.x = (1 + rand() % (SCREEN_WIDTH - snake.kichthuoc) / snake.kichthuoc) * snake.kichthuoc;
        target.y = (3 + rand() % (((SCREEN_HEIGHT - snake.kichthuoc) / snake.kichthuoc) - 3 )) * snake.kichthuoc;
        target.w = snake.kichthuoc;
        target.h = snake.kichthuoc;
        for(int i = 0; i < snake.dodai; i++){
            if(snake.dot[i].x != target.x || snake.dot[i].y != target.y){
                check = true;
                break;
            }
        }
    }
}
void khoiTao(){
    snake.score = 0;
    snake.x = 30;
    snake.y = 120;
    snake.dodai = 3;
    snake.tt = RIGHT;
    snake.kichthuoc = 30;
    snake.life = 3;
    snake.die = false;
    //dot[0] == dau
    snake.dot[0].x = snake.x + snake.kichthuoc + snake.kichthuoc;
    snake.dot[0].y = snake.y;
    snake.dot[0].w = snake.kichthuoc;
    snake.dot[0].h = snake.kichthuoc;
    //dot[1]
    snake.dot[1].x = snake.x + snake.kichthuoc;
    snake.dot[1].y = snake.y;
    snake.dot[1].w = snake.kichthuoc;
    snake.dot[1].h = snake.kichthuoc;
    //dot[2]
    snake.dot[2].x = snake.x;
    snake.dot[2].y = snake.y;
    snake.dot[2].w = snake.kichthuoc;
    snake.dot[2].h = snake.kichthuoc;
    randomTarget();
}

void hienThi(){
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( gRenderer );

    //vekhung
    veKhung();

    string Score = "SCORE ";
    SDL_Color fg = { 10, 170, 210 };
    int Score_x = 800;
    SizeText(40);
    vietChu(Score, Score_x, 10, fg);
    string score = numToString(snake.score);
    vietChu(score, Score_x + 150, 10, fg);
    SDL_RenderSetViewport( gRenderer, &target );

    //Render texture to screen
    target_1 = clock();
    if(target_1 % 2 == 0)
        SDL_RenderCopy( gRenderer, gTarget, NULL, NULL );
    else
        SDL_RenderCopy( gRenderer, gTarget_2, NULL, NULL );
    SDL_Rect heart[snake.life];
    heart[0].x = 25;
    heart[0].y = 20;
    heart[0].w = 70;
    heart[0].h = 45;
    for(int i = 1; i < snake.life; i++){
        heart[i].x = heart[i - 1].x + 80;
        heart[i].y = heart[i - 1].y;
        heart[i].w = heart[i - 1].w;
        heart[i].h = heart[i - 1].h;
    }
    for(int i = 0; i < snake.life; i++){
        SDL_RenderSetViewport( gRenderer, &heart[i]);
        SDL_RenderCopy( gRenderer, gHeart, NULL, NULL);
    }
    SDL_RenderSetViewport( gRenderer, &snake.dot[0] );
    //Render texture to screen
    //cai dau

    if(snake.tt == UP)
        gDau = gDau_up;
    else if(snake.tt == RIGHT)
        gDau = gDau_right;
    else if(snake.tt == DOWN)
        gDau = gDau_down;
    else if(snake.tt == LEFT)
        gDau = gDau_left;
    SDL_RenderCopy( gRenderer, gDau, NULL, NULL );
    for(int i = 1; i < snake.dodai - 1; i++){
        SDL_RenderSetViewport( gRenderer, &snake.dot[i] );
        if(snake.dot[i].x < snake.dot[i - 1].x && snake.dot[i].y == snake.dot[i - 1].y)
            gDot = gDot_ngangphai;
        else if(snake.dot[i].x > snake.dot[i - 1].x && snake.dot[i].y == snake.dot[i - 1].y)
            gDot = gDot_ngangtrai;
        else if(snake.dot[i].x == snake.dot[i - 1].x && snake.dot[i].y > snake.dot[i - 1].y)
            gDot = gDot_doctren;
        else if(snake.dot[i].x == snake.dot[i - 1].x && snake.dot[i].y < snake.dot[i - 1].y)
            gDot = gDot_docduoi;

        //Render texture to screen
        if(snake.dot[i].x == snake.dot[i - 1].x && snake.dot[i].y == snake.dot[i + 1].y){//dang di ngang
            if(snake.dot[i].x > snake.dot[i + 1].x){ // di ngang tu trai sang
                if(snake.dot[i].y == 90 && snake.dot[i - 1].y == SCREEN_HEIGHT - snake.kichthuoc)
                    gDot = gDot_left_top;
                else if(snake.dot[i].y == SCREEN_HEIGHT - snake.kichthuoc && snake.dot[i - 1].y == 90)
                    gDot = gDot_left_bot;
                else if(snake.dot[i].y > snake.dot[i - 1].y) //di tu trai len tren
                    gDot = gDot_left_top;
                else if(snake.dot[i].y < snake.dot[i - 1].y)//di tu trai xuong duoi
                    gDot = gDot_left_bot;


            }
            else if(snake.dot[i].x < snake.dot[i + 1].x){//di ngang tu phai sang
                if(snake.dot[i].y == 90 && snake.dot[i - 1].y == SCREEN_HEIGHT - snake.kichthuoc)
                    gDot = gDot_right_top;
                else if(snake.dot[i].y == SCREEN_HEIGHT - snake.kichthuoc && snake.dot[i - 1].y == 90)
                    gDot = gDot_right_bot;
                else if(snake.dot[i].y > snake.dot[i - 1].y)// di tu phai len tren
                    gDot = gDot_right_top;
                else if(snake.dot[i].y < snake.dot[i - 1].y)//di tu phai xuong duoi
                    gDot = gDot_right_bot;
            }
        }
        else if(snake.dot[i].x == snake.dot[i + 1].x && snake.dot[i].y == snake.dot[i - 1].y){//dang di doc
            if(snake.dot[i].y > snake.dot[i + 1].y){ // di tu tren xuong
                if(snake.dot[i].x == 0 && snake.dot[i - 1].x == SCREEN_WIDTH - snake.kichthuoc)
                    gDot = gDot_top_left;
                else if(snake.dot[i].x ==  SCREEN_WIDTH - snake.kichthuoc && snake.dot[i - 1].x == 0)
                    gDot = gDot_top_right;
                else if(snake.dot[i].x > snake.dot[i - 1].x)//di tu tren sang trai
                    gDot = gDot_top_left;
                else if(snake.dot[i].x < snake.dot[i - 1].x)//di tu tren sang phai
                    gDot = gDot_top_right;
            }
            else if(snake.dot[i].y < snake.dot[i + 1].y){//di tu duoi len
                if(snake.dot[i].x == 0 && snake.dot[i - 1].x == SCREEN_WIDTH - snake.kichthuoc)
                    gDot = gDot_bot_left;
                else if(snake.dot[i].x ==  SCREEN_WIDTH - snake.kichthuoc && snake.dot[i - 1].x == 0)
                    gDot = gDot_bot_right;
                else if(snake.dot[i].x > snake.dot[i - 1].x)//di tu duoi sang trai
                    gDot = gDot_bot_left;
                else if(snake.dot[i].x < snake.dot[i - 1].x)//di tu duoi sang phai
                    gDot = gDot_bot_right;
            }
        }
        SDL_RenderCopy( gRenderer, gDot, NULL, NULL );
    }
    //cai duoi
    SDL_RenderSetViewport( gRenderer, &snake.dot[snake.dodai - 1]);
    if(snake.dot[snake.dodai - 1].x < snake.dot[snake.dodai - 2].x && snake.dot[snake.dodai - 1].y == snake.dot[snake.dodai - 2].y)
        gDuoi = gDuoi_ngangphai;
    else if(snake.dot[snake.dodai - 1].x > snake.dot[snake.dodai - 2].x && snake.dot[snake.dodai - 1].y == snake.dot[snake.dodai - 2].y)
        gDuoi = gDuoi_ngangtrai;
    else if(snake.dot[snake.dodai - 1].x == snake.dot[snake.dodai - 2].x && snake.dot[snake.dodai - 1].y > snake.dot[snake.dodai - 2].y)
        gDuoi = gDuoi_doctren;
    else if(snake.dot[snake.dodai - 1].x == snake.dot[snake.dodai - 2].x && snake.dot[snake.dodai - 1].y < snake.dot[snake.dodai - 2].y)
        gDuoi = gDuoi_docduoi;
    SDL_RenderCopy( gRenderer, gDuoi, NULL, NULL);
    SDL_RenderPresent( gRenderer );
}
void afterDie();
void xuLy(){
    //xu ly trang thai len, xuong, trai, phai
    for(int i = snake.dodai - 1; i > 0; i--){
            snake.dot[i] = snake.dot[i - 1];
    }
    switch(snake.tt){
    case UP:
        snake.dot[0].y -= snake.kichthuoc;
        break;
    case DOWN:
        snake.dot[0].y += snake.kichthuoc;
        break;
    case RIGHT:
        snake.dot[0].x += snake.kichthuoc;
        break;
    case LEFT:
        snake.dot[0].x -= snake.kichthuoc;
        break;
    }
    switch(snake.tt){
    case RIGHT:
        if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_UP){
                snake.tt = UP;
            }else if(e.key.keysym.sym == SDLK_DOWN){
                snake.tt = DOWN;
            }
        }
        break;
    case LEFT:
        if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_UP){
                snake.tt = UP;
            }else if(e.key.keysym.sym == SDLK_DOWN){
                snake.tt = DOWN;
            }
        }
        break;
    case UP:
        if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_RIGHT){
                snake.tt = RIGHT;
            }else if(e.key.keysym.sym == SDLK_LEFT){
                snake.tt = LEFT;
            }
        }
        break;
    case DOWN:
        if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_RIGHT){
                snake.tt = RIGHT;
            }else if(e.key.keysym.sym == SDLK_LEFT){
                snake.tt = LEFT;
            }
        }
        break;
    }
    //xu li bien
    if(snake.dot[0].x < 0)
        snake.dot[0].x = SCREEN_WIDTH - snake.kichthuoc;
    else if(snake.dot[0].x >= SCREEN_WIDTH)
        snake.dot[0].x = 0;
    else if(snake.dot[0].y < 90)
        snake.dot[0].y = SCREEN_HEIGHT - snake.kichthuoc;
    else if(snake.dot[0].y >= SCREEN_HEIGHT)
        snake.dot[0].y = 90;
    //xu li an muc tieu
    if(snake.dot[0].x == target.x && snake.dot[0].y == target.y){
        Mix_PlayChannel( -1, gEat, 0);
        snake.dot[snake.dodai] = snake.dot[snake.dodai - 1];
        snake.dodai++;
        snake.score += 10;
        randomTarget();
    }
    //xu li chet
    for(int i = 1; i < snake.dodai; i++){
        if(snake.dot[0].x == snake.dot[i].x && snake.dot[0].y == snake.dot[i].y){
            snake.life--;
            if(snake.life == 0)
            {
                snake.die = true;
                break;
            }

            Mix_PlayChannel( -1, gEat, 0);

            snake.dodai = i;
        }
    }


}

void exitGame(){
    bool check = false;
    SDL_Rect rect_Exit;
    rect_Exit.x = 0;
    rect_Exit.y = 0;
    rect_Exit.h = SCREEN_HEIGHT;
    rect_Exit.w = SCREEN_WIDTH;
    SDL_Rect rect_Ef;
    rect_Ef.x = 0;
    rect_Ef.y = 0;
    rect_Ef.h = SCREEN_HEIGHT;
    rect_Ef.w = SCREEN_WIDTH;
    //SDL_RenderSetViewport( gRenderer, &rect_Ef );
    //SDL_RenderCopy( gRenderer, gEffectMo, NULL, NULL );
    SDL_RenderSetViewport( gRenderer, &rect_Exit );
    SDL_RenderCopy( gRenderer, gExitGame, NULL, NULL );
    SDL_RenderPresent( gRenderer );
    //Render texture to screen
    while(!quit && !check)
    {
            //User requests quit
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            SDL_GetMouseState( &mouse_x, &mouse_y );
            if(mouse_x >= 325 && mouse_x <= 470 && mouse_y >= 365 && mouse_y <= 395){
                if( SDL_GetMouseState ( NULL ,  NULL ) & SDL_BUTTON(SDL_BUTTON_LEFT )){
                    Mix_PlayChannel( -1, gMenu_click, 0);
                    quit = true;
                    close();
                    exit(1);
                }
            }
            if(e.key.keysym.sym == SDLK_RETURN){
                Mix_PlayChannel( -1, gMenu_click, 0);
                quit = true;
                close();
                exit(1);
            }
            if(e.key.keysym.sym == SDLK_ESCAPE){
                Mix_PlayChannel( -1, gMenu_click, 0);
                check = true;
                break;
            }
            if(mouse_x >= 725 && mouse_x <= 870 && mouse_y >= 365 && mouse_y <= 395){
                if( SDL_GetMouseState ( NULL ,  NULL ) & SDL_BUTTON(SDL_BUTTON_LEFT )){
                    Mix_PlayChannel( -1, gMenu_click, 0);
                    check = true;
                    break;
                }
            }
        }

    }
}

void rankList(){
    bool quit1 = false;
    for(int i = 0; i < rankListMax - 1; i++){
        for(int j = i + 1; j < rankListMax; j++){
            if(rankPerson[i].score < rankPerson[j].score){
                RankPerson tmp = rankPerson[i];
                rankPerson[i] = rankPerson[j];
                rankPerson[j] = tmp;
            }
        }
    }
    rank_out.open("rank/rank.txt", ios :: out);
    int tmp = 0;
    while(tmp < rankListMax){
        rank_out << rankPerson[tmp].score << rankPerson[tmp].name << endl ;
        tmp++;
    }
    //rank_out << rankPerson[tmp].name;
    rank_out.close();
    SDL_Rect rect_rank;

    rect_rank.x = 0;
    rect_rank.y = 0;
    rect_rank.w = SCREEN_WIDTH;
    rect_rank.h = SCREEN_HEIGHT;

    SizeText(30);
    SDL_Color fgstt{223, 242, 233};
    int posxstt = 350;
    int posystt = 150;
    int stt = 4;

    //rank_top
    SDL_Rect rect_rank_top_1;

    rect_rank_top_1.x = posxstt - 10;
    rect_rank_top_1.y = posystt;
    rect_rank_top_1.w = 30;
    rect_rank_top_1.h = 30;
    SDL_Rect rect_rank_top_2;

    rect_rank_top_2.x = posxstt - 10;
    rect_rank_top_2.y = posystt + 45;
    rect_rank_top_2.w = 30;
    rect_rank_top_2.h = 30;

    SDL_Rect rect_rank_top_3;

    rect_rank_top_3.x = posxstt - 10;
    rect_rank_top_3.y = posystt + 90;
    rect_rank_top_3.w = 30;
    rect_rank_top_3.h = 30;
    posystt += 135;

    while(!quit && !quit1){
        while(SDL_PollEvent( &e) != 0){
            if(e.type == SDL_QUIT){
                exitGame();
            }
            if(e.key.keysym.sym == SDLK_ESCAPE){
                quit1 = true;
                break;
            }else{
                SDL_GetMouseState( &mouse_x, &mouse_y );
                if(mouse_x >= 870 && mouse_x <= 900 && mouse_y >= 70 && mouse_y <= 100){
                    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
                        Mix_PlayChannel( -1, gMenu_click, 0 );
                        quit1 = true;
                        break;
                    }
                }
            }
        }
        SDL_RenderSetViewport( gRenderer, &rect_rank);
        SDL_RenderCopy( gRenderer, gRankList, NULL, NULL);
        SDL_RenderSetViewport( gRenderer, &rect_rank_top_1);
        SDL_RenderCopy( gRenderer, gRank_top_1, NULL, NULL);
        SDL_RenderSetViewport( gRenderer, &rect_rank_top_2);
        SDL_RenderCopy( gRenderer, gRank_top_2, NULL, NULL);
        SDL_RenderSetViewport( gRenderer, &rect_rank_top_3);
        SDL_RenderCopy( gRenderer, gRank_top_3, NULL, NULL);
        SDL_RenderSetViewport( gRenderer, &rect_rank);
        for(int i = 0; i < rankListMax; i++){
            string s = numToString(stt);
            vietChu(s, posxstt, posystt, fgstt);
            posystt += 45;
            stt++;
            if(stt == rankListMax + 1)
                stt = 4;
            if(posystt == 600)
                posystt = 150 + 135;
        }
        SDL_Color fgname[rankListMax];
        fgname[0] = { 251, 253, 43};
        fgname[1] = { 146, 246, 220};
        fgname[2] = { 49, 12, 26};
        for(int i = 3; i < rankListMax; i++){
            fgname[i] = { 223, 242, 233};
        }
        int posyname = 150;
        int posxname = 400;
        for(int i = 0; i < rankListMax; i++){
            string s = rankPerson[i].name;
            vietChu(s, posxname , posyname, fgname[i]);
            posyname += 45;
            if(posyname == 600)
                posyname = 150;
        }
        int posyscore = 150;
        int posxscore = 800;

        for(int i = 0; i < rankListMax; i++){
            string s = numToString(rankPerson[i].score);
            vietChu(s, posxscore , posyscore, fgstt);
            posyscore += 45;
            if(posyscore == 600)
                posyscore = 150;
        }
        SDL_RenderPresent( gRenderer );
    }

}
void playAGame();

void afterDie(){
    Mix_PlayChannel( -1, gDie, 0);
    SDL_Rect after;
    after.x = 0;
    after.y = 0;
    after.w = SCREEN_WIDTH;
    after.h = SCREEN_HEIGHT;
    bool high = false;
    if(snake.score > rankPerson[rankListMax - 1].score)
        high = true;
    if(high){
        Mix_PlayChannel( -1, gCongratulation, 0);
        int posHigh;
        string name = " ";
        bool quit1 = false;
        for(int i = rankListMax - 1; i >= 0; i--){
            if(rankPerson[i].score < snake.score)
                posHigh = i;
            if(rankPerson[i].score > snake.score)
                break;
        }
        SDL_StartTextInput();
        while(!quit && !quit1){

            while(SDL_PollEvent( &e) != 0){
                if(e.type == SDL_QUIT){
                    exitGame();
                }else if( e.type == SDL_KEYDOWN){
                    if( e.key.keysym.sym == SDLK_BACKSPACE && name.length() > 0 ){
							//lop off character
							name.erase(name.length() - 1, 1);
                    }
                    else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
                    {
                        SDL_SetClipboardText( name.c_str() );
                    }
                    //Handle paste
                    else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
                    {
                        name = SDL_GetClipboardText();
                    }
                }
                else if( e.type == SDL_TEXTINPUT )
                {
                    //Not copy or pasting
                    if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
                    {
                        //Append character
                        name += e.text.text;
                    }
                }
                if(e.key.keysym.sym == SDLK_RETURN){
                    quit1 = true;
                }
                else {
                    SDL_GetMouseState( &mouse_x, &mouse_y);
                    if(mouse_x >= 580 && mouse_x <= 620 && mouse_y >= 415 && mouse_y <= 505){
                        if(e.type = SDL_GetMouseState( NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
                            Mix_PlayChannel( - 1, gMenu_click, 0);
                            quit1 = true;
                        }
                    }
                }
            }
            SDL_RenderSetViewport( gRenderer, &after);
            SDL_RenderCopy( gRenderer, gInput, NULL, NULL);
            SizeText(50);
            SDL_Color fg = { 223, 242, 233};
            string s = numToString(snake.score);
            int posx = 550;
            int posy = 315;
            if(snake.score < 100)
                posx += 30;
            else if(snake.score < 1000)
                posx += 20;
            vietChu(s, posx, posy, fg);
            SizeText(20);
            SDL_RenderSetViewport( gRenderer, &after);
            vietChu(name, 525, 442, fg);
            SDL_RenderPresent( gRenderer);
        }
        SDL_StopTextInput();
        for(int i = rankListMax - 1; i > posHigh; i--){
            rankPerson[i] = rankPerson[i - 1];
        }
        rankPerson[posHigh].score = snake.score;
        rankPerson[posHigh].name = name;
        rankList();
    }else{
        Mix_PlayChannel( -1, gOver, 0);
        bool quit1 = false;
        while(!quit && !quit1){
            while(SDL_PollEvent(&e) != 0){
                if(e.type == SDL_QUIT)
                    exitGame();
                SDL_GetMouseState( &mouse_x, &mouse_y);
                if(mouse_x >= 490 && mouse_x <= 710){
                    if(mouse_y >= 442 && mouse_y <= 478){
                        if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
                        {
                            Mix_PlayChannel( -1, gMenu_click, 0);
                            playAGame();
                        }
                    }
                    if(mouse_y >= 497 && mouse_y <= 534){
                        if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
                        {

                            Mix_PlayChannel( -1, gMenu_click, 0);
                            quit1 = true;
                        }
                    }
                }
            }
        SDL_RenderSetViewport( gRenderer, &after);
        SDL_RenderCopy( gRenderer, gGameover, NULL, NULL);
        SDL_Color fg = { 223, 242, 233};
        SizeText(50);
        string s = numToString(snake.score);
        int posx = 550;
        int posy = 350;
        if(snake.score < 100)
            posx += 30;
        else if(snake.score < 1000)
            posx += 20;
        vietChu(s, posx, posy, fg);
        SDL_RenderPresent( gRenderer);
        }

    }

}

void pause(){
    bool check1 = false;
    while(!quit && !check1 && !returnmenu){
        while(SDL_PollEvent( &e) != 0){
            if(e.type == SDL_QUIT){
                exitGame();
            }
            SDL_GetMouseState( &mouse_x, &mouse_y);
            if(mouse_x >= 500 && mouse_x <= 700 && mouse_y >= 280 && mouse_y <= 333){
                if(SDL_GetMouseState( NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
                    Mix_PlayChannel( -1, gMenu_click, 0);
                    check1 = true;
                    break;
                }
            }
            if(mouse_x >= 500 && mouse_x <= 700 && mouse_y >= 356 && mouse_y <= 408){
                if(SDL_GetMouseState( NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
                    Mix_PlayChannel( -1, gMenu_click, 0);
                    returnmenu = true;
                    break;
                }
            }
        }
        SDL_Rect rect_pause;
        rect_pause.x = 0;
        rect_pause.y = 0;
        rect_pause.w = SCREEN_WIDTH;
        rect_pause.h = SCREEN_HEIGHT;
        SDL_RenderSetViewport( gRenderer, &rect_pause);
        SDL_RenderCopy( gRenderer, gPause, NULL, NULL);
        SDL_RenderPresent( gRenderer);
    }
}

void playAGame(){
    khoiTao();
    //Event handler
    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                exitGame();
            }
            SDL_GetMouseState( &mouse_x, &mouse_y);
            if(mouse_x >= 1116 && mouse_x <= 1455 && mouse_y >= 30 && mouse_y <= 70){
                if(SDL_GetMouseState( NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
                    pause();
                }
            }
        }

        hienThi();
        xuLy();
        if(snake.die)
            break;
        if(returnmenu)
            break;
        Sleep(70);

    }
    if(snake.die)
        afterDie();
}


void Menu(){
    //Menu
    SDL_Rect menu;
    menu.x = 0;
    menu.y = 0;
    menu.w = SCREEN_WIDTH;
    menu.h = SCREEN_HEIGHT;
    //play
    SDL_Rect menu_play;
    menu_play.x = 30;
    menu_play.y = 250;
    menu_play.w = 300;
    menu_play.h = 100;

    SDL_Rect menu_play_choose;
    menu_play_choose.x = 70;
    menu_play_choose.y = 250;
    menu_play_choose.w = 300;
    menu_play_choose.h = 100;
    //setting
    SDL_Rect menu_setting;
    menu_setting.x = 30;
    menu_setting.y = 400;
    menu_setting.w = 148;
    menu_setting.h = 50;

    SDL_Rect menu_setting_choose;
    menu_setting_choose.x = 25;
    menu_setting_choose.y = 395;
    menu_setting_choose.w = 148;
    menu_setting_choose.h = 50;
    //rank
    SDL_Rect menu_rank;
    menu_rank.x = 30;
    menu_rank.y = 470;
    menu_rank.w = 120;
    menu_rank.h = 50;

    SDL_Rect menu_rank_choose;
    menu_rank_choose.x = 25;
    menu_rank_choose.y = 465;
    menu_rank_choose.w = 120;
    menu_rank_choose.h = 50;
    //guide
    SDL_Rect menu_guide;
    menu_guide.x = 30;
    menu_guide.y = 550;
    menu_guide.w = 300;
    menu_guide.h = 50;

    SDL_Rect menu_guide_choose;
    menu_guide_choose.x = 25;
    menu_guide_choose.y = 545;
    menu_guide_choose.w = 300;
    menu_guide_choose.h = 50;
    //quit
    SDL_Rect menu_quit;
    menu_quit.x = 30;
    menu_quit.y = 620;
    menu_quit.w = 100;
    menu_quit.h = 50;

    SDL_Rect menu_quit_choose;
    menu_quit_choose.x = 25;
    menu_quit_choose.y = 615;
    menu_quit_choose.w = 100;
    menu_quit_choose.h = 50;
    bool check1 = false;
    while(!quit){
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT )
            {
                exitGame();
            }
            //User requests quit            if( e.type == SDL_MOUSEMOTION )

            SDL_GetMouseState( &mouse_x, &mouse_y );


            if(mouse_x <= 330 && mouse_y <= 350 && mouse_x >= 30 && mouse_y >= 250){
                if( SDL_GetMouseState ( NULL ,  NULL ) & SDL_BUTTON(SDL_BUTTON_LEFT)){
                    Mix_PlayChannel( -1, gMenu_click, 0 );
                    playAGame();
                }
            }
            if(mouse_x <= 110 && mouse_y <= 510 && mouse_x >= 30 && mouse_y >= 460){
                if( SDL_GetMouseState( NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
                    Mix_PlayChannel( -1, gMenu_click, 0);
                    rankList();
                }
            }
            if(mouse_x <= 90 && mouse_y <= 650 && mouse_x >= 30 && mouse_y >= 610){
                if( SDL_GetMouseState ( NULL ,  NULL ) & SDL_BUTTON(SDL_BUTTON_LEFT)){
                    Mix_PlayChannel( -1, gMenu_click, 0 );
                    exitGame();
                }
            }
            if(e.key.keysym.sym == SDLK_ESCAPE){
                Mix_PlayChannel( -1, gMenu_click, 0);
                check1 = true;
                break;
            }
        }
        SDL_RenderSetViewport( gRenderer, &menu );
        //Render texture to screen
        //play
        SDL_RenderCopy( gRenderer, gBackground, NULL, NULL );
        if(mouse_x <= 330 && mouse_y <= 350 && mouse_x >= 30 && mouse_y >= 250){
            SDL_RenderSetViewport( gRenderer, &menu_play_choose );
            //Render texture to screen
            SDL_RenderCopy( gRenderer, gMenu_play_choose, NULL, NULL );
        }else{
            SDL_RenderSetViewport( gRenderer, &menu_play );
            //Render texture to screen
            SDL_RenderCopy( gRenderer, gMenu_play, NULL, NULL );
        }
        /*
        //setting
        if(mouse_x <= 140 && mouse_y <= 450 && mouse_x >= 30 && mouse_y >= 400){
            SDL_RenderSetViewport( gRenderer, &menu_setting_choose );
            //Render texture to screen
            SDL_RenderCopy( gRenderer, gMenu_setting_choose, NULL, NULL );
        }else{
            SDL_RenderSetViewport( gRenderer, &menu_setting );
            //Render texture to screen
            SDL_RenderCopy( gRenderer, gMenu_setting, NULL, NULL );
        }
        */
        //rank
        if(mouse_x <= 110 && mouse_y <= 510 && mouse_x >= 30 && mouse_y >= 460){
            SDL_RenderSetViewport( gRenderer, &menu_rank_choose );
            //Render texture to screen
            SDL_RenderCopy( gRenderer, gMenu_rank_choose, NULL, NULL );
        }else{
            SDL_RenderSetViewport( gRenderer, &menu_rank );
            //Render texture to screen
            SDL_RenderCopy( gRenderer, gMenu_rank, NULL, NULL );
        }
        /*
        //guide
        if(mouse_x <= 240 && mouse_y <= 590 && mouse_x >= 50 && mouse_y >= 540){
            SDL_RenderSetViewport( gRenderer, &menu_guide_choose );
            //Render texture to screen
            SDL_RenderCopy( gRenderer, gMenu_guide_choose, NULL, NULL );
        }else{
            SDL_RenderSetViewport( gRenderer, &menu_guide );
            //Render texture to screen
            SDL_RenderCopy( gRenderer, gMenu_guide, NULL, NULL );


        }
        */

        //SDL_RenderPresent( gRenderer );
        //quit
        if(mouse_x <= 90 && mouse_y <= 650 && mouse_x >= 30 && mouse_y >= 610){
            SDL_RenderSetViewport( gRenderer, &menu_quit_choose );
            //Render texture to screen
            SDL_RenderCopy( gRenderer, gMenu_quit_choose, NULL, NULL );
        }else{
            SDL_RenderSetViewport( gRenderer, &menu_quit );
            //Render texture to screen
            SDL_RenderCopy( gRenderer, gMenu_quit, NULL, NULL );
        }
        SDL_RenderPresent( gRenderer );
    }


    //SDL_RenderPresent( gRenderer );
}


int main( int argc, char* args[] )
{
    srand(time(NULL));
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			while(!quit)
                Menu();
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
