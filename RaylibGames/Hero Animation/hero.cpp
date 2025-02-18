#include <raylib.h>
#include <vector>

const int W = 600;
const int H = 600;

enum Direction {
  LEFT = -1,
  RIGHT = 1,
};

enum CurrentState {
  DEAD = 0,
  RUNNING = 1,
  IDLE = 2,
  ROLLING = 3,
};

enum AnimationType {
  REPEATING,
  ONESHOT
};

struct Animation {
  int fst;
  int lst;
  int cur;
  int offset;
  float spd;
  float rem;
  AnimationType type;
};

struct Player {
  Rectangle rect;
  Vector2 vel;
  Texture2D sprite;
  Direction dir;
  CurrentState state;
  std::vector<Animation> animations;
};

void update_animation(Animation *self) {
  float dt = GetFrameTime();
  self->rem -= dt;
  if (self->rem < 0) {
    self->rem = self->spd;
    self->cur++;
    if (self->cur > self->lst) {
      switch (self->type) {
      case REPEATING:
        // Repeating
        self->cur = self->fst;
        break;
      case ONESHOT:
        self->cur = self->lst;
        break;
      }
    }
  }
}

Rectangle animation_frame(const Animation *self) {
  int x = (self->cur % (self->lst + 1)) * 16;
  int y = self->offset * 16;
  return (Rectangle) {
    .x = (float)x, .y = (float)y, .width = 16.0f, .height = 16.0f
  };
}

void drawPlayer(const Player *player) {
  //DrawTexture(player->sprite, player->rect.x, player->rect.y, WHITE);
  Rectangle source = animation_frame(&(player->animations[player->state]));
  source.width = source.width * static_cast<float>(player->dir);
  DrawTexturePro(player->sprite, source, player->rect, {0, 0}, 0.0f, WHITE);
}

void movePlayer(Player *player) {
  player->vel.x = 0.0f;
  player->vel.y = 0.0f;
  player->state = IDLE;
  if (IsKeyDown(KEY_A)) {
    player->vel.x = -200.0f;
    player->dir = LEFT;
    player->state = RUNNING;
  } else if (IsKeyDown(KEY_D)) {
    player->vel.x = 200.0f;
    player->dir = RIGHT;
    player->state = RUNNING;
  }
  if (IsKeyDown(KEY_SPACE)) {
    player->vel.x = static_cast<float>(player->dir) * 300.0f;
    player->state = ROLLING;
  }
}

void moveRectByVel(Rectangle *rect, const Vector2 *vel) {
  rect->x += vel->x * GetFrameTime();
  rect->y += vel->y * GetFrameTime();
}

int main() {
  InitWindow(W, H, "Hero Animation Example");

  Texture2D hero = LoadTexture("assets/herochar-sprites/herochar_spritesheet.png");

  Player player = Player{.rect = (Rectangle){
    .x = W / 2.0f,
    .y = H / 2.0f,
    .width = 64.0f,
    .height = 64.0f},
    .vel = (Vector2){.x = 0.0f, .y = 0.0f},
    .sprite = hero,
    .dir = RIGHT,
    .state = IDLE,
    .animations = {
        (Animation) {
            .fst = 0,
            .lst = 7,
            .cur = 0,
            .offset = 0,
            .spd = 0.1f,
            .rem = 0.1f,
            .type = ONESHOT,
        },
        (Animation) {
            .fst = 0,
            .lst = 5,
            .cur = 0,
            .offset = 1,
            .spd = 0.1f,
            .rem = 0.1f,
            .type = REPEATING,
        },
        (Animation){
            .fst = 0,
            .lst = 3,
            .cur = 0,
            .offset = 5,
            .spd = 0.1f,
            .rem = 0.1f,
            .type = REPEATING,
        },
        (Animation){
            .fst = 0,
            .lst = 2,
            .cur = 0,
            .offset = 9,
            .spd = 0.1f,
            .rem = 0.1f,
            .type = REPEATING,
        }
    }
};

  while (!WindowShouldClose()) {
    // Update
    movePlayer(&player);
    moveRectByVel(&(player.rect), &(player.vel));
    update_animation(&(player.animations[player.state]));
    // Drawing
    BeginDrawing();
    ClearBackground(SKYBLUE);
    drawPlayer(&player);
    EndDrawing();
  }

  UnloadTexture(hero);
  CloseWindow();
  return 0;
}