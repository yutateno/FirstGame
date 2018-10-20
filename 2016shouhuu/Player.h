#pragma once

//更新
void Player_Update();

//ラスボスの更新
void Player_LastUpdate();

//描画
void Player_Draw();

//最後の描画
void Player_LastDraw();

//制限時間
void LIMIT();

//void HEART();

void Player_R();

int GetPlayerXpoint();

int GetPattack();

int GetPlayerYpoint();

bool GetOneBGMStop();

//攻撃速度上昇を使っているかどうか
bool GetRabitUseFlag();

//攻撃速度上昇を使っているかどうか
bool GetLastRabitUseFlag();