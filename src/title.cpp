#include "raylib.h"

int main(void)
{
    // ウィンドウの初期化
    InitWindow(900, 600, "Sato who came to let us know of his love");

    // 背景画像のロード
    Texture background = LoadTexture("resources/Satos/Title/TitleSato.jpg");  // 背景画像のパス

    // メインループ
    while (!WindowShouldClose())
    {
        // 描画の開始
        BeginDrawing();

        // 背景色の設定（画像が描画される前に）
        ClearBackground(RAYWHITE);

        // 画像を描画（画面全体に表示するため、画像のサイズをウィンドウサイズに合わせる）
        DrawTexture(background, 0, 0, WHITE);


        // 描画の終了
        EndDrawing();
    }

    // 画像のアンロード
    UnloadTexture(background);

    // ウィンドウを閉じる
    CloseWindow();

    return 0;
}
