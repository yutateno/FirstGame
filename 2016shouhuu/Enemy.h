#pragma once

//画像の読み込み
void Enemy_Read();

//更新
void Enemy_Update();

//一人用の更新
void ENEMY_Update();

//描画
void Enemy_Draw();

//一人用の描画
void ENEMY_Draw();

//ラスボスの描画
void LastEnemy_Draw();

//一人用のラスボスの描画
void LastENEMY_Draw();

void Enemy_R();

//攻撃が通った
int EAttackHit();

//何体目か
int EnemyBreak();

//罠で盾に攻撃
int PShATTACKHIT();

int GetYAttackHit();