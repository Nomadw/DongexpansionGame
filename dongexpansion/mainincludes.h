#include <list>
//--Include other project files (LOCAL)
#include "MainObject.h"
#include "ball.h"
#include "paddle.h"
#include "wave.h"
#include "titlescreen.h"
#include "scoreitem.h"
#include "background.h"
#include "border.h"
#include "gameinfo.h"

//--Create iterator lists, used for handling updating and rendering
std::list<MainObject *> objects;
std::list<MainObject *>::iterator it01;
std::list<MainObject *>::iterator it02;

std::list<ScoreItem *> scoreobjects;
std::list<ScoreItem *>::iterator sc01;

std::list<Border *> platforms;
std::list<Border *>::iterator pl01;

std::list<Ball *> balls;
std::list<Ball *>::iterator ba01;

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
ID3D11DepthStencilView *zbuffer;       // the pointer to our depth buffer
ID3D11InputLayout *pLayout;            // the pointer to the input layout
ID3D11VertexShader *pVS;               // the pointer to the vertex shader
ID3D11PixelShader *pPS;                // the pointer to the pixel shader
ID3D11Buffer *pVBuffer;                // the pointer to the vertex buffer
ID3D11Buffer *pIBuffer;                // the pointer to the index buffer
ID3D11Buffer *pCBuffer;                // the pointer to the constant buffer

vector <ID3D11ShaderResourceView *> textures; //Vector array of texture buffers

//set up audio features
IXAudio2 *engine;
IXAudio2SourceVoice	*music;
IXAudio2SourceVoice *sfx;
IXAudio2SourceVoice *sfx2;
IXAudio2SourceVoice *sfx3;
IXAudio2SourceVoice *sfx4;
IXAudio2MasteringVoice *g_master;

Wave musicbuffer;
Wave music2buffer;
Wave soundbuffer;
Wave sound2buffer;
Wave sound3buffer;
Wave sound4buffer;

bool okeypress = false;
bool enterpress = false;
bool pausepress = false;

int platformdelay = PLATFORMDELAYMAX;

float platformspeed = 0.0f;

//Create devices for direct input
IDirectInputDevice8 *DIKeyboard;
IDirectInputDevice8 *DIMouse;

HWND hWnd;
HRESULT hr;

DIMOUSESTATE mouseLastState;
LPDIRECTINPUT8 DirectInput;

// function prototypes
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void RenderFrame(gameInfo *currentGame);     // renders a single frame
void CleanD3D(void);        // closes Direct3D and releases memory
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders
void UpdateProgram(Paddle *player, gameInfo *currentGame);
void Scored(Paddle *player);
void Unscored(Paddle *player, gameInfo *currentGame);
void GameWon(gameInfo *currentGame);
void RunTitleScreen(TitleScreen *title, gameInfo *currentGame);

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);