#include "Grid/Grid.h"
#include "GameManager.h"
#include "Headers/ClientRequestManager.h"
#include "Headers/ClientNetworkMessageWindow.h"
#include "Headers/ClientApp.h"

ClientApp::ClientApp() 
{
	InitializeCriticalSection(&mMutex);// pour creer la critical section

	mMessageWindow = new ClientNetworkMessageWindow(this);
	mMessageWindow->InitWindow(L"ClientNetworkMessageWindow");
	mRequestManager = ClientRequestManager::GetInstance();
}

ClientApp::~ClientApp() {
	delete mMessageWindow;
	//delete mRequestManager;
}

bool ClientApp::Init() 
{
	mRequestManager = ClientRequestManager::GetInstance();
	mGame = new GameManager();
	mGame->InitWindow();
	mRequestManager->mGame = mGame;

	if (!mRequestManager->Init())
		return false;

	if (!CreateSocketThread())
		return false;
	
	return true;
}

int ClientApp::Run() 
{
	if (ResumeThread(mSocketThread) == -1)
	{
		printf("Erreur thread socket\n");
		return 1;
	}

	// Boucle de messages principale :
	bool endGame = false;
	do
	{
		Update();

		EnterMutex();
		endGame = mRequestManager->GameIsEnded();
		LeaveMutex();

		mGame->Render();

		bool test2 = true;
	} while (WaitForSingleObject(mSocketThread, 0) != WAIT_OBJECT_0 && !endGame);

	
	DeleteCriticalSection(&mMutex);// quand c'est fini

	CloseHandle(mSocketThread);

	// FERMETURE DU CLIENT
	if (!mRequestManager->Close())
		return 1;

	return 0; 
}

void ClientApp::EnterMutex()
{
	EnterCriticalSection(&mMutex);// pour bloquer un bloc d'instructions
}

void ClientApp::LeaveMutex()
{
	LeaveCriticalSection(&mMutex);// pour lib�rer le bloc
}

void ClientApp::Update()
{
	switch (mGame->mState)
	{
		case LOBBY:
			UpdateInLobby();
		break;
		case IN_GAME:
			UpdateInGame();
		break;
		case GAME_OVER:
			UpdateGameOver();
		break;
	}
}

void ClientApp::UpdateInLobby()
{
	auto event = mGame->GetEvent();
	while (mGame->mWindow->GetWindow()->pollEvent(*event))
	{
		if (mGame->IsPressEsc(event)) mGame->mWindow->GetWindow()->close();
		if (mGame->IsMouseClick(event))
		{
			EnterMutex();
			mRequestManager->JoinGame();
			LeaveMutex();
		}
	}
}

void ClientApp::UpdateInGame()
{
	auto event = mGame->GetEvent();
	while (mGame->mWindow->GetWindow()->pollEvent(*event))
	{
		if (mGame->IsPressEsc(event)) mGame->mWindow->GetWindow()->close();
		
		int x, y = -1;

		if (mGame->IsMouseClick(event) && mGame->IsMove(&x, &y))
		{
			EnterMutex();

			if (mRequestManager->IsMyTurn())
				mRequestManager->Play(x, y);

			LeaveMutex();
		}
	}
}

void ClientApp::UpdateGameOver()
{
	auto event = mGame->GetEvent();
	while (mGame->mWindow->GetWindow()->pollEvent(*event))
	{
		if (mGame->IsPressEsc(event)) mGame->mWindow->GetWindow()->close();

		if (mGame->IsMouseClick(event))
		{
			EnterMutex();

			mRequestManager->LeaveGame();

			LeaveMutex();
		}
	}
}

bool ClientApp::CreateSocketThread()
{
	mSocketThread = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		SocketThreadFunction,	// thread function name
		this,					// argument to thread function 
		CREATE_SUSPENDED,		// Attend l'appel de ResumeThread pour ex�cuter le thread
		NULL);					// returns the thread identifier 

	return true;
}

DWORD WINAPI ClientApp::SocketThreadFunction(LPVOID lpParam)
{
	ClientApp* pApp = (ClientApp*)lpParam;

	MSG msg = { 0 };

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			break;
	}

	return 0;
}

