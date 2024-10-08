#include <sstream>
#include <string>
#include <iostream>
#include "raylib.h"

void turnOne(Texture2D texture, double x, double y)
{
    DrawTexture(texture, x, y, WHITE);
}

void DrawTextInt(const char *text, int drawInt, int posX, int posY, int fontSize, Color color)
{
    // textとdrawIntを1つの文字列に結合
    char buffer[256]; // 必要に応じてサイズを調整
    snprintf(buffer, sizeof(buffer), "%s%d", text, drawInt);
    
    // raylibのDrawText関数を使って結合した文字列を描画
    DrawText(buffer, posX, posY, fontSize, color);
}

int main()
{
    const int screenWidth = 900;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "");
    SetTargetFPS(60);

    // オーディオデバイスを初期化
    InitAudioDevice();

    bool healthOF = true;
    int health = 5;

    bool debugMode = false;
    bool OneHP = false;

    double playerMoveSpeed = 5.0;

    bool turnOneAttack = true;

    bool shieldOF = true;
    double shieldDefault = 180;
    double shield = shieldDefault;

    double turn = 0.5;

    int flameColor = 1; // 1 = black  2 = purple  3 = red
    Color Purple = CLITERAL(Color){ 112, 31, 126, 255 };
    Color Black = CLITERAL(Color){ 0, 0, 0, 255 };
    Color darkRed = CLITERAL(Color){ 139, 0, 0, 255 };

    bool attack = false;
    int Onetime = 0;

    int outerFrameX = 300;
    int outerFrameY = 200;
    int innerFrameX = 305;
    int innerFrameY = 205;

    int outerFrameWidth = 250;
    int outerFrameHeight = 250;
    
    int innerFrameWidth = 240;
    int innerFrameHeight = 240;

    double playerMPDefault = 800.0;
    double playerMP = 800.0;

    double startTime = GetTime(); // 開始時刻を取得

    double bossHP = 1000.0;
    double bossHPDefault = 1000.0;
    double bossMPDefault = 1000.0;
    double bossMP = 114514.0;
    bool bossHPWatch = false;
    int nomalAttackDamage = 5;
    int nomalAttackShield = 3;

    double playerHPMax = 100.0;
    double playerHPDefault = 20.0;
    double playerHP = playerHPDefault;
    double playerPositionX = 410.0;
    double playerPositionY = 480.0;
    double playerLevel = 1;
// X 405 Y 305

    Sound damage = LoadSound("music/damage.mp3");
    Music deathBGM = LoadMusicStream("music/death.mp3");
    Music mainBGM = LoadMusicStream("music/final.mp3");
    Sound shieldBGM = LoadSound("music/shield.wav");
    float mainVolume = 20.0f;
    float damageVolume = 8.0f;

    SetMusicVolume(mainBGM, mainVolume);
    SetMusicVolume(deathBGM, mainVolume);
    SetSoundVolume(shieldBGM, damageVolume + 10);
    SetSoundVolume(damage, damageVolume);

    // 音楽の再生を開始
    PlayMusicStream(mainBGM);
    PlayMusicStream(deathBGM);
    // Play

    Image summonSatoOneTextureTexture = LoadImage("resources/hamster/dededon.png");
    Image summonSatoTwoTextureTexture = LoadImage("resources/hamster/waterBug.png");
    Image bossTextureTexture = LoadImage("resources/finalBossTexture/nomal.png");
    Image attackTextureTexture = LoadImage("resources/finalBossTexture/playerArmDot.png");
    Image playerTextureTexture = LoadImage("resources/Textures/player/playerImage.png");
    Image playerTextureGreenTexture = LoadImage("resources/Textures/Beef.png");

    ImageResize(&summonSatoOneTextureTexture, 100, 100);
    ImageResize(&summonSatoTwoTextureTexture, 100, 100);
    ImageResize(&bossTextureTexture, 900, 600);
    ImageResize(&attackTextureTexture, 50, 50);
    ImageResize(&playerTextureTexture, 18, 18);
    ImageResize(&playerTextureGreenTexture, 18, 18);

    Texture2D summonSatoOneTexture = LoadTextureFromImage(summonSatoOneTextureTexture);
    Texture2D summonSatoTwoTexture = LoadTextureFromImage(summonSatoTwoTextureTexture);
    Texture2D bossTexture = LoadTextureFromImage(bossTextureTexture);
    Texture2D attackTexture = LoadTextureFromImage(attackTextureTexture);
    Texture2D playerTexture = LoadTextureFromImage(playerTextureTexture);
    Texture2D playerTextureGreen = LoadTextureFromImage(playerTextureGreenTexture);

    Image bossSumonOneTextureTexture = LoadImage("resources/finalBossTexture/One.png");
    Image bossSumonTwoTextureTexture = LoadImage("resources/finalBossTexture/Two.png");
    Image bossSumonThreeTextureTexture = LoadImage("resources/finalBossTexture/Three.png");
    ImageResize(&bossSumonOneTextureTexture, 900, 600);
    ImageResize(&bossSumonTwoTextureTexture, 900, 600);
    ImageResize(&bossSumonThreeTextureTexture, 900, 600);
    Texture2D bossSumonOneTexture = LoadTextureFromImage(bossSumonOneTextureTexture);
    Texture2D bossSumonTwoTexture = LoadTextureFromImage(bossSumonTwoTextureTexture);
    Texture2D bossSumonThreeTexture = LoadTextureFromImage(bossSumonThreeTextureTexture);
    UnloadImage(bossSumonOneTextureTexture);
    UnloadImage(bossSumonTwoTextureTexture);
    UnloadImage(bossSumonThreeTextureTexture);

    // 不要なイメージデータを解放
    UnloadImage(summonSatoOneTextureTexture);
    UnloadImage(summonSatoTwoTextureTexture);
    UnloadImage(bossTextureTexture);
    UnloadImage(attackTextureTexture);
    UnloadImage(playerTextureTexture);
    UnloadImage(playerTextureGreenTexture);

    Rectangle playerRect = { static_cast<float>(playerPositionX), static_cast<float>(playerPositionY), 30, 30 };

    Rectangle turnOneAttackRect = { 410, 305, 50, 30 };
    Rectangle turnOneAttackRectReset = { 410, 305, 50, 30 };

    double damageCooldown = 0;  // プレイヤーのダメージクールダウンタイマー
    double invincibilityTime = 0.5;  // ダメージを受けた後の無敵時間（0.4秒）

    double shieldCooldown = 0;  // プレイヤーshieldのダメージクールダウンタイマー
    double shieldInvincibilityTime = 0.0;  // shield後の無敵時間（0.4秒）

    while (!WindowShouldClose())
    {
        playerRect.x = playerPositionX;
        playerRect.y = playerPositionY;
        double damageTime = GetTime(); // 現在の時刻を取得
        // クールダウンタイマーを時間経過で減少させる
        if (damageCooldown > 0)
        {
            damageCooldown -= GetFrameTime();  // フレーム時間に応じてクールダウンを減少
        }

        if (playerHP > playerHPDefault)
        {
            playerHP = playerHPDefault;
        }
        if (playerHP < 1)
        {
            if (debugMode)
            {
                playerHP = playerHPDefault;
            }
            else if (!debugMode)
            {
                BeginDrawing();

                ClearBackground(BLACK);

                StopMusicStream(mainBGM);
                UpdateMusicStream(deathBGM);
                DrawText("Game Over", 180, 80, 100, WHITE);

                EndDrawing();
            }
        }
        else if (bossHP < 0)
        {
            BeginDrawing();

            ClearBackground(BLACK);

            UpdateMusicStream(deathBGM);
            DrawText("You win!", 180, 80, 100, WHITE);
            DrawText("Fuck you!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", 180, 200, 30, WHITE);

            EndDrawing();
        }
        else
        {
            UpdateMusicStream(mainBGM);
            double currentTime = GetTime(); // 現在の時刻を取得
            // プレイヤーの位置を更新
            if (IsKeyDown(KEY_UP)) playerPositionY -= playerMoveSpeed;
            if (IsKeyDown(KEY_DOWN)) playerPositionY += playerMoveSpeed;
            if (IsKeyDown(KEY_RIGHT)) playerPositionX += playerMoveSpeed;
            if (IsKeyDown(KEY_LEFT)) playerPositionX -= playerMoveSpeed;
            if (IsKeyDown(KEY_X)) playerMoveSpeed = 1;
            else if (IsKeyUp(KEY_X)) playerMoveSpeed = 5;

            // UpdateMusicStream(damageBGM);
            // 当たり判定
            if (shieldOF)
            {
                if (IsKeyDown(KEY_C))
                {
                    if (shield > 0)
                    {
                        if (CheckCollisionRecs(playerRect, turnOneAttackRect))
                        {
                            if (currentTime > 7)
                            {
                                PlaySound(shieldBGM);
                                shield -= nomalAttackShield;
                                shieldCooldown = shieldInvincibilityTime;  // クールダウンをリセット
                            }
                        }
                    }
                    if (!shieldOF)
                    {
                        if (damageCooldown <= 0)  // クールダウンが0以下の場合のみダメージを適用
                        {
                            if (CheckCollisionRecs(playerRect, turnOneAttackRect))
                            {
                                if (currentTime > 7 && currentTime < 10)
                                PlaySound(damage);
                                playerHP -= nomalAttackDamage;
                                damageCooldown = invincibilityTime;  // クールダウンをリセット
                            }
                        }
                    }
                    else if (shield < 0)
                    {
                        if (damageCooldown <= 0)  // クールダウンが0以下の場合のみダメージを適用
                        {
                            if (CheckCollisionRecs(playerRect, turnOneAttackRect))
                            {
                                if (currentTime > 7 && currentTime < 10)
                                {
                                    PlaySound(damage);
                                    playerHP -= nomalAttackDamage;
                                    damageCooldown = invincibilityTime;  // クールダウンをリセット
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (damageCooldown <= 0)  // クールダウンが0以下の場合のみダメージを適用
                    {
                        if (CheckCollisionRecs(playerRect, turnOneAttackRect))
                        {
                            if (currentTime > 7 && currentTime < 10)
                            {
                                PlaySound(damage);
                                playerHP -= nomalAttackDamage;
                                damageCooldown = invincibilityTime;  // クールダウンをリセット
                            }
                        }
                    }
                }
            }
            else
            {
                if (damageCooldown <= 0)  // クールダウンが0以下の場合のみダメージを適用
                {
                    if (CheckCollisionRecs(playerRect, turnOneAttackRect))
                    {
                        if (currentTime > 7 && currentTime < 10)
                        {
                            PlaySound(damage);
                            playerHP -= nomalAttackDamage;
                            damageCooldown = invincibilityTime;  // クールダウンをリセット
                        }
                    }
                }
            }

            if (shieldOF)
            {
                if (IsKeyDown(KEY_C))
                {
                    if (shield < 0)
                    {}
                    else
                    {
                        shield -= 2.8;
                    }
                }
            }
            if (IsKeyUp(KEY_C))
            {
                if (shield == shieldDefault)
                {}
                else if (shield < shieldDefault)
                {
                    shield += 1.8;
                }
            }
            
            BeginDrawing();
            ClearBackground(BLACK);

            if (currentTime > 2) DrawTexture(bossSumonOneTexture, 0, 0, WHITE);
            if (currentTime > 4) DrawTexture(bossSumonTwoTexture, 0, 0, WHITE);
            if (currentTime > 5) DrawTexture(bossSumonThreeTexture, 0, 0, WHITE);
            if (currentTime > 6) DrawTexture(bossTexture, 0, 0, WHITE);
            std::cout << currentTime << "\n";

            if (currentTime > 7 && currentTime < 10)
            {
                turnOne(attackTexture, turnOneAttackRect.x, turnOneAttackRect.y);
                turnOneAttackRect.y += 5;
            }
            else turnOneAttackRect = turnOneAttackRectReset;

            DrawRectangle(screenWidth - 875, screenHeight - 60, playerHPDefault * 5, 28, darkRed);
            DrawRectangle(screenWidth - 875, screenHeight - 60, playerHP * 5, 28, DARKPURPLE);

            if (shieldOF)
            {
                DrawRectangle(screenWidth - 200, screenHeight - 60, shieldDefault, 28, darkRed);
                DrawRectangle(screenWidth - 200, screenHeight - 60, shield, 28, GREEN);
            }

            DrawTexture(playerTexture, playerPositionX, playerPositionY, WHITE);

            // プレイヤーと攻撃の矩形を更新
            playerRect.x = playerPositionX;
            playerRect.y = playerPositionY;

            if (healthOF)
            {
                DrawTextInt("Health : ", health, 680, 20, 40, WHITE);
                if (health == 0)
                {}
                else
                {
                    if(IsKeyPressed(KEY_SPACE))
                    {
                        health -= 1;
                        playerHP += 50;
                    }
                }
            }

            if (IsKeyPressed(KEY_D))
            {
                if (debugMode == true) debugMode = false;
                else if (debugMode == false) debugMode = true;
            }
            if (debugMode)
            {
                DrawText("DEBUG", 10, 10, 40, WHITE);
                DrawText(TextFormat("FPS: %i", GetFPS()), 10, 100, 50, RED);
                if (IsKeyPressed(KEY_S))
                {
                    if (shieldOF) shieldOF = false;
                    else if (!shieldOF) shieldOF = true;
                }
                if (IsKeyPressed(KEY_H))
                {
                    if (healthOF) healthOF = false;
                    else if (!healthOF) healthOF = true;
                }
                if (IsKeyPressed(KEY_J)) playerHP -= 1;
                if (IsKeyPressed(KEY_L)) playerHP += 1;
            }
            else if (debugMode == false) DrawText("", 10, 10, 40, WHITE);

            if (IsKeyPressed(KEY_O)) if (!OneHP) OneHP = true;
            if (OneHP)
            {
                playerHPDefault = 1;
                shieldOF = false;
            }
            EndDrawing();
        }
    }

    // オーディオデバイスを閉じる
    CloseAudioDevice();

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
// 43
