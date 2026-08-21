# Engine Architecture Plan: Lili2D Boilerplate Reduction & Subsystem Roadmap

## Overview

**Lili2D** is a modern C++20 2D game engine built on SDL3 / SDL_GPU. It already features low-level primitives like windowing, SDL_GPU rendering, basic shapes, sprites/animated sprites, tilemaps, a custom ECS, scene management, and a C++20 thread pool.

To eliminate repetitive boilerplate across game projects, the engine can introduce higher-level, genre-agnostic subsystems.

---

## Proposed Subsystem Modules

Below is the categorized list of general 2D engine features designed to remove boilerplate code in game projects:

### 1. Asset & Resource Management (`lili::ResourceManager`)

Done (including hot-reloading for textures, fonts, atlases, and shaders with automatic graphics pipeline rebuilding). Hot-reloading can be enabled per-project via `Assets::setHotReloadEnabled(true)`.

---

### 2. Input Action & Mapping System (`lili::InputMap` / `lili::InputActionManager`)

- **Problem in game code:** Checking raw keycodes (e.g., `keyboard.isKeyDown(Key::W)`) throughout game logic, making remapping, gamepad support, and key bindings tedious.
- **Proposed Features:**
    - **Logical Action Binding:** Map virtual actions to physical keys/buttons (e.g., `"MoveUp" -> { Key::W, Key::Up, Gamepad::DpadUp }`).
    - **Action State Helpers:** Simple unified queries: `isActionJustPressed("Jump")`, `isActionDown("MoveRight")`, `getActionAxis("Horizontal")`.
    - **Input Contexts / Layers:** Switch input contexts seamlessly (e.g., Gameplay Mode vs. Menu Navigation Mode).

---

### 3. Audio Subsystem (`lili::AudioEngine` & `lili::SoundManager`)

- **Problem in game code:** Direct SDL sound calls or managing sound channels manually for SFX and music.
- **Proposed Features:**
    - **BGM & SFX Channels:** Separate volume control buses (`Master`, `Music`, `SFX`, `UI`).
    - **Spatial 2D Sound:** Automatically adjust panning and attenuation of SFX based on emitter location relative to the camera listener.
    - **Audio Utilities:** Fade-in, fade-out, crossfading background music tracks, and automatic channel pooling for overlapping SFX.

---

### 4. Timers, Tweens & Easing Utilities (`lili::Timer` & `lili::TweenManager`)

- **Problem in game code:** Implementing custom delta time accumulators for delays, cooldowns, smooth movement, fading, or UI scaling.
- **Proposed Features:**
    - **One-Shot & Periodic Timers:** `Timer::schedule(duration, callback, is_repeating)`.
    - **Tweening Engine (`lili::Tween`):** Smoothly interpolate numbers, `Vec2` positions, colors (`Vec4`), or scale over time with easing functions (Linear, EaseInOutQuad, Elastic, Bounce, etc.).
    - **Time Scale Support:** Global `time_scale` for pause, slow-motion bullet time, or fast-forward without breaking fixed updates.

---

### 5. Advanced 2D Camera Controls & Screen FX (`lili::Camera2D`)

- **Problem in game code:** Writing manual camera follow math, clamping camera position to map edges, or implementing screen shake logic per game.
- **Proposed Features:**
    - **Target Tracking & Lerp:** Smooth camera follow with configurable dampening, lead-ahead offsets, and deadzones (`camera.follow(targetPosition, smoothness)`).
    - **World Boundary Clamping:** Restrict camera movement within a bounding box (e.g., tilemap bounds).
    - **Screen Shake & Effects:** `camera.shake(intensity, duration)` and built-in screen transitions (fade-to-black, color wash).

---

### 6. Lightweight 2D UI System (`lili::ui::*`)

- **Problem in game code:** Hand-crafting buttons, text alignments, mouse hover detection, and UI layout positioning for menus and HUDs.
- **Proposed Features:**
    - **Core UI Widgets:** `Button`, `Label`, `Slider`, `CheckBox`, `Panel`.
    - **Responsive Anchors & Layouts:** Align elements by screen anchor points (`TopLeft`, `Center`, `BottomRight`) and auto-arrange using `VBoxContainer` / `HBoxContainer`.
    - **Widget State Events:** `onClick`, `onHover`, `onFocus`.

---

### 7. Movement Controllers & Physical Collision Resolution (`lili::CharacterController2D`)

- **Problem in game code:** Lili2D currently provides overlap checks (`intersect()`), but game code must manually calculate penetration depth, push players out of walls, handle wall sliding, slopes, and ground detection.
- **Proposed Features:**
    - **Solid Wall Collision & Resolution (Move-and-Slide):** Automatically resolve collisions by pushing entities out of walls/floors using Minimum Translation Vectors (MTV). Supports sliding along walls when moving diagonally.
    - **Tilemap Physical Collision:** Grid/AABB sweeping against solid tiles in `TileMap` so players don't clip through walls or catch on tile seams.
    - **Solid Bodies vs. Trigger Sensors:**
        - _Solid Colliders:_ Physical barriers that block movement (walls, platforms, obstacles).
        - _Trigger Colliders:_ Non-solid areas that allow passage but fire `onTriggerEnter(entity)` and `onTriggerExit(entity)` events (coin pickups, checkpoint lines, damage zones).
    - **Kinematic Character State Hooks:** `controller.isGrounded()`, `controller.isAgainstWall()`, `controller.isOnCeiling()`.
    - **Collision Filtering (Layers & Masks):** Bitmask matrix for layer collision filtering (`Player`, `Enemy`, `WorldSolid`, `TriggerZone`).

---

### 8. General 2D Particle System (`lili::ParticleSystem`)

- **Problem in game code:** Re-writing particle emitters for explosions, dust, sparks, trails, or floating text.
- **Proposed Features:**
    - **Configurable Emitter (`ParticleEmitter`):** Particle spawn rate, lifetime, initial speed/direction spread, size over time, color gradient over time, gravity influence, and texture.
    - **Floating Text Helper:** Quick API for dynamic popups like floating damage numbers, score pickups, or notifications.

---

### 9. Enhanced Scene Management Stack (`lili::SceneManager`)

- **Problem in game code:** Basic scene switching without support for pause menus overlaying game scenes or clean lifecycle hooks.
- **Proposed Features:**
    - **Scene Stack Operations:** `pushScene("PauseMenu")`, `popScene()`, keeping underlying scenes visible or paused.
    - **Lifecycle Callbacks:** `onEnter()`, `onExit()`, `onPause()`, `onResume()`.

---

### 10. Save System & Data Serialization (`lili::SaveSystem` / `lili::Config`)

- **Problem in game code:** Manually opening and parsing text files for game settings (volume, screen resolution) and save game state.
- **Proposed Features:**
    - **Key-Value & JSON Wrapper:** Simple getters/setters: `Config::setFloat("sound_volume", 0.8f)`, `SaveSystem::save("slot1.dat")`.
    - **Encryption / Hash Check (Optional):** Basic checksum to prevent casual save file tampering.

---

### 11. Debug Utilities & Visualization (`lili::DebugRenderer` / `lili::Profiler`)

- **Problem in game code:** Adding separate rendering loops to draw colliders, velocity vectors, or FPS counters for debugging.
- **Proposed Features:**
    - **Debug Overlay:** FPS counter, frame time graph, active entity count, memory usage, draw call counter.
    - **Debug Primitive Drawing:** Simple one-liner overlay functions: `Debug::drawBox(aabb, Color::Red)`, `Debug::drawLine(start, end, Color::Green)`.

---

## User Review Required

> [!IMPORTANT]
> Please review the 11 subsystem proposals above.
> Let me know which features you would like to include in **Lili2D**, or if you have specific preferences on their prioritization!
