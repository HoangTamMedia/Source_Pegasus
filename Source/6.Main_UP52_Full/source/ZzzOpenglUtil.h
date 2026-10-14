#pragma once
#define GMT (24 + (7))

extern float g_fFrameEstimate;
extern float PerspectiveX;
extern float PerspectiveY;
extern int OpenglWindowWidth;
extern int OpenglWindowHeight;
extern unsigned int WindowWidth;
extern unsigned int WindowHeight;
// Upscaling - resolução de renderização (menor que a tela para melhor performance)
extern unsigned int RenderWidth;
extern unsigned int RenderHeight;
extern float UpscaleFactor; // Fator de upscale (ex: 0.5 = renderiza em 50% e upscale para 100%)
extern bool EnableUpscaling; // Habilitar/desabilitar upscaling
extern vec3_t CollisionPosition;
extern double  FPS;
extern double  FPS_AVG;
extern float  FPS_ANIMATION_FACTOR;
extern double  WorldTime;
extern int     g_Ping;
extern float Zoom3D;
extern float  Camera3DPosY;
extern float  Camera3DPosZ;
extern bool  Camera3DZoom;
extern int	LevelCamera;
extern bool  Camera3DSetMove;
extern bool   Camera3DEnable;
extern bool   Camera3DSet;
extern bool   CameraTopViewEnable;
extern float  CameraViewNear;
extern float  CameraViewFar;
extern float  CameraFOV;
extern float  Camera3DFov;
extern vec3_t CameraPosition;
extern vec3_t CameraAngle;
extern float  CameraMatrix[3][4];
extern float  g_fCameraCustomDistance;
extern bool   FogEnable;
extern bool   TextureEnable;
extern bool   DepthTestEnable;
extern bool   CullFaceEnable;
extern bool   DepthMaskEnable;
extern vec3_t       MousePosition;
extern vec3_t       MouseTarget;
extern int          MouseX;
extern int          MouseY;
extern int          BackMouseX;
extern int          BackMouseY;
extern bool         MouseLButton;
extern bool     	MouseLButtonPop;
extern bool 		MouseLButtonPush;
extern bool         MouseRButton;
extern bool 		MouseRButtonPop;
extern bool 		MouseRButtonPush;
extern bool 		MouseLButtonDBClick;
extern bool 		MouseRButtonDBClick;
extern bool         MouseMButton;
extern bool 		MouseMButtonPop;
extern bool 		MouseMButtonPush;
extern int          MouseWheel;
extern DWORD		MouseRButtonPress;
extern char         GrabFileName[];
extern bool         GrabEnable;

bool CheckID_HistoryDay(char* Name, WORD day);
void SaveScreen();
void gluPerspective2(float Fov, float Aspect, float ZNear, float ZFar);
void glViewport2(int x, int y, int Width, int Height);
void CreateScreenVector(int sx, int sy, vec3_t Target, bool bFixView = true);
void Projection(vec3_t Position, int* sx, int* sy);
void GetOpenGLMatrix(float Matrix[3][4]);
void TransformPosition(vec3_t Position, vec3_t WorldPosition, int* x, int* y);
bool TestDepthBuffer(vec3_t Position);
void BeginSprite();
void EndSprite();
void EnableDepthTest();
void DisableDepthTest();
void EnableCullFace();
void DisableCullFace();
void EnableDepthMask();
void DisableDepthMask();
void DisableTexture(bool AlphaTest = false);
void DisableAlphaBlend();
void EnableLightMap();
void EnableAlphaTest(bool DepthMake = true);
void EnableAlphaBlend();
void EnableAlphaBlendMinus();
void EnableAlphaBlend2();
void EnableAlphaBlend3();
void EnableAlphaBlend4();
void BindTexture(int tex);
void BindTextureStream(int tex);
void EndTextureStream();
void BeginOpengl(int x = 0, int y = 0, int Width = 640, int Height = 480);
void EndOpengl();
// Funções de upscaling
void InitUpscaling(float factor = 0.5f); // Inicializar upscaling (factor: 0.5 = 50%, 0.75 = 75%, etc)
void SetUpscalingEnabled(bool enabled); // Habilitar/desabilitar upscaling
void UpdateUpscalingResolution(); // Atualizar resolução de renderização baseado no fator
void UpdateMousePositionn();

#define NFPS 1

#if NFPS
void InitVSync();
bool IsVSyncAvailable();
bool IsVSyncEnabled();
void EnableVSync();
void DisableVSync();
int GetFPSLimit();
#endif

extern inline void TEXCOORD(float* c, float u, float v);
void RenderBox(float Matrix[3][4]);
void RenderPlane3D(float Width, float Height, float Matrix[3][4]);
void RenderSprite(int Texture, vec3_t Position, float Width, float Height, vec3_t Light, float Angle = 0.f, float u = 0.f, float v = 0.f, float uWidth = 1.f, float vHeight = 1.f);
void RenderSpriteUV(int Texture, vec3_t Position, float Width, float Height, float(*UV)[2], vec3_t Light[4], float Alpha = 1.f);
void RenderNumber(vec3_t Position, QWORD Num, vec3_t Color, float Alpha = 1.f, float Scale = 15.f);
void RenderNumber2(vec3_t Position, QWORD Num, vec3_t Color, float Alpha = 1.f, float Scale = 15.f);
float RenderNumber2D(float x, float y, int Num, float Width, float Height);
void RenderColor(float x, float y, float Width, float Height, float Alpha = 0.f, int Flag = 0);
void EndRenderColor();
void RenderBitmapScales(int Texture, float x, float y, float Width, float Height, float u = 0.f, float v = 0.f, float uWidth = 1.f, float vHeight = 1.f, bool Scale = true, bool StartScale = true, float Alpha = 0.f, float ScaleW = 1.0f, float ScaleH = 1.0f);
void RenderBitmap(int Texture, float x, float y, float Width, float Height, float u = 0.f, float v = 0.f, float uWidth = 1.f, float vHeight = 1.f, bool Scale = true, bool StartScale = true, float Alpha = 0.f);
void RenderBitmapTrade(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale, float Alpha, DWORD color);
void RenderBitmapCircle(int Texture, float x, float y, float Radius, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale, float Alpha);
void RenderColorBitmap(int Texture, float x, float y, float Width, float Height, float u = 0.f, float v = 0.f, float uWidth = 1.f, float vHeight = 1.f, unsigned int color = 0xffffffff);
void RenderBitmapRotate(int Texture, float x, float y, float Width, float Height, float Angle, float u = 0.f, float v = 0.f, float uWidth = 1.f, float vHeight = 1.f);
void RenderBitRotate(int Texture, float x, float y, float Width, float Height, float Rotate);
void RenderPointRotate(int Texture, float ix, float iy, float iWidth, float iHeight, float x, float y, float Width, float Height, float Rotate, float Rotate_Loc, float uWidth, float vHeight, int Num = -1);
void RenderBitmapLocalRotate(int Texture, float x, float y, float Width, float Height, float Rotate, float u = 0.f, float v = 0.f, float uWidth = 1.f, float vHeight = 1.f);
void RenderBitmapAlpha(int Texture, float sx, float sy, float Width, float Height);
void RenderBitmapUV(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight);
void BeginBitmap();
void EndBitmap();
float absf(float a);
float minf(float a, float b);
float maxf(float a, float b);
void InitCollisionDetectLineToFace();
bool CollisionDetectLineToFace(vec3_t Position, vec3_t Target, int Polygon, float* v1, float* v2, float* v3, float* v4, vec3_t Normal, bool Collision = true);
bool CollisionDetectLineToOBB(vec3_t p1, vec3_t p2, OBB_t obb);
void CalcFPS();
void BDrawOutLine(int iPos_x, int iPos_y, int iWidth, int iHeight);
void BDrawOutLine2(int iPos_x, int iPos_y, int iWidth, int iHeight);
void BRenderTabLine(float iPos_x, float iPos_y, float iTabWidth, float iTabHeight, int iTabNum, int iSelectNum);
void GetDrawCircle(int ID, float X, float Y, float W, float CurrenX, float CurrenY, float SetScale, int ScaleSize, int ScalePosicion, float Alpha);
void GetDrawSquare(int ID, float X, float Y, float W, float CurrenX, float CurrenY, float SetScale, int ScaleSize, int ScalePosicion, float Alpha);
bool rand_fps_check(int reference_frames);
