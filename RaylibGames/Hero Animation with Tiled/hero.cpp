#include <raylib.h>
#include <vector>
#include <format>
#define RAYTMX_IMPLEMENTATION
#include "raytmx.h"

const int W = 600;
const int H = 600;
const float MAX_GRAV = 300.0f;

enum Direction {
  LEFT = -1,
  RIGHT = 1,
};

enum CurrentState {
  DEAD = 0,
  RUNNING = 1,
  IDLE = 2,
  ROLLING = 3,
  JUMPING = 4,
  FALLING = 5,
  ATTACKING = 6
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
  int width;
  int height;
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
  int x = (self->cur % (self->lst + 1)) * self->width;
  int y = self->offset * self->height;

  return (Rectangle) {
    .x = (float)x,
    .y = (float)y,
    .width = (float)self->width,
    .height = (float)self->height
  };
}

void drawPlayer(const Player *player) {
  //DrawTexture(player->sprite, player->rect.x, player->rect.y, WHITE);
  Rectangle source = animation_frame(&(player->animations[player->state]));
  source.width = source.width * static_cast<float>(player->dir);
  DrawTexturePro(player->sprite,
                 source,
                 player->rect,
                 {0, 0},
                 0.0f,
                 WHITE);
}

void movePlayer(Player *player) {
  player->vel.x = 0.0f;
  player->vel.y = 0.0f;
  bool changedState = false;
  if (IsKeyDown(KEY_A)) {
    player->vel.x = -200.0f;
    player->dir = LEFT;
    player->state = RUNNING;
    changedState = true;
  } else if (IsKeyDown(KEY_D)) {
    player->vel.x = 200.0f;
    player->dir = RIGHT;
    player->state = RUNNING;
    changedState = true;
  }
  if (IsKeyDown(KEY_SPACE)) {
    player->vel.y = -250.0f;
    player->state = JUMPING;
    changedState = true;
  } else if (player->vel.y > 0) {
    player->state = FALLING;
    changedState = true;
  }
  if (IsKeyDown(KEY_ENTER)) {
    player->state = ATTACKING;
    changedState = true;
  }

  // if (IsKeyDown(KEY_SPACE)) {
  //   player->vel.x = static_cast<float>(player->dir) * 300.0f;
  //   player->state = ROLLING;
  // }
  if (changedState == false) {
    player->state = IDLE;
  }
}

void applyGravity(Vector2 *vel) {
  vel->y += 100.0f;
}

void moveRectByVel(Rectangle *rect, const Vector2 *vel) {
  rect->x += vel->x * GetFrameTime();
  rect->y += vel->y * GetFrameTime();
}

void keepPlayerInScreen(Player *player) {
  if (player->rect.y > (H - player->rect.height)) {
    player->vel.y = 0.0;
    player->rect.y = (H - player->rect.height);
  }
}

void checkTileCollisions(TmxMap *map, Player *player) {
  for (unsigned int i = 0; i < map->layersLength; i++) {
    TraceLog(LOG_DEBUG, "current layer is %d: %s", i, map->layers[i].name);
    if (strcmp(map->layers[i].name, "collisions") == 0 && map->layers[i].type == LAYER_TYPE_OBJECT_GROUP) {
      TmxObject* col;
      if (CheckCollisionTMXObjectGroupRec(map->layers[i].exact.objectGroup,
                                          player->rect,
                                          col)) {
        TraceLog(LOG_DEBUG, "We've made contact!");
        player->vel.y = 0.0f;
        player->rect.y = (col->aabb.y - player->rect.height);
        // player->rect.y = (H - player->rect.height);
      }
    }
  }
  return;
}

void cameraFollow(Camera2D *camera, const Player *player) {
  camera->target = (Vector2) {
    .x = player->rect.x,
    .y = player->rect.y,
  };
}

int main() {
  InitWindow(W, H, "Hero Animation Example");

  const char* tmx = "resources/map.tmx";
  TmxMap* map = LoadTMX(tmx);
  if (map == nullptr) {
    TraceLog(LOG_ERROR, "couldn't load da map: %s", tmx);
    return EXIT_FAILURE;
  }

  Texture2D hero = LoadTexture("assets/herochar-sprites/herochar_spritesheet.png");

  Player player = Player{.rect = (Rectangle){.x = 16,
                                             .y = 500,
                                             .width = 64.0f,
                                             .height = 64.0f},
                         .vel = (Vector2){.x = 0.0f, .y = 0.0f},
                         .sprite = hero,
                         .dir = RIGHT,
                         .state = IDLE,
                         .animations = {(Animation){
                                            .fst = 0,
                                            .lst = 7,
                                            .cur = 0,
                                            .offset = 0,
                                            .width = 16,
                                            .height = 16,
                                            .spd = 0.1f,
                                            .rem = 0.1f,
                                            .type = ONESHOT,
                                        },
                                        (Animation){
                                            .fst = 0,
                                            .lst = 5,
                                            .cur = 0,
                                            .offset = 1,
                                            .width = 16,
                                            .height = 16,
                                            .spd = 0.1f,
                                            .rem = 0.1f,
                                            .type = REPEATING,
                                        },
                                        (Animation){
                                            .fst = 0,
                                            .lst = 3,
                                            .cur = 0,
                                            .offset = 5,
                                            .width = 16,
                                            .height = 16,
                                            .spd = 0.1f,
                                            .rem = 0.1f,
                                            .type = REPEATING,
                                        },
                                        (Animation){
                                            .fst = 0,
                                            .lst = 2,
                                            .cur = 0,
                                            .offset = 9,
                                            .width = 16,
                                            .height = 16,
                                            .spd = 0.1f,
                                            .rem = 0.1f,
                                            .type = REPEATING,
                                        },
                                        (Animation){
                                            .fst = 0,
                                            .lst = 2,
                                            .cur = 0,
                                            .offset = 7,
                                            .width = 16,
                                            .height = 16,
                                            .spd = 0.1f,
                                            .rem = 0.1f,
                                            .type = REPEATING,
                                        },
                                        (Animation){
                                            .fst = 0,
                                            .lst = 2,
                                            .cur = 0,
                                            .offset = 6,
                                            .width = 16,
                                            .height = 16,
                                            .spd = 0.1f,
                                            .rem = 0.1f,
                                            .type = REPEATING,
                                        },
                                        (Animation){
                                            .fst = 0,
                                            .lst = 3,
                                            .cur = 0,
                                            .offset = 6,
                                            .width = 32,
                                            .height = 16,
                                            .spd = 0.15f,
                                            .rem = 0.15f,
                                            .type = ONESHOT,
                                        },
                         }};

  Camera2D camera = (Camera2D){
    .offset = (Vector2){
      .x = W / 2.0f,
      .y = H / 2.0f,
    },
    .target = (Vector2){
      .x = W / 2.0f,
      .y = H / 2.0f,
    },
    .rotation = 0.0f,
    .zoom = 1.0f,
  };

  while (!WindowShouldClose()) {
    // Update
    AnimateTMX(map);
    movePlayer(&player);
    applyGravity(&(player.vel));
    moveRectByVel(&(player.rect), &(player.vel));
    checkTileCollisions(map, &player);
    keepPlayerInScreen(&player);
    update_animation(&(player.animations[player.state]));
    cameraFollow(&camera, &player);
    // Drawing
    BeginDrawing();
    {
      ClearBackground(SKYBLUE);
      BeginMode2D(camera);
      DrawTMX(map, &camera, 0, 0, WHITE);
      drawPlayer(&player);
      EndMode2D();
      DrawFPS(5, 5);
      DrawText(std::format("(x: {:0.0f}, y: {:0.0f})",
                           player.rect.x, player.rect.y)
               .c_str(),
               5, 550, 32, RED);
    }
    EndDrawing();
  }

  UnloadTMX(map);
  UnloadTexture(hero);
  CloseWindow();
  return 0;
}