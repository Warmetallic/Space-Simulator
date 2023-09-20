/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "Cube.h"
#include "Tetrahedron.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "CatmullRom.h"
#include "FrameBufferObject.h"



// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pPlanarTerrain = NULL;
	m_pFtFont = NULL;
	m_pBarrelMesh = NULL;
	m_pHorseMesh = NULL;
	m_pCarMesh = NULL;
	m_pPlanet = NULL;
	m_pPlanet1 = NULL;
	m_pAsteroid = NULL;
	m_pSphere = NULL;
	m_pCube = NULL;
	m_pTetrahedron = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pCatmullRom = NULL;
	m_pFBO = NULL;
	m_t = 0.0;
	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	m_currentDistance = 0.0f;
	m_cameraRotation = 0.0f;
	count = 0;
}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pPlanarTerrain;
	delete m_pFtFont;
	delete m_pBarrelMesh;
	delete m_pHorseMesh;
	delete m_pCarMesh;
	delete m_pPlanet;
	delete m_pPlanet1;
	delete m_pAsteroid;
	delete m_pSphere;
	delete m_pCube;
	delete m_pTetrahedron;
	delete m_pAudio;
	delete m_pCatmullRom;


	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{



	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	//spaceship
	m_t = 0.0f;
	m_speed = 0.0f;
	m_spaceShipOrientation[0];
	m_spaceShipPosition = glm::vec3(0, 0, 0);
	//camera
	m_thirdPersonCamera = false;
	m_leftCamera = false;
	m_rightCamera = false;
	m_firstCamera = false;
	//lights
	 lightX = 50.0f;
	 lightZ = 50.0f;

	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pPlanarTerrain = new CPlane;
	m_pFtFont = new CFreeTypeFont;
	m_pBarrelMesh = new COpenAssetImportMesh;
	m_pHorseMesh = new COpenAssetImportMesh;
	m_pCarMesh = new COpenAssetImportMesh;
	m_pPlanet = new COpenAssetImportMesh;
	m_pPlanet1 = new COpenAssetImportMesh;
	m_pSphere = new CSphere;
	m_pCube = new CCube;
	m_pTetrahedron = new CTetrahedron;
	m_pAudio = new CAudio;
	m_pCatmullRom = new CCatmullRom;
	m_pFBO = new CFrameBufferObject;


	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("sphereShader.vert");
	sShaderFileNames.push_back("sphereShader.frag");
	sShaderFileNames.push_back("Jel.vert");
	sShaderFileNames.push_back("Jel.frag");
	sShaderFileNames.push_back("flicker.vert");
	sShaderFileNames.push_back("flicker.frag");
	sShaderFileNames.push_back("fog.vert");
	sShaderFileNames.push_back("fog.frag");


	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}


	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	// You can follow this pattern to load additional shaders
	// Create the sphere shader program
	CShaderProgram *pSphereProgram = new CShaderProgram;
	pSphereProgram->CreateProgram();
	pSphereProgram->AddShaderToProgram(&shShaders[4]);
	pSphereProgram->AddShaderToProgram(&shShaders[5]);
	pSphereProgram->LinkProgram();
	m_pShaderPrograms->push_back(pSphereProgram);

	//blur shader
	CShaderProgram *pJel = new CShaderProgram;
	pJel->CreateProgram();
	pJel->AddShaderToProgram(&shShaders[6]);
	pJel->AddShaderToProgram(&shShaders[7]);
	pJel->LinkProgram();
	m_pShaderPrograms->push_back(pJel);


	//reflection shader
	CShaderProgram *pFlicker = new CShaderProgram;
	pFlicker->CreateProgram();
	pFlicker->AddShaderToProgram(&shShaders[8]);
	pFlicker->AddShaderToProgram(&shShaders[9]);
	pFlicker->LinkProgram();
	m_pShaderPrograms->push_back(pFlicker);


	//reflection shader
	CShaderProgram *pFog = new CShaderProgram;
	pFog->CreateProgram();
	pFog->AddShaderToProgram(&shShaders[10]);
	pFog->AddShaderToProgram(&shShaders[11]);
	pFog->LinkProgram();
	m_pShaderPrograms->push_back(pFog);

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);
	
	// Create the planar terrain
	//m_pPlanarTerrain->Create("resources\\textures\\", "grassfloor01.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	m_pFtFont->LoadSystemFont("arial.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	//m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
	//m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
	m_pCarMesh->Load("resources\\models\\Car\\E 45 Aircraft_obj.obj"); //Downloaded from https://www.turbosquid.com
	m_pPlanet->Load("resources\\models\\Planet\\Earth.obj"); //Downloaded from https://www.turbosquid.com
	m_pPlanet1->Load("resources\\models\\Planet1\\Moon.obj"); //Downloaded from https://www.turbosquid.com
	// Create a sphere
	m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	m_pCube->Create("resources\\textures\\dirtpile01.jpg");  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	m_pTetrahedron->Create("resources\\textures\\dirtpile01.jpg");
	glEnable(GL_CULL_FACE);

	// Create the path
	m_pCatmullRom->CreatePath(glm::vec3(-500, 10, -200), glm::vec3(0, 10, -200), glm::vec3(0, 10, 200), glm::vec3(-500, 10, 200));

	//Create the centreline
	m_pCatmullRom->CreateCentreline();

	//Create offset curves
	m_pCatmullRom->CreateOffsetCurves();

	//Create the net
	m_pCatmullRom->CreateNet();


	//m_pFBO->Create(300,300);
	





	// Initialise audio and play background music
	//m_pAudio->Initialise();
	//m_pAudio->LoadEventSound("Resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	//m_pAudio->LoadMusicStream("Resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
	//m_pAudio->PlayMusicStream();
}


// Render method runs repeatedly in a loop
void Game::Render()
{
	
	// Clear the buffers and enable depth testing (z-buffering)
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10; 
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);
	
	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

	
	// Set light and materials in main shader program
	glm::vec4 lightPosition1 = glm::vec4(lightX, 150, lightZ, 1); // Position of light source *in world coordinates*
	pMainProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", glm::vec3(1.0f));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f));		// Specular colour of light
	pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property
		
	glm::vec4 lightPosition2(0, 300, 150, 1); // Position of light source *in world coordinates*
	pMainProgram->SetUniform("light2.position", viewMatrix*lightPosition2); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light2.La", glm::vec3(0.15f));		// Ambient colour of light
	pMainProgram->SetUniform("light2.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light2.Ls", glm::vec3(1.0f));		// Specular colour of light
	pMainProgram->SetUniform("material2.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material2.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material2.Ms", glm::vec3(0.5f));	// Specular material reflectance
	pMainProgram->SetUniform("material2.shininess", 15.0f);		// Shininess material property

	glm::vec4 lightPosition3(10, 500, -150, 1); // Position of light source *in world coordinates*
	pMainProgram->SetUniform("light3.position", viewMatrix*lightPosition3); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light3.La", glm::vec3(0.15f));		// Ambient colour of light
	pMainProgram->SetUniform("light3.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light3.Ls", glm::vec3(1.0f));		// Specular colour of light
	pMainProgram->SetUniform("material3.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material3.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material3.Ms", glm::vec3(0.5f));	// Specular material reflectance
	pMainProgram->SetUniform("material3.shininess", 15.0f);		// Shininess material property




	// Switch to the sphere program
	CShaderProgram *pSphereProgram = (*m_pShaderPrograms)[2];
	pSphereProgram->UseProgram();

	// Set light and materials in sphere programme
	pSphereProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Light position in eye coordinates
	pSphereProgram->SetUniform("light1.La", glm::vec3(1.0f, 0.0f, 1.0f));
	pSphereProgram->SetUniform("light1.Ld", glm::vec3(1.0f, 0.0f, 1.0f));
	pSphereProgram->SetUniform("light1.Ls", glm::vec3(1.0f, 0.0f, 1.0f));
	pSphereProgram->SetUniform("material1.shininess", 15.0f);
	pSphereProgram->SetUniform("material1.Ma", glm::vec3(0.0f, 0.0f, 0.2f));
	pSphereProgram->SetUniform("material1.Md", glm::vec3(0.0f, 0.0f, 1.0f));
	pSphereProgram->SetUniform("material1.Ms", glm::vec3(1.0f, 1.0f, 1.0f));


	pSphereProgram->SetUniform("light1.direction",
		glm::normalize(viewNormalMatrix*glm::vec3(0, -1, 0)));
	pSphereProgram->SetUniform("light1.exponent", 20.0f);
	pSphereProgram->SetUniform("light1.cutoff", 30.0f);

	// second light
	pSphereProgram->SetUniform("light2.position", viewMatrix*lightPosition2); // Light position in eye coordinates
	pSphereProgram->SetUniform("light2.La", glm::vec3(1.0f, 0.0f, 0.0f));
	pSphereProgram->SetUniform("light2.Ld", glm::vec3(1.0f, 0.0f, 0.0f));
	pSphereProgram->SetUniform("light2.Ls", glm::vec3(1.0f, 0.0f, 0.0f));
	pSphereProgram->SetUniform("material2.shininess", 15.0f);
	pSphereProgram->SetUniform("material2.Ma", glm::vec3(0.0f, 0.0f, 0.2f));
	pSphereProgram->SetUniform("material2.Md", glm::vec3(0.0f, 0.0f, 1.0f));
	pSphereProgram->SetUniform("material2.Ms", glm::vec3(1.0f, 1.0f, 1.0f));


	pSphereProgram->SetUniform("light2.direction",
		glm::normalize(viewNormalMatrix*glm::vec3(0, -1, 0)));
	pSphereProgram->SetUniform("light2.exponent", 20.0f);
	pSphereProgram->SetUniform("light2.cutoff", 30.0f);



	// third light
	pSphereProgram->SetUniform("light3.position", viewMatrix*lightPosition3); // Light position in eye coordinates
	pSphereProgram->SetUniform("light3.La", glm::vec3(0.0f, 1.0f, 0.0f));
	pSphereProgram->SetUniform("light3.Ld", glm::vec3(0.0f, 1.0f, 0.0f));
	pSphereProgram->SetUniform("light3.Ls", glm::vec3(0.0f, 1.0f, 0.0f));
	pSphereProgram->SetUniform("material3.shininess", 15.0f);
	pSphereProgram->SetUniform("material3.Ma", glm::vec3(0.0f, 0.0f, 0.2f));
	pSphereProgram->SetUniform("material3.Md", glm::vec3(0.0f, 0.0f, 1.0f));
	pSphereProgram->SetUniform("material3.Ms", glm::vec3(1.0f, 1.0f, 1.0f));


	pSphereProgram->SetUniform("light3.direction",
		glm::normalize(viewNormalMatrix*glm::vec3(0, -1, 0)));
	pSphereProgram->SetUniform("light3.exponent", 20.0f);
	pSphereProgram->SetUniform("light3.cutoff", 30.0f);

			// Render the sphere
			modelViewMatrixStack.Push();
			modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), m_EarthRotation*50);
			modelViewMatrixStack.Translate(glm::vec3(lightX, -250.0f,lightZ));
			modelViewMatrixStack.Scale(150.0f);
			pSphereProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
			pSphereProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pSphereProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pSphere->Render();
			modelViewMatrixStack.Pop();
	


	// Use the main shader program 
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);


	// Switch to the Jel program
	CShaderProgram *pJel = (*m_pShaderPrograms)[3];
	pJel->UseProgram();

	// Set light and materials in jel programme
	pJel->SetUniform("material1.Ma", glm::vec3(1.0f, 0.0f, 0.0f));
	pJel->SetUniform("material1.Md", glm::vec3(1.0f, 0.0f, 0.0f));
	pJel->SetUniform("material1.Ms", glm::vec3(1.0f, 1.0f, 1.0f));
	pJel->SetUniform("material1.shininess", 50.0f);
	pJel->SetUniform("light1.La", glm::vec3(0.15f, 0.15f, 0.15f));
	pJel->SetUniform("light1.Ld", glm::vec3(1.0f, 1.0f, 1.0f));
	pJel->SetUniform("light1.Ls", glm::vec3(1.0f, 1.0f, 1.0f));
	pJel->SetUniform("light1.position", viewMatrix*lightPosition1);
	pJel->SetUniform("t", m_t);

	pJel->SetUniform("light1.direction",
		glm::normalize(viewNormalMatrix*glm::vec3(0, -1, 0)));
	pJel->SetUniform("light1.exponent", 20.0f);
	pJel->SetUniform("light1.cutoff", 30.0f);




		modelViewMatrixStack.Push();
		modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), m_EarthRotation * 10);
		modelViewMatrixStack.Translate(glm::vec3(200, 0, 200));
		modelViewMatrixStack.Scale(20);
		pJel->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pJel->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pJel->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSphere->Render();
		modelViewMatrixStack.Pop();

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), m_EarthRotation * 10);
		modelViewMatrixStack.Translate(glm::vec3(205, 15, 205));
		modelViewMatrixStack.Scale(10);
		pJel->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pJel->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pJel->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSphere->Render();
		modelViewMatrixStack.Pop();

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), m_EarthRotation * 10);
		modelViewMatrixStack.Translate(glm::vec3(195, -20, 195));
		modelViewMatrixStack.Scale(15);
		pJel->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pJel->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pJel->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSphere->Render();
		modelViewMatrixStack.Pop();
		
	


	// Use the main shader program 
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);

	// Switch to the Jel program
	CShaderProgram *pFlicker = (*m_pShaderPrograms)[4];
	pFlicker->UseProgram();

	// Set light and materials in pReflection programme
	pFlicker->SetUniform("material1.Ma", glm::vec3(0.0f, 0.0f, 1.0f));
	pFlicker->SetUniform("material1.Md", glm::vec3(0.0f, 0.0f, 1.0f));
	pFlicker->SetUniform("material1.Ms", glm::vec3(0.0f, 0.0f, 1.0f));
	pFlicker->SetUniform("material1.shininess", 100.0f);
	pFlicker->SetUniform("light1.La", glm::vec3(1.0f, 1.0f, 1.0f));
	pFlicker->SetUniform("light1.Ld", glm::vec3(1.0f, 1.0f, 1.0f));
	pFlicker->SetUniform("light1.Ls", glm::vec3(1.0f, 1.0f, 1.0f));
	pFlicker->SetUniform("light1.position", viewMatrix*lightPosition1);
	pFlicker->SetUniform("t", m_t);

	pFlicker->SetUniform("light1.direction",
		glm::normalize(viewNormalMatrix*glm::vec3(0, -1, 0)));
	pFlicker->SetUniform("light1.exponent", 20.0f);
	pFlicker->SetUniform("light1.cutoff", 30.0f);


	modelViewMatrixStack.Push();

	modelViewMatrixStack.Translate(glm::vec3(1500, -20, 1500));
	modelViewMatrixStack.Rotate(glm::vec3(1, 0, 0), m_EarthRotation * 10);
	modelViewMatrixStack.Scale(5);
	pFlicker->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	pFlicker->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pFlicker->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pSphere->Render();
	modelViewMatrixStack.Pop();


	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(1500, -20, 1500));
	modelViewMatrixStack.Rotate(glm::vec3(1, 0, 0), -m_EarthRotation * 10);
	modelViewMatrixStack.Scale(5);
	pFlicker->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	pFlicker->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pFlicker->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pSphere->Render();
	modelViewMatrixStack.Pop();


	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(1500, -20, 1500));
	modelViewMatrixStack.Rotate(glm::vec3(1, 0, 0), m_EarthRotation * 5);
	modelViewMatrixStack.Scale(8);
	pFlicker->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	pFlicker->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pFlicker->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pSphere->Render();
	modelViewMatrixStack.Pop();
	// Use the main shader program 
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);


	// Render the skybox and terrain with full ambient reflectance 
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderSkybox", true);
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSkybox->Render(cubeMapTextureUnit);
		pMainProgram->SetUniform("renderSkybox", false);
	modelViewMatrixStack.Pop();


	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	



	// Switch to the sphere program
	CShaderProgram *pFog = (*m_pShaderPrograms)[5];
	pFog->UseProgram();

	// Set light and materials in sphere programme
	pFog->SetUniform("material1.Ma", glm::vec3(0.0f, 1.0f, 0.0f));
	pFog->SetUniform("material1.Md", glm::vec3(0.0f, 1.0f, 0.0f));
	pFog->SetUniform("material1.Ms", glm::vec3(1.0f, 1.0f, 1.0f));
	pFog->SetUniform("material1.shininess", 50.0f);
	pFog->SetUniform("light1.La", glm::vec3(0.15f, 0.15f, 0.15f));
	pFog->SetUniform("light1.Ld", glm::vec3(1.0f, 1.0f, 1.0f));
	pFog->SetUniform("light1.Ls", glm::vec3(1.0f, 1.0f, 1.0f));
	pFog->SetUniform("light1.position", viewMatrix*lightPosition1);
	

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(100, -20, 100));
	//modelViewMatrixStack.Rotate(glm::vec3(1, 0, 0), m_EarthRotation * 5);
	modelViewMatrixStack.Scale(2);
	pFog->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	pFog->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pFog->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pSphere->Render();
	modelViewMatrixStack.Pop();
	// Use the main shader program 
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);


	// Render the planet (Earth) 
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), m_EarthRotation);
	modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
	modelViewMatrixStack.Scale(20.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPlanet->Render();
	modelViewMatrixStack.Pop();
	
	// Use the main shader program 
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);



	// Render the planet (Moon) 
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), m_MoonRotation);
	modelViewMatrixStack.Translate(glm::vec3(3.0f, 30.0f, 0.0f));
	modelViewMatrixStack.Scale(0.07f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPlanet1->Render();
	modelViewMatrixStack.Pop();




	// Render the car 
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(m_spaceShipPosition);
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);
	modelViewMatrixStack *= m_spaceShipOrientation;
	modelViewMatrixStack.Scale(5.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pCarMesh->Render();
	modelViewMatrixStack.Pop();



	// Render the sphere
	//modelViewMatrixStack.Push();
	//	modelViewMatrixStack.Translate(glm::vec3(0.0f, 2.0f, 150.0f));
	//	modelViewMatrixStack.Scale(2.0f);
	//	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//	//pMainProgram->SetUniform("bUseTexture", false);
	//	m_pSphere->Render();
	//modelViewMatrixStack.Pop();



	for (float i = 0; i < 700; i = i + 100) {

		for (float j = 0; j < 700; j = j + 50) {

			// Render the cube
			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(glm::vec3(i-300, 500, j-300));
			modelViewMatrixStack.Scale(10.0f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
			//pMainProgram->SetUniform("bUseTexture", false);
			m_pCube->Render();
			modelViewMatrixStack.Pop();

			// Render the tetrahedron
			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(glm::vec3(i-300, -500, j-300));
			modelViewMatrixStack.Scale(1.0f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
			//pMainProgram->SetUniform("bUseTexture", false);
			m_pTetrahedron->Render();
			modelViewMatrixStack.Pop();

		}
	}

	// Render the centreline
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("bUseTexture", false); // turn off texturing
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix",
		m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pCatmullRom->RenderCentreline();
	//m_pCatmullRom->RenderPath();
	modelViewMatrixStack.Pop();

	// Render the offset curves
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("bUseTexture", false); // turn off texturing
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix",
		m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pCatmullRom->RenderOffsetCurves();
	modelViewMatrixStack.Pop();

	// Render the net
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("bUseTexture", false); // turn off texturing
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix",
		m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pCatmullRom->RenderNet();
	modelViewMatrixStack.Pop();




	// Draw the 2D graphics after the 3D graphics
	DisplayFrameRate();

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());		

}

//void Game::Render()
//{
//
//	// Draw the 2D graphics after the 3D graphics
//	DisplayFrameRate();
//
//	// Swap buffers to show the rendered image
//	SwapBuffers(m_gameWindow.Hdc());
//
//
//	m_pFBO->Bind();
//	RenderScene(0);
//	glBindFramebuffer(GL_FRAMEBUFFER,0);
//
//	RenderScene(1);
//
//	m_pFBO->BindTexture(0);
//	
//}
// Update method runs repeatedly with the Render method
void Game::Update() 
{
	// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
	m_pCamera->Update(m_dt);

	/*static float t = 0.0f;
	t += 0.0005f * (float)m_dt;
	if (t > 1.0f)

		t = 0.0f;

	m_pCamera->Set((m_pCatmullRom->Interpolate(glm::vec3(-500, 10, -200), glm::vec3(0, 10, -200), glm::vec3(0, 10, 200), glm::vec3(-500, 10, 200), t)),glm::vec3(0,0,0),glm::vec3(0,1,0));*/
	m_t += (float)(0.0005f * m_dt);
	//distance
	m_currentDistance = m_currentDistance + m_dt * m_speed;
	glm::vec3 p;
	glm::vec3 pNext;
	m_pCatmullRom->Sample(m_currentDistance, p);
	m_pCatmullRom->Sample(m_currentDistance+1.0f,pNext);
   // glm::vec3 T=glm::normalize(glm::atan(p/pNext));
	glm::vec3 T1 = pNext - p;
	glm::vec3 T = glm::normalize(T1);
	//glm::vec3 e = glm::cross(c, d);
	glm::vec3 N = glm::normalize(glm::cross(T, glm::vec3(0, 1, 0)));
	glm::vec3 B = glm::normalize(glm::cross(N, T));
	
	//m_pCamera->Set(p, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//rotate camera
	glm::vec3 up = glm::rotate(glm::vec3(0, 1, 0), m_cameraRotation, T);
	//camera on the car
	glm::vec3 k = p - 30.0f*T;
	glm::vec3 kleft = p - 8.0f*N - 15.0f*T;
	glm::vec3 kright = p + 8.0f*N - 15.0f*T;
	
	//third person camera press down key
	if (m_thirdPersonCamera == true) {
		m_pCamera->Set((k + B) + glm::vec3(0, 20, 0), k + 90.0f * T, up);
	}

	//first person camera press down key
	if (m_firstCamera == true) {
		m_pCamera->Set((p + B) + glm::vec3(0, 10, 0), k + 90.0f * T, up);
	}

	//left camera press down key
	if (m_leftCamera == true) {
		m_pCamera->Set((kleft + B) + glm::vec3(0, 10, 0), k + 90.0f * T, up);
	}

	//right camera press down key
	if (m_rightCamera == true) {
		m_pCamera->Set((kright + B) + glm::vec3(0, 10, 0), k + 90.0f * T, up);
	}

	//tutorial 5 car movement around the spline
	m_t += 0.001f * (float)m_dt;
	float r = 75.0f;
	glm::vec3 x = glm::vec3(1, 0, 0);
	glm::vec3 y = glm::vec3(0, 1, 0);
	glm::vec3 z = glm::vec3(0, 0, 1);
	m_spaceShipPosition = p + B, p + 5.0f * T, glm::vec3(0, 1, 0);
	m_spaceShipOrientation = glm::mat4(glm::mat3(T, B, N));


	m_MoonRotation += 0.02f * m_dt;
	m_EarthRotation += 0.003f * m_dt;;

	m_pAudio->Update();
}



void Game::DisplayFrameRate()
{


	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
    }

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
	}

	//timer dispay
	std::clock_t start;
	start = std::clock();

	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	m_pFtFont->Render(20, height - 500, 20, "Time: %d", start);


	//counter
	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	m_pFtFont->Render(650, height - 500, 20, "Speed: %d", count);

}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/
	
	
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();
	

}


WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case '1':
			m_pAudio->PlayEventSound();
			break;
		case VK_F1:
			m_pAudio->PlayEventSound();
			break;

		case VK_UP:
			m_thirdPersonCamera = false;
			m_leftCamera = false;
			m_rightCamera = false;
			m_firstCamera = true;
			break;
		case VK_LEFT:
			//m_cameraRotation -= m_dt * 0.4f;
			m_thirdPersonCamera = false;
			m_leftCamera = true;
			m_rightCamera = false;
			m_firstCamera = false;
			break;
		case VK_RIGHT:
			//m_cameraRotation += m_dt * 0.4f;
			m_thirdPersonCamera = false;
			m_leftCamera = false;
			m_rightCamera = true;
			m_firstCamera = false;
			break;
		case 0x32:
			count++;
			m_speed+=0.005;
			break;
		case 0x33:
			count--;
			m_speed -= 0.005;
			if (m_speed < 0.0) {
				m_speed = 0.0f;
				count = 0;
			}
			break;
		case 0x34:
			m_cameraRotation -= m_dt * 0.1f;

			break;
		case 0x35:
			m_cameraRotation += m_dt * 0.1f;

			break;
		case VK_DOWN:
			//m_cameraRotation -= m_dt * 0.1f;
			m_thirdPersonCamera = true;
			m_leftCamera = false;
			m_rightCamera = false;
			m_firstCamera = false;
			break;
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}
