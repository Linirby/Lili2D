# Lili2D Engine Architecture & Systems Walkthrough

Welcome to the technical architectural overview of **Lili2D**, a high-performance 2D game engine built with **C++20** and **SDL3 / SDL_GPU**.

This document details the core system designs, concurrency patterns, memory layouts, transform pipelines, and GPU optimization strategies implemented across the engine. It serves as an in-depth reference for engine architects, technical leads, and developers.

---

## Table of Contents

| # | Subsystem / Topic | Key Focus & Patterns |
| :-: | :--- | :--- |
| **1** | [Engine Core & The Game Loop](#1-engine-core--the-game-loop) | Fixed-timestep simulation, render interpolation (alpha), RAII subsystem ownership. |
| **2** | [Priority-Scheduled Multithreading (`ThreadPool`)](#2-priority-scheduled-multithreading-threadpool) | C++20 `std::jthread` workers, cooperative cancellation, 3-tier task priority queues. |
| **3** | [Data-Oriented Entity Component System (ECS)](#3-data-oriented-entity-component-system-ecs) | Contiguous `ComponentPool<T>` storage, custom iterator `ECSView`, deferred `CommandBuffer`. |
| **4** | [Unified Scoped Asset & Resource Engine](#4-unified-scoped-asset--resource-engine) | Polymorphic `ResourceManager<T>`, tag-based scope memory isolation, static facade. |
| **5** | [Live Asset & Shader Hot-Reloading Architecture](#5-live-asset--shader-hot-reloading-architecture) | Filesystem timestamp watcher, observer pipeline rebuilding, runtime safety guards. |
| **6** | [UI Layout Engine & Matrix Transform Pipeline](#6-ui-layout-engine--matrix-transform-pipeline) | Normalized Anchor/Pivot layout math, 3x3 affine transforms, inverse matrix hit tests. |
| **7** | [Virtual Viewport Scaling & Logical Resolution](#7-virtual-viewport-scaling--logical-resolution) | Display-independent aspect ratio letterboxing, physical-to-logical coordinate mapping. |
| **8** | [Spatial Physics & Collision Query Subsystem](#8-spatial-physics--collision-query-subsystem) | `AABB2`, `AABB3`, `CircleCollider`, narrow-phase clamping math, zero-alloc debug draw. |
| **9** | [Hardware Rendering & GPU Memory Optimizations](#9-hardware-rendering--gpu-memory-optimizations) | Frustum culling, dynamic `SpriteBatch`, progressive chunk budget, GPU idle barriers. |
| **10** | [Custom Shader Cross-Compilation & Dynamic Uniform Buffering](#10-custom-shader-cross-compilation--dynamic-uniform-buffering) | HLSL -> SPIR-V -> Driver shader transpilation, direct command buffer uniform push. |
| **11** | [Input Action Mapping Subsystem (`ActionMap`)](#11-input-action-mapping-subsystem-actionmap) | String-keyed logical action bindings, tri-state input polling (`held`, `justPressed`, `justReleased`). |
| **12** | [Chunk-Based 3D Grid Tilemap & Asynchronous Meshing](#12-chunk-based-3d-grid-tilemap--asynchronous-meshing) | `Point3` spatial chunk maps, multithreaded vertex/index baking, layer collision sweep. |
| **13** | [Frame Animation Pipeline & Sprite Slicing (`AtlasMap`, `AnimationPlayer`)](#13-frame-animation-pipeline--sprite-slicing-atlasmap-animationplayer) | UV grid coordinate slicing, frame accumulator playback, frame-specific event hooks. |
| **14** | [Modern C++ Standards, Compile-Time Optimizations & Calling Conventions](#14-modern-c-standards-compile-time-optimizations--calling-conventions) | Pass-by-value (<= 16B PODs), ABI registers, `constexpr` constructors, `[[nodiscard]]` triggers, `noexcept` move guarantees, header inlining. |

---

## 1. Engine Core & The Game Loop

Lili2D centers around a synchronized fixed-timestep game loop that decouples deterministic physics updates (TPS) from variable rendering framerates (FPS) using an accumulator pattern.

```mermaid
graph TD
    A[Engine Init] --> B[Game Loop Start]
    B --> C[Poll Input / SDL Events]
    C --> D[Calculate Delta Time]
    D --> E{Accumulator >= Fixed DT?}
    E -- Yes --> F[onFixedUpdate / Physics & ECS]
    F --> G[Consume Accumulator Time]
    G --> E
    E -- No --> H[onUpdate / Gameplay Logic]
    H --> I[Interpolate Alpha]
    I --> J[onRender / Submit Draw Calls]
    J --> K[Renderer Swapchain Present]
    K --> B
```

### Core Architecture Highlights

* **Decoupled Simulation & Render Interpolation**: Subclasses of `lili::Game` implement `onFixedUpdate()` for tick-rate deterministic state updates, and `onRender(alpha)` for smooth, frame-rate independent visual interpolation.
* **RAII & Encapsulated Subsystems**: Low-level subsystem lifecycles (`Window`, `Renderer`, `ThreadPool`, `SceneManager`) are strictly owned by `Game`. Subsystem handles are exposed via read-only accessors to protect state invariants.
* **Global Runtime Bridge (`GameConfig`)**: Provides a centralized state manager for runtime window mode toggles (fullscreen, borderless, resizable, relative mouse) synced safely back to the active `Game` instance.

---

## 2. Priority-Scheduled Multithreading (`ThreadPool`)

To maximize multi-core CPU utilization while eliminating frame-pacing stutter, Lili2D provides a custom C++20 task scheduler (`lili::ThreadPool`) featuring explicit **task prioritization**.

### Concurrency Mechanics

* **Fixed Thread Allocation**: Worker threads default to `std::thread::hardware_concurrency() - 1`, preserving one dedicated core for OS scheduling and the main engine loop.
* **RAII Lifecycle & Cooperative Cancellation**: Worker threads run via `std::jthread` and monitor `std::stop_token` for clean thread-group tear-downs.
* **Priority-Based Task Queues**: Tasks are submitted with a `TaskPriority` enum (`HIGH`, `NORMAL`, `LOW`):
  * **HIGH**: Frame-critical tasks (parallel ECS physics, collision dispatch, camera updates).
  * **NORMAL**: Asynchronous asset decoding, geometry processing.
  * **LOW**: Background world generation, tilemap chunk mesh baking.

```mermaid
graph LR
    Sub[Task Submission] --> P{Priority?}
    P -- HIGH --> HQ[High Priority Queue]
    P -- NORMAL --> NQ[Normal Priority Queue]
    P -- LOW --> LQ[Low Priority Queue]
    HQ --> W[Worker Thread Pool]
    NQ --> W
    LQ --> W
```

```cpp
// Task selection routine executed by worker threads
if (!high_tasks.empty()) {
    task = std::move(high_tasks.front());
    high_tasks.pop();
} else if (!normal_tasks.empty()) {
    task = std::move(normal_tasks.front());
    normal_tasks.pop();
} else if (!low_tasks.empty()) {
    task = std::move(low_tasks.front());
    low_tasks.pop();
}
```

---

## 3. Data-Oriented Entity Component System (ECS)

Lili2D implements a cache-friendly Entity Component System (`lili::ECSRegistry`) designed around contiguous memory layouts and data-oriented design (DOD).

### Contiguous Memory Allocation (`ComponentPool<T>`)

Components of type `T` are stored sequentially in tightly packed arrays (`std::vector<T>`). When ECS systems execute, CPU L1/L2 data caches prefetch contiguous component slices, eliminating pointer-chasing and cache miss penalties inherent to traditional OOP hierarchies.

```txt
[ComponentPool<Position>] -> [ Pos0 ][ Pos1 ][ Pos2 ][ Pos3 ]  <-- (Packed Contiguous Block)
[ComponentPool<Velocity>] -> [ Vel0 ][ Vel1 ][ Vel2 ][ Vel3 ]  <-- (Packed Contiguous Block)
```

### Multi-Component Query Engine & Custom Iterator (`ECSView`)

A core challenge in Entity Component Systems is executing queries across entities possessing an arbitrary combination of components (e.g., all entities with both `PositionComponent`, `VelocityComponent`, and `RenderComponent`) with zero heap allocation and maximal CPU cache utilization.

Lili2D solves this with `lili::ECSView<Components...>`, a zero-overhead view pipeline backed by a specialized custom C++20 iterator (`ECSView::Iterator`).

```mermaid
graph TD
    V["ECSView Construction (registry.view<T...>())"] --> LP["Identify Smallest Pool: 'lead_pool'"]
    LP --> ItInit["Iterator::begin() (index = 0)"]
    ItInit --> FV["find_valid(): Inspect Current Lead Entity"]
    FV --> Chk{"All other pools have Entity?"}
    Chk -- No --> Inc["Increment index in lead pool"]
    Inc --> Bounds{"index >= lead_pool.size()?"}
    Bounds -- No --> FV
    Bounds -- Yes --> End["Reached Iterator::end()"]
    Chk -- Yes --> Ready["Iterator Positioned at Valid Entity"]
    Ready --> Deref["operator*(): Return std::tuple<Entity, Components&...>"]
    Deref --> User["User System Logic (Structured Binding)"]
    User --> Next["operator++(): Advance index & find_valid()"]
    Next --> Inc
```

#### 1. Smallest Pool Heuristic ("Lead Pool")

When constructing an `ECSView`, the view inspects the registered `ComponentPool<T>` for every requested component type using C++17/20 fold expressions:

```cpp
explicit ECSView(ECSRegistry& registry)
    : pool_ptrs(&registry.getPool<Components>()...) {
    std::apply(
        [this](ComponentPool<Components>*... pools) {
            auto inspect = [this](const IComponentPool* pool) {
                if (!lead_pool || pool->size() < lead_pool->size())
                    lead_pool = pool;
            };
            (inspect(pools), ...);
        },
        pool_ptrs
    );
}
```

* **Mathematical Rationale**: The set of matching entities (`E_match`) is an intersection across all requested component pools:
  ```txt
  E_match = E_C1 ∩ E_C2 ∩ ... ∩ E_Cn
  ```
  Therefore:
  ```txt
  count(E_match) <= min(count(E_C1), count(E_C2), ..., count(E_Cn))
  ```
* By driving iteration strictly along the smallest pool (`lead_pool`), the iteration candidate space is minimized. For example, if there are 10,000 entities with `PositionComponent` and only 15 with `PlayerInputComponent`, the iterator only tests 15 candidates rather than looping through 10,000 items.

#### 2. Custom Forward Iterator Architecture (`ECSView::Iterator`)

The iterator conforms to standard `std::forward_iterator_tag` requirements and encapsulates:
* A `std::tuple` of raw pool pointers (`pool_ptrs`).
* A direct raw pointer to the lead pool's entity array (`lead_entities = lead_pool->getEntities().data()`), bypassing vector accessor overhead in inner loops.
* The current index within the lead pool and the current `Entity` handle.

#### 3. Short-Circuit Filtering (`find_valid`)

Whenever the iterator is initialized or advanced (`operator++`), it invokes `find_valid()`:

```cpp
void find_valid() {
    while (index < max_size) {
        current_entity = lead_entities[index];
        bool valid = std::apply(
            [this](ComponentPool<Components>*... pool) {
                return (
                    ((pool == lead_pool) || pool->has(current_entity)) &&
                    ...
                );
            },
            pool_ptrs
        );
        if (valid) return;
        ++index;
    }
}
```

* **Fold-Expression Short-Circuiting**: The fold expression `((pool == lead_pool || pool->has(current_entity)) && ...)` evaluates with boolean short-circuit behavior. If any component pool does not contain `current_entity`, subsequent pool checks are skipped immediately.
* **Lead Pool Fast-Path**: The identity test `pool == lead_pool` evaluates to `true` for the driving pool, completely avoiding redundant sparse-set lookups for the component whose pool is currently being traversed.

#### 4. Zero-Copy Tuple Dereferencing & Structured Binding (`operator*`)

When dereferencing (`operator*`), `ECSView::Iterator` returns references directly to the contiguous component storage:

```cpp
[[nodiscard]] reference operator*() const {
    return std::apply(
        [this](ComponentPool<Components>*... pools) {
            auto get_comp = [this](auto* pool) -> decltype(auto) {
                if (pool == lead_pool) {
                    return pool->getComponents()[index]; // Direct O(1) contiguous index
                }
                return pool->get(current_entity);        // Fast sparse lookup
            };
            return std::tuple<Entity, Components&...>(
                current_entity, get_comp(pools)...
            );
        },
        pool_ptrs
    );
}
```

* **Direct Array Indexing for Lead Component**: For the `lead_pool`, the component is accessed by direct array index `pool->getComponents()[index]`, yielding maximal L1 data cache prefetching.
* **In-Place Modification**: Returning `std::tuple<Entity, Components&...>` guarantees that components are mutated directly in their respective pools without temporary copies or heap allocations.
* **C++17/20 Structured Binding**: Enables idiomatic and expressive iteration syntax across game systems:
  ```cpp
  auto view = registry.view<PositionComponent, VelocityComponent, RenderComponent>();
  for (auto [entity, pos, vel, render] : view) {
      pos.value += vel.value * dt;
      // In-place updates to contiguous component memory
  }
  ```

### Thread-Safe Deferred Command Buffer

Modifying ECS structures (spawning entities, attaching components, deleting entities) during multithreaded system execution creates critical data races. 

Lili2D resolves this using a deferred **Command Buffer** pattern:

1. Worker threads query components concurrently and submit structural mutation commands to `lili::CommandBuffer`.
2. Commands are accumulated in thread-safe deferred queues.
3. At the end of the frame tick, the main thread flushes `CommandBuffer` operations sequentially into `ECSRegistry`, guaranteeing lock-free system execution.

---

## 4. Unified Scoped Asset & Resource Engine

Asset management in Lili2D is powered by a dual-tier architecture: templated scoped resource managers (`lili::ResourceManager<T>`) and a centralized static facade (`lili::AssetManager` / `lili::Assets`).

```mermaid
graph TD
    Facade["Assets / AssetManager (Static Facade)"]
    Facade --> TexMgr["ResourceManager<Texture>"]
    Facade --> ShdMgr["ResourceManager<Shader>"]
    Facade --> FntMgr["ResourceManager<BitmapFont>"]
    Facade --> AtlMgr["ResourceManager<AtlasMap>"]
    Facade --> CustMgr["Custom User Managers (ResourceManager<T>)"]
    
    TexMgr --> HR["Hot-Reload File Watcher (std::filesystem)"]
    ShdMgr --> HR
    FntMgr --> HR
    AtlMgr --> HR
    CustMgr --> HR
```

### Key Design Patterns

* **Polymorphic Base Interface (`IResourceManager`)**: Type-erased base class enabling uniform lifecycle control, scope unloading, and hot-reload polling across heterogeneous resource types.
* **Scoped Memory Isolation (`unloadScope`)**: Resources are assigned scope tags (e.g. `"global"`, `"main_menu"`, `"level_01"`). Calling `Assets::unloadScope("level_01")` purges all unused textures, fonts, and shaders associated with that scene, preventing memory leaks during scene switches.
* **Extensible Type Registry**: Custom user asset types `T` can be registered dynamically via `Assets::getManager<T>()`, granting them full caching, scope control, and hot-reloading capabilities.

---

## 5. Live Asset & Shader Hot-Reloading Architecture

Lili2D implements an asynchronous, multi-stage hot-reloading pipeline that allows developers to modify textures, fonts, sprite sheets, and custom HLSL shaders (`.vert.hlsl` / `.frag.hlsl`) on disk while the game is running, updating GPU state in real-time without application restarts.

```mermaid
sequenceDiagram
    autonumber
    actor Dev as Developer / Editor
    participant Disk as Filesystem (.hlsl / .png)
    participant GameLoop as Game Loop (Game::run)
    participant RM as ResourceManager<Shader>
    participant S as Shader Instance
    participant P as MainGraphicsPipeline
    participant M as Material / MainRenderPass

    Dev->>Disk: Edits shader source (e.g. rect.frag.hlsl)
    GameLoop->>RM: Assets::checkHotReload() (per-frame poll)
    RM->>Disk: Checks last_write_time across all WatchedFiles
    RM->>RM: Detects timestamp change on vertex or fragment file
    RM->>S: Executes ReloaderFunc (compiles HLSL -> SPIR-V -> SDL_GPUShader)
    alt Compilation Error (Syntax Error in Shader)
        RM-->>Dev: Logs diagnostic error to std::cerr, keeps active GPU shaders
    else Compilation Success
        S->>S: Move-assigns new GPU shader handles (target = std::move(*reloaded))
        S->>P: Triggers Shader reload listeners (notifyReloaded)
        P->>P: rebuild() creates new SDL_GPUGraphicsPipeline
        P->>P: Replaces internal pipeline handle safely
        M->>P: getPipeline() returns fresh SDL_GPUGraphicsPipeline next frame
        M->>M: MainRenderPass binds updated pipeline with zero downtime
    end
```

### Multi-File Asset Tracking (`WatchedFile`)

Certain assets depend on multiple source files on disk (e.g., a `Shader` depends on both a vertex shader file and a fragment shader file). 

To ensure complete coverage:
* Each `ResourceRecord` stores a collection of `WatchedFile` entries:
  ```cpp
  struct WatchedFile {
      std::string path;
      std::filesystem::file_time_type last_write_time{};
  };
  ```
* When `Assets::loadShader(key, vertPath, fragPath, ...)` is called, `ResourceManager<Shader>` registers both paths in `watched_files`.
* During `checkHotReload()`, timestamps for all associated files are polled. If **any** watched file changes, the asset reloader triggers and all timestamps are updated simultaneously upon success.

### Observer-Driven Graphics Pipeline Rebuilding

In modern graphics APIs (Vulkan, Direct3D 12, Metal), a pipeline state object (`SDL_GPUGraphicsPipeline`) is immutable and baked at creation time with specific shader bytecode. Simply updating the `Shader`'s internal shader modules is insufficient because existing pipelines remain bound to the old state.

Lili2D resolves this using a reactive observer pattern:

1. **Shader Observer Interface**: `Shader` maintains a registry of reload listeners (`addReloadListener`, `removeReloadListener`, `notifyReloaded`).
2. **Automatic Pipeline Subscription**: When a `MainGraphicsPipeline` is constructed with a `Shader*`, it registers a callback on that shader.
3. **In-Place Pipeline Rebuilding**: When the shader reloads successfully:
   * `Shader::operator=(Shader&&)` executes `notifyReloaded()`.
   * The pipeline invokes `MainGraphicsPipeline::rebuild()`, cross-compiling and instantiating a new `SDL_GPUGraphicsPipeline` handle.
   * Smart pointer swapping (`pipeline.reset(new_pipeline)`) safely deallocates the previous pipeline once GPU execution has completed.
4. **Stable Material Binding (`Material::getPipeline()`)**: `Material` stores a `MainGraphicsPipeline*` pointer instead of a stale raw GPU handle. `Material::getPipeline()` dynamically returns the current active pipeline handle, ensuring all materials using that pipeline render with the new shader without requiring per-frame manual re-assignments in gameplay code.

### Exception Guards & Resilient Live Editing

When developers write shader code in an external editor, file saves frequently write partial or syntactically invalid code before completion:
* Shader recompilation is wrapped in exception boundaries inside `ReloaderFunc`.
* If compilation fails, the error message is printed to `std::cerr`, the reload returns `false`, and the active GPU shader and pipeline handles are left untouched.
* The game continues running smoothly using the previous valid shader until the syntax error is corrected.

---

## 6. UI Layout Engine & Matrix Transform Pipeline

All rendered 2D objects inherit from `lili::IRenderable`, establishing a unified interface for positions, scales, rotations, materials, and UI layout positioning.

### UI Pivot & Anchor Normalization

Lili2D features a coordinate-independent UI positioning pipeline based on normalized **Anchors** (screen/viewport relative) and **Pivots** (element bounding box relative).

* **Anchor**: Position relative to the active screen viewport (e.g. `TOP_LEFT` `(0,0)`, `CENTER` `(0.5,0.5)`, `BOTTOM_RIGHT` `(1,1)`).
* **Pivot**: Alignment origin inside the element's bounding rect (e.g. `CENTER` aligns rotation and translation to the element's mid-point).

```txt
GlobalPos = (ViewportSize * AnchorVector) + Offset - (ObjSize * PivotVector)
```

### 3x3 Affine Matrix Transformation (`Mat3`)

Renderables construct a 3x3 transformation matrix combining translation, pivot shifts, scale factors, and 2D rotation:

```txt
M = Translation(Position + AnchorOffset) * Rotation(theta) * Scale(scale) * Translation(-PivotOffset)
```

```cpp
// Calculating 3x3 transformation matrix for UI layout rendering
Mat3 transform = Mat3::translation(screen_pos) *
                 Mat3::rotation(rotation_rad) *
                 Mat3::scale(scale) *
                 Mat3::translation(-pivot_offset);
```

### Inverse-Matrix Point Containment (`containsPoint`)

To test if a screen/mouse coordinate intersects a transformed renderable element, `containsPoint()` multiplies the point by the **inverse** transformation matrix (`inverse(M)`), mapping the coordinate into local unrotated element space for exact bounding box collision.

---

## 7. Virtual Viewport Scaling & Logical Resolution

To protect game logic and UI layouts from physical display resolution changes, window resizing, and aspect ratio variations, Lili2D provides a **Logical Resolution** subsystem (`Window::setLogicalResolution`).

### Aspect-Ratio Aware Letterboxing

When logical resolution is enabled (e.g., `800x600`), the engine computes scale factors and centers the game viewport within physical window dimensions using dynamic pillarboxing or letterboxing:

```txt
Scale = min(PhysicalWidth / LogicalWidth, PhysicalHeight / LogicalHeight)
```

### Coordinate Space Mapping (`toLogicalCoords`)

Input events (mouse cursor coordinates, touch points) recorded in physical screen pixels are transformed into logical game space automatically:

```txt
LogicalPos = (PhysicalPos - ViewportOffset) / Scale
```

This ensures mouse interactions, UI hit tests, and gameplay logic operate strictly in logical game coordinates regardless of monitor DPI or window resizing.

---

## 8. Spatial Physics & Collision Query Subsystem

Lili2D provides lightweight spatial primitive colliders for 2D gameplay physics, raycasting, and broad-phase/narrow-phase queries.

### Collision Primitives & Math

* **`AABB2` / `AABB3`**: Axis-Aligned Bounding Boxes for fast bounding volume hierarchies and rectangular/cuboid hitboxes.
* **`CircleCollider`**: Precise circle-to-circle, circle-to-box, and circle-to-line segment collision solver.

```cpp
// Circle-to-AABB Intersection Algorithm
Vec2 closest_point = {
    std::clamp(circle.center.x, aabb.min.x, aabb.max.x),
    std::clamp(circle.center.y, aabb.min.y, aabb.max.y)
};
Vec2 distance_vec = circle.center - closest_point;
bool intersects = (distance_vec.lengthSquared() <= (circle.radius * circle.radius));
```

### Zero-Allocation Debug Visualization

All colliders include a `debugDraw(renderer, color)` method that submits debug outlines directly to the line rendering pipeline without triggering heap allocations during frame steps.

---

## 9. Hardware Rendering & GPU Memory Optimizations

Lili2D relies on SDL3's `SDL_GPU` abstraction layer for direct modern graphics hardware execution (Vulkan, Direct3D 12, Metal).

### Camera Viewport Frustum Culling

Before submitting tilemap geometry to GPU command buffers, the `TileMap` culls all chunks lying outside the active `Camera` viewport bounds:

* Viewport bounds are calculated in world space by taking screen dimensions and applying inverse camera zoom/translation matrices.
* Chunks outside camera AABB boundaries skip mesh building and draw submission entirely.

### Automated Texture Batching (`SpriteBatch`)

To prevent driver bottlenecks caused by repetitive GPU draw call submissions, Lili2D groups sprites sharing identical texture bindings and render passes into a unified `SpriteBatch`:

* Sprite quads are merged into a single dynamic vertex array.
* Index and vertex data are transferred to GPU VRAM in a single memory upload.
* The batch is rendered via a single `SDL_DrawGPUIndexedPrimitives` draw call.

### Dynamic Rebuild Budgeting

When moving through large tilemap worlds (e.g. `1500x1500` grid maps), camera movement can expose many dirty chunks simultaneously. Rebuilding and uploading GPU buffers for hundreds of chunks in a single frame would exhaust command queue ring buffers and stutter the frame rate.

Lili2D enforces a strict **chunk rebuild budget**:

* A maximum of **8 chunk rebuilds** are enqueued to worker threads per frame tick.
* Excess dirty chunks are queued and rebuilt progressively over subsequent frames.
* Ensures a stable framerate during rapid camera movement across large worlds.

```cpp
int rebuilds_this_frame = 0;
for (auto &pair : chunks) {
    if (chunk.dirty || chunk.rebuilding) {
        if (chunk.dirty) {
            if (rebuilds_this_frame >= 8) {
                continue; // Defer remaining chunk rebuilds to next frame
            }
            rebuilds_this_frame++;
        }
        chunk.rebuildBatches(renderer, thread_pool, chunk_pos, tile_size);
    }
}
```

### GPU Idle Synchronization & Deleter Protections

To eliminate hardware race conditions, GPU use-after-free, and Vulkan descriptor validation errors (`VUID-VkWriteDescriptorSet-descriptorType-02997`) during resource reloads or scene teardowns, Lili2D enforces strict GPU synchronization:

* **RAII GPU Deleters**: Smart deleters for all device-dependent handles (`SDLGPUTextureDeleter`, `SDLGPUSamplerDeleter`, `SDLGPUBufferDeleter`, `SDLGPUShaderDeleter`, `SDLGPUGraphicsPipelineDeleter`) call `SDL_WaitForGPUIdle(device)` before invoking `SDL_ReleaseGPU...`.
* **Renderer Teardown Synchronization**: `Renderer::~Renderer()` idles the active GPU device (`SDL_WaitForGPUIdle`) prior to destroying pipelines, shaders, or GPU meshes during engine shutdown.
* **Defensive Render Pass Validation**: `MainRenderPass::render()` checks that vertex buffers, index buffers, textures, and samplers are non-null before binding and drawing. If any GPU resource is temporarily unallocated or undergoing hot-reload, the pass skips the draw call gracefully for that frame tick rather than passing null handles to the driver.

---

## 10. Custom Shader Cross-Compilation & Dynamic Uniform Buffering

Lili2D unifies shader authoring around **HLSL** as the canonical shading language, transpiling and cross-compiling shaders dynamically to the active backend driver via `SDL_ShaderCross`.

```mermaid
graph LR
    HLSL[HLSL Source .vert / .frag] --> DXC[SDL_ShaderCross / DXC]
    DXC --> SPIRV[SPIR-V Bytecode]
    SPIRV --> Refl[SPIR-V Metadata Reflection]
    SPIRV --> GPUComp[SDL_ShaderCross_CompileGraphicsShaderFromSPIRV]
    GPUComp --> Backend[Vulkan / D3D12 / Metal GPU Shader]
```

### Direct Uniform Push Architecture

Rather than allocating separate persistent uniform buffers per draw call, Lili2D leverages SDL3 GPU's immediate command buffer uniform push model:

* **Vertex Binding Slot 0 (Engine Uniforms)**: Standard MVP 3x3 matrix expanded to 12 floats (3x4 columns), tint color (`Vec4`), UV bounds (`Vec4`), render layer, and elapsed engine time.
* **Vertex Binding Slot 1 (Custom Vertex Uniforms)**: User-defined struct pushed via `material.setVertexUniforms(data)`.
* **Fragment Binding Slot 0 (Custom Fragment Uniforms)**: User-defined struct pushed via `material.setFragmentUniforms(data)`.

```cpp
// Arbitrary uniform struct definition in game code
struct WaveUniforms {
    float time;
    float amplitude;
    float frequency;
    float speed;
};

WaveUniforms uniforms{ clock.getTime(), 0.2f, 30.0f, 5.0f };
rect.getMaterial()->setVertexUniforms(uniforms);
```

At render time, `MainRenderPass::render` transmits the raw byte payloads directly into the command buffer stream via `SDL_PushGPUVertexUniformData` and `SDL_PushGPUFragmentUniformData`, achieving zero heap allocation during uniform updates.

---

## 11. Input Action Mapping Subsystem (`ActionMap`)

To eliminate hardcoded keyboard and mouse checks throughout gameplay systems, Lili2D provides a centralized, string-keyed **Action Mapping** subsystem (`lili::ActionMap`).

### Logical Action Abstraction

Actions decouple game logic from physical hardware input devices:

```cpp
// Register composite key and mouse bindings
ActionMap::get().add("Jump", { Key::SPACE, Key::W });
ActionMap::get().add("Shoot", {}, { MouseButton::LEFT });
ActionMap::get().add("MoveRight", { Key::D, Key::RIGHT });
```

### Tri-State Frame Queries

The action map processes input transitions per frame, allowing systems to query discrete input states:
* `isHeld("MoveRight")`: Returns `true` continuously while any assigned physical key/button is depressed.
* `isJustPressed("Jump")`: Returns `true` strictly on the exact frame the action was activated.
* `isJustReleased("Shoot")`: Returns `true` strictly on the frame the action was released.

---

## 12. Chunk-Based 3D Grid Tilemap & Asynchronous Meshing

Lili2D's world tilemap system (`lili::TileMap`) is architected for expansive grid-based environments using 3D chunk spatial partitioning (`Point3(chunkX, chunkY, layerZ)`).

```mermaid
graph TD
    WorldPos[World Tile Coord Point3] --> Hash[getChunkCoord / getLocalCoord]
    Hash --> Chunk[Target Chunk in std::map]
    Chunk --> Batch[Asynchronous Batch Mesh Generation]
    Batch --> TP[ThreadPool Worker Tasks]
    TP --> GPUMesh[Baked Dynamic GPUMesh Buffers]
```

### Key Architectural Mechanisms:
* **Spatial Chunk Indexing**: World coordinates are divided into fixed-dimension chunks (e.g. 16x16 tiles). Chunks are indexed in a sorted spatial map using `Point3Compare`.
* **Asynchronous Geometry Baking**: When tiles within a chunk change (`setTile`), the chunk marks itself dirty and dispatches geometry baking tasks to the `ThreadPool`. Worker threads construct vertex and index lists in parallel without stalling the main render loop.
* **Layer Depth & Collision Sweep**: The `checkCollision(AABB3)` solver queries solid tile IDs directly within relevant local chunk coordinates, performing rapid bounding box overlap tests without inspecting empty or non-collidable air tiles.

---

## 13. Frame Animation Pipeline & Sprite Slicing (`AtlasMap`, `AnimationPlayer`)

Lili2D handles 2D sprite animations through a decoupled animation model comprising `AtlasMap`, `AnimationRegistry`, and `AnimationPlayer`.

### Sub-Texture UV Slicing (`AtlasMap`)

Spritesheets are sliced into uniform grid cells (`slice(cols, rows)`). The engine computes normalized UV bounding coordinates for each frame:

```txt
UV_min = (col / Cols, row / Rows)
UV_max = ((col + 1) / Cols, (row + 1) / Rows)
```

### Playback & Event Hooks (`AnimationPlayer`)

* **Normalized Frame Accumulation**: `AnimationPlayer` accumulates game delta time against `frame_duration`, advancing the active frame index and wrapping according to the selected `LoopMode` (`Loop`, `Once`, `PingPong`).
* **Frame Callbacks**: Custom events (footstep SFX, attack hitbox activation, projectile spawning) can be attached to specific animation frame indices via `onFrame(frame_idx, callback)`.

---

## 14. Modern C++ Standards, Compile-Time Optimizations & Calling Conventions

To maximize runtime throughput, ensure API safety, and align with modern **C++20** standard best practices, Lili2D follows strict parameter passing, compile-time evaluation, diagnostic attribute enforcement, and inlining guidelines across its subsystems.

### 14.1 Pass-by-Value for Small Trivially Copyable Types (<= 16 Bytes / 128 Bits)

In historical C++ (C++98/03), passing non-primitive types by `const &` was standard practice. In modern C++ on 64-bit architectures, passing small trivially copyable structs (<= 16 bytes) by value is strictly superior:

```mermaid
graph TD
    subgraph ConstReference["Pass by const T& (Pointer Indirection)"]
        CR1["Caller creates stack reference"] --> CR2["Passes 8-byte pointer in General Purpose Register (RDI/RCX)"]
        CR2 --> CR3["Callee dereferences pointer (Memory Load / Cache Dependency)"]
        CR3 --> CR4["Pointer Aliasing: Compiler must assume memory might change!"]
    end

    subgraph ByValue["Pass by Value T (Hardware Registers)"]
        BV1["Caller loads 8B/16B data into CPU registers (XMM0-XMM7 / GPRs)"] --> BV2["Callee typically operates directly on registers (avoids memory indirection)"]
        BV2 --> BV3["No Aliasing: Trivially proved local (actual latency depends on optimization & register pressure)"]
    end
```

#### Hardware ABI Mechanics:
* **x86-64 System V ABI (Linux, macOS)**: Homogeneous floating-point aggregates up to 16 bytes (`Vec2` [8B], `Vec3` [12B], `Vec4` [16B], `RectShape` [16B]) are classified as `SSE` and passed directly in `XMM0`–`XMM7` vector registers. Small integer types (`Point2`, `Point3`, `Entity`) are classified as `INTEGER` and passed in general-purpose registers (`RDI`, `RSI`, `RDX`).
  * *Mixed-Field Types*: For types with mixed integer and floating-point fields such as `CircleShape` (`Vec2` [8B float] + `float` [4B] + `int` [4B]), System V ABI classifies an eightbyte containing both `INTEGER` and `SSE` fields as `INTEGER`. Thus, `CircleShape` is split across an `SSE` register (`XMM` for `Vec2`) and a general-purpose register (`RDI`/`RSI` for `float` + `int`), still passing entirely in registers.
* **ARM64 AAPCS (Apple Silicon, Android, ARM Linux)**: Homogeneous Floating-Point Aggregates (HFAs) up to 4 floats (`Vec2`, `Vec3`, `Vec4`) are passed in SIMD/FP registers `v0`–`v7` (or `s0`–`s3`).
* **Windows x64 ABI**: Types <= 8 bytes (`Vec2`, `Point2`) are passed directly in integer registers (`RCX`, `RDX`, `R8`, `R9`).
* **Zero Aliasing**: By-value parameters guarantee that the passed object cannot alias with other pointers or member fields (`*this`), allowing compiler optimizers to reorder instructions, unroll loops, and auto-vectorize safely.
* **Large Types Exception**: Types exceeding 16 bytes (e.g. `SliceUV` [32B], `std::string`, `MeshData`) exceed the two-eightbyte hardware register limit and must remain passed by `const &` to avoid stack copying.

---

### 14.2 Targeted Header Inlining Strategy & Trade-Offs

Inlining is an enabling optimization that unlocks **constant folding**, **SIMD auto-vectorization**, and **dead code elimination** across call sites.

#### Engineering Trade-offs:
* **Compile-Time Overhead**: Inlining code in headers increases parsing volume across translation units.
* **Instruction Cache (L1i) Pressure**: Excessive inlining duplicates assembly instructions, potentially causing instruction cache thrashing.

#### Lili2D Inlining Strategy:
* **Header-Inlined (`.hpp`)**: Verified per-frame hot paths, leaf primitives, and zero-cost abstractions:
  * **Header-Only Modules**: `lili::geometry` (`Vec2`, `Vec3`, `Vec4`, `Point2`, `Point3`, `Mat3`, `Mat4`, `utils.hpp`) and `lili::physics` (`AABB2`, `AABB3`, `CircleCollider`, `RectShape`, `CircleShape`) are 100% header-only for maximum cross-unit optimization.
  * **Math & Geometry Operators**: Vector, matrix, and point arithmetic operators, dot products, length calculations, and matrix transformations.
  * **Spatial & Collision Solvers**: `AABB2::intersect`, `CircleCollider::intersect`, clamping, and bounding volume tests.
  * **Easing & Functors**: 23 animation easing functions (`lili::Easing`), transparent hash functors (`StringHash`), index calculators (`Chunk::flattenIndex`), and spatial comparators (`Point3Compare`).
  * **1-Line Property Accessors & Forwarding Wrappers**: Fast inline getters/setters (`getPosition`, `getScale`, `getLayer`, `getName`, `getManager`, `registerTile`).
  * **Empty Virtual Base Class Hooks (`{}`)**: Default no-op fallbacks in polymorphic base classes (`Game::onInit`, `Game::onUpdate`, `Game::onRender`, `Scene::onEnter`, `Scene::onExit`, `Scene::onPause`, `Scene::onResume`) generate minimal assembly and avoid redundant `.cpp` stubs.
  * **Class Templates**: Generic systems (`ResourceManager<T>`, `ComponentPool<T>`, `AssetRegistry<T, IdType>`) have inline linkage by default under C++ ODR rules.
* **Source-Separated (`.cpp`)**: Non-hot paths, memory-allocating routines, and subsystem drivers:
  * **Heap Allocations & String Constructors**: Object constructors managing dynamic memory (e.g. `Scene::Scene(const std::string&, Renderer*)`, `Window::Window`, `Game::Game`) prevent code bloat across translation units.
  * **Meyers' Singletons & Private Constructors**: Central registries (`TileRegistry::get()`, `MaterialRegistry::get()`, `AnimationRegistry::get()`, `ActionMap::get()`, `AssetManager::get()`) encapsulate static instances and startup registration within dedicated translation units.
  * **Subsystem Orchestration & Game Loop Drivers**: `Game::run()`, `Game::onEvent()`, `Renderer::initPipeline()`, `Chunk::generateMeshData()`, `AtlasMap::loadXml()`.

---

### 14.3 `constexpr` Constructors & Compile-Time Evaluation

In high-performance game architectures, initializing math primitives and configuration state must incur **zero runtime latency**. Lili2D designates constructors of geometric, physical, and timing primitives (`Vec2`, `Vec3`, `Vec4`, `Point2`, `Point3`, `RectShape`, `CircleCollider`, `Clock`) as `constexpr`.

```mermaid
graph TD
    subgraph ConstexprInit["constexpr Constructor (Compile-Time)"]
        CC1["Code: constexpr Vec2 v(10.0f, 20.0f);"] --> CC2["Compiler Constant Evaluator (AST)"]
        CC2 --> CC3["Values baked directly into .rodata / immediate registers"]
        CC3 --> CC4["Zero runtime cycles / No Static Initialization Order Fiasco (SIOF)"]
    end

    subgraph DynamicInit["Runtime Non-constexpr Constructor"]
        DC1["Code: Vec2 v(10.0f, 20.0f);"] --> DC2["Emits runtime function prologue / call"]
        DC2 --> DC3["Dynamic initialization table executed at startup"]
        DC3 --> DC4["Potential startup latency / Static Init race conditions"]
    end
```

#### Core Architectural Benefits:

1. **Literal Types & `static_assert` Validation**:
   * Marking constructors (including `= default` constructors) as `constexpr` promotes structs to **Literal Types**.
   * Geometric structures can participate in compile-time layout validation, matrix precomputations, and static verification:
     ```cpp
     // Compile-time verification of geometry invariants
     constexpr Vec2 origin{0.0f, 0.0f};
     constexpr CircleCollider collider{origin, 15.0f};
     static_assert(collider.contains(Vec2{5.0f, 5.0f}), "Collision math must be valid at compile-time!");
     ```
2. **Zero-Cost Static Constant Initialization**:
   * Global or static engine constants (e.g., `Vec2::ZERO`, `Mat3::IDENTITY`, predefined UV boxes) constructed with `constexpr` are evaluated during compilation and embedded directly into the binary's read-only data segment (`.rodata`).
   * Eliminates the **Static Initialization Order Fiasco (SIOF)**, guaranteeing that global constants are initialized before any runtime translation unit code executes.
3. **Aggressive Compiler Constant Folding in Hot Paths**:
   * When inline `constexpr` constructors are invoked in inner frame loops (e.g. `Vec2 diff = center - other.center;`), the compiler does not emit function calls or stack frames. It folds constants, evaluates member expressions directly inside CPU registers, and optimizes temporary structs away entirely.

---

### 14.4 `[[nodiscard]]` Attribute: Compiler Enforcement & Warning Triggers

Lili2D extensively applies the standard C++ `[[nodiscard]]` attribute to functions, member accessors, and state-transition routines. `[[nodiscard]]` instructs the compiler to emit a compilation diagnostic (`-Wunused-result` on GCC/Clang, `C4834` on MSVC) whenever the caller invokes a function without consuming, assigning, or testing its return value.

```mermaid
graph TD
    Call["Caller invokes function: e.g. clock.step(), keyboard.justPressed(K), vec.operator+()"] --> Check{"Is return value captured, assigned, or evaluated in a condition?"}
    Check -- Yes --> OK["Compiled cleanly without warning (Correct logic flow)"]
    Check -- No --> Warn["Compiler Warning: -Wunused-result\n'ignoring return value of function declared with [[nodiscard]]'"]
    Warn --> Fix1["Capture result: bool active = keyboard.justPressed(Key::SPACE);"]
    Warn --> Fix2["Use in condition: while (clock.step()) { onFixedUpdate(); }"]
    Warn --> Fix3["Explicit intent suppression: (void)clock.step();"]
```

#### What Triggers a `[[nodiscard]]` Warning in Lili2D:

| Trigger Scenario | Code Example & Signature | Bug Prevented / Rationale |
| :--- | :--- | :--- |
| **State Mutation with Progress Signal** | `[[nodiscard]] inline bool Clock::step()` | Calling `clock.step();` as an unassigned statement decrements the accumulator by `fixed_dt` without executing the required physics tick, causing silent simulation skips. |
| **Pure Input Queries** | `[[nodiscard]] inline bool Keyboard::justPressed(Scancode) const` | Querying `keyboard.justPressed(Key::SPACE);` without reading the boolean indicates a logic omission or forgotten conditional branch. |
| **Pure Timing & Interpolation Accessors** | `[[nodiscard]] constexpr float Clock::getDt() const`<br>`[[nodiscard]] inline float Clock::getAlpha() const` | Reading delta time or render alpha without passing it to physics solvers or render interpolation indicates dead code. |
| **Out-of-Place Geometric Transforms** | `[[nodiscard]] constexpr Vec2 Vec2::operator-(Vec2) const`<br>`[[nodiscard]] constexpr AABB2 CircleCollider::getAABB() const` | Mathematical operations in Lili2D return a newly calculated instance rather than mutating `*this`. Discarding the return value instantly drops the calculated result. |
| **Transparent Hash Calculations** | `[[nodiscard]] constexpr std::size_t StringHash::operator()(string_view) const` | Invoking the transparent hash functor without using the resulting 64-bit integer hash is a redundant CPU operation. |

#### Intentional Discard Suppression:

In rare testing scenarios or benchmarking setups where a return value is intentionally ignored, callers must explicitly document this intent to bypass the compiler diagnostic:

```cpp
// Explicit cast to void documents intentional discard and suppresses -Wunused-result
static_cast<void>(clock.step());
// or using std::ignore:
std::ignore = keyboard.justPressed(Key::ESCAPE);
```

---

### 14.5 `noexcept` Exception Specifications & Move Semantics Guarantees

In modern C++ and low-latency game engine architecture, exception specifications are not merely documentation—they directly dictate code generation, binary size, and standard container performance.

```mermaid
graph TD
    subgraph NoexceptMove["noexcept Move Operations (Lili2D Standard)"]
        NM1["std::vector grows / reallocates capacity"] --> NM2["Checks std::move_if_noexcept<T>"]
        NM2 --> NM3["True: Fast In-Place Memory Move (Pointer Swap / Register Move)"]
        NM3 --> NM4["Zero heap allocations, optimal O(1) transfer per element"]
    end

    subgraph ThrowingMove["Non-noexcept Move Operations (Fallback)"]
        TM1["std::vector grows / reallocates capacity"] --> TM2["Checks std::move_if_noexcept<T>"]
        TM2 --> TM3["False: Falls back to deep copy constructor for strong exception guarantee"]
        TM3 --> TM4["High CPU overhead, deep heap duplication, or compile error on move-only types"]
    end
```

#### Core Architectural Mechanics:

1. **`std::vector` Reallocation & `std::move_if_noexcept`**:
   * Standard library containers (`std::vector<Vertex>`, `std::vector<WatchedFile>`, `ComponentPool<T>`) enforce the **strong exception guarantee**. If an exception is thrown while moving an element during dynamic array reallocation, the vector cannot roll back without corrupting state.
   * To prevent this, standard containers query `std::is_nothrow_move_constructible<T>`. If a type's move constructor or move assignment operator lacks `noexcept`, `std::vector` falls back to **deep copying** every element.
   * For **move-only RAII resources** ([`Shader`](file:///home/lili/Documents/Lili2D/include/lili2d/render/core/shader.hpp), [`GPUMesh`](file:///home/lili/Documents/Lili2D/include/lili2d/render/core/gpu_mesh.hpp), [`Texture`](file:///home/lili/Documents/Lili2D/include/lili2d/render/core/texture.hpp), [`MainGraphicsPipeline`](file:///home/lili/Documents/Lili2D/include/lili2d/render/pipelines/main_graphics_pipeline.hpp), [`Window`](file:///home/lili/Documents/Lili2D/include/lili2d/core/window.hpp)), omitting `noexcept` causes compiler errors or disables safe container relocation. In Lili2D, all move constructors and move assignment operators are explicitly marked `noexcept`:
     ```cpp
     // Example: RAII Move semantics in GPUMesh and Shader
     GPUMesh(GPUMesh&& other) noexcept = default;
     GPUMesh& operator=(GPUMesh&& other) noexcept = default;
     ```

2. **Elimination of Exception Unwinding Overhead (`.eh_frame` / Landing Pads)**:
   * Non-`noexcept` functions require compilers to emit landing pads and stack unwinding metadata tables (`.eh_frame` on Linux/ELF, `.pdata`/`.xdata` on Windows PE) to clean up stack variables if an exception unwinds through the call frame.
   * Marking leaf arithmetic, string hashing ([`StringHash::operator()`](file:///home/lili/Documents/Lili2D/include/lili2d/core/string_hash.hpp)), easing curves ([`Easing`](file:///home/lili/Documents/Lili2D/include/lili2d/core/easing.hpp)), and destructors as `noexcept` strips all unwinding tables. This reduces binary size, conserves instruction cache (L1i) space, and removes branch prediction penalties.

3. **Compiler Optimization & Safe Instruction Reordering**:
   * When a function is declared `noexcept`, the compiler optimizer guarantees that control flow will never abruptly branch to an exception handler at runtime.
   * This allows the compiler to perform aggressive dead-store elimination, register caching across function calls, and automatic loop vectorization that would otherwise be blocked by potential exception escape paths.

---

### 14.6 Implemented Modernization & Optimization Highlights

The engine includes the following verified optimizations:
1. **Header-Only Geometry Modernization (`lili::geometry`)**:
   * `Vec2`, `Vec3`, `Vec4`, `Point2`, `Point3`, `Mat3`, `Mat4`, and `utils.hpp` are fully `constexpr inline` header-only implementations.
   * Arithmetic parameters pass trivially copyable types by value in registers (`VecN`, `PointN`).
   * Compound assignment operators (`operator+=`, `-=`, `*=`) return `T&` for copy-free chaining.
2. **Header-Only Physics & Spatial Collision (`lili::physics`)**:
   * Bounding primitives (`RectShape`, `CircleShape`, `AABB2`, `CircleCollider`) passed by value.
   * Fast bounding box and circle intersection/containment routines fully inlined.
3. **Core & Gameloop Inlining (`lili::core`)**:
   * `Easing` curves, `StringHash`, `Timer`, and input state checks (`Keyboard`, `Mouse`) inlined.
   * Empty polymorphic lifecycle hooks (`Game::onInit`, `Game::onUpdate`, `Game::onFixedUpdate`, `Game::onRender`, `Game::onExit`, `Scene::onEnter`, etc.) inlined in headers as `{}`.
4. **Rendering & Default Shaders (`lili::render`)**:
   * `Vec4` color tints and shape descriptors passed by value across `Renderer`, `Sprite`, `AnimatedSprite`, and `SpriteBatch`.
   * 1-line property getters/setters inlined across `Sprite`, `AnimatedSprite`, `SpriteBatch`, `Circle`, `Line`, and `Rect`.
   * Default 2D shaders embedded as `inline constexpr const char*` HLSL raw string literals in `default_shaders.hpp`, eliminating external shader asset dependencies and build-time code generators.

