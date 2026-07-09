# Guide: Resolving Conflicts in a DAG Scheduler

When building a Directed Acyclic Graph (DAG) for parallel system execution, you must handle two types of data access conflicts:
1. **Write-Write Conflicts:** Two systems want to write to the same component type.
2. **Read-Write Conflicts:** One system wants to write to a component type while another wants to read from it.

Here is how modern ECS engines (like Bevy, Flecs, or EnTT) solve these problems.

---

## 1. Scheduling Barriers (Forcing Sequence)

If two systems access the same component pool and at least one access is a **Write**, they **cannot run at the same time**. 

Instead of locking them with mutexes at runtime (which slows down performance), the scheduler resolves this when building the execution graph: **it forces them to run one after the other by adding an edge in the DAG.**

```
If:
- System A writes to: Position
- System B writes to: Position

The Scheduler automatically adds a dependency: System A -> System B (or vice versa).
```

### The DAG Solver Logic:
When building the frame execution plan, the scheduler performs the following check for every pair of systems:

```cpp
bool hasConflict(const SystemDependency& a, const SystemDependency& b) {
    // Check if one writes to what the other reads
    for (auto w : a.write_components) {
        if (std::ranges::contains(b.read_components, w)) return true;
        if (std::ranges::contains(b.write_components, w)) return true;
    }
    for (auto w : b.write_components) {
        if (std::ranges::contains(a.read_components, w)) return true;
    }
    return false;
}
```
If `hasConflict` is true, the scheduler inserts a dependency edge between them, guaranteeing they run sequentially.

---

## 2. Command Buffers / Deferred Operations (For Structure Changes)

What if System A and System B both want to create/destroy entities or add/remove components during their update?
Since these operations modify the registry's internal sparse sets (reallocating vectors, swapping elements), doing this concurrently will corrupt memory.

The solution is the **Command Buffer** pattern.

```
[Parallel Systems]                    [Main Thread (Sync Point)]
System A  ---> Writes "Create Entity"  ---> [ Command Queue ] ---> Apply changes
System B  ---> Writes "Add Component"  ---> [                ]      to Registry
```

Instead of modifying the registry directly, systems write commands into a thread-local queue. At a synchronization point (e.g., at the end of the frame or between system groups), the main thread flushes these queues sequentially.

### Implementing a Basic Command Buffer:

```cpp
#include <vector>
#include <functional>

namespace lili {

class CommandBuffer {
public:
    // Queue up modifications
    void createEntity() {
        commands.push_back([](ECSRegistry& registry) {
            registry.createEntity();
        });
    }

    template <typename T, typename... Args>
    void emplaceComponent(Entity entity, Args&&... args) {
        // Capture arguments by moving them into the lambda
        commands.push_back([entity, ...args = std::forward<Args>(args)](ECSRegistry& registry) mutable {
            registry.emplaceComponent<T>(entity, std::move(args)...);
        });
    }

    void destroyEntity(Entity entity) {
        commands.push_back([entity](ECSRegistry& registry) {
            registry.destroyEntity(entity);
        });
    }

    // Run sequentially on the main thread
    void play(ECSRegistry& registry) {
        for (const auto& command : commands) {
            command(registry);
        }
        commands.clear();
    }

private:
    std::vector<std::function<void(ECSRegistry&)>> commands;
};

} // namespace lili
```

### How to use it in your Frame Loop:
1. Pass a thread-local `CommandBuffer` to each system.
2. Systems run in parallel and write to their own private `CommandBuffer` (no mutexes needed, as each thread writes to its own memory).
3. Once all parallel systems finish, call `.play(registry)` on each command buffer sequentially on the main thread.

---

## Summary of Best Practices
* **For component data changes (e.g., changing position coordinates):** Use **DAG Scheduling Barriers** to force conflicting systems to run sequentially.
* **For structural changes (e.g., creating/destroying entities, adding/removing components):** Use **Command Buffers** to defer changes to a single-threaded execution phase at the end of the frame.
