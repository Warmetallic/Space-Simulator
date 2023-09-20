#pragma once

#include "Common.h"
#include "GameWindow.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class CCube;
class CTetrahedron;
class COpenAssetImportMesh;
class CAudio;
class CCatmullRom;
class CFrameBufferObject;
class CFog;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void RenderScene(int pass);
	void Render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	vector <CShaderProgram *> *m_pShaderPrograms;
	CPlane *m_pPlanarTerrain;
	CFreeTypeFont *m_pFtFont;
	COpenAssetImportMesh *m_pBarrelMesh;
	COpenAssetImportMesh *m_pHorseMesh;
	COpenAssetImportMesh *m_pCarMesh;
	COpenAssetImportMesh *m_pPlanet;
	COpenAssetImportMesh *m_pPlanet1;
	COpenAssetImportMesh *m_pAsteroid;
	CSphere *m_pSphere;
	CCube *m_pCube;
	CTetrahedron *m_pTetrahedron;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	CCatmullRom *m_pCatmullRom;
	CFrameBufferObject *m_pFBO;

	// Some other member variables
	float lightX;
	float lightZ;
	double m_dt;
	double m_speed;
	float m_MoonRotation;
	float m_EarthRotation;
	int m_framesPerSecond;
	bool m_appActive;
	float m_currentDistance;
	float m_t;
	float m_cameraRotation;
	glm::vec3 m_spaceShipPosition;
	glm::mat4 m_spaceShipOrientation;
	bool m_thirdPersonCamera;
	bool m_leftCamera;
	bool m_rightCamera;
	bool m_firstCamera;
	int count;

public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	static const int FPS = 60;
	void DisplayFrameRate();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;


};
