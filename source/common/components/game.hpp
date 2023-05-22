#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {
  // This component denotes that the PhyscisSystem will describe the owning entity by a certain physcis effects like gravity, collision, etc.
  class GameComponent : public Component {
  public:
    int health = 100;
    int score = 0;
    int level = 1;
    int lives = 3;
    bool isDead = false;
    bool isWin = false;
    bool isPaused = false;
    bool isStarted = false;
    bool isGameOver = false;
    bool isGameWin = false;
    bool isGameLost = false;

    // The ID of this component type is "Physics"
    static std::string getID() { return "Physics"; }

    // Reads position, velocity, mass, friction, and isStatic from the given json object
    void deserialize(const nlohmann::json& data) override;
};
}