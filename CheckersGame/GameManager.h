﻿#pragma once

class SFMLWindow;
class Grid;
struct Tile;


enum GAME_STATE
{
    IDLE,
    SELECTING,
    PLAYING,
};


class GameManager
{
    int mPlayerTurn;
    bool mForceEat;

    GAME_STATE mCurrentState;
    
    SFMLWindow* mMainWindow;
    Grid* mMainGrid;
    Tile* mPieceSelected;
    

public:
    static GameManager* mInstance;
    
    static GameManager* GetInstance();

    GameManager();
    ~GameManager() = default;

    void InitGame() const;

    void RunGame();

    void OnClick();

    void DrawPlayerTurn() const;

    void RenderGame() const;
};
