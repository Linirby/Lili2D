# Guide: Building a Custom Sparse-Set ECS from Scratch

This guide is designed as an educational reference to help you build your own custom Entity Component System (ECS) and a templated Asset Registry in `Lili2D` step-by-step. 

> [!NOTE]
> **Implementation Note:** This document serves as a design specification and reference guide. The developer will be writing the codebase themselves; the AI should act strictly as an advisor/consultant to explain concepts, review logic, and help debug.

---

## Part 1: Understanding the Sparse-Set ECS

An ECS is designed to solve two problems:
1. **Cache Locality:** Keep components of the same type packed contiguously in memory (`std::vector<T>`) so the CPU can loop through them incredibly fast.
2. **Dynamic Composition:** Allow entities to add/remove components at runtime without complex inheritance hierarchies.

To achieve this, we use the **Sparse-Set** architecture.

### Concept 1: The Generational Entity ID
Instead of using classes, an entity is just a 32-bit integer ID (`uint32_t`). To prevent bugs where a system keeps a reference to a dead entity and accidentally modifies a new entity that reused its ID, we split the 32 bits:
* **Index (lower 24 bits):** The entity's slot in the registry.
* **Generation (upper 8 bits):** Incremented every time an entity slot is destroyed and recycled.

#### Visualizing the Bit Shifting:
```
32-bit Integer: [ Generation (8 bits) ] [ Index (24 bits) ]
Example Entity: [      0x01           ] [    0x000005     ] -> 0x01000005
```

When you query an entity index, you mask out the generation. When you check if an entity is alive, you compare the entire 32-bit ID (including generation) against the registry's record. If the generations do not match, the handle is stale.

---

### Concept 2: The Sparse Set (The core data structure)
For each component type (e.g., `PositionComponent`), you maintain a **Component Pool** implemented as a Sparse Set. This pool consists of two key vectors:

1. **Dense Array (`dense_components`):** A contiguous vector of your component data (`std::vector<T>`). Iterating over this vector is incredibly fast because it has zero memory fragmentation.
2. **Sparse Array (`sparse_entities`):** A lookup vector mapping an `Entity Index` to its position inside the dense array.

#### Visualizing Lookup:
Suppose you want the `PositionComponent` of Entity ID `5`:
```
Entity ID: 5
1. Go to sparse_entities[5] -> returns, say, 2
2. Go to dense_components[2] -> returns the component data!
```

#### Swap-and-Pop Deletion ($O(1)$)
If we delete a component from the middle of the dense array, it creates a hole, which breaks cache locality. To prevent this, we use the **Swap-and-Pop** pattern:
1. Copy the **last** element of the dense array into the slot of the deleted element.
2. Update the sparse array index of the swapped element so it points to its new location.
3. Mark the deleted element's sparse array slot as "tombstoned" (e.g., set it to `-1` or `0xFFFFFFFF`).
4. Pop the last element off the dense array (`pop_back()`).

This keeps all components packed tightly at the front of the vector with zero gaps.

---

### Concept 3: Compile-Time Type IDs
To map a component type (like `PositionComponent`) to its respective pool, you need a unique integer ID for each type. Instead of registering them manually, you can use a classic C++ template trick:

```cpp
template <typename T>
static uint32_t getComponentTypeID() {
    static uint32_t type_id = counter++;
    return type_id;
}
static inline uint32_t counter = 0;
```
Every time this function is called with a new type `T`, C++ instantiates a brand new function with its own static variables. Thus, `counter++` is evaluated once per type, yielding a unique, sequential integer starting at 0.

---

## Part 2: Implementation Checklist

Follow this layout as you write your code:

### 🚀 Step 1: Entity & Helper Functions
Define the `Entity` type and bitwise helper functions in `include/lili2d/core/ecs.hpp`:
- [x] Define `Entity` as `uint32_t`.
- [x] Write helper functions to extract the index (`& 0x00FFFFFF`).
- [x] Write helper functions to extract the generation (`>> 24`).
- [x] Write a helper function to merge index and generation back into a single `Entity`.

### 🚀 Step 2: ComponentPool
Write the templated class `ComponentPool<T>` inheriting from a non-templated `IComponentPool` base interface:
- [x] Build the dynamic vectors: `dense_components` (stores `T`), `dense_entities` (stores `Entity`), and `sparse_entities` (stores lookup indices).
- [x] Implement `emplace(Entity, args...)` using `std::forward` to construct components in-place.
- [x] Implement `remove(Entity)` using the **Swap-and-Pop** pattern.
- [x] Implement `get(Entity)` and `has(Entity)`.

### 🚀 Step 3: Registry
Create the central coordinator class `Registry`:
- [x] Manage an `entities` vector tracking active entity IDs.
- [x] Manage a `free_indices` vector tracking which entity slots can be recycled.
- [x] Manage a list of `IComponentPool` pointers (`std::vector<std::unique_ptr<IComponentPool>>`).
- [x] Implement `createEntity()`: reuse indices from `free_indices` (incrementing generation) or push a new one.
- [x] Implement `destroyEntity(Entity)`: clean components out of all active pools and recycle the index.
- [x] Implement helper methods: `emplaceComponent<T>`, `removeComponent<T>`, `getComponent<T>`, and `hasComponent<T>`.

---

## Part 3: Cleaning Up Asset Registries

Currently, your registries (`TileRegistry`, `AnimationRegistry`, `MaterialRegistry`) map string keys to internal arrays. 

Instead of writing custom lookup logic for each registry, you can write a clean, generic **Asset Registry** base class using templates.

### The Design
1. **Generic base class (`AssetRegistry<T>`)**:
   * Contains a `std::unordered_map<std::string, uint32_t>` for human-readable lookup (like `"player:idle"`).
   * Contains a `std::vector<T>` to store the actual assets.
   * Provides general methods like `registerAsset`, `get(string)`, `get(id)`, and `hasAsset`.
2. **Subclasses**:
   * Inherit from `AssetRegistry<T>`.
   * Add specific singleton helper accesses (e.g. `TileRegistry::get()`).

ps: not sure for the methods names, maybe I sould remove assets from registerAsset and hasAsset because it could be annoying.
This cleanly separates the assets (Tile definitions, textures) from the ECS simulation entities.

---

## Tips for Success
* **Keep it Readable:** Avoid over-optimizing. Start with simple vectors. Use descriptive variable names (e.g., `dense_idx` and `last_dense_idx` in your swap-and-pop code).
* **Test Incrementally:** Once you write the `ComponentPool`, write a small main program to add 3 components, delete 1, and print the vectors to verify that the swap-and-pop works exactly as expected.
