#pragma once

//更新
void PlayerSword_Update();

//体力制のラスボス更新
void PlayerSword_LastUpdate();

//描画
void PlayerSword_Draw();

//体力制のラスボス描画
void PlayerSword_LastDraw();

//制限時間
void Limit();

void PlayerSword_R();

//位置を得る
int GetswordXpoint();
int GetswordYpoint();
bool AttackShield();
int GetPSattack();
bool GetTwoBGMStop();